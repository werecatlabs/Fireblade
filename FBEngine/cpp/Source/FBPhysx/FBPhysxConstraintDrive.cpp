#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxConstraintDrive.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace physics
    {
        real_Num PhysxConstraintDrive::getForceLimit() const
        {
            return 0;
        }

        void PhysxConstraintDrive::setForceLimit( real_Num forceLimit )
        {
        }

        D6JointDriveFlag::Enum PhysxConstraintDrive::getDriveFlags() const
        {
            return static_cast<D6JointDriveFlag::Enum>(0);
        }

        void PhysxConstraintDrive::setDriveFlags( D6JointDriveFlag::Enum driveFlags )
        {
        }

        void PhysxConstraintDrive::setIsAcceleration( bool acceleration ) const
        {
        }

        bool PhysxConstraintDrive::isAcceleration() const
        {
            return false;
        }

        real_Num PhysxConstraintDrive::getStiffness() const
        {
            return 0;
        }

        void PhysxConstraintDrive::setStiffness( real_Num stiffness )
        {
        }

        real_Num PhysxConstraintDrive::getDamping() const
        {
            return 0;
        }

        void PhysxConstraintDrive::setDamping( real_Num damping )
        {
        }
    } // namespace physics
}     // namespace fb
