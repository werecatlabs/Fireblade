#ifndef StateObject_h__
#define StateObject_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/IStateContext.h>

namespace fb
{

    /** StateObject is a base class for state objects. */
    class StateContext : public IStateContext
    {
    public:
        StateContext();
        ~StateContext() override;

        void setOwner( SmartPtr<ISharedObject> owner ) override;

        SmartPtr<ISharedObject> &getOwner() override;
        const SmartPtr<ISharedObject> &getOwner() const override;

        bool isDirty() const override;

        void setDirty( bool dirty, bool cascade = true ) override;

        void addMessage( Thread::Task taskId, SmartPtr<IStateMessage> message ) override;

        void addStateListener( SmartPtr<IStateListener> stateListner ) override;

        bool removeStateListener( SmartPtr<IStateListener> stateListner ) override;

        SharedPtr<ConcurrentArray<SmartPtr<IStateListener>>> getStateListeners() const override;

        /** @copydoc IStateObject::addEventListener */
        void addEventListener( SmartPtr<IEventListener> eventListner ) override;

        /** @copydoc IStateObject::addEventListener */
        bool removeEventListener( SmartPtr<IEventListener> eventListner ) override;

        /** @copydoc IStateObject::addEventListener */
        SharedPtr<Array<SmartPtr<IEventListener>>> getEventListeners() const override;

        SmartPtr<IState> getState() const;

        void addState( SmartPtr<IState> state ) override;
        void removeState( SmartPtr<IState> state );

        SmartPtr<IState> &getStateByTypeId( u32 typeId ) override;
        const SmartPtr<IState> &getStateByTypeId( u32 typeId ) const override;

        Array<SmartPtr<IState>> getStates() const override;

        SharedPtr<ConcurrentArray<SmartPtr<IState>>> getStatesPtr() const override;

        void setStatesPtr( SharedPtr<ConcurrentArray<SmartPtr<IState>>> states ) override;

        void sendMessage( SmartPtr<IStateMessage> message ) override;

        SmartPtr<Properties> getProperties() const override;

        void setProperties( SmartPtr<Properties> properties ) override;

        /** @copydoc StateObject::triggerEvent */
        Parameter triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

    protected:
        SmartPtr<ISharedObject> m_owner;
        SmartPtr<IState> m_state;
        SharedPtr<ConcurrentArray<SmartPtr<IState>>> m_states;
        SharedPtr<Array<SmartPtr<IEventListener>>> m_eventListeners;
    };
}  // end namespace fb

#endif  // StateObject_h__
