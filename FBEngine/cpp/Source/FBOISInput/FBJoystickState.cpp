#include <FBOISInput/FBJoystickState.h>
#include <FBCore/FBCore.h>

namespace fb
{
    JoystickState::JoystickState() :
        m_joystick( 0 ),
        m_buttonId( 0 ),
        m_eventType( 0 ),
        m_pov( 0 ),
        m_isPressed( false ),
        m_isButtonPressed( false )
    {
    }

    JoystickState::~JoystickState()
    {
    }

    u32 JoystickState::getJoystick() const
    {
        return m_joystick;
    }

    void JoystickState::setJoystick( u32 id )
    {
        m_joystick = id;
    }

    u16 JoystickState::getPOV() const
    {
        return m_pov;
    }

    void JoystickState::setPOV( u32 position )
    {
        m_pov = position;
    }

    f32 JoystickState::getAxis( u32 axisIndex ) const
    {
        return m_axis[axisIndex];
    }

    void JoystickState::setAxis( u32 axisIndex, f32 position )
    {
        m_axis[axisIndex] = position;
    }

    u32 JoystickState::getButtonId() const
    {
        return m_buttonId;
    }

    void JoystickState::setButtonId( u32 buttonId )
    {
        m_buttonId = buttonId;
    }

    bool JoystickState::isPressedDown() const
    {
        return m_isPressed;
    }

    void JoystickState::setPressedDown( bool pressed )
    {
        m_isPressed = pressed;
    }

    bool JoystickState::isButtonPressed( u32 id ) const
    {
        return false;
    }

    void JoystickState::setButtonPressed( u32 id, bool isPressed )
    {
    }

    u32 JoystickState::getEventType() const
    {
        return m_eventType;
    }

    void JoystickState::setEventType( u32 eventType )
    {
        m_eventType = eventType;
    }
}  // end namespace fb
