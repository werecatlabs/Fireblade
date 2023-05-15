#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxSphereShape.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace physics
    {
        PhysxSphereShape::PhysxSphereShape()
        {
        }

        PhysxSphereShape::~PhysxSphereShape()
        {
        }

        void PhysxSphereShape::load( SmartPtr<ISharedObject> data )
        {
        }

        void PhysxSphereShape::unload( SmartPtr<ISharedObject> data )
        {
        }

        void PhysxSphereShape::setRadius( real_Num radius )
        {
        }

        real_Num PhysxSphereShape::getRadius() const
        {
            return 0.f;
        }
    } // namespace physics
}     // namespace fb
