#ifndef IConstraintLinearLimit_h__
#define IConstraintLinearLimit_h__

#include <FBCore/Interface/Physics/IConstraintLimit.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief The IConstraintLinearLimit class is an interface for linear constraints that limit the motion
         * of a body along a single axis.
         */
        class IConstraintLinearLimit : public IConstraintLimit
        {
        public:
            /**
             * @brief Destructor.
             */
            ~IConstraintLinearLimit() override = default;

            /**
             * @brief Gets the current limit value.
             * @return The current limit value.
             */
            virtual real_Num getValue() const = 0;

            /**
             * @brief Sets the limit value.
             * @param value The limit value to set.
             */
            virtual void setValue( real_Num value ) = 0;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IConstraintLinearLimit_h__
