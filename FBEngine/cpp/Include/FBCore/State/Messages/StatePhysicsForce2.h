#ifndef StatePhysicsForce2_h__
#define StatePhysicsForce2_h__

#include <FBCore/State/Messages/StateMessage.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    //---------------------------------------------
    class StatePhysicsForce2 : public StateMessage
    {
    public:
        /// Some commonly used types.
        static const hash_type ADD_FORCE_HASH;
        static const hash_type SET_FORCE_HASH;

        StatePhysicsForce2();

        Vector2<real_Num> getForce() const;
        void setForce( const Vector2<real_Num> &val );

        Vector2<real_Num> getRelativePosition() const;
        void setRelativePosition( const Vector2<real_Num> &val );

        FB_CLASS_REGISTER_DECL;

    protected:
        Vector2<real_Num> m_force;
        Vector2<real_Num> m_relativePosition;
    };
}  // end namespace fb

#endif  // StatePhysicsForce2_h__
