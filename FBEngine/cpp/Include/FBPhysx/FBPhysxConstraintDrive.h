#ifndef FBPhysxConstraintDrive_h__
#define FBPhysxConstraintDrive_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBPhysx/FBPhysxSharedObject.h>
#include <FBCore/Interface/Physics/IConstraintDrive.h>

namespace fb
{
    namespace physics
    {
        
        class PhysxConstraintDrive : public PhysxSharedObject<IConstraintDrive>
        {
        public:
            PhysxConstraintDrive() = default;
            ~PhysxConstraintDrive() override = default;

            real_Num getForceLimit() const override;

            void setForceLimit( real_Num forceLimit ) override;

            PxD6JointDriveFlag::Enum getDriveFlags() const override;

            void setDriveFlags( PxD6JointDriveFlag::Enum driveFlags ) override;

            void setIsAcceleration( bool acceleration ) const override;

            bool isAcceleration() const override;

            real_Num getStiffness() const override;

            void setStiffness( real_Num stiffness ) override;

            real_Num getDamping() const override;

            void setDamping( real_Num damping ) override;

        protected:
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxConstraintDrive_h__
