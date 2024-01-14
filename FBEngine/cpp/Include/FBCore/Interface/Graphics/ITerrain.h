#ifndef ITerrain_h__
#define ITerrain_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Ray3.h>
#include <FBCore/Math/Transform3.h>

namespace fb
{
    namespace render
    {

        /** An interface for a render terrain. */
        class ITerrain : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~ITerrain() override = default;

            /** Gets the world transform of the terrain.
             * @return The world transform of the terrain.
             */
            virtual Transform3<real_Num> getWorldTransform() const = 0;

            /** Sets the world transform of the terrain.
             * @param worldTransform The new world transform of the terrain.
             */
            virtual void setWorldTransform( const Transform3<real_Num> &worldTransform ) = 0;

            /** Gets the position of the terrain.
             * @return A `Vector3` representing the position of the terrain.
             */
            virtual Vector3<real_Num> getPosition() const = 0;

            /** Sets the position of the terrain.
             * @param position The new position of the terrain.
             */
            virtual void setPosition( const Vector3<real_Num> &position ) = 0;

            /** Gets the height at a position on the terrain.
             * @param position The world position to query for the height.
             * @return The height of the terrain at the specified position.
             */
            virtual f32 getHeightAtWorldPosition( const Vector3<real_Num> &position ) const = 0;

            /** Get the size of the terrain.
             * @return The size of the terrain.
             */
            virtual u16 getSize() const = 0;

            /** Gets a terrain space position from a world space position.
             * @param worldSpace The world space position to convert.
             * @return The converted terrain space position.
             */
            virtual Vector3<real_Num> getTerrainSpacePosition(
                const Vector3<real_Num> &worldSpace ) const = 0;

            /** Gets an `Array` of height data.
             * @return An `Array` containing the height data of the terrain.
             */
            virtual Array<f32> getHeightData() const = 0;

            /** Gets whether the terrain is visible.
             * @return `true` if the terrain is visible, `false` otherwise.
             */
            virtual bool isVisible() const = 0;

            /** Sets whether the terrain is visible.
             * @param visible The new visibility state of the terrain.
             */
            virtual void setVisible( bool visible ) = 0;

            /** Gets the terrain material name.
             * @return The name of the terrain's material.
             */
            virtual String getMaterialName() const = 0;

            /** Sets the terrain material name.
             * @param materialName The new name of the terrain's material.
             */
            virtual void setMaterialName( const String &materialName ) = 0;

            /** Gets a terrain layer blend map.
             * @param index The index of the blend map to retrieve.
             * @return A `SmartPtr` to the requested blend map.
             */
            virtual SmartPtr<ITerrainBlendMap> getBlendMap( u32 index ) = 0;

            /** Gets the blend map size.
             * @return The size of the terrain's blend map.
             */
            virtual u16 getLayerBlendMapSize() const = 0;

            /** Casts a ray against the terrain.
             * @param ray The ray to cast against the terrain.
             * @return A `SmartPtr` to an `ITerrainRayResult` object containing information about the intersection, or
             *         `nullptr` if the ray does not intersect the terrain.
             */
            virtual SmartPtr<ITerrainRayResult> intersects( const Ray3F &ray ) const = 0;

            /** Gets the terrain represented as a mesh.
             * @return A `SmartPtr` to an `IMesh` object containing mesh data.
             */
            virtual SmartPtr<IMesh> getMesh() const = 0;

            /** Gets the height scale.
             * @return The scale factor for the height of the terrain.
             */
            virtual f32 getHeightScale() const = 0;

            /** Sets the height scale.
             * @param val The new height scale value to set.
             */
            virtual void setHeightScale( f32 heightScale ) = 0;

            /** Gets the scene manager.
             * @return A SmartPtr to the scene manager associated with the terrain.
             */
            virtual SmartPtr<IGraphicsScene> getSceneManager() const = 0;

            /** Sets the scene manager.
             * @param sceneManager A SmartPtr to the new scene manager to associate with the terrain.
             */
            virtual void setSceneManager( SmartPtr<IGraphicsScene> sceneManager ) = 0;

            /** Gets a pointer to the underlying graphics object. This is dependent on the graphics
             * library used.
             * @param ppObject A pointer to the pointer to the underlying graphics object.
             */
            virtual void _getObject( void **ppObject ) const = 0;

            virtual SmartPtr<ITexture> getHeightMap() const = 0;

            virtual void setHeightMap( SmartPtr<ITexture> heightMap ) = 0;

            /** Sets a texture layer.
             * @param layer The layer index.
             * @param textureName The name of the texture to set.
             */
            virtual void setTextureLayer( s32 layer, const String &textureName ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // ITerrain_h__
