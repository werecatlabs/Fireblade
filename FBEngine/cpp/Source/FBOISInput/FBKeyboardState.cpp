#include <FBOISInput/FBKeyboardState.h>
#include <FBCore/FBCore.h>

namespace fb
{
    KeyboardState::KeyboardState() :
        m_char( 0 ),
        m_keyCode( 0 ),
        m_rawKeyCode( 0 ),
        m_isPressedDown( false ),
        m_isShiftPressed( false ),
        m_isControlPressed( false )
    {
    }

    KeyboardState::~KeyboardState()
    {
    }

    u32 KeyboardState::getChar() const
    {
        return m_char;
    }

    void KeyboardState::setChar( u32 c )
    {
        m_char = c;
    }

    u32 KeyboardState::getKeyCode() const
    {
        return m_keyCode;
    }

    void KeyboardState::setKeyCode( u32 keyCode )
    {
        m_keyCode = keyCode;
    }

    u32 KeyboardState::getRawKeyCode() const
    {
        return m_rawKeyCode;
    }

    void KeyboardState::setRawKeyCode( u32 rawKeyCode )
    {
        m_rawKeyCode = rawKeyCode;
    }

    bool KeyboardState::isPressedDown() const
    {
        return m_isPressedDown;
    }

    void KeyboardState::setPressedDown( bool pressedDown )
    {
        m_isPressedDown = pressedDown;
    }

    bool KeyboardState::isShiftPressed() const
    {
        return m_isShiftPressed;
    }

    void KeyboardState::setShiftPressed( bool shiftPressed )
    {
        m_isShiftPressed = shiftPressed;
    }

    bool KeyboardState::isControlPressed() const
    {
        return m_isControlPressed;
    }

    void KeyboardState::setControlPressed( bool controlPressed )
    {
        m_isControlPressed = controlPressed;
    }
}  // end namespace fb
