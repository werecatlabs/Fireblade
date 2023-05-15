#include <FBCore/FBCorePCH.h>
#include <FBCore/System/StateManager.h>
#include <FBCore/Interface/IApplicationManager.h>
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
        setGarbageCollected( false );

        m_stateQueues.resize( static_cast<u32>(Thread::Task::Count) );

        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        for(u32 i = 0; i < m_stateQueues.size(); ++i)
        {
            auto queue = factoryManager->make_ptr<StateQueueStandard>();
            queue->setGarbageCollected( false );
            m_stateQueues[i] = queue;
        }

        auto pNewStateObjects = fb::make_shared<ConcurrentArray<SmartPtr<IStateContext>>>();
        setStateObjectsPtr( pNewStateObjects );
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
        for(auto stateQueue : m_stateQueues)
        {
            stateQueue->unload( nullptr );
        }

        m_stateQueues.clear();
        m_stateObjects = nullptr;

        m_states.clear();
        m_states2d.clear();
    }

    SmartPtr<IStateContext> StateManager::addStateObject()
    {
        auto stateObject = fb::make_ptr<StateContextStandard>();

        auto pStateObjects = getStateObjectsPtr();
        pStateObjects->push_back( stateObject );

        return stateObject;
    }

    void StateManager::addStateObject( SmartPtr<IStateContext> context )
    {
        auto stateObject = fb::make_ptr<StateContextStandard>();

        auto pStateObjects = getStateObjectsPtr();
        pStateObjects->push_back( stateObject );
    }

    bool StateManager::removeStateObject( SmartPtr<IStateContext> stateObject )
    {
        auto pStateObjects = getStateObjectsPtr();
        auto &stateObjects = *pStateObjects;

        auto newStateObjects =
            Array<SmartPtr<IStateContext>>( stateObjects.begin(), stateObjects.end() );

        auto it = std::find( newStateObjects.begin(), newStateObjects.end(), stateObject );
        if(it != newStateObjects.end())
        {
            newStateObjects.erase( it );

            auto pNewStateObjects = fb::make_shared<ConcurrentArray<SmartPtr<IStateContext>>>();
            *pNewStateObjects = ConcurrentArray<SmartPtr<IStateContext>>( newStateObjects.begin(),
                newStateObjects.end() );
            setStateObjectsPtr( pNewStateObjects );

            return true;
        }

        return false;
    }

    bool StateManager::removeStateObject( u32 id )
    {
        return false;
    }

    SmartPtr<IStateContext> StateManager::findStateObject( u32 id ) const
    {
        // RecursiveMutex::ScopedLock lock(Mutex);
        // for(u32 i=0; i<m_stateObjects.size(); ++i)
        //{
        //	const SmartPtr<IStateObject>& stateObject = m_stateObjects[i];
        //	if(stateObject->getId() == id)
        //		return stateObject;
        // }

        return nullptr;
    }

    Array<SmartPtr<IStateContext>> StateManager::getStateObjects() const
    {
        return Array<SmartPtr<IStateContext>>();
    }

    void StateManager::update()
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto task = Thread::getCurrentTask();

        auto stateQueue = m_stateQueues[static_cast<u32>(task)];
        if(stateQueue)
        {
            auto pStateObjects = getStateObjectsPtr();
            auto stateObjects = *pStateObjects;

            if(!stateQueue->isEmpty())
            {
                auto pMessages = stateQueue->getMessagesAndClear();
                auto &messages = *pMessages;
                for(auto &message : messages)
                {
                    for(auto &stateObject : stateObjects)
                    {
                        auto pListeners = stateObject->getStateListeners();
                        auto listeners = *pListeners;
                        for(auto &listener : listeners)
                        {
                            listener->handleStateChanged( message );
                        }
                    }
                }
            }

            for(auto &stateObject : stateObjects)
            {
                stateObject->update();
            }
        }
    }

    void StateManager::sendMessage( Thread::Task taskId, SmartPtr<IStateMessage> message )
    {
        auto stateQueue = m_stateQueues[static_cast<u32>(taskId)];
        stateQueue->queueMessage( message );
    }

    SmartPtr<IStateQueue> StateManager::getQueue( Thread::Task taskId )
    {
        return m_stateQueues[static_cast<u32>(taskId)];
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

    bool StateManager::removeState( SmartPtr<IState> state )
    {
        return false;
    }

    Array<SmartPtr<IState>> StateManager::getStates( const u32 typeInfo )
    {
        return Array<SmartPtr<IState>>();
    }

    void StateManager::addState2D( u32 index, SmartPtr<IState> state )
    {
        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        if(m_states2d.empty())
        {
            auto totalNumTypes = typeManager->getTotalNumTypes();
            m_states2d.resize( totalNumTypes );
        }

        auto typeInfo = state->getTypeInfo();
        if(typeInfo)
        {
            auto typeIndex = typeManager->getTypeIndex( typeInfo );
            auto &states2d = m_states2d[typeIndex];
            if(index >= states2d.size())
            {
                while(index >= states2d.size())
                {
                    states2d.push_back( Array<SmartPtr<IState>>() );
                }
            }

            auto states = states2d[index];
            states.push_back( state );
        }
    }

    void StateManager::removeState2D( u32 index, SmartPtr<IState> state )
    {
        if(!m_states2d.empty())
        {
            auto typeInfo = state->getTypeInfo();
            if(typeInfo)
            {
                auto typeManager = TypeManager::instance();
                FB_ASSERT( typeManager );

                auto typeIndex = typeManager->getTypeIndex( typeInfo );
                auto &states2d = m_states2d[typeIndex];
                auto &states = states2d[index];

                auto it = std::find( states.begin(), states.end(), state );
                if(it != states.end())
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

        if(m_states2d.empty())
        {
            auto totalNumTypes = typeManager->getTotalNumTypes();
            m_states2d.resize( totalNumTypes );
        }

        if(typeInfo)
        {
            auto typeIndex = typeManager->getTypeIndex( typeInfo );
            auto &states2d = m_states2d[typeIndex];
            states2d = states;
        }
    }

    void StateManager::removeStates2D( const u32 typeInfo, Array<Array<SmartPtr<IState>>> states )
    {
    }

    Array<Array<SmartPtr<IState>>> StateManager::getStates2D( const u32 typeInfo )
    {
        auto typeManager = TypeManager::instance();
        FB_ASSERT( typeManager );

        auto typeIndex = typeManager->getTypeIndex( typeInfo );
        if(typeIndex < m_states2d.size())
        {
            return m_states2d[typeIndex];
        }

        return Array<Array<SmartPtr<IState>>>();
    }

    SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> StateManager::getStateObjectsPtr() const
    {
        return m_stateObjects;
    }

    void StateManager::setStateObjectsPtr(
        SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> stateObjects )
    {
        m_stateObjects = stateObjects;
    }

} // end namespace fb
