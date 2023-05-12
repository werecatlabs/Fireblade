#ifndef PhysicsBodyState_h__
#define PhysicsBodyState_h__

#include <FBCore/State/States/BaseState.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Base/FixedArray.h>

namespace fb
{
    class PhysicsBodyState : public BaseState
    {
    public:
        PhysicsBodyState();
        ~PhysicsBodyState() override;

        Vector3<real_Num> position;
        Quaternion<real_Num> orientation;

        Vector3<real_Num> linearVelocity;
        Vector3<real_Num> angularVelocity;

        real_Num mass = static_cast<real_Num>( 1.0 );

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif  // RigidbodyState_h__
