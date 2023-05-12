#ifndef StatePhysicsVelocity2_h__
#define StatePhysicsVelocity2_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    //---------------------------------------------
    class StatePhysicsVelocity2 : public StateMessage
    {
    public:
        /// Some commonly used types.
        static const hash_type ADD_VELOCITY_HASH;
        static const hash_type SET_VELOCITY_HASH;

        StatePhysicsVelocity2();
        StatePhysicsVelocity2( const Vector2<real_Num> &velocity );

        Vector2<real_Num> getVelocity() const;
        void setVelocity( const Vector2<real_Num> &val );

        Vector2<real_Num> getRelativePosition() const;
        void setRelativePosition( const Vector2<real_Num> &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector2<real_Num> m_velocity;
        Vector2<real_Num> m_relativePosition;
    };
}  // end namespace fb

#endif  // StatePhysicsVelocity2_h__
