#ifndef StateObjectStandard_h__
#define StateObjectStandard_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/System/StateQueueStandard.h>
#include <FBCore/System/Job.h>
#include <FBCore/Core/ConcurrentArray.h>

namespace fb
{

    /** Standard implementation of the IStateObject interface. */
    class StateContextStandard : public IStateContext
    {
    public:
        /** Default constructor. */
        StateContextStandard();

        /** Constructor. */
        explicit StateContextStandard( u32 id );

        /** Destructor. */
        ~StateContextStandard() override;

        /** @copydoc IStateContext::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IStateContext::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IStateContext::update */
        void update() override;

        /** @copydoc IStateContext::addMessage */
        void addMessage( Thread::Task taskId, SmartPtr<IStateMessage> message ) override;

        /** @copydoc IStateContext::addStateUpdate */
        void setDirty( bool dirty, bool cascade = true ) override;

        /** @copydoc IStateContext::isDirty */
        bool isDirty() const override;

        bool isStateDirty() const;
        void setStateDirty( bool dirty );

        void addQuery( SmartPtr<IStateQuery> query );

        u32 getQueryTask() const;
        void setQueryTask( u32 taskId );

        /** Adds a listener to listen to this objects state. */
        void addStateListener( SmartPtr<IStateListener> stateListner ) override;

        /** Removes a listener. */
        bool removeStateListener( SmartPtr<IStateListener> stateListner ) override;

        /** Gets a list of listeners. */
        SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> getStateListeners() const override;

        /** @copydoc IStateObject::addEventListener */
        void addEventListener( SmartPtr<IEventListener> eventListener ) override;

        /** @copydoc IStateObject::removeEventListener */
        bool removeEventListener( SmartPtr<IEventListener> eventListener ) override;

        /** @copydoc IStateObject::getEventListeners */
        SharedPtr<Array<SmartPtr<IEventListener>>> getEventListeners() const override;

        /** Sets this objects id. */
        void setId( u32 id );

        /** Gets this objects id. */
        u32 getId() const;

        /** Sets the owner. */
        void setOwner( SmartPtr<ISharedObject> owner ) override;

        /** Gets the owner. */
        SmartPtr<ISharedObject> &getOwner() override;
        const SmartPtr<ISharedObject> &getOwner() const override;

        /** Gets the state queue.
        @param taskId The task id.
        @return The state queue.
        */
        SmartPtr<IStateQueue> &getStateQueue( u32 taskId );

        /** Gets the state queue.
        @param taskId The task id.
        @return The state queue.
        */
        const SmartPtr<IStateQueue> &getStateQueue( u32 taskId ) const;

        void sendMessage( SmartPtr<IStateMessage> message ) override;
        void _processStateUpdate( SmartPtr<IState> &state );
        void _processQuery( SmartPtr<IStateQuery> &query );

        /** Add the state object to the state manager.  */
        void add();

        /** Removes the state object from the state manager. */
        void remove();

        /** Returns a boolean indicating whether this object is added to the state manager. */
        bool isAdded() const;

        void addState( SmartPtr<IState> state ) override;
        void removeState( SmartPtr<IState> state );

        SmartPtr<IState> &getStateByTypeId( u32 typeId );
        const SmartPtr<IState> &getStateByTypeId( u32 typeId ) const;

        Array<SmartPtr<IState>> getStates() const;

        SharedPtr<ConcurrentArray<SmartPtr<IState>>> getStatesPtr() const;

        void setStatesPtr( SharedPtr<ConcurrentArray<SmartPtr<IState>>> states );

        bool getUpdateState() const;
        void setUpdateState( bool val );

        /** Gets the data as a properties object.
        @return The data as a properties object.
        */
        SmartPtr<Properties> getProperties() const override;

        /** Sets the data as a properties object.
        @param properties The properties object.
        */
        void setProperties( SmartPtr<Properties> properties ) override;

        /** @copydoc StateObject::triggerEvent */
        Parameter triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

        /** @copydoc ISharedObject::isValid */
        bool isValid() const override;

        FB_CLASS_REGISTER_DECL;

    protected:
        class SharedObjectListener : public IEventListener
        {
        public:
            SharedObjectListener();
            ~SharedObjectListener();

            Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

            SmartPtr<StateContextStandard> getOwner() const;

            void setOwner( SmartPtr<StateContextStandard> owner );

        private:
            WeakPtr<StateContextStandard> m_owner;
        };

        bool isBitSet( u32 flags, s32 bitIdx ) const;

        u32 getDirtyFlags() const;
        void setDirtyFlags( u32 dirtyFlags );
        void setDirtyFlag( u32 flag, bool val );

        void setStateListeners( SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> listeners );
        void setEventListeners( SharedPtr<Array<SmartPtr<IEventListener>>> eventListeners );

        bool getEnableMessageQueues() const;

        SharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> getStateQueuesPtr() const;
        void setStateQueuesPtr( SharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> ptr );

        SmartPtr<IEventListener> m_sharedObjectListener;

        SmartPtr<ISharedObject> m_owner;

        SmartPtr<IState> m_state;

        u32 m_taskId = 0;

        ///
        u32 m_id = 0;

        ///
        bool m_isAdded = false;

        ///
        bool m_enableMessageQueues = true;

        atomic_bool m_isDirty = false;

        atomic_bool m_bUpdateState = false;

        atomic_u32 m_stateChangeCount;
        atomic_u32 m_stateUpdateCount;

        atomic_u32 m_removeCount;

        SharedPtr<Array<SmartPtr<IEventListener>>> m_eventListeners;

        SharedPtr<ConcurrentArray<SmartPtr<IState>>> m_states;

        ///
        AtomicSharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> m_listeners;

        AtomicSharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> m_stateQueues;

        ///
        static u32 m_nextGeneratedNameExt;
    };

    inline SharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> StateContextStandard::getStateQueuesPtr()
        const
    {
        return m_stateQueues;
    }

    inline SharedPtr<ConcurrentArray<SmartPtr<IState>>> StateContextStandard::getStatesPtr() const
    {
        return m_states;
    }

}  // end namespace fb

#endif  // StateObjectStandard_h__
