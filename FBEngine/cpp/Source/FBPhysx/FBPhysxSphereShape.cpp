#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxSphereShape.h>
#include <FBCore/FBCore.h>

namespace fb::physics
{
    PhysxSphereShape::PhysxSphereShape() = default;

    PhysxSphereShape::~PhysxSphereShape() = default;

    void PhysxSphereShape::load( SmartPtr<ISharedObject> data )
    {
    }

    void PhysxSphereShape::unload( SmartPtr<ISharedObject> data )
    {
    }

    void PhysxSphereShape::setRadius( real_Num radius )
    {
    }

    auto PhysxSphereShape::getRadius() const -> real_Num
    {
        return 0.f;
    }
}  // namespace fb::physics
