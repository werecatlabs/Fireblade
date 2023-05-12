#ifndef StatePhysicsDynamicState2_h__
#define StatePhysicsDynamicState2_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    //---------------------------------------------
    class StatePhysicsDynamicState2 : public StateMessage
    {
    public:
        StatePhysicsDynamicState2() = default;
        ~StatePhysicsDynamicState2() override = default;

        Vector2<real_Num> getPosition() const;
        void setPosition( const Vector2<real_Num> &val );

        Vector2<real_Num> getVelocity() const;
        void setVelocity( const Vector2<real_Num> &val );

        Vector2<real_Num> getForce() const;
        void setForce( const Vector2<real_Num> &val );

        real_Num getAngularVelocity() const;
        void setAngularVelocity( real_Num val );

        real_Num getOrientation() const;
        void setOrientation( real_Num val );

        FB_CLASS_REGISTER_DECL;

    protected:
        ///
        Vector2<real_Num> m_position;

        ///
        Vector2<real_Num> m_velocity;

        ///
        Vector2<real_Num> m_force;

        ///
        real_Num m_angularVelocity = static_cast<real_Num>( 0.0 );

        ///
        real_Num m_orientation = static_cast<real_Num>( 0.0 );
    };
}  // end namespace fb

#endif  // StatePhysicsDynamicState2_h__
