#ifndef __IPhysicsParticle2__H
#define __IPhysicsParticle2__H

#include "FBCore/Interface/Physics/IPhysicsBody2D.h"
#include "FBCore/Interface/Physics/IPhysicsShape2.h"
#include "FBCore/Math/AABB2.h"
#include "FBCore/Math/Vector2.h"
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Core/StringTypes.h"

namespace fb
{
    namespace physics
    {
        //-------------------------------------------------
        /** An interface for a 2d particle.
         */
        class IPhysicsParticle2 : public IPhysicsBody2D
        {
        public:
            ~IPhysicsParticle2() override = default;

            /** */
            virtual void setCollisionShape( SmartPtr<IPhysicsShape2> shape ) = 0;

            /** */
            virtual const SmartPtr<IPhysicsShape2> &getCollisionShape() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
