#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateManager.h>
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
    StateManager::StateManager()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "StateManager" );
        }

        m_stateQueues.resize( static_cast<u32>( Thread::Task::Count ) );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        for( auto &m_stateQueue : m_stateQueues )
        {
            auto queue = factoryManager->make_ptr<StateQueueStandard>();
            queue->setGarbageCollected( false );
            m_stateQueue = queue;
        }

        auto pNewStateObjects = fb::make_shared<ConcurrentArray<SmartPtr<IStateContext>>>();
        setStateContextsPtr( pNewStateObjects );
    }

    StateManager::~StateManager()
    {
        unload( nullptr );
    }

    void StateManager::load( SmartPtr<ISharedObject> data )
    {
    }

    void StateManager::unload( SmartPtr<ISharedObject> data )
    {
        for( auto stateQueue : m_stateQueues )
        {
            stateQueue->unload( nullptr );
        }

        m_stateQueues.clear();
        m_stateObjects = nullptr;

        m_states.clear();
        m_states2d.clear();
    }

    auto StateManager::addStateObject() -> SmartPtr<IStateContext>
    {
        auto stateContext = fb::make_ptr<StateContextStandard>();

        auto pStateObjects = getStateContextsPtr();
        pStateObjects->push_back( stateContext );

        return stateContext;
    }

    void StateManager::addStateObject( SmartPtr<IStateContext> context )
    {
        auto stateContext = fb::make_ptr<StateContextStandard>();

        auto pStateObjects = getStateContextsPtr();
        pStateObjects->push_back( stateContext );
    }

    auto StateManager::removeStateObject( SmartPtr<IStateContext> stateContext ) -> bool
    {
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
                *pNewStateObjects = ConcurrentArray<SmartPtr<IStateContext>>( newStateObjects.begin(),
                                                                              newStateObjects.end() );
                setStateContextsPtr( pNewStateObjects );

                return true;
            }
        }

        return false;
    }

    auto StateManager::removeStateObject( u32 id ) -> bool
    {
        return false;
    }

    auto StateManager::findStateObject( u32 id ) const -> SmartPtr<IStateContext>
    {
        // RecursiveMutex::ScopedLock lock(Mutex);
        // for(u32 i=0; i<m_stateObjects.size(); ++i)
        //{
        //	const SmartPtr<IStateObject>& stateContext = m_stateObjects[i];
        //	if(stateContext->getId() == id)
        //		return stateContext;
        // }

        return nullptr;
    }

    auto StateManager::getStateContexts() const -> Array<SmartPtr<IStateContext>>
    {
        return {};
    }

    void StateManager::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto task = Thread::getCurrentTask();

        auto stateQueue = m_stateQueues[static_cast<u32>( task )];
        if( stateQueue )
        {
            auto pStateObjects = getStateContextsPtr();
            auto stateObjects = *pStateObjects;

            if( !stateQueue->isEmpty() )
            {
                auto pMessages = stateQueue->getMessagesAndClear();
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

            for( auto &stateContext : stateObjects )
            {
                stateContext->update();
            }
        }
    }

    void StateManager::sendMessage( Thread::Task taskId, SmartPtr<IStateMessage> message )
    {
        auto stateQueue = m_stateQueues[static_cast<u32>( taskId )];
        stateQueue->queueMessage( message );
    }

    auto StateManager::getQueue( Thread::Task taskId ) -> SmartPtr<IStateQueue>
    {
        return m_stateQueues[static_cast<u32>( taskId )];
    }

    void StateManager::destroyQueue( SmartPtr<IStateQueue> queue )
    {
        queue = nullptr;
    }

    void StateManager::addState( s32 task, SmartPtr<IState> state )
    {
        //state->setRegistered( true );

        // StateData& states = m_states[task];

        // SpinRWMutex::ScopedLock lock(states.Mutex);
        // states.m_states[state.get()] = state;
    }

    void StateManager::addState( SmartPtr<IState> state )
    {
    }

    void StateManager::removeState( s32 task, SmartPtr<IState> state )
    {
        // state->setRegistered(false);

        // StateData& states = m_states[task];

        ////SpinRWMutex::ScopedLock lock(states.Mutex);
        // states.m_states.erase(state.get());
    }

    auto StateManager::removeState( SmartPtr<IState> state ) -> bool
    {
        return false;
    }

    auto StateManager::getStates( const u32 typeInfo ) -> Array<SmartPtr<IState>>
    {
        return {};
    }

    void StateManager::addState2D( u32 index, SmartPtr<IState> state )
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

    void StateManager::removeState2D( u32 index, SmartPtr<IState> state )
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

    void StateManager::addStates2D( const u32 typeInfo, Array<Array<SmartPtr<IState>>> states )
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

    void StateManager::removeStates2D( const u32 typeInfo, Array<Array<SmartPtr<IState>>> states )
    {
    }

    auto StateManager::getStates2D( const u32 typeInfo ) -> Array<Array<SmartPtr<IState>>>
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

    auto StateManager::getStateContextsPtr() const -> SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>>
    {
        return m_stateObjects;
    }

    void StateManager::setStateContextsPtr(
        SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> stateObjects )
    {
        m_stateObjects = stateObjects;
    }

}  // end namespace fb
