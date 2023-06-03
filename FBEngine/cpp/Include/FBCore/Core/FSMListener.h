#ifndef __FiniteStateMachineListener_h__
#define __FiniteStateMachineListener_h__

#include <FBCore/Interface/FSM/IFSMListener.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    class FSMListener : public SharedObject<IFSMListener>
    {
    public:
        FSMListener() = default;
        ~FSMListener() override;

        /** @copydoc IObject::load */
        void load( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IFSMListener::handleEvent */
        IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

        /** @copydoc IFSMListener::setFSM */
        SmartPtr<IFSM> getFSM() const override;

        /** @copydoc IFSMListener::setFSM */
        void setFSM( SmartPtr<IFSM> fsm ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        AtomicWeakPtr<IFSM> m_fsm;
    };
}  // end namespace fb

#endif  // FiniteStateMachineListener_h__
