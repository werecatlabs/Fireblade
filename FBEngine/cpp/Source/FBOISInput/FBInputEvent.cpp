#include <FBOISInput/FBInputEvent.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, InputEvent, IInputEvent );

    InputEvent::InputEvent() : m_gameInputId( 0 ), m_userData( nullptr ), m_window( nullptr )
    {
    }

    InputEvent::~InputEvent()
    {
    }

    SmartPtr<IMouseState> InputEvent::getMouseState() const
    {
        return m_mouseState;
    }

    void InputEvent::setMouseState( SmartPtr<IMouseState> mouseState )
    {
        m_mouseState = mouseState;
    }

    SmartPtr<IKeyboardState> InputEvent::getKeyboardState() const
    {
        return m_keyboardState;
    }

    void InputEvent::setKeyboardState( SmartPtr<IKeyboardState> keyboardState )
    {
        m_keyboardState = keyboardState;
    }

    SmartPtr<IJoystickState> InputEvent::getJoystickState() const
    {
        return m_joystickState;
    }

    void InputEvent::setJoystickState( SmartPtr<IJoystickState> joystickState )
    {
        m_joystickState = joystickState;
    }

    SmartPtr<IGameInputState> InputEvent::getGameInputState() const
    {
        return m_gameInputState;
    }

    void InputEvent::setGameInputState( SmartPtr<IGameInputState> gameInputState )
    {
        m_gameInputState = gameInputState;
    }

    hash32 InputEvent::getGameInputId() const
    {
        return m_gameInputId;
    }

    void InputEvent::setGameInputId( hash32 gameInputId )
    {
        m_gameInputId = gameInputId;
    }

    IInputEvent::EventType InputEvent::getEventType() const
    {
        return m_eventType;
    }

    void InputEvent::setEventType( EventType eventType )
    {
        m_eventType = eventType;
    }

    void *InputEvent::getUserData() const
    {
        return m_userData;
    }

    void InputEvent::setUserData( void *data )
    {
        m_userData = data;
    }

    void *InputEvent::getWindow() const
    {
        return m_window;
    }

    void InputEvent::setWindow( void *window )
    {
        m_window = window;
    }

    f32 InputEvent::getWheel() const
    {
        return 0.0f;
    }

    void InputEvent::setWheel( f32 val )
    {
    }

    bool InputEvent::isLeftPressed() const
    {
        return false;
    }

    bool InputEvent::isRightPressed() const
    {
        return false;
    }

    bool InputEvent::isMiddlePressed() const
    {
        return false;
    }

    bool InputEvent::isLeftReleased() const
    {
        return false;
    }

    bool InputEvent::isRightReleased() const
    {
        return false;
    }

    bool InputEvent::isMiddleReleased() const
    {
        return false;
    }
}  // end namespace fb
