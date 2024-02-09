#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateManagerStandard.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Set.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/System/StateQueueStandard.h>
#include <FBCore/System/StateContextStandard.h>

namespace fb
{

    FB_CLASS_REGISTER_DERIVED( fb, StateManagerStandard, IStateManager );

    StateManagerStandard::StateManagerStandard()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "StateManagerStandard" );
        }

        auto iTaskCount = static_cast<u32>( Thread::Task::Count );

        auto p = fb::make_shared<ConcurrentArray<AtomicSmartPtr<IStateQueue>>>();
        auto &stateQueues = *p;
        m_stateQueues = p;
        stateQueues.resize( iTaskCount );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        for( auto &stateQueue : stateQueues )
        {
            auto queue = factoryManager->make_ptr<StateQueueStandard>();
            //queue->setGarbageCollected( false );
            stateQueue = queue;
        }

        m_dirtyQueue.resize( static_cast<u32>( Thread::Task::Count ) );

        auto pNewStateObjects = fb::make_shared<ConcurrentArray<SmartPtr<IStateContext>>>();
        setStateContextsPtr( pNewStateObjects );
    }

    StateManagerStandard::~StateManagerStandard()
    {
        unload( nullptr );
    }

    void StateManagerStandard::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                if( auto p = getStateContextsPtr() )
                {
                    auto &stateObjects = *p;
                    for( auto &stateContext : stateObjects )
                    {
                        stateContext->unload( nullptr );
                    }

                    setStateContextsPtr( nullptr );
                }

                auto numStateQueues = getNumStateQueues();
                for( size_t i = 0; i < numStateQueues; ++i )
                {
                    if( auto stateQueue = getStateQueue( static_cast<u32>( i ) ) )
                    {
                        stateQueue->unload( nullptr );
                        //stateQueue->setGarbageCollected( true );
                    }
                }

                if( auto p = getStateQueuesPtr() )
                {
                    auto &stateQueues = *p;
                    stateQueues.clear();
                }

                m_states.clear();
                m_states2d.clear();

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto StateManagerStandard::addStateObject() -> SmartPtr<IStateContext>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto stateContext = factoryManager->make_ptr<StateContextStandard>();
        FB_ASSERT( stateContext );

        stateContext->load( nullptr );

        if( auto pStateObjects = getStateContextsPtr() )
        {
            auto &stateObjects = *pStateObjects;
            stateObjects.push_back( stateContext );
        }

        return stateContext;
    }

    void StateManagerStandard::addStateObject( SmartPtr<IStateContext> context )
    {
        if( auto pStateObjects = getStateContextsPtr() )
        {
            auto &stateObjects = *pStateObjects;
            stateObjects.push_back( context );
        }
    }

    auto StateManagerStandard::removeStateObject( SmartPtr<IStateContext> stateContext ) -> bool
    {
        if( stateContext )
        {
            stateContext->unload( nullptr );

            if( auto pStateObjects = getStateContextsPtr() )
            {
                auto &stateObjects = *pStateObjects;

                auto newStateObjects =
                    Array<SmartPtr<IStateContext>>( stateObjects.begin(), stateObjects.end() );

                auto it = std::find( newStateObjects.begin(), newStateObjects.end(), stateContext );
                if( it != newStateObjects.end() )
                {
                    newStateObjects.erase( it );

                    auto pNewStateObjects = fb::make_shared<ConcurrentArray<SmartPtr<IStateContext>>>();
                    *pNewStateObjects = ConcurrentArray<SmartPtr<IStateContext>>(
                        newStateObjects.begin(), newStateObjects.end() );
                    setStateContextsPtr( pNewStateObjects );

                    return true;
                }
            }
        }

        return false;
    }

    auto StateManagerStandard::removeStateObject( u32 id ) -> bool
    {
        auto stateContext = findStateObject( id );
        return removeStateObject( stateContext );
    }

    auto StateManagerStandard::findStateObject( u32 id ) const -> SmartPtr<IStateContext>
    {
        if( auto p = getStateContextsPtr() )
        {
            auto &stateObjects = *p;
            for( auto stateContext : stateObjects )
            {
                if( auto handle = stateContext->getHandle() )
                {
                    if( handle->getId() == id )
                    {
                        return stateContext;
                    }
                }
            }
        }

        return nullptr;
    }

    auto StateManagerStandard::getStateContexts() const -> Array<SmartPtr<IStateContext>>
    {
        if( auto p = getStateContextsPtr() )
        {
            auto &stateObjects = *p;
            return Array<SmartPtr<IStateContext>>( stateObjects.begin(), stateObjects.end() );
        }

        return {};
    }

    void StateManagerStandard::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        //if (auto application = applicationManager->getApplication())
        //{
        //    auto &loadingState = application->getLoadingState();
        //    if (loadingState != LoadingState::Loaded)
        //    {
        //        return;
        //    }
        //}

        auto task = Thread::getCurrentTask();
        auto iTask = static_cast<u32>( task );

        if( auto stateQueue = getStateQueue( iTask ) )
        {
            if( auto pStateObjects = getStateContextsPtr() )
            {
                auto &stateObjects = *pStateObjects;

                if( !stateQueue->isEmpty() )
                {
                    if( auto pMessages = stateQueue->getMessagesAndClear() )
                    {
                        auto &messages = *pMessages;
                        for( auto &message : messages )
                        {
                            for( auto &stateContext : stateObjects )
                            {
                                auto pListeners = stateContext->getStateListeners();
                                auto listeners = *pListeners;
                                for( auto &listener : listeners )
                                {
                                    listener->handleStateChanged( message );
                                }
                            }
                        }
                    }
                }

                auto maxDirtyQueueSize = 1024;
                Array<SmartPtr<IStateContext>> dirtyQueue;
                dirtyQueue.reserve( maxDirtyQueueSize );

                auto &queue = m_dirtyQueue[iTask];
                if( !queue.empty() )
                {
                    SmartPtr<IStateContext> context;
                    while( queue.try_pop( context ) )
                    {
                        dirtyQueue.push_back( context );

                        if( dirtyQueue.size() >= maxDirtyQueueSize )
                        {
                            break;
                        }
                    }
                }

                std::sort( dirtyQueue.begin(), dirtyQueue.end() );
                dirtyQueue.erase( std::unique( dirtyQueue.begin(), dirtyQueue.end() ),
                                  dirtyQueue.end() );

                for( auto &context : dirtyQueue )
                {
                    context->update();
                }
            }
        }
    }

    void StateManagerStandard::sendMessage( Thread::Task taskId, SmartPtr<IStateMessage> message )
    {
        auto iTask = static_cast<u32>( taskId );
        auto stateQueue = getStateQueue( iTask );
        stateQueue->queueMessage( message );
    }

    auto StateManagerStandard::getQueue( Thread::Task taskId ) -> SmartPtr<IStateQueue>
    {
        if( const auto p = getStateQueuesPtr() )
        {
            const auto &stateQueues = *p;

            auto iTask = static_cast<u32>( taskId );
            return stateQueues.at( iTask );
        }

        return nullptr;
    }

    void StateManagerStandard::destroyQueue( SmartPtr<IStateQueue> queue )
    {
        queue = nullptr;
    }

    void StateManagerStandard::addState( s32 task, SmartPtr<IState> state )
    {
        //state->setRegistered( true );

        // StateData& states = m_states[task];

        // SpinRWMutex::ScopedLock lock(states.Mutex);
        // states.m_states[state.get()] = state;
    }

    void StateManagerStandard::addState( SmartPtr<IState> state )
    {
    }

    void StateManagerStandard::removeState( s32 task, SmartPtr<IState> state )
    {
        // state->setRegistered(false);

        // StateData& states = m_states[task];

        ////SpinRWMutex::ScopedLock lock(states.Mutex);
        // states.m_states.erase(state.get());
    }

    auto StateManagerStandard::removeState( SmartPtr<IState> state ) -> bool
    {
        return false;
    }

    auto StateManagerStandard::getStates( const u32 typeInfo ) -> Array<SmartPtr<IState>>
    {
        return {};
    }

    void StateManagerStandard::addState2D( u32 index, SmartPtr<IState> state )
    {
        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        if( m_states2d.empty() )
        {
            auto totalNumTypes = typeManager->getTotalNumTypes();
            m_states2d.resize( totalNumTypes );
        }

        auto typeInfo = state->getTypeInfo();
        if( typeInfo )
        {
            auto typeIndex = typeManager->getTypeIndex( typeInfo );
            auto &states2d = m_states2d[typeIndex];
            if( index >= states2d.size() )
            {
                while( index >= states2d.size() )
                {
                    states2d.emplace_back();
                }
            }

            auto states = states2d[index];
            states.push_back( state );
        }
    }

    void StateManagerStandard::removeState2D( u32 index, SmartPtr<IState> state )
    {
        if( !m_states2d.empty() )
        {
            auto typeInfo = state->getTypeInfo();
            if( typeInfo )
            {
                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                auto typeIndex = typeManager->getTypeIndex( typeInfo );
                auto &states2d = m_states2d[typeIndex];
                auto &states = states2d[index];

                auto it = std::find( states.begin(), states.end(), state );
                if( it != states.end() )
                {
                    states.erase( it );
                }
            }
        }
    }

    void StateManagerStandard::addStates2D( const u32 typeInfo, Array<Array<SmartPtr<IState>>> states )
    {
        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        if( m_states2d.empty() )
        {
            auto totalNumTypes = typeManager->getTotalNumTypes();
            m_states2d.resize( totalNumTypes );
        }

        if( typeInfo )
        {
            auto typeIndex = typeManager->getTypeIndex( typeInfo );
            auto &states2d = m_states2d[typeIndex];
            states2d = states;
        }
    }

    void StateManagerStandard::removeStates2D( const u32 typeInfo,
                                               Array<Array<SmartPtr<IState>>> states )
    {
    }

    auto StateManagerStandard::getStates2D( const u32 typeInfo ) -> Array<Array<SmartPtr<IState>>>
    {
        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        auto typeIndex = typeManager->getTypeIndex( typeInfo );
        if( typeIndex < m_states2d.size() )
        {
            return m_states2d[typeIndex];
        }

        return {};
    }

    auto StateManagerStandard::getNumStateQueues() const -> u32
    {
        if( const auto p = getStateQueuesPtr() )
        {
            const auto &stateQueues = *p;
            return static_cast<u32>( stateQueues.size() );
        }

        return 0;
    }

    auto StateManagerStandard::getStateQueue( u32 index ) const -> SmartPtr<IStateQueue>
    {
        if( const auto p = getStateQueuesPtr() )
        {
            const auto &stateQueues = *p;

            FB_ASSERT( index < stateQueues.size() );
            return stateQueues[index];
        }

        return nullptr;
    }

    auto StateManagerStandard::getStateContextsPtr() const
        -> SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>>
    {
        return m_stateObjects;
    }

    void StateManagerStandard::setStateContextsPtr(
        SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> stateObjects )
    {
        m_stateObjects = stateObjects;
    }

    void StateManagerStandard::addDirty( SmartPtr<IStateContext> context )
    {
        for( size_t i = 0; i < static_cast<u32>( Thread::Task::Count ); ++i )
        {
            m_dirtyQueue[i].push( context );
        }
    }

    void StateManagerStandard::addDirty( SmartPtr<IStateContext> context, Thread::Task task )
    {
        auto iTask = static_cast<u32>( task );
        m_dirtyQueue[iTask].push( context );
    }

    auto StateManagerStandard::getStateQueuesPtr() const
        -> SharedPtr<ConcurrentArray<AtomicSmartPtr<IStateQueue>>>
    {
        return m_stateQueues;
    }

    void StateManagerStandard::setStateQueuesPtr(
        SharedPtr<ConcurrentArray<AtomicSmartPtr<IStateQueue>>> stateQueues )
    {
        m_stateQueues = stateQueues;
    }
}  // end namespace fb
