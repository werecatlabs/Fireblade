#ifndef IPhysicsConstraint3_h__
#define IPhysicsConstraint3_h__

#include <FBCore/Interface/Physics/IPhysicsConstraint.h>
#include <FBCore/Interface/Physics/FBPhysicsTypes.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief Interface for a 3D physics constraint.
         *
         * This interface extends the IPhysicsConstraint interface and adds methods specific to 3D constraints.
         */
        class IPhysicsConstraint3 : public IPhysicsConstraint
        {
        public:
            /**
             * @brief Destructor for IPhysicsConstraint3.
             */
            ~IPhysicsConstraint3() override = default;

            /**
             * @brief Get the first body involved in the constraint.
             *
             * @return SmartPtr<IPhysicsBody3> A smart pointer to the first body involved in the constraint.
             */
            virtual SmartPtr<IPhysicsBody3> getBodyA() const = 0;

            /**
             * @brief Set the first body involved in the constraint.
             *
             * @param bodyA A smart pointer to the first body to involve in the constraint.
             */
            virtual void setBodyA( SmartPtr<IPhysicsBody3> bodyA ) = 0;

            /**
             * @brief Get the second body involved in the constraint.
             *
             * @return SmartPtr<IPhysicsBody3> A smart pointer to the second body involved in the constraint.
             */
            virtual SmartPtr<IPhysicsBody3> getBodyB() const = 0;

            /**
             * @brief Set the second body involved in the constraint.
             *
             * @param bodyB A smart pointer to the second body to involve in the constraint.
             */
            virtual void setBodyB( SmartPtr<IPhysicsBody3> bodyB ) = 0;

            /**
             * @brief Set the local pose of a joint actor.
             *
             * @param actor The actor whose local pose to set.
             * @param localPose The local pose to set.
             */
            virtual void setLocalPose( JointActorIndex::Enum actor,
                                       const Transform3<real_Num> &localPose ) = 0;

            /**
             * @brief Get the local pose of a joint actor.
             *
             * @param actor The actor whose local pose to get.
             * @return Transform3<real_Num> The local pose of the joint actor.
             */
            virtual Transform3<real_Num> getLocalPose( JointActorIndex::Enum actor ) const = 0;

            /**
             * @brief Set a constraint flag.
             *
             * @param flag The flag to set.
             * @param value The value to set the flag to.
             */
            virtual void setConstraintFlag( ConstraintFlag::Enum flag, bool value ) = 0;

            /**
             * @brief Get the constraint flags.
             *
             * @return PxConstraintFlag::Enum The constraint flags.
             */
            virtual ConstraintFlag::Enum getConstraintFlags() const = 0;

            /**
             * @brief Set the force at which the constraint should break.
             *
             * @param force The force at which to break the constraint.
             * @param torque The torque at which to break the constraint.
             */
            virtual void setBreakForce( real_Num force, real_Num torque ) = 0;

            /**
             * @brief Get the force at which the constraint breaks.
             *
             * @param force The force at which the constraint breaks.
             * @param torque The torque at which the constraint breaks.
             */
            virtual void getBreakForce( real_Num &force, real_Num &torque ) const = 0;

            /**
             * @brief Set the linear tolerance for constraint projection.
             *
             * @param tolerance The linear tolerance for constraint
             */
            virtual void setProjectionLinearTolerance( real_Num tolerance ) = 0;

            /**
             * @brief Get the linear tolerance for constraint projection.
             *
             * @return real_Num The linear tolerance for constraint projection.
             */
            virtual real_Num getProjectionLinearTolerance() const = 0;

            /**
             * @brief Set the angular tolerance for constraint projection.
             *
             * @param tolerance The angular tolerance for constraint projection.
             */
            virtual void setProjectionAngularTolerance( real_Num tolerance ) = 0;

            /** @brief Get the angular tolerance for constraint projection.
             */
            virtual real_Num getProjectionAngularTolerance() const = 0;

            /**
             * @brief Set the user data for the constraint.
             *
             * @param userData The user data to set.
             */
            void *getUserData() const override = 0;

            /**
             * @brief Get the user data for the constraint.
             */
            void setUserData( void *userData ) override = 0;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsConstraint3_h__
