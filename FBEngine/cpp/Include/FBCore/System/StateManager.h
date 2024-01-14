#ifndef StateManager_h__
#define StateManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Core/ConcurrentArray.h>

namespace fb
{

    /**
     * @brief Implementation for a state manager.
     *
     * The aim of this class is to keep it light weight with minimal dependencies.
     */
    class StateManager : public IStateManager
    {
    public:
        /**
         * @brief Constructor.
         */
        StateManager();

        /**
         * @brief Destructor.
         */
        ~StateManager() override;

        /**
         * @brief Loads a shared object into the state manager.
         * @param data The shared object to load.
         */
        void load( SmartPtr<ISharedObject> data ) override;

        /**
         * @brief Unloads a shared object from the state manager.
         * @param data The shared object to unload.
         */
        void unload( SmartPtr<ISharedObject> data ) override;

        /**
         * @brief Updates the state manager.
         */
        void update() override;

        /**
         * @brief Sends a message to a specific task.
         * @param taskId The task to send the message to.
         * @param message The message to send.
         */
        void sendMessage( Thread::Task taskId, SmartPtr<IStateMessage> message ) override;

        /**
         * @brief Adds a state object to the state manager.
         * @return The newly added state object.
         */
        SmartPtr<IStateContext> addStateObject() override;

        /**
         * @brief Adds a state object to the state manager.
         * @param context The state object to add.
         */
        void addStateObject( SmartPtr<IStateContext> context );

        /**
         * @brief Removes a state object from the state manager.
         * @param stateContext The state object to remove.
         * @return True if the state object was removed, false otherwise.
         */
        bool removeStateObject( SmartPtr<IStateContext> stateContext ) override;

        /**
         * @brief Removes a state object from the state manager.
         * @param id The ID of the state object to remove.
         * @return True if the state object was removed, false otherwise.
         */
        bool removeStateObject( u32 id );

        /**
         * @brief Finds a state object by ID.
         * @param id The ID of the state object to find.
         * @return The state object with the given ID, or a null pointer if no such object exists.
         */
        SmartPtr<IStateContext> findStateObject( u32 id ) const override;

        /**
         * @brief Gets an array of all state objects in the state manager.
         * @return An array of all state objects in the state manager.
         */
        Array<SmartPtr<IStateContext>> getStateContexts() const override;

        /**
         * @brief Gets the state queue for a given task.
         * @param taskId The task to get the state queue for.
         * @return The state queue for the given task.
         */
        SmartPtr<IStateQueue> getQueue( Thread::Task taskId ) override;

        /**
         * @brief Destroys a state queue.
         * @param queue The state queue to destroy.
         */
        void destroyQueue( SmartPtr<IStateQueue> queue );

        /**
         * @brief Adds a state to a given task.
         * @param task The task to add the state to.
         * @param state The state to add.
         */
        void addState( s32 task, SmartPtr<IState> state );

        /**
         * @brief Removes a state from a given task.
         * @param task The task to remove the state from.
         * @param state The state to remove.
         */
        void removeState( s32 task, SmartPtr<IState> state );

        void addState( SmartPtr<IState> state ) override;
        bool removeState( SmartPtr<IState> state ) override;
        Array<SmartPtr<IState>> getStates( u32 typeInfo );

        void addState2D( u32 index, SmartPtr<IState> state );

        void removeState2D( u32 index, SmartPtr<IState> state );

        /** Useful for object hierarchies. */
        void addStates2D( u32 typeInfo, Array<Array<SmartPtr<IState>>> states );

        void removeStates2D( u32 typeInfo, Array<Array<SmartPtr<IState>>> states );

        Array<Array<SmartPtr<IState>>> getStates2D( u32 typeInfo );

    protected:
        SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> getStateContextsPtr() const;
        void setStateContextsPtr( SharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> stateObjects );

        ConcurrentArray<SmartPtr<IStateQueue>> m_stateQueues;
        AtomicSharedPtr<ConcurrentArray<SmartPtr<IStateContext>>> m_stateObjects;

        Array<Array<SmartPtr<IState>>> m_states;
        Array<Array<Array<SmartPtr<IState>>>> m_states2d;
    };
}  // end namespace fb

#endif  // StateManager_h__
