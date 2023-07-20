#ifndef IInputEvent_h__
#define IInputEvent_h__

#include <FBCore/Interface/System/IEvent.h>

namespace fb
{

    /** Interface for an input event. */
    class IInputEvent : public IEvent
    {
    public:
        /** Event types. */
        enum class EventType
        {
            None,      ///< A none input event.
            Mouse,     ///< A mouse input event.
            Key,       ///< A key input event.
            Joystick,  ///< A joystick input event.
            User,      ///< A user event with user data.
            Count      ///< The number of event types.
        };

        /** Input types. */
        enum class InputType
        {
            Reset,         ///< Reset the input state.
            MouseMoved,    ///< The mouse has moved.
            MousePressed,  ///< A mouse button has been pressed.
            MouseRelease,  ///< A mouse button has been released.
            KeyUp,         ///< A key has been released.
            KeyDown,       ///< A key has been pressed.
            Count          ///< The number of input types.
        };

        /** Destructor. */
        ~IInputEvent() override = default;

        /** Gets the current mouse state. */
        virtual SmartPtr<IMouseState> getMouseState() const = 0;

        /** Sets the current mouse state. */
        virtual void setMouseState( SmartPtr<IMouseState> mouseState ) = 0;

        /** Gets the current keyboard state. */
        virtual SmartPtr<IKeyboardState> getKeyboardState() const = 0;

        /** Sets the current keyboard state. */
        virtual void setKeyboardState( SmartPtr<IKeyboardState> keyboardState ) = 0;

        /** Gets the current joystick state. */
        virtual SmartPtr<IJoystickState> getJoystickState() const = 0;

        /** Sets the current joystick state. */
        virtual void setJoystickState( SmartPtr<IJoystickState> joystickState ) = 0;

        /** Gets the current game input state. */
        virtual SmartPtr<IGameInputState> getGameInputState() const = 0;

        /** Sets the current game input state. */
        virtual void setGameInputState( SmartPtr<IGameInputState> gameInputState ) = 0;

        /** Gets the ID of the current game input state. */
        virtual hash32 getGameInputId() const = 0;

        /** Sets the ID of the current game input state. */
        virtual void setGameInputId( hash32 gameInputId ) = 0;

        /** Gets the type of the event. */
        virtual EventType getEventType() const = 0;

        /** Sets the type of the event. */
        virtual void setEventType( EventType eventType ) = 0;

        /** Gets a pointer to the window. */
        virtual void *getWindow() const = 0;

        /** Sets a pointer to the window. */
        virtual void setWindow( void *window ) = 0;

        /** Gets the relative movement of the mouse wheel. */
        virtual f32 getWheel() const = 0;

        /** Sets the relative movement of the mouse wheel. */
        virtual void setWheel( f32 wheel ) = 0;

        /** Checks if the left mouse button is currently pressed. */
        virtual bool isLeftPressed() const = 0;

        /** Checks if the right mouse button is currently pressed. */
        virtual bool isRightPressed() const = 0;

        /** Checks if the middle mouse button is currently pressed. */
        virtual bool isMiddlePressed() const = 0;

        /** Checks if the left mouse button is currently released. */
        virtual bool isLeftReleased() const = 0;

        /** Checks if the right mouse button is currently released. */
        virtual bool isRightReleased() const = 0;

        /** Checks if the middle mouse button is currently released. */
        virtual bool isMiddleReleased() const = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IInputEvent_h__
