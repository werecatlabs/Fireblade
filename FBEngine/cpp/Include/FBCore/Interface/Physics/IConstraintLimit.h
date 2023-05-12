#ifndef IConstraintLimit_h__
#define IConstraintLimit_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief Interface for constraint limits used in physics simulation.
         */
        class IConstraintLimit : public ISharedObject
        {
        public:
            /**
             * @brief Destructor for IConstraintLimit.
             */
            ~IConstraintLimit() override = default;

            /**
             * @brief Get the coefficient of restitution.
             * @return The coefficient of restitution.
             */
            virtual real_Num getRestitution() const = 0;

            /**
             * @brief Set the coefficient of restitution.
             * @param val The new coefficient of restitution.
             */
            virtual void setRestitution( real_Num val ) = 0;

            /**
             * @brief Get the bounce threshold.
             * @return The bounce threshold.
             */
            virtual real_Num getBounceThreshold() const = 0;

            /**
             * @brief Set the bounce threshold.
             * @param val The new bounce threshold.
             */
            virtual void setBounceThreshold( real_Num val ) = 0;

            /**
             * @brief Get the stiffness of the limit.
             * @return The stiffness of the limit.
             */
            virtual real_Num getStiffness() const = 0;

            /**
             * @brief Set the stiffness of the limit.
             * @param val The new stiffness of the limit.
             */
            virtual void setStiffness( real_Num val ) = 0;

            /**
             * @brief Get the damping of the limit.
             * @return The damping of the limit.
             */
            virtual real_Num getDamping() const = 0;

            /**
             * @brief Set the damping of the limit.
             * @param val The new damping of the limit.
             */
            virtual void setDamping( real_Num val ) = 0;

            /**
             * @brief Get the contact distance of the limit.
             * @return The contact distance of the limit.
             */
            virtual real_Num getContactDistance() const = 0;

            /**
             * @brief Set the contact distance of the limit.
             * @param val The new contact distance of the limit.
             */
            virtual void setContactDistance( real_Num val ) = 0;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IConstraintLimit_h__
