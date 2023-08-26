#ifndef IFSMManager_h__
#define IFSMManager_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for a state machine manager. Provides FSM functionality and the corresponding events. */
    class IFSMManager : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IFSMManager() override = default;

        /** Creates a new FSM. */
        virtual SmartPtr<IFSM> createFSM() = 0;

        /** Destroys an FSM. */
        virtual void destroyFSM( SmartPtr<IFSM> fsm ) = 0;

        /** Gets the time of the state change for a specific FSM state.
        @param id The ID of the FSM state.
        @return A value containing the time of the state change.
        */
        virtual time_interval getStateTime( u32 id ) const = 0;

        /** Sets the time of the state change for a specific FSM state.
        @param id The ID of the FSM state.
        @param stateTime The time of the state change.
        */
        virtual void setStateTime( u32 id, time_interval stateTime ) = 0;

        /** Gets the time of the last state change for a specific FSM state.
        @param id The ID of the FSM state.
        @return A value containing the time of the last state change.
        */
        virtual f64 getStateChangeTime( u32 id ) const = 0;

        /** Sets the time of the last state change for a specific FSM state.
        @param id The ID of the FSM state.
        @param stateChangeTime The time of the last state change.
        */
        virtual void setStateChangeTime( u32 id, const f64 &stateChangeTime ) = 0;

        /** Gets the previous state for a specific FSM state.
        @param id The ID of the FSM state.
        @return Returns an integer containing the previous state.
        */
        virtual u8 getPreviousState( u32 id ) const = 0;

        /** Gets the current state for a specific FSM state.
        @param id The ID of the FSM state.
        @return Returns an integer containing the current state.
        */
        virtual u8 getCurrentState( u32 id ) const = 0;

        /** Gets the new state for a specific FSM state.
        @param id The ID of the FSM state.
        @return Returns an integer containing the new state.
        */
        virtual u8 getNewState( u32 id ) const = 0;

        /** Sets the new state for a specific FSM state.
        @param id The ID of the FSM state.
        @param state An integer value containing the new state.
        @param changeNow Used to the know if the state should be change immediately.
        */
        virtual void setNewState( u32 id, s32 state, bool changeNow = false ) = 0;

        /** Adds a listener to a specific FSM state.
        @param id The ID of the FSM state.
        @param val The listener to add.
        */
        virtual void addListener( u32 id, SmartPtr<IFSMListener> val ) = 0;

        /** Removes a listener from a specific FSM state.
        @param id The ID of the FSM state.
        @param val The listener to remove.
        */
        virtual void removeListener( u32 id, SmartPtr<IFSMListener> val ) = 0;

        /** Removes all listeners from a specific FSM state.
        @param id The ID of the FSM state.
        */
        virtual void removeListeners( u32 id ) = 0;

        /** Gets the listener priority of a specific id.
            @param id The id of the state machine.
            @return An integer value containing the priority.
         */
        virtual u32 getListenerPriority( u32 id ) = 0;

        /** Sets the listener priority of a specific id.
            @param id The id of the state machine.
            @param priority The priority value to set.
         */
        virtual void setListenerPriority( u32 id, u32 priority ) = 0;

        virtual u32 *getFlagsPtr( u32 id ) const = 0;

        /** Gets an array of listeners for a specific id.
            @param id The id of the state machine.
            @return A shared pointer to an array of listeners for the state machine.
         */
        virtual SharedPtr<Array<SmartPtr<IFSMListener>>> getListeners( u32 id ) const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IFSMManager_h__
