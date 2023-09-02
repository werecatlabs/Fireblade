#ifndef FBJoystickState_h__
#define FBJoystickState_h__

#include <FBOISInput/FBOISInputPrerequisites.h>
#include <FBCore/Interface/Input/IJoystickState.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class JoystickState : public IJoystickState
    {
    public:
        JoystickState();
        ~JoystickState() override;

        u32 getJoystick() const override;
        void setJoystick( u32 id ) override;

        u16 getPOV() const override;
        void setPOV( u32 position ) override;

        f32 getAxis( u32 axisIndex ) const override;
        void setAxis( u32 axisIndex, f32 position ) override;

        u32 getButtonId() const override;
        void setButtonId( u32 buttonId ) override;

        bool isPressedDown() const override;
        void setPressedDown( bool pressed ) override;

        bool isButtonPressed( u32 id ) const override;
        void setButtonPressed( u32 id, bool isPressed ) override;

        u32 getEventType() const override;
        void setEventType( u32 eventType ) override;

        FB_CLASS_REGISTER_DECL;

    protected:
        u32 m_joystick;
        u32 m_buttonId;
        u32 m_eventType;
        u16 m_pov;
        bool m_isPressed;
        bool m_isButtonPressed;
        f32 m_axis[static_cast<int>( Axis::NUMBER_OF_AXES )];
    };
}  // end namespace fb

#endif  // FBJoystickState_h__
