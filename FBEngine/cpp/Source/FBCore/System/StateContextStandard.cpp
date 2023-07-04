#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateContextStandard.h>
#include <FBCore/System/StateQueueStandard.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Interface/IApplicationManager.h>
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
#include <FBCore/System/EventJob.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, StateContextStandard, SharedObject<IStateContext> );

    u32 StateContextStandard::m_nextGeneratedNameExt = 1000;

    StateContextStandard::StateContextStandard() :
        SharedObject<IStateContext>(),
        m_isAdded( false ),
        m_enableMessageQueues( true ),
        m_isDirty( false ),
        m_removeCount( 0 )
    {
        m_id = m_nextGeneratedNameExt++;
    }

    StateContextStandard::StateContextStandard( u32 id ) :
        SharedObject<IStateContext>(),
        m_id( id ),
        m_isAdded( false )
    {
    }

    StateContextStandard::~StateContextStandard()
    {
        unload( nullptr );

        FB_ASSERT( getOwner() == nullptr );
        FB_ASSERT( getState() == nullptr );
    }

    void StateContextStandard::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::IApplicationManager::instance();
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

                if( auto state = getState() )
                {
                    state->setStateContext( nullptr );
                    state->unload( nullptr );
                    setState( nullptr );
                }

                m_states.clear();

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
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto task = Thread::getCurrentTask();

            //if( task == Thread::Task::Render )
            //{
            //    int stop = 0;
            //    stop = 0;
            //}

            //if( auto owner = getOwner() )
            //{
            //    if( owner->isDerived<render::IOverlay>() )
            //    {
            //        if( task == Thread::Task::Render )
            //        {
            //            int stop = 0;
            //            stop = 0;
            //        }
            //    }
            //}

            if( auto state = getState() )
            {
                if( bool dirty = state->isDirty() )
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
                    }

                    if( state->isDirty() )
                    {
                        stateManager->addDirty( this, task );
                    }
                }
            }

            if( auto owner = getOwner() )
            {
                const auto &loadingState = owner->getLoadingState();
                if( loadingState != LoadingState::Loaded )
                {
                    stateManager->addDirty( this, task );
                    return;
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
            message->setStateObject( this );

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

            auto applicationManager = core::IApplicationManager::instance();
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

    bool StateContextStandard::removeStateListener( SmartPtr<IStateListener> stateListner )
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

    SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> StateContextStandard::getStateListeners() const
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

    bool StateContextStandard::removeEventListener( SmartPtr<IEventListener> eventListener )
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

    SharedPtr<Array<SmartPtr<IEventListener>>> StateContextStandard::getEventListeners() const
    {
        return m_eventListeners;
    }

    void StateContextStandard::setId( u32 id )
    {
        m_id = id;
    }

    u32 StateContextStandard::getId() const
    {
        return m_id;
    }

    void StateContextStandard::setOwner( SmartPtr<ISharedObject> owner )
    {
        m_owner = owner;
    }

    SmartPtr<ISharedObject> StateContextStandard::getOwner() const
    {
        if( auto p = m_owner.load() )
        {
            return p.lock();
        }

        return nullptr;
    }

    SmartPtr<IStateQueue> StateContextStandard::getStateQueue( u32 taskId ) const
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

        return nullptr;
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
            auto engine = core::IApplicationManager::instance();
            auto stateMgr = engine->getStateManager();
            //stateMgr->addStateObject( this );
            m_isAdded = true;
        }
    }

    void StateContextStandard::remove()
    {
        if( m_isAdded )
        {
            auto engine = core::IApplicationManager::instance();
            auto stateMgr = engine->getStateManager();
            //stateMgr->removeStateObject( this );
            m_isAdded = false;
        }
    }

    bool StateContextStandard::isAdded() const
    {
        return m_isAdded;
    }

    u32 StateContextStandard::getQueryTask() const
    {
        return m_taskId;
    }

    void StateContextStandard::setQueryTask( u32 taskId )
    {
        m_taskId = taskId;
    }

    SmartPtr<IState> StateContextStandard::getLatestOutputState() const
    {
        auto state = getState();
        return state->clone();
    }

    SmartPtr<IState> StateContextStandard::getOutputState( time_interval time ) const
    {
        auto state = getState();
        return state->clone();
    }

    void StateContextStandard::addOutputState( SmartPtr<IState> state )
    {
        m_states.push_back( state );
    }

    SmartPtr<IState> StateContextStandard::getState() const
    {
        return m_state;
    }

    void StateContextStandard::setState( SmartPtr<IState> state )
    {
        m_state = state;

        if( state )
        {
            state->setStateContext( this );
        }
    }

    bool StateContextStandard::isDirty() const
    {
        return m_isDirty;
    }

    void StateContextStandard::setDirty( bool dirty, bool cascade )
    {
        m_isDirty = dirty;

        if( cascade )
        {
            if( auto state = getState() )
            {
                state->setDirty( dirty );
            }
        }
    }

    bool StateContextStandard::isStateDirty() const
    {
        return m_stateChangeCount != m_stateUpdateCount;
    }

    void StateContextStandard::setStateDirty( bool dirty )
    {
        if( dirty )
            ++m_stateChangeCount;
        else
            m_stateUpdateCount = static_cast<u32>( m_stateChangeCount );
    }

    bool StateContextStandard::isBitSet( u32 flags, s32 bitIdx ) const
    {
        u32 flag = ( 1 << bitIdx );
        if( ( flags & flag ) != 0 )
        {
            return true;
        }

        return false;
    }

    u32 StateContextStandard::getDirtyFlags() const
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

    bool StateContextStandard::getEnableMessageQueues() const
    {
        return m_enableMessageQueues;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> StateContextStandard::getStateQueuesPtr() const
    {
        return m_stateQueues;
    }

    void StateContextStandard::setStateQueuesPtr( SharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> ptr )
    {
        m_stateQueues = ptr;
    }

    bool StateContextStandard::getUpdateState() const
    {
        return m_bUpdateState;
    }

    void StateContextStandard::setUpdateState( bool val )
    {
        m_bUpdateState = val;
    }

    SmartPtr<Properties> StateContextStandard::getProperties() const
    {
        auto applicationManager = core::IApplicationManager::instance();
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

    Parameter StateContextStandard::triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                                  const Array<Parameter> &arguments,
                                                  SmartPtr<ISharedObject> sender,
                                                  SmartPtr<ISharedObject> object,
                                                  SmartPtr<IEvent> event )
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto jobQueue = applicationManager->getJobQueue();
        if( jobQueue )
        {
            auto eventJob = fb::make_ptr<EventJob>();
            eventJob->owner = this;
            eventJob->eventType = eventType;
            eventJob->eventValue = eventValue;
            eventJob->arguments = arguments;
            eventJob->setSender( sender );
            eventJob->setObject( object );
            eventJob->event = event;

            jobQueue->queueJobAll( eventJob );

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

        return Parameter();
    }

    bool StateContextStandard::isValid() const
    {
        auto owner = getOwner();
        if( owner && getState() )
        {
            return true;
        }

        return false;
    }
}  // end namespace fb
