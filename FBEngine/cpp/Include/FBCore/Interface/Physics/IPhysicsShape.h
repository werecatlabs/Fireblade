#ifndef IPhysicsShape_h__
#define IPhysicsShape_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief Interface for a physics collision shape.
         *
         * This class represents an interface for a physics collision shape. It is used by
         * the physics engine to calculate collisions between objects. This is an abstract
         * class and should not be used directly.
         */
        class IPhysicsShape : public ISharedObject
        {
        public:
            static const hash_type CREATE_SHAPE_HASH;

            /**
             * @brief Destructor.
             */
            ~IPhysicsShape() override = default;

            /**
             * @brief Gets the data as a properties object.
             *
             * This method returns the data of the shape as a `Properties` object.
             *
             * @return The data as a `Properties` object.
             */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /**
             * @brief Sets the data as a properties object.
             *
             * This method sets the data of the shape from a `Properties` object.
             *
             * @param properties The `Properties` object containing the data to set.
             */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            /**
             * @brief Sets whether the collision shape is enabled.
             *
             * If the collision shape is disabled, it will not be considered for collision detection
             * with other objects.
             *
             * @param enabled Whether the collision shape is enabled.
             */
            virtual void setEnabled( bool enabled ) = 0;

            /**
             * @brief Gets whether the collision shape is enabled.
             *
             * @return Whether the collision shape is enabled.
             */
            virtual bool isEnabled() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IPhysicsShape_h__
