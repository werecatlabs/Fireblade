#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateContextStandard.h>
#include <FBCore/System/StateQueueStandard.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/IAsyncOperation.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/State/States/GraphicsSystemState.h>
#include <FBCore/Jobs/EventJob.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateContextStandard, IStateContext );

    u32 StateContextStandard::m_nextGeneratedNameExt = 1000;

    StateContextStandard::StateContextStandard() :
        IStateContext(),

        m_isDirty( false ),
        m_removeCount( 0 )
    {
        m_id = m_nextGeneratedNameExt++;
    }

    StateContextStandard::StateContextStandard( u32 id ) : IStateContext(), m_id( id )

    {
    }

    StateContextStandard::~StateContextStandard()
    {
        unload( nullptr );

        FB_ASSERT( getOwner() == nullptr );
    }

    void StateContextStandard::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            m_bUpdateState = true;

            auto pStateQueues = factoryManager->make_shared<ConcurrentArray<SmartPtr<IStateQueue>>>();
            auto &stateQueues = *pStateQueues;
            stateQueues.resize( static_cast<u32>( Thread::Task::Count ) );

            for( auto &queue : stateQueues )
            {
                queue = factoryManager->make_ptr<StateQueueStandard>();
            }

            setStateQueuesPtr( pStateQueues );

            m_sharedObjectListener = factoryManager->make_ptr<SharedObjectListener>();

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void StateContextStandard::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                if( auto pStateQueues = getStateQueuesPtr() )
                {
                    auto stateQueues = *pStateQueues;
                    for( auto &stateQueue : stateQueues )
                    {
                        if( stateQueue )
                        {
                            if( auto pMessages = stateQueue->getMessagesAndClear() )
                            {
                                auto &messages = *pMessages;

                                for( auto &message : messages )
                                {
                                    message->unload( nullptr );
                                }
                            }
                        }
                    }
                }

                auto states = getStates();
                for( auto state : states )
                {
                    state->setStateContext( nullptr );
                    state->unload( nullptr );
                }

                m_states = nullptr;

                if( auto pStateQueues = getStateQueuesPtr() )
                {
                    auto &stateQueues = *pStateQueues;
                    for( auto stateQueue : stateQueues )
                    {
                        stateQueue->unload( nullptr );
                    }

                    setStateQueuesPtr( nullptr );
                }

                m_listeners = nullptr;
                m_owner = nullptr;

                m_sharedObjectListener = nullptr;

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void StateContextStandard::update()
    {
        try
        {
            auto task = Thread::getCurrentTask();

            auto states = getStates();
            for( auto state : states )
            {
                if( state->isDirty() )
                {
                    auto stateTask = state->getTaskId();
                    if( task == stateTask )
                    {
                        if( auto pListeners = getStateListeners() )
                        {
                            auto &listeners = *pListeners;
                            for( auto &listener : listeners )
                            {
                                if( listener )
                                {
                                    listener->handleStateChanged( state );
                                }
                            }
                        }

                        if( state->isDirty() )
                        {
                            auto &applicationManager = core::ApplicationManager::instance();
                            auto &stateManager = applicationManager->getStateManager();
                            stateManager->addDirty( this, task );
                        }
                    }
                }
            }

            if( auto stateQueue = getStateQueue( static_cast<u32>( task ) ) )
            {
                if( !stateQueue->isEmpty() )
                {
                    if( auto pMessages = stateQueue->getMessagesAndClear() )
                    {
                        auto &messages = *pMessages;

                        for( auto &message : messages )
                        {
                            if( message )
                            {
                                if( auto pListeners = getStateListeners() )
                                {
                                    auto &listeners = *pListeners;
                                    for( auto &listener : listeners )
                                    {
                                        try
                                        {
                                            if( listener )
                                            {
                                                listener->handleStateChanged( message );
                                            }

                                            message->unload( nullptr );
                                        }
                                        catch( std::exception &e )
                                        {
                                            FB_LOG_EXCEPTION( e );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if( stateQueue->isEmpty() )
                {
                    setDirtyFlag( ( 1 << static_cast<u32>( task ) ), false );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void StateContextStandard::addMessage( Thread::Task taskId, SmartPtr<IStateMessage> message )
    {
        if( message )
        {
            message->setStateContext( this );

            if( auto enableMessageQueues = getEnableMessageQueues() )
            {
                auto iTask = static_cast<u32>( taskId );
                if( auto stateQueue = getStateQueue( iTask ) )
                {
                    stateQueue->queueMessage( message );
                }
            }
            else
            {
                sendMessage( message );
            }

            setDirtyFlag( ( 1 << static_cast<u32>( taskId ) ), true );

            m_removeCount = 0;

            auto applicationManager = core::ApplicationManager::instance();
            auto stateManager = applicationManager->getStateManager();
            stateManager->addDirty( this, taskId );
        }
    }

    void StateContextStandard::addStateListener( SmartPtr<IStateListener> stateListner )
    {
        auto pListeners = getStateListeners();
        if( !pListeners )
        {
            pListeners = fb::make_shared<ConcurrentArray<SmartPtr<IStateListener>>>();
            setStateListeners( pListeners );
        }

        if( pListeners )
        {
            auto &listeners = *pListeners;
            listeners.push_back( stateListner );
        }
    }

    auto StateContextStandard::removeStateListener( SmartPtr<IStateListener> stateListner ) -> bool
    {
        if( auto pListeners = getStateListeners() )
        {
            auto listeners = Array<SmartPtr<IStateListener>>( pListeners->begin(), pListeners->end() );
            auto it = std::find( listeners.begin(), listeners.end(), stateListner );
            if( it != listeners.end() )
            {
                listeners.erase( it );

                auto pNewListeners = fb::make_shared<ConcurrentArray<SmartPtr<IStateListener>>>();
                *pNewListeners =
                    ConcurrentArray<SmartPtr<IStateListener>>( listeners.begin(), listeners.end() );
                setStateListeners( pNewListeners );

                return true;
            }
        }

        return false;
    }

    auto StateContextStandard::getStateListeners() const
        -> SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>>
    {
        return m_listeners;
    }

    void StateContextStandard::addEventListener( SmartPtr<IEventListener> eventListener )
    {
        auto pEventListeners = getEventListeners();
        if( !pEventListeners )
        {
            pEventListeners = fb::make_shared<Array<SmartPtr<IEventListener>>>();
            setEventListeners( pEventListeners );
        }

        if( pEventListeners )
        {
            auto &eventListeners = *pEventListeners;
            eventListeners.push_back( eventListener );
        }
    }

    auto StateContextStandard::removeEventListener( SmartPtr<IEventListener> eventListener ) -> bool
    {
        if( auto pEventListeners = getEventListeners() )
        {
            auto &eventListeners = *pEventListeners;
            auto it = std::find( eventListeners.begin(), eventListeners.end(), eventListener );
            if( it != eventListeners.end() )
            {
                eventListeners.erase( it );
                return true;
            }
        }

        return false;
    }

    auto StateContextStandard::getEventListeners() const -> SharedPtr<Array<SmartPtr<IEventListener>>>
    {
        return m_eventListeners;
    }

    void StateContextStandard::setId( u32 id )
    {
        m_id = id;
    }

    auto StateContextStandard::getId() const -> u32
    {
        return m_id;
    }

    void StateContextStandard::setOwner( SmartPtr<ISharedObject> owner )
    {
        if( m_owner )
        {
            m_owner->removeObjectListener( m_sharedObjectListener );
        }

        m_owner = owner;

        if( m_owner )
        {
            m_owner->addObjectListener( m_sharedObjectListener );
        }
    }

    auto StateContextStandard::getOwner() -> SmartPtr<ISharedObject> &
    {
        return m_owner;
    }

    auto StateContextStandard::getOwner() const -> const SmartPtr<ISharedObject> &
    {
        return m_owner;
    }

    auto StateContextStandard::getStateQueue( u32 taskId ) -> SmartPtr<IStateQueue> &
    {
        if( auto p = getStateQueuesPtr() )
        {
            auto &stateQueues = *p;
            if( !stateQueues.empty() )
            {
                if( taskId < stateQueues.size() )
                {
                    return stateQueues[taskId];
                }
            }
        }

        static SmartPtr<IStateQueue> nullQueue;
        return nullQueue;
    }

    auto StateContextStandard::getStateQueue( u32 taskId ) const -> const SmartPtr<IStateQueue> &
    {
        if( auto p = getStateQueuesPtr() )
        {
            auto &stateQueues = *p;
            if( !stateQueues.empty() )
            {
                if( taskId < stateQueues.size() )
                {
                    return stateQueues[taskId];
                }
            }
        }

        static SmartPtr<IStateQueue> nullQueue;
        return nullQueue;
    }

    void StateContextStandard::sendMessage( SmartPtr<IStateMessage> message )
    {
        if( auto pListeners = getStateListeners() )
        {
            auto &listeners = *pListeners;

            for( auto &listener : listeners )
            {
                listener->handleStateChanged( message );
            }
        }
    }

    void StateContextStandard::_processStateUpdate( SmartPtr<IState> &state )
    {
        if( auto pListeners = getStateListeners() )
        {
            auto &listeners = *pListeners;

            for( auto &listener : listeners )
            {
                listener->handleStateChanged( state );
            }
        }
    }

    void StateContextStandard::_processQuery( SmartPtr<IStateQuery> &query )
    {
        if( auto pListeners = getStateListeners() )
        {
            auto &listeners = *pListeners;

            for( auto &listener : listeners )
            {
                listener->handleQuery( query );
            }
        }
    }

    void StateContextStandard::add()
    {
        if( !m_isAdded )
        {
            auto engine = core::ApplicationManager::instance();
            auto stateMgr = engine->getStateManager();
            //stateMgr->addStateObject( this );
            m_isAdded = true;
        }
    }

    void StateContextStandard::remove()
    {
        if( m_isAdded )
        {
            auto engine = core::ApplicationManager::instance();
            auto stateMgr = engine->getStateManager();
            //stateMgr->removeStateObject( this );
            m_isAdded = false;
        }
    }

    auto StateContextStandard::isAdded() const -> bool
    {
        return m_isAdded;
    }

    auto StateContextStandard::getQueryTask() const -> u32
    {
        return m_taskId;
    }

    void StateContextStandard::setQueryTask( u32 taskId )
    {
        m_taskId = taskId;
    }

    void StateContextStandard::addState( SmartPtr<IState> state )
    {
        if( state )
        {
            state->setStateContext( this );
        }

        auto p = getStatesPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IState>>>();
            setStatesPtr( p );
        }

        if( p )
        {
            auto &states = *p;
            states.push_back( state );
        }
    }

    void StateContextStandard::removeState( SmartPtr<IState> state )
    {
        auto states = getStates();
        states.erase( std::remove( states.begin(), states.end(), state ), states.end() );

        auto newStates =
            fb::make_shared<ConcurrentArray<SmartPtr<IState>>>( states.begin(), states.end() );
        setStatesPtr( newStates );
    }

    auto StateContextStandard::getStateByTypeId( u32 typeId ) -> SmartPtr<IState> &
    {
        if( auto p = getStatesPtr() )
        {
            auto &states = *p;
            for( auto &state : states )
            {
                if( state->derived( typeId ) )
                {
                    return state;
                }
            }
        }

        static SmartPtr<IState> nullState;
        return nullState;
    }

    auto StateContextStandard::getStateByTypeId( u32 typeId ) const -> const SmartPtr<IState> &
    {
        if( auto p = getStatesPtr() )
        {
            auto &states = *p;
            for( auto &state : states )
            {
                if( state->derived( typeId ) )
                {
                    return state;
                }
            }
        }

        static SmartPtr<IState> nullState;
        return nullState;
    }

    auto StateContextStandard::getStates() const -> Array<SmartPtr<IState>>
    {
        if( auto p = getStatesPtr() )
        {
            auto states = Array<SmartPtr<IState>>( p->begin(), p->end() );
            return states;
        }

        return {};
    }

    auto StateContextStandard::isDirty() const -> bool
    {
        return m_isDirty;
    }

    void StateContextStandard::setDirty( bool dirty, bool cascade )
    {
        m_isDirty = dirty;

        if( cascade )
        {
            auto states = getStates();
            for( auto state : states )
            {
                state->setDirty( dirty );
            }
        }
    }

    auto StateContextStandard::isStateDirty() const -> bool
    {
        return m_stateChangeCount != m_stateUpdateCount;
    }

    void StateContextStandard::setStateDirty( bool dirty )
    {
        if( dirty )
        {
            ++m_stateChangeCount;
        }
        else
        {
            m_stateUpdateCount = static_cast<u32>( m_stateChangeCount );
        }
    }

    auto StateContextStandard::isBitSet( u32 flags, s32 bitIdx ) const -> bool
    {
        u32 flag = ( 1 << bitIdx );
        if( ( flags & flag ) != 0 )
        {
            return true;
        }

        return false;
    }

    auto StateContextStandard::getDirtyFlags() const -> u32
    {
        return m_isDirty;
    }

    void StateContextStandard::setDirtyFlags( u32 dirtyFlags )
    {
        m_isDirty = dirtyFlags;
    }

    void StateContextStandard::setDirtyFlag( u32 flag, bool val )
    {
        auto dirtyFlags = getDirtyFlags();

        if( val )
        {
            dirtyFlags |= flag;
        }
        else
        {
            dirtyFlags &= ~flag;
        }

        setDirtyFlags( dirtyFlags );
    }

    void StateContextStandard::setStateListeners(
        SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> listeners )
    {
        m_listeners = listeners;
    }

    void StateContextStandard::setEventListeners(
        SharedPtr<Array<SmartPtr<IEventListener>>> eventListeners )
    {
        m_eventListeners = eventListeners;
    }

    auto StateContextStandard::getEnableMessageQueues() const -> bool
    {
        return m_enableMessageQueues;
    }

    void StateContextStandard::setStateQueuesPtr( SharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> ptr )
    {
        m_stateQueues = ptr;
    }

    void StateContextStandard::setStatesPtr( SharedPtr<ConcurrentArray<SmartPtr<IState>>> states )
    {
        m_states = states;
    }

    auto StateContextStandard::getUpdateState() const -> bool
    {
        return m_bUpdateState;
    }

    void StateContextStandard::setUpdateState( bool val )
    {
        m_bUpdateState = val;
    }

    auto StateContextStandard::getProperties() const -> SmartPtr<Properties>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto properties = factoryManager->make_ptr<Properties>();
        FB_ASSERT( properties );

        return properties;
    }

    void StateContextStandard::setProperties( SmartPtr<Properties> properties )
    {
    }

    auto StateContextStandard::triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                             const Array<Parameter> &arguments,
                                             SmartPtr<ISharedObject> sender,
                                             SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        auto &applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto &jobQueue = applicationManager->getJobQueue();
        if( jobQueue )
        {
            auto eventJob = fb::make_ptr<EventJob>();
            eventJob->setOwner( this );
            eventJob->setEventType( eventType );
            eventJob->setEventValue( eventValue );
            eventJob->setArguments( arguments );
            eventJob->setSender( sender );
            eventJob->setObject( object );
            eventJob->setEvent( event );

            jobQueue->addJobAllTasks( eventJob );

            if( auto pEventListeners = getEventListeners() )
            {
                auto &eventListeners = *pEventListeners;
                for( auto eventListener : eventListeners )
                {
                    if( eventListener )
                    {
                        eventListener->handleEvent( eventType, eventValue, arguments, sender, object,
                                                    event );
                    }
                }
            }
        }

        return {};
    }

    auto StateContextStandard::isValid() const -> bool
    {
        auto owner = getOwner();
        if( owner )
        {
            auto states = getStates();
            for( auto state : states )
            {
                if( state )
                {
                    return true;
                }
            }
        }

        return false;
    }

    StateContextStandard::SharedObjectListener::SharedObjectListener() = default;
    StateContextStandard::SharedObjectListener::~SharedObjectListener() = default;

    //void StateContextStandard::SharedObjectListener::loadingStateChanged( ISharedObject *sharedObject,
    //                                                                      LoadingState oldState,
    //                                                                      LoadingState newState )
    //{
    //    auto context = getOwner();
    //    if( context )
    //    {
    //        if( auto owner = context->getOwner() )
    //        {
    //            if( !owner->isLoaded() )
    //            {
    //                auto states = context->getStates();
    //                for( auto state : states )
    //                {
    //                    auto task = state->getTaskId();

    //                    auto &applicationManager = core::ApplicationManager::instance();
    //                    auto &stateManager = applicationManager->getStateManager();
    //                    stateManager->addDirty( this, task );
    //                }
    //            }
    //        }
    //    }
    //}

    Parameter StateContextStandard::SharedObjectListener::handleEvent(
        IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
        SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
    {
        if( eventValue == IEvent::loadingStateChanged )
        {
            auto loadingState = (LoadingState)arguments[1].getU32();
            if( loadingState == LoadingState::Loaded )
            {
                if( auto context = getOwner() )
                {
                    if( auto owner = context->getOwner() )
                    {
                        if( !owner->isLoaded() )
                        {
                            auto states = context->getStates();
                            for( auto state : states )
                            {
                                auto task = state->getTaskId();

                                auto &applicationManager = core::ApplicationManager::instance();
                                auto &stateManager = applicationManager->getStateManager();
                                stateManager->addDirty( this, task );
                            }
                        }
                    }
                }
            }
        }

        return {};
    }

    fb::SmartPtr<fb::StateContextStandard> StateContextStandard::SharedObjectListener::getOwner() const
    {
        auto p = m_owner.lock();
        return p;
    }

    void StateContextStandard::SharedObjectListener::setOwner( SmartPtr<StateContextStandard> owner )
    {
        m_owner = owner;
    }

}  // end namespace fb
