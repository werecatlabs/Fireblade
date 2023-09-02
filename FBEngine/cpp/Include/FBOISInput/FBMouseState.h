#ifndef FBMouseInput_h__
#define FBMouseInput_h__

#include <FBOISInput/FBOISInputPrerequisites.h>
#include <FBCore/Interface/Input/IMouseState.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/HashMap.h>

namespace fb
{
    class MouseState : public IMouseState
    {
    public:
        MouseState();
        ~MouseState() override;

        Vector2F getRelativeMove() const override;
        void setRelativeMove( const Vector2F &movePosition ) override;

        Vector2F getRelativePosition() const override;
        void setRelativePosition( const Vector2F &position ) override;

        Vector2F getAbsolutePosition() const override;
        void setAbsolutePosition( const Vector2F &position ) override;

        f32 getWheelPosition() const override;
        void setWheelPosition( f32 wheelPosition ) override;

        bool isShiftPressed() const override;
        void setShiftPressed( bool shiftPressed ) override;

        bool isControlPressed() const override;
        void setControlPressed( bool controlPressed ) override;

        bool isButtonPressed( u32 id ) const override;
        void setButtonPressed( u32 id, bool pressed ) override;

        Event getEventType() const override;
        void setEventType( Event eventType ) override;

    protected:
        Vector2F m_movePosition;

        Vector2F m_relativePosition;
        Vector2F m_absolutePosition;
        f32 m_wheelPosition;
        Event m_eventType;
        bool m_isShiftPressed = false;
        bool m_isControlPressed = false;

        using MouseButtonStates = HashMap<u32, bool>;
        MouseButtonStates m_mouseButtonStates;
    };
}  // end namespace fb

#endif  // FBMouseInput_h__
