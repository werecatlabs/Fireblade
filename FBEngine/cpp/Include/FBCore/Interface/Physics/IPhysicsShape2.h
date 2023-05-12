#ifndef __IPhysicsCollisionShape2__H
#define __IPhysicsCollisionShape2__H

#include "FBCore/Interface/Physics/IPhysicsShape.h"
#include "FBCore/Interface/Physics/IMassData2.h"
#include "FBCore/Math/Sphere2.h"
#include "FBCore/Math/AABB2.h"

namespace fb
{
    namespace physics
    {
        //-------------------------------------------------
        class IPhysicsShape2 : public IPhysicsShape
        {
        public:
            ~IPhysicsShape2() override = default;

            virtual const Sphere2<real_Num> &getSphere() const = 0;
            virtual const AABB2<real_Num> &getAABB() const = 0;

            virtual void getPoints( Array<Vector2<real_Num>> &points ) const = 0;

            /// Compute the mass properties of this shape using its dimensions and density.
            /// The inertia tensor is computed about the local origin.
            /// @param massData returns the mass data for this shape.
            /// @param density the density in kilograms per meter squared.
            virtual void computeMass( SmartPtr<IMassData2> massData, real_Num density ) const = 0;

            /** Gets a pointer to the underlying object. This is dependent on the physics library used.
             */
            virtual void _getObject( void **ppObject ) const = 0;

            /** Gets the type of shape. */
            virtual u8 getType() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace physics
}  // end namespace fb

#endif
