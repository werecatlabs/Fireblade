#ifndef __IFSMListener_h__
#define __IFSMListener_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/FSM/IFSM.h>

namespace fb
{

    /** Interface for a listener of a state machine. */
    class IFSMListener : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IFSMListener() override = default;

        /**
         * Handles a state machine event.
         * @param state The current state of the state machine.
         * @param eventType The type of event that occurred.
         * @return The return value of the state machine action, indicating whether the event was accepted, rejected, or ignored.
         */
        virtual IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) = 0;

        /** Gets the state machine associated with the listener. */
        virtual SmartPtr<IFSM> getFSM() const = 0;

        /** Sets the state machine associated with the listener. */
        virtual void setFSM( SmartPtr<IFSM> fsm ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif  // __IFSMListener_h__
