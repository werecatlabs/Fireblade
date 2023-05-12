#ifndef __IPHYSICSCOLLISIONSHAPE3__H
#define __IPHYSICSCOLLISIONSHAPE3__H

#include <FBCore/Interface/Physics/IPhysicsShape.h>
#include <FBCore/Interface/Physics/FBPhysicsTypes.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief Interface for a 3D physics collision shape.
         *
         * This class represents a physics shape that can be used in 3D physics simulations.
         * It is an abstract base class that defines the basic interface for a 3D collision shape.
         * This includes methods for calculating the volume, center of mass, and inertia of the shape,
         * as well as for checking if a point is contained within the shape and for testing for intersections
         * with other shapes.
         *
         * The `IPhysicsShape3` class is intended to be subclassed to create specific types of 3D collision
         * shapes, such as boxes, spheres, and capsules. Each subclass should implement the pure virtual
         * methods defined in this class to provide the specific functionality for the shape.
         *
         * @see IPhysicsShape, IPhysicsShape2
         */
        class IPhysicsShape3 : public IPhysicsShape
        {
        public:
            /** Destructor. */
            ~IPhysicsShape3() override = default;

            /**
             * @brief Gets the physics material assigned to the shape.
             *
             * @return The physics material assigned to the shape.
             */
            virtual SmartPtr<IPhysicsMaterial3> getMaterial() const = 0;

            /**
             * @brief Sets the physics material assigned to the shape.
             *
             * @param material The physics material to assign to the shape.
             */
            virtual void setMaterial( SmartPtr<IPhysicsMaterial3> material ) = 0;

            /**
             * @brief Sets the collision shape's local transformation.
             *
             * @param pose The local transformation to set for the shape.
             */
            virtual void setLocalPose( const Transform3<real_Num> &pose ) = 0;

            /**
             * @brief Gets the collision shape's local transformation.
             *
             * @return The collision shape's local transformation.
             */
            virtual Transform3<real_Num> getLocalPose() const = 0;

            /**
             * @brief Sets the collision filter data.
             *
             * @param data The collision filter data to set for the shape.
             */
            virtual void setSimulationFilterData( const PxFilterData &data ) = 0;

            /**
             * @brief Gets the collision filter data.
             *
             * @return The collision filter data.
             */
            virtual PxFilterData getSimulationFilterData() const = 0;

            /**
             * @brief Sets the actor this collision shape is attached to.
             *
             * @param body The actor to attach the collision shape to.
             */
            virtual void setActor( SmartPtr<IPhysicsBody3> body ) = 0;

            /**
             * @brief Gets the actor this collision shape is attached to.
             *
             * @return The actor the collision shape is attached to.
             */
            virtual SmartPtr<IPhysicsBody3> getActor() const = 0;

            /**
             * @brief Gets a pointer to the underlying object.
             *
             * This method returns a pointer to the underlying object that represents the collision shape in the
             * physics library being used. The type of the object pointer returned depends on the specific physics
             * library being used.
             *
             * @param ppObject A pointer to the object pointer to set.
             */
            virtual void _getObject( void **ppObject ) const = 0;

            /**
             * @brief Gets whether this collision shape has data.
             *
             * @return True if the collision shape has data, false otherwise.
             */
            virtual bool hasShapeData() const = 0;

            /**
             * @brief Determines whether this collision shape is a trigger.
             *
             * A trigger is a shape that does not generate a physical collision response,
             * but instead generates a trigger event when another shape enters it.
             * This can be useful for implementing game mechanics such as checkpoints,
             * teleporters, and collectibles.
             *
             * @return True if this collision shape is a trigger, false otherwise.
             */
            virtual bool isTrigger() const = 0;

            /**
             * @brief Sets whether this collision shape is a trigger.
             *
             * A trigger is a collision shape that is used for collision detection, but does not generate a
             * physical response when a collision occurs. Instead, a trigger generates an event that can be
             * detected by other objects in the simulation.
             *
             * @param trigger A boolean value indicating whether this collision shape is a trigger.
             */
            virtual void setTrigger( bool trigger ) = 0;

            /**
             * @brief Creates a new collision shape that is a copy of this one.
             *
             * This method creates a new collision shape that is identical to this one, with the same size, shape,
             * and properties. The new shape will not be attached to any actor or physics simulation until it is explicitly
             * added using the appropriate method.
             *
             * @return A smart pointer to the new collision shape.
             */
            virtual SmartPtr<IPhysicsShape3> clone() = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
