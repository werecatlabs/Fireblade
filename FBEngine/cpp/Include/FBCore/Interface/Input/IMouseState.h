#ifndef IMouseState_h__
#define IMouseState_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{

    class IMouseState : public ISharedObject
    {
    public:
        static const u32 MOUSE_LEFT_BUTTON;
        static const u32 MOUSE_RIGHT_BUTTON;
        static const u32 MOUSE_MIDDLE_BUTTON;

        //! Enumeration for all mouse input events
        enum class Event
        {
            //! Left mouse button was pressed down.
            LeftPressed = 0,

            //! Right mouse button was pressed down.
            RightPressed,

            //! Middle mouse button was pressed down.
            MiddlePressed,

            //! Left mouse button was left up.
            LeftReleased,

            //! Right mouse button was left up.
            RightReleased,

            //! Middle mouse button was left up.
            MiddleReleased,

            //! The mouse cursor changed its position.
            Moved,

            //! The mouse wheel was moved. Use Wheel value in event data to find out
            //! in what direction and how fast.
            Wheel,

            //! No real event. Just for convenience to get number of events
            Count
        };

        ~IMouseState() override = default;

        virtual Vector2<real_Num> getRelativeMove() const = 0;
        virtual void setRelativeMove( const Vector2<real_Num> &move ) = 0;

        virtual Vector2<real_Num> getRelativePosition() const = 0;
        virtual void setRelativePosition( const Vector2<real_Num> &position ) = 0;

        virtual Vector2<real_Num> getAbsolutePosition() const = 0;
        virtual void setAbsolutePosition( const Vector2<real_Num> &position ) = 0;

        virtual f32 getWheelPosition() const = 0;
        virtual void setWheelPosition( f32 wheelPosition ) = 0;

        virtual bool isShiftPressed() const = 0;
        virtual void setShiftPressed( bool shiftPressed ) = 0;

        virtual bool isControlPressed() const = 0;
        virtual void setControlPressed( bool controlPressed ) = 0;

        virtual bool isButtonPressed( u32 id ) const = 0;
        virtual void setButtonPressed( u32 id, bool isPressed ) = 0;

        virtual Event getEventType() const = 0;
        virtual void setEventType( Event eventType ) = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // IMouseState_h__
