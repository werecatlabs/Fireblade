#ifndef StateObjectStandard_h__
#define StateObjectStandard_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/System/StateQueueStandard.h>
#include <FBCore/System/Job.h>
#include <FBCore/Core/ConcurrentArray.h>

namespace fb
{

    /** Standard implementation of the IStateObject interface. */
    class StateContextStandard : public CSharedObject<IStateContext>
    {
    public:
        /** Default constructor. */
        StateContextStandard();

        /** Constructor. */
        StateContextStandard( u32 id );

        /** Destructor. */
        ~StateContextStandard() override;

        /** @copydoc IStateObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IStateObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IStateObject::update */
        void update() override;

        /** @copydoc IStateObject::addMessage */
        void addMessage( Thread::Task taskId, SmartPtr<IStateMessage> message ) override;

        /** @copydoc IStateObject::addStateUpdate */
        void setDirty( bool dirty, bool cascade = true ) override;

        /** @copydoc IStateObject::isDirty */
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
        SmartPtr<ISharedObject> getOwner() const override;

        /** Gets the state proxy object. */
        SmartPtr<IStateQueue> getStateQueue( u32 taskId ) const;

        void sendMessage( SmartPtr<IStateMessage> message ) override;
        void _processStateUpdate( SmartPtr<IState> &state );
        void _processQuery( SmartPtr<IStateQuery> &query );

        /** Add the state object to the state manager.  */
        void add();

        /** Removes the state object from the state manager. */
        void remove();

        /** Returns a boolean indicating whether this object is added to the state manager. */
        bool isAdded() const;

        /** @copydoc IStateObject::getLatestOutputState */
        SmartPtr<IState> getLatestOutputState() const override;

        /** @copydoc IStateObject::getOutputState */
        SmartPtr<IState> getOutputState( time_interval time ) const override;

        /** @copydoc IStateObject::addOutputState */
        void addOutputState( SmartPtr<IState> state ) override;

        SmartPtr<IState> getState() const override;
        void setState( SmartPtr<IState> state ) override;

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
        bool isBitSet( u32 flags, s32 bitIdx ) const;

        u32 getDirtyFlags() const;
        void setDirtyFlags( u32 dirtyFlags );
        void setDirtyFlag( u32 flag, bool val );

        void setStateListeners( SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> listeners );
        void setEventListeners( SharedPtr<Array<SmartPtr<IEventListener>>> eventListeners );

        bool getEnableMessageQueues() const;

        SharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> getStateQueuesPtr() const;
        void setStateQueuesPtr( SharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> ptr );

        AtomicWeakPtr<ISharedObject> m_owner;

        AtomicSmartPtr<IState> m_state;

        Array<SmartPtr<IState>> m_states;

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

        ///
        AtomicSharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> m_listeners;

        AtomicSharedPtr<ConcurrentArray<SmartPtr<IStateQueue>>> m_stateQueues;

        ///
        static u32 m_nextGeneratedNameExt;
    };
} // end namespace fb

#endif  // StateObjectStandard_h__
