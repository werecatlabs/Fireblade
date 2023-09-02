#ifndef FBPhysxConstraintD6_h__
#define FBPhysxConstraintD6_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBPhysx/FBPhysxConstraint.h>
#include <FBCore/Interface/Physics/IConstraintD6.h>

namespace fb
{
    namespace physics
    {
        
        class PhysxConstraintD6 : public PhysxConstraint<IConstraintD6>
        {
        public:
            PhysxConstraintD6() = default;
            ~PhysxConstraintD6() override;

            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            void setDrivePosition( const Transform3<real_Num> &pose ) override;
            Transform3<real_Num> getDrivePosition() const override;

            RawPtr<physx::PxD6Joint> getJoint() const;
            void setJoint( RawPtr<physx::PxD6Joint> joint );

            void setDrive( D6Drive::Enum index, SmartPtr<IConstraintDrive> drive ) override;
            SmartPtr<IConstraintDrive> getDrive( D6Drive::Enum index ) const override;

            void setLinearLimit( SmartPtr<IConstraintLinearLimit> limit ) override;
            SmartPtr<IConstraintLinearLimit> getLinearLimit() const override;

            void setMotion( D6Axis::Enum axis, D6Motion::Enum type ) override;
            D6Motion::Enum getMotion( D6Axis::Enum axis ) const override;

        private:
            physx::PxD6Joint *m_joint = nullptr;
            Transform3<real_Num> m_drivePosition;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxConstraintD6_h__
