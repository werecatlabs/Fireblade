#ifndef IPhysicsParticle3_h__
#define IPhysicsParticle3_h__

#include "FBCore/Interface/Physics/IPhysicsBody3.h"
#include "FBCore/Interface/Physics/IPhysicsShape3.h"

namespace fb
{
    namespace physics
    {

        class IPhysicsParticle3 : public IPhysicsBody3
        {
        public:
            ~IPhysicsParticle3() override = default;

            /** */
            virtual SmartPtr<IPhysicsShape3> &getCollisionShape() = 0;

            /** */
            virtual const SmartPtr<IPhysicsShape3> &getCollisionShape() const = 0;

            /** */
            virtual void setCollisionShape( SmartPtr<IPhysicsShape3> shape ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsParticle3_h__
