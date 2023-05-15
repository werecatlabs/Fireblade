#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxVehicleWheel3.h>

namespace fb
{
    namespace physics
    {
        PhysxVehicleWheel3::PhysxVehicleWheel3()
        {
        }

        PhysxVehicleWheel3::~PhysxVehicleWheel3()
        {
        }

        void PhysxVehicleWheel3::initialise( const Vector3F &pos, f32 radius, f32 width,
                                             f32 suspensionRestLength, f32 suspension_Ks,
                                             f32 suspension_Kd, bool powered, bool steering,
                                             bool brakes )
        {
        }

        Vector3F PhysxVehicleWheel3::getPosition() const
        {
            return Vector3F::zero();
        }

        QuaternionF PhysxVehicleWheel3::getOrientation() const
        {
            return QuaternionF::identity();
        }

        Vector3F PhysxVehicleWheel3::getVelocity() const
        {
            return Vector3F::zero();
        }

        void PhysxVehicleWheel3::setMaterialId( u32 materialId )
        {
        }

        u32 PhysxVehicleWheel3::getMaterialId() const
        {
            return 0;
        }

        f32 PhysxVehicleWheel3::getAngularVelocity() const
        {
            return 0;
        }

        AABB3F PhysxVehicleWheel3::getLocalAABB() const
        {
            return AABB3F();
        }

        AABB3F PhysxVehicleWheel3::getWorldAABB() const
        {
            return AABB3F();
        }

    }  // namespace physics
}  // namespace fb

// end namespace fb
