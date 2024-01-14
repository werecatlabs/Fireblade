#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxVehicleWheel3.h>

namespace fb::physics
{
    PhysxVehicleWheel3::PhysxVehicleWheel3() = default;

    PhysxVehicleWheel3::~PhysxVehicleWheel3() = default;

    void PhysxVehicleWheel3::initialise( const Vector3F &pos, f32 radius, f32 width,
                                         f32 suspensionRestLength, f32 suspension_Ks, f32 suspension_Kd,
                                         bool powered, bool steering, bool brakes )
    {
    }

    auto PhysxVehicleWheel3::getPosition() const -> Vector3F
    {
        return Vector3F::zero();
    }

    auto PhysxVehicleWheel3::getOrientation() const -> QuaternionF
    {
        return QuaternionF::identity();
    }

    auto PhysxVehicleWheel3::getVelocity() const -> Vector3F
    {
        return Vector3F::zero();
    }

    void PhysxVehicleWheel3::setMaterialId( u32 materialId )
    {
    }

    auto PhysxVehicleWheel3::getMaterialId() const -> u32
    {
        return 0;
    }

    auto PhysxVehicleWheel3::getAngularVelocity() const -> f32
    {
        return 0;
    }

    auto PhysxVehicleWheel3::getLocalAABB() const -> AABB3F
    {
        return {};
    }

    auto PhysxVehicleWheel3::getWorldAABB() const -> AABB3F
    {
        return {};
    }

}  // namespace fb::physics

// end namespace fb
