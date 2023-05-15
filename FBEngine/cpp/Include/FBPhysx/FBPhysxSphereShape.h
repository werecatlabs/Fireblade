#ifndef FBPhysxSphereShape_h__
#define FBPhysxSphereShape_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/ISphereShape3.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBPhysx/FBPhysxShape.h>
#include <geometry/PxBoxGeometry.h>

namespace fb
{
    namespace physics
    {

        /** Implementation of a box shape. */
        class PhysxSphereShape : public PhysxShape<ISphereShape3>
        {
        public:
            PhysxSphereShape();
            ~PhysxSphereShape() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void setRadius( real_Num radius );
            real_Num getRadius() const;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // FBPhysxSphereShape_h__
