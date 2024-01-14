#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxConstraintDrive.h>
#include <FBCore/FBCore.h>

namespace fb::physics
{
    auto PhysxConstraintDrive::getForceLimit() const -> real_Num
    {
        return 0;
    }

    void PhysxConstraintDrive::setForceLimit( real_Num forceLimit )
    {
    }

    auto PhysxConstraintDrive::getDriveFlags() const -> D6JointDriveFlag::Enum
    {
        return static_cast<D6JointDriveFlag::Enum>( 0 );
    }

    void PhysxConstraintDrive::setDriveFlags( D6JointDriveFlag::Enum driveFlags )
    {
    }

    void PhysxConstraintDrive::setIsAcceleration( bool acceleration ) const
    {
    }

    auto PhysxConstraintDrive::isAcceleration() const -> bool
    {
        return false;
    }

    auto PhysxConstraintDrive::getStiffness() const -> real_Num
    {
        return 0;
    }

    void PhysxConstraintDrive::setStiffness( real_Num stiffness )
    {
    }

    auto PhysxConstraintDrive::getDamping() const -> real_Num
    {
        return 0;
    }

    void PhysxConstraintDrive::setDamping( real_Num damping )
    {
    }
}  // namespace fb::physics
