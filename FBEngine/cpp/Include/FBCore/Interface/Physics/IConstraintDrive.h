#ifndef __IConstraintDrive_h__
#define __IConstraintDrive_h__

#include <FBCore/Interface/Physics/IPhysicsSpring.h>
#include <FBCore/Interface/Physics/FBPhysicsTypes.h>

namespace fb
{
    namespace physics
    {

        /** Interface for a constraint drive. */
        class IConstraintDrive : public IPhysicsSpring
        {
        public:
            /** Destructor. */
            ~IConstraintDrive() override = default;

            /** Gets the force limit. */
            virtual real_Num getForceLimit() const = 0;

            /** Sets the force limit. */
            virtual void setForceLimit( real_Num forceLimit ) = 0;

            /** Gets the flags. */
            virtual D6JointDriveFlag::Enum getDriveFlags() const = 0;

            /** Sets the flags. */
            virtual void setDriveFlags( D6JointDriveFlag::Enum driveFlags ) = 0;

            /** Sets acceleration. */
            virtual void setIsAcceleration( bool acceleration ) const = 0;

            /** Gets acceleration. */
            virtual bool isAcceleration() const = 0;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // __IConstraintDrive_h__
