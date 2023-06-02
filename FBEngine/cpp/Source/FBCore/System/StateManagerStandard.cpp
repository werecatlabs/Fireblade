#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateManagerStandard.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Set.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/IState.h>
#include <FBCore/Interface/System/IStateMessage.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/System/StateQueueStandard.h>
#include <FBCore/System/StateContextStandard.h>

#include "FBCore/Interface/IApplicationClient.h"

namespace fb
{
    StateManagerStandard::StateManagerStandard()
    {
        auto iTaskCount = static_cast<u32>( Thread::Task::Count );

        auto p = fb::make_shared<ConcurrentArray<AtomicSmartPtr<IStateQueue>>>();
        auto &stateQueues = *p;
        m_stateQueues = p;
        stateQueues.resize( iTaskCount );

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        for( u32 i = 0; i < stateQueues.size(); ++i )
        {
            auto queue = factoryManager->make_ptr<StateQueueStandard>();
            //queue->setGarbageCollected( false );
            stateQueues[i] = queue;
        }

        m_dirtyQueue.resize( (u32)Thread::Task::Count );

        auto pNewStateObjects = fb::make_shared<ConcurrentArray<SmartPtr<IStateContext>>>();
        setStateObjectsPtr( pNewStateObjects );
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

                if (auto p = getStateObjectsPtr())
                {
                    auto& stateObjects = *p;
                    for(auto &stateObject : stateObjects)
                    {
                        stateObject->unload( nullptr );
                    }

                    setStateObjectsPtr(nullptr);
                }

                auto numStateQueues = getNumStateQueues();
                for( size_t i = 0; i < numStateQueues; ++i )
                {
                    if( auto stateQueue = getStateQueue( (u32)i ) )
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

    SmartPtr<IStateContext> StateManagerStandard::addStateObject()
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto stateObject = factoryManager->make_ptr<StateContextStandard>();
        FB_ASSERT( stateObject );

        stateObject->load(nullptr);

        if( auto pStateObjects = getStateObjectsPtr() )
        {
            auto &stateObjects = *pStateObjects;
            stateObjects.push_back( stateObject );
        }

        return stateObject;
    }

    void StateManagerStandard::addStateObject( SmartPtr<IStateContext> context )
    {
        if( auto pStateObjects = getStateObjectsPtr() )
        {
            auto &stateObjects = *pStateObjects;
            stateObjects.push_back( context );
        }
    }

    bool StateManagerStandard::removeStateObject( SmartPtr<IStateContext> stateObject )
    {
        if( stateObject )
        {
            stateObject->unload( nullptr );

            if( auto pStateObjects = getStateObjectsPtr() )
            {
                auto &stateObjects = *pStateObjects;

                auto newStateObjects =
                    Array<SmartPtr<IStateContext>>( stateObjects.begin(), stateObjects.end() );

                auto it = std::find( newStateObjects.begin(), newStateObjects.end(), stateObject );
                if( it != newStateObjects.end() )
                {
                    newStateObjects.erase( it );

                    auto pNewStateObjects = fb::make_shared<ConcurrentArray<SmartPtr<IStateContext>>>();
                    *pNewStateObjects = ConcurrentArray<SmartPtr<IStateContext>>(
                        newStateObjects.begin(), newStateObjects.end() );
                    setStateObjectsPtr( pNewStateObjects );

                    return true;
                }
            }
        }

        return false;
    }

    bool StateManagerStandard::removeStateObject( u32 id )
    {
        auto stateObject = findStateObject( id );
        return removeStateObject( stateObject );
    }

    SmartPtr<IStateContext> StateManagerStandard::findStateObject( u32 id ) const
    {
        if( auto p = getStateObjectsPtr() )
        {
            auto &stateObjects = *p;
            for( auto stateObject : stateObjects )
            {
                if( auto handle = stateObject->getHandle() )
                {
                    if( handle->getId() == id )
                    {
                        return stateObject;
                    }
                }
            }
        }

        return nullptr;
    }

    Array<SmartPtr<IStateContext>> StateManagerStandard::getStateObjects() const
    {
        if( auto p = getStateObjectsPtr() )
        {
            auto &stateObjects = *p;
            return Array<SmartPtr<IStateContext>>( stateObjects.begin(), stateObjects.end() );
        }

        return Array<SmartPtr<IStateContext>>();
    }

    void StateManagerStandard::update()
    {
        auto applicationManager = core::IApplicationManager::instance();
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
            if( auto pStateObjects = getStateObjectsPtr() )
            {
                auto &stateObjects = *pStateObjects;

                if( !stateQueue->isEmpty() )
                {
                    if( auto pMessages = stateQueue->getMessagesAndClear() )
                    {
                        auto &messages = *pMessages;
                        for( auto &message : messages )
                        {
                            for( auto &stateObject : stateObjects )
                            {
                                auto pListeners = stateObject->getStateListeners();
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

    SmartPtr<IStateQueue> StateManagerStandard::getQueue( Thread::Task taskId )
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

    bool StateManagerStandard::removeState( SmartPtr<IState> state )
    {
        return false;
    }

    Array<SmartPtr<IState>> StateManagerStandard::getStates( const u32 typeInfo )
    {
        return Array<SmartPtr<IState>>();
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
                    states2d.push_back( Array<SmartPtr<IState>>() );
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

    Array<Array<SmartPtr<IState>>> StateManagerStandard::getStates2D( const u32 typeInfo )
    {
        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        auto typeIndex = typeManager->getTypeIndex( typeInfo );
        if( typeIndex < m_states2d.size() )
        {
            return m_states2d[typeIndex];
        }

        return Array<Array<SmartPtr<IState>>>();
    }

    u32 StateManagerStandard::getNumStateQueues() const
    {
        if( const auto p = getStateQueuesPtr() )
        {
            const auto &stateQueues = *p;
            return (u32)stateQueues.size();
        }

        return 0;
    }

    SmartPtr<IStateQueue> StateManagerStandard::getStateQueue( u32 index ) const
    {
        if( const auto p = getStateQueuesPtr() )
        {
            const auto &stateQueues = *p;

            FB_ASSERT( index < stateQueues.size() );
            return stateQueues[index];
        }

        return nullptr;
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> StateManagerStandard::getStateObjectsPtr() const
    {
        return m_stateObjects;
    }

    void StateManagerStandard::setStateObjectsPtr(
        SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> stateObjects )
    {
        m_stateObjects = stateObjects;
    }

    void StateManagerStandard::addDirty( SmartPtr<IStateContext> context )
    {
        for( size_t i = 0; i < (u32)Thread::Task::Count; ++i )
        {
            m_dirtyQueue[i].push( context );
        }
    }

    void StateManagerStandard::addDirty( SmartPtr<IStateContext> context, Thread::Task task )
    {
        auto iTask = (u32)task;
        m_dirtyQueue[iTask].push( context );
    }

    SharedPtr<ConcurrentArray<AtomicSmartPtr<IStateQueue>>> StateManagerStandard::getStateQueuesPtr()
        const
    {
        return m_stateQueues;
    }

    void StateManagerStandard::setStateQueuesPtr(
        SharedPtr<ConcurrentArray<AtomicSmartPtr<IStateQueue>>> stateQueues )
    {
        m_stateQueues = stateQueues;
    }
}  // end namespace fb
