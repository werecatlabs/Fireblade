#ifndef IKeyboardState_h__
#define IKeyboardState_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /** Interface for keyboard state.
     */
    class IKeyboardState : public ISharedObject
    {
    public:
        /** Virtual destructor. */
        ~IKeyboardState() override = default;

        /** Gets the ASCII character code corresponding to the key that was pressed. */
        virtual u32 getChar() const = 0;

        /** Sets the ASCII character code corresponding to the key that was pressed. */
        virtual void setChar( u32 c ) = 0;

        /** Gets the key code corresponding to the key that was pressed. */
        virtual u32 getKeyCode() const = 0;

        /** Sets the key code corresponding to the key that was pressed. */
        virtual void setKeyCode( u32 keyCode ) = 0;

        /** Gets the raw key code corresponding to the key that was pressed. */
        virtual u32 getRawKeyCode() const = 0;

        /** Sets the raw key code corresponding to the key that was pressed. */
        virtual void setRawKeyCode( u32 rawKeyCode ) = 0;

        /** Gets a boolean indicating whether the key is pressed down. */
        virtual bool isPressedDown() const = 0;

        /** Sets a boolean indicating whether the key is pressed down. */
        virtual void setPressedDown( bool pressedDown ) = 0;

        /** Gets a boolean indicating whether the shift key is pressed. */
        virtual bool isShiftPressed() const = 0;

        /** Sets a boolean indicating whether the shift key is pressed. */
        virtual void setShiftPressed( bool shiftPressed ) = 0;

        /** Gets a boolean indicating whether the control key is pressed. */
        virtual bool isControlPressed() const = 0;

        /** Sets a boolean indicating whether the control key is pressed. */
        virtual void setControlPressed( bool controlPressed ) = 0;
    };

}  // end namespace fb

#endif  // IKeyboardState_h__
