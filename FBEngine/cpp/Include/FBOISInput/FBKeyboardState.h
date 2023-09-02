#ifndef FBKeyInput_h__
#define FBKeyInput_h__

#include <FBOISInput/FBOISInputPrerequisites.h>
#include <FBCore/Interface/Input/IKeyboardState.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class KeyboardState : public IKeyboardState
    {
    public:
        KeyboardState();
        ~KeyboardState() override;

        u32 getChar() const override;
        void setChar( u32 val ) override;

        u32 getKeyCode() const override;
        void setKeyCode( u32 keyCode ) override;

        u32 getRawKeyCode() const override;
        void setRawKeyCode( u32 rawKeyCode ) override;

        bool isPressedDown() const override;
        void setPressedDown( bool pressedDown ) override;

        bool isShiftPressed() const override;
        void setShiftPressed( bool shiftPressed ) override;

        bool isControlPressed() const override;
        void setControlPressed( bool controlPressed ) override;

    protected:
        u32 m_char;
        u32 m_keyCode;
        u32 m_rawKeyCode;
        bool m_isPressedDown;
        bool m_isShiftPressed;
        bool m_isControlPressed;
    };
}  // end namespace fb

#endif  // FBKeyInput_h__
