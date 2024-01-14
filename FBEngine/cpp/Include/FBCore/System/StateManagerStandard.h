#ifndef StateManagerStandard_h__
#define StateManagerStandard_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Core/HashTable.h>
#include <FBCore/Core/HashMap.h>

namespace fb
{

    /** Implementation for a state manager. */
    class StateManagerStandard : public IStateManager
    {
    public:
        /** Constructor. */
        StateManagerStandard();

        /** Destructor. */
        ~StateManagerStandard() override;

        /** @copydoc IStateManager::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IStateManager::update */
        void update() override;

        /** @copydoc IStateManager::sendMessage */
        void sendMessage( Thread::Task taskId, SmartPtr<IStateMessage> message ) override;

        /** @copydoc IStateManager::addStateObject */
        SmartPtr<IStateContext> addStateObject() override;

        /** @copydoc IStateManager::addStateObject */
        void addStateObject( SmartPtr<IStateContext> context ) override;

        /** @copydoc IStateManager::removeStateObject */
        bool removeStateObject( SmartPtr<IStateContext> stateContext ) override;

        /** @copydoc IStateManager::findStateObject */
        SmartPtr<IStateContext> findStateObject( u32 id ) const override;

        /** @copydoc IStateManager::getStateContexts */
        Array<SmartPtr<IStateContext>> getStateContexts() const override;

        /** @copydoc IStateManager::getQueue */
        SmartPtr<IStateQueue> getQueue( Thread::Task taskId ) override;

        /** @copydoc IStateManager::addState */
        void addState( SmartPtr<IState> state ) override;

        /** @copydoc IStateManager::removeState */
        bool removeState( SmartPtr<IState> state ) override;

        /** @copydoc IStateManager::getStates */
        Array<SmartPtr<IState>> getStates( u32 typeInfo ) override;

        /** @copydoc IStateManager::addDirty */
        void addDirty( SmartPtr<IStateContext> context ) override;

        /** @copydoc IStateManager::addDirty */
        void addDirty( SmartPtr<IStateContext> context, Thread::Task task ) override;

    protected:
        u32 getNumStateQueues() const;

        SmartPtr<IStateQueue> getStateQueue( u32 index ) const;

        SharedPtr<ConcurrentArray<AtomicSmartPtr<IStateQueue>>> getStateQueuesPtr() const;
        void setStateQueuesPtr( SharedPtr<ConcurrentArray<AtomicSmartPtr<IStateQueue>>> stateQueues );

        SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> getStateContextsPtr() const;
        void setStateContextsPtr( SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> stateObjects );

        void addState2D( u32 index, SmartPtr<IState> state );

        void removeState2D( u32 index, SmartPtr<IState> state );

        /** Useful for object hierarchies. */
        void addStates2D( u32 typeInfo, Array<Array<SmartPtr<IState>>> states );

        void removeStates2D( u32 typeInfo, Array<Array<SmartPtr<IState>>> states );

        bool removeStateObject( u32 id );

        Array<Array<SmartPtr<IState>>> getStates2D( u32 typeInfo );

        void destroyQueue( SmartPtr<IStateQueue> queue );

        void addState( s32 task, SmartPtr<IState> state );

        void removeState( s32 task, SmartPtr<IState> state );

        ConcurrentArray<ConcurrentQueue<SmartPtr<IStateContext>>> m_dirtyQueue;

        AtomicSharedPtr<ConcurrentArray<AtomicSmartPtr<IStateQueue>>> m_stateQueues;
        AtomicSharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> m_stateObjects;

        Array<Array<SmartPtr<IState>>> m_states;
        Array<Array<Array<SmartPtr<IState>>>> m_states2d;
    };
}  // end namespace fb

#endif  // StateManagerStandard_h__
