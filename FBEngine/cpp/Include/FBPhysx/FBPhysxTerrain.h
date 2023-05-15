#ifndef FBPhysxTerrain_h__
#define FBPhysxTerrain_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBPhysx/FBPhysxShape.h>
#include <FBCore/Interface/Physics/ITerrainShape.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Math/Transform3.h>
#include <geometry/PxHeightFieldGeometry.h>
#include <PxSimpleTypes.h>
#include <PxQueryReport.h>

namespace fb
{
    namespace physics
    {
        /**
         * @brief Implementation of the `ITerrainShape` interface using the PhysX physics engine.
         *
         * The `PhysxTerrain` class provides an implementation of the `ITerrainShape` interface
         * using the PhysX physics engine. It provides functions to set and retrieve the
         * collision mask and type of the terrain, as well as to enable or disable the terrain
         * for collision. It also provides functions to get and set the height data for the
         * terrain, and to create and manipulate a `PxHeightFieldGeometry` shape object.
         *
         * In addition to the functions inherited from the `ITerrainShape` interface, this
         * class also provides several member functions to create and manage the underlying
         * `PxShape` object for the terrain, as well as to perform raycasting against the
         * terrain for collision detection and response.
         *
         * @see ITerrainShape, PhysxShape
         */
        class PhysxTerrain : public PhysxShape<ITerrainShape>
        {
        public:
            /** Constructor. */
            PhysxTerrain();

            /** Destructor. */
            ~PhysxTerrain() override;

            /** Loads the terrain from the given shared object data. */
            void load( SmartPtr<ISharedObject> data ) override;

            /** Unloads the terrain from the given shared object data. */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** Sets the collision type of the terrain. */
            void setCollisionType( u32 mask ) override;

            /** Gets the collision type of the terrain. */
            u32 getCollisionType() const override;

            /** Sets the collision mask of the terrain. */
            void setCollisionMask( u32 mask ) override;

            /** Gets the collision mask of the terrain. */
            u32 getCollisionMask() const override;

            /** Sets whether the terrain is enabled for collision. */
            void setEnabled( bool enabled ) override;

            /** Gets whether the terrain is enabled for collision. */
            bool isEnabled() const override;

            /** Gets the user data associated with the terrain. */
            void *getUserData() const override;

            /** Sets the user data associated with the terrain. */
            void setUserData( void *userData ) override;

            /** Gets the height data for the terrain. */
            Array<f32> getHeightData() const;

            /** Sets the height data for the terrain. */
            void setHeightData( const Array<f32> &heightData );

            Vector2I getSize() const;

            void setSize( const Vector2I& size );

            Vector3<real_Num> getTerrainSize() const;

            void setTerrainSize( const Vector3<real_Num>& terrainSize );

            Vector3<real_Num> getHeightScale() const;

            void setHeightScale( const Vector3<real_Num>& heightScale );

        protected:
            /** Callback class for raycasting against the terrain. */
            class RaycastCallback : public physx::PxRaycastCallback
            {
            public:
                /** Constructor. */
                RaycastCallback( PhysxTerrain *collision );

                /** Processes the raycast touch data. */
                physx::PxAgain processTouches( const physx::PxRaycastHit *data,
                                               physx::PxU32 nbHits ) override;

                physx::PxRaycastHit buffer[10]; /**< Buffer for storing raycast hit data. */
                physx::PxRaycastHit m_hit;      /**< Data for the most recent raycast hit. */
                PhysxTerrain *m_collision;      /**< Pointer to the terrain */
            };

            /**
             * @brief Creates a height field collision shape for the terrain.
             *
             * The `createTerrainShape` function creates a height field collision shape for the terrain,
             * based on the specified height data and other parameters, and returns the resulting
             * height field geometry.
             *
             * @param heightData The height data for the terrain.
             * @param size The dimensions of the height field.
             * @param heightScale The vertical scale factor for the terrain.
             * @param terrainSize The size of the terrain.
             * @param t The transform for the terrain.
             *
             * @return The height field geometry for the terrain.
             */
            physx::PxHeightFieldGeometry createTerrainGeometry( const Array<float> &heightData,
                                                                const Vector2I &size,
                                                                const Vector3F &heightScale,
                                                                const Vector3F &terrainSize,
                                                                const Transform3<f32> &t );

            /**
             * @brief Creates a height field collision shape for the terrain.
             *
             * The `createTerrainShape` function creates a height field collision shape for the terrain,
             * based on the specified height data and other parameters, and returns the resulting
             * height field geometry.
             *
             * @param heightData The height data for the terrain.
             * @param size The dimensions of the height field.
             * @param heightScale The vertical scale factor for the terrain.
             * @param terrainSize The size of the terrain.
             * @param t The transform for the terrain.
             *
             * @return The height field geometry for the terrain.
             */
            physx::PxShape *createDefaultTerrain( s32 sizeX, s32 sizeY, f32 maxHeight );

            /**< The scale factor for the terrain. */
            Vector3<real_Num> m_heightScale;

            /** */
            Vector3<real_Num> m_terrainSize;

            /** */
            Vector2I m_size;

            /** */
            Array<f32> m_heightData;

            /**
             * @brief Transform of the collision shape in the local space of the parent actor.
             */
            Transform3<real_Num> m_colliderTransform;

            /**
             * @brief Raw pointer to the physics material used by the terrain shape.
             */
            RawPtr<physx::PxMaterial> m_material;

            /**
             * @brief Transform of the terrain collision shape in the local space of its parent actor.
             */
            Transform3<real_Num> m_transform;

            /**
             * @brief Atomic boolean flag indicating whether the terrain shape is static (i.e. not movable).
             */
            atomic_bool m_isStatic;

            /**
             * @brief Raw pointer to the physics actor object associated with the terrain collision shape.
             */
            RawPtr<physx::PxRigidActor> m_actor;

            /**
             * @brief Smart pointer to the physics material used by the terrain shape.
             */
            SmartPtr<IPhysicsMaterial3> m_physicsMaterial;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxTerrain_h__
