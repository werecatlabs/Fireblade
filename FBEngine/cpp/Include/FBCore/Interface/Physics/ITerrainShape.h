#ifndef ITerrainShape_h__
#define ITerrainShape_h__

#include <FBCore/Interface/Physics/IPhysicsShape3.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief Interface for a terrain collision shape.
         *
         * The `ITerrainShape` class is a subtype of `IPhysicsShape3` that defines the
         * interface for a collision shape representing a terrain. Terrain shapes can be
         * used with physics engines to enable collision detection and response between
         * rigid bodies and terrain features in a 3D environment.
         *
         * This class provides the necessary functions to describe the shape of a terrain
         * collision object, such as its dimensions, orientation, and position, as well as
         * to query properties of the shape, such as its bounding box or volume. These
         * functions can be overridden by subclasses to implement specific terrain shapes.
         *
         * In addition to the functions inherited from `IPhysicsShape3`, this class also
         * defines a number of specific member functions for terrain shapes, including
         * functions to set and retrieve the heightfield data for the terrain, and to
         * adjust the scaling and offset of the heightfield data.
         *
         * @see IPhysicsShape3
         */
        class ITerrainShape : public IPhysicsShape3
        {
        public:
            /** Destructor. */
            ~ITerrainShape() override = default;

            /**
             * @brief Sets the collision type mask for this terrain shape.
             *
             * The collision type mask is used to determine which types of collision objects
             * this terrain should collide with. The mask is a bitfield where each bit
             * represents a collision type, and a value of 1 indicates that the terrain should
             * collide with that type. The collision types are defined by the physics engine
             * being used, and should be set accordingly.
             *
             * @param mask The collision type mask to set.
             */
            virtual void setCollisionType( u32 mask ) = 0;

            /**
             * @brief Gets the collision type mask for this terrain shape.
             *
             * @return The collision type mask for this terrain shape.
             */
            virtual u32 getCollisionType() const = 0;

            /**
             * @brief Sets the collision mask for this terrain shape.
             *
             * The collision mask is used to determine which types of collision objects this
             * terrain should be considered for collision with. The mask is a bitfield where
             * each bit represents a collision type, and a value of 1 indicates that the
             * terrain should be considered for collision with that type. The collision types
             * are defined by the physics engine being used, and should be set accordingly.
             *
             * @param mask The collision mask to set.
             */
            virtual void setCollisionMask( u32 mask ) = 0;

            /**
             * @brief Gets the collision mask for this terrain shape.
             *
             * @return The collision mask for this terrain shape.
             */
            virtual u32 getCollisionMask() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // ITerrainShape_h__
