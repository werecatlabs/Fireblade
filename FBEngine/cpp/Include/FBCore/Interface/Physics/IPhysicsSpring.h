#ifndef IPhysicsSpring_h__
#define IPhysicsSpring_h__

#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief Interface for a physics spring.
         */
        class IPhysicsSpring : public ISharedObject
        {
        public:
            /**
             * @brief Destructor.
             */
            ~IPhysicsSpring() override = default;

            /**
             * @brief Gets the stiffness of the spring.
             * @return The stiffness of the spring.
             */
            virtual real_Num getStiffness() const = 0;

            /**
             * @brief Sets the stiffness of the spring.
             * @param stiffness The stiffness of the spring.
             */
            virtual void setStiffness( real_Num stiffness ) = 0;

            /**
             * @brief Gets the damping of the spring.
             * @return The damping of the spring.
             */
            virtual real_Num getDamping() const = 0;

            /**
             * @brief Sets the damping of the spring.
             * @param damping The damping of the spring.
             */
            virtual void setDamping( real_Num damping ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsSpring_h__
