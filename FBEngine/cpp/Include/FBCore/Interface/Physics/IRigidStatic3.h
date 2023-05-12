#ifndef IPhysicsRigidStatic3_h__
#define IPhysicsRigidStatic3_h__

#include <FBCore/Interface/Physics/IRigidBody3.h>

namespace fb
{
    namespace physics
    {

        class IRigidStatic3 : public IRigidBody3
        {
        public:
            ~IRigidStatic3() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsRigidStatic3_h__
