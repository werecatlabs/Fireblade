#ifndef __IConstraintD6_h__
#define __IConstraintD6_h__

#include <FBCore/Interface/Physics/IPhysicsConstraint3.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief Interface for a 6-DOF constraint.
         *
         * This interface extends the IPhysicsConstraint3 interface with additional methods for configuring
         * 6-DOF constraints, including setting the drive position and motion, and setting linear limits.
         */
        class IConstraintD6 : public IPhysicsConstraint3
        {
        public:
            ~IConstraintD6() override = default;

            /**
             * @brief Sets the drive position for the constraint.
             *
             * @param pose The desired drive position.
             */
            virtual void setDrivePosition( const Transform3<real_Num> &pose ) = 0;

            /**
             * @brief Gets the drive position for the constraint.
             *
             * @return The current drive position.
             */
            virtual Transform3<real_Num> getDrivePosition() const = 0;

            /**
             * @brief Sets the drive for the specified index.
             *
             * @param index The index of the drive to set.
             * @param drive The drive to set.
             */
            virtual void setDrive( D6Drive::Enum index, SmartPtr<IConstraintDrive> drive ) = 0;

            /**
             * @brief Gets the drive for the specified index.
             *
             * @param index The index of the drive to get.
             * @return The drive for the specified index.
             */
            virtual SmartPtr<IConstraintDrive> getDrive( D6Drive::Enum index ) const = 0;

            /**
             * @brief Sets the linear limit for the constraint.
             *
             * @param limit The linear limit to set.
             */
            virtual void setLinearLimit( SmartPtr<IConstraintLinearLimit> limit ) = 0;

            /**
             * @brief Gets the linear limit for the constraint.
             *
             * @return The current linear limit.
             */
            virtual SmartPtr<IConstraintLinearLimit> getLinearLimit() const = 0;

            /**
             * @brief Sets the motion for the specified axis.
             *
             * @param axis The axis to set the motion for.
             * @param type The motion type to set.
             */
            virtual void setMotion( D6Axis::Enum axis, D6Motion::Enum type ) = 0;

            /**
             * @brief Gets the motion type for the specified axis.
             *
             * @param axis The axis to get the motion type for.
             * @return The motion type for the specified axis.
             */
            virtual D6Motion::Enum getMotion( D6Axis::Enum axis ) const = 0;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IConstraintD6_h__
