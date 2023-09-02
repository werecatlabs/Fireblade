#ifndef __FB_INPUT_EVENT__
#define __FB_INPUT_EVENT__

#include <FBCore/Interface/Input/IInputEvent.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    /** A structure to store input event data. */
    class InputEvent : public IInputEvent
    {
    public:
        InputEvent();
        ~InputEvent() override;

        SmartPtr<IMouseState> getMouseState() const override;
        void setMouseState( SmartPtr<IMouseState> mouseState ) override;

        SmartPtr<IKeyboardState> getKeyboardState() const override;
        void setKeyboardState( SmartPtr<IKeyboardState> keyboardState ) override;

        SmartPtr<IJoystickState> getJoystickState() const override;
        void setJoystickState( SmartPtr<IJoystickState> joystickState ) override;

        SmartPtr<IGameInputState> getGameInputState() const override;
        void setGameInputState( SmartPtr<IGameInputState> gameInputState ) override;

        hash32 getGameInputId() const override;
        void setGameInputId( hash32 gameInputId ) override;

        EventType getEventType() const override;
        void setEventType( EventType eventType ) override;

        void *getUserData() const override;
        void setUserData( void *data ) override;

        void *getWindow() const override;
        void setWindow( void *window ) override;

        f32 getWheel() const override;
        void setWheel( f32 wheel ) override;

        bool isLeftPressed() const override;
        bool isRightPressed() const override;
        bool isMiddlePressed() const override;

        bool isLeftReleased() const override;
        bool isRightReleased() const override;
        bool isMiddleReleased() const override;

        FB_CLASS_REGISTER_DECL;

    protected:
        ///
        SmartPtr<IMouseState> m_mouseState;

        ///
        SmartPtr<IKeyboardState> m_keyboardState;

        ///
        SmartPtr<IJoystickState> m_joystickState;

        ///
        SmartPtr<IGameInputState> m_gameInputState;

        /// The id of the game input that was used to trigger this event.
        hash32 m_gameInputId = 0;

        /// The event type.
        EventType m_eventType = EventType::None;

        /// User data as an unsigned int.
        void *m_userData = nullptr;

        ///
        void *m_window = nullptr;
    };
}  // end namespace fb

#endif
