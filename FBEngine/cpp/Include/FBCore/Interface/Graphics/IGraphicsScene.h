#ifndef _FB_ISceneManager_H_
#define _FB_ISceneManager_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Math/Ray3.h>
#include <FBCore/Interface/Graphics/IGraphicsObject.h>

namespace fb
{
    namespace render
    {
        /**
         * @class IGraphicsScene
         * @brief Interface representing a graphics scene manager.
         *
         * This interface defines methods and enums for managing graphics objects and scene elements.
         */
        class IGraphicsScene : public ISharedObject
        {
        public:
            /**
             * @enum FrameBufferType
             * @brief Defines the types of frame buffers.
             */
            enum FrameBufferType
            {
                FBT_COLOUR = 0x1,
                ///< Color frame buffer.
                FBT_DEPTH = 0x2,
                ///< Depth frame buffer.
                FBT_STENCIL = 0x4 ///< Stencil frame buffer.
            };

            /**
             * @enum FogMode
             * @brief Defines the modes for applying fog in the scene.
             */
            enum FogMode
            {
                FOG_NONE,
                ///< No fog.
                FOG_EXP,
                ///< Exponential fog.
                FOG_EXP2,
                ///< Exponential squared fog.
                FOG_LINEAR ///< Linear fog.
            };

            static const u32 VIEWPORT_MASK_TERRAIN;  ///< Viewport mask for terrain.
            static const u32 VIEWPORT_MASK_OCCLUDER; ///< Viewport mask for occluders.
            static const u32 VIEWPORT_MASK_USER;     ///< Viewport mask for user-defined.
            static const u32 VIEWPORT_MASK_SHADOW;   ///< Viewport mask for shadows.

            /**
             * @brief Virtual destructor.
             */
            ~IGraphicsScene() override = default;

            /**
             * @brief Gets the scene manager name.
             * @return A string containing the scene manager name.
             */
            String getName() const override = 0;

            /**
             * @brief Sets the scene manager name.
             * @param name A string containing the new scene manager name.
             */
            void setName( const String &name ) override = 0;

            /**
             * @brief Clears the contents of the scene.
             */
            virtual void clearScene() = 0;

            /** Returns a boolean indicating whether an animation exists. */
            virtual bool hasAnimation( const String &animationName ) = 0;

            /** Destroys an animation of the given name. */
            virtual bool destroyAnimation( const String &animationName ) = 0;

            /** Creates a graphics object according to the type specified.
            @remarks
            Generates a name. Will throw an exception if the object could not be created.
            @return
            Returns the created graphics object. Will return null if exceptions are not enabled.
            */
            virtual SmartPtr<IGraphicsObject> addGraphicsObject( const String &name,
                                                                 const String &type ) = 0;

            /** Creates a graphics object according to the type specified.
            @remarks
            Generates a name. Will throw an exception if the object could not be created.
            @return
            Returns the created graphics object. Will return null if exceptions are not enabled.
            */
            virtual SmartPtr<IGraphicsObject> addGraphicsObject( const String &type ) = 0;

            virtual SmartPtr<IGraphicsObject> addGraphicsObjectByTypeId( u32 id ) = 0;

            /** Removes a graphics object from the scene manager.
            @param
            graphicsObject The graphics object to be removed. The pointer will be set to null.
            @return
            Returns true if successful.
            */
            virtual bool removeGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject ) = 0;

            /** Gets an array of the graphics objects. */
            virtual Array<SmartPtr<IGraphicsObject>> getGraphicsObjects() const = 0;

            /** Gets an array of the graphics objects. */
            template <class T>
            Array<SmartPtr<T>> getGraphicsObjectsByType() const;

            /** Gets a graphics objects. */
            template <class T>
            SmartPtr<T> getGraphicsObjectByType() const;

            /** Sets the ambient light colour.
             * @param colour The colour of the ambient light.
             */
            virtual void setAmbientLight( const ColourF &colour ) = 0;

            virtual ColourF getUpperHemisphere() const = 0;
            virtual void setUpperHemisphere( const ColourF &upperHemisphere ) = 0;

            virtual ColourF getLowerHemisphere() const = 0;
            virtual void setLowerHemisphere( const ColourF &lowerHemisphere ) = 0;

            virtual Vector3<real_Num> getHemisphereDir() const = 0;
            virtual void setHemisphereDir( Vector3<real_Num> hemisphereDir ) = 0;

            virtual f32 getEnvmapScale() const = 0;
            virtual void setEnvmapScale( f32 envmapScale ) = 0;

            /** Adds a camera to the scene manager. */
            virtual SmartPtr<ICamera> addCamera( const String &name ) = 0;

            /** Gets a camera by name. */
            virtual SmartPtr<ICamera> getCamera( const String &name ) = 0;

            /** Gets the default camera. */
            virtual SmartPtr<ICamera> getCamera() const = 0;

            /**
             * @brief Get the currently active camera in the scene.
             * @return A SmartPtr to the active camera.
             */
            virtual SmartPtr<ICamera> getActiveCamera() const = 0;

            /**
             * @brief Set the active camera for the scene.
             * @param camera A SmartPtr to the camera to set as active.
             */
            virtual void setActiveCamera( SmartPtr<ICamera> camera ) = 0;

            /**
             * @brief Check if a camera with the given name exists in the scene.
             * @param name The name of the camera to check.
             * @return True if a camera with the specified name exists, false otherwise.
             */
            virtual bool hasCamera( const String &name ) = 0;

            /**
             * @brief Add a new light to the scene.
             * @param name The name of the light to add.
             * @return A SmartPtr to the newly added light.
             */
            virtual SmartPtr<ILight> addLight( const String &name ) = 0;

            /**
             * @brief Get a scene node by its name.
             * @param name The name of the scene node to retrieve.
             * @return A SmartPtr to the retrieved scene node.
             */
            virtual SmartPtr<ISceneNode> getSceneNode( const String &name ) const = 0;

            /**
             * @brief Get a scene node by its unique identifier.
             * @param id The unique identifier of the scene node to retrieve.
             * @return A SmartPtr to the retrieved scene node.
             */
            virtual SmartPtr<ISceneNode> getSceneNodeById( hash32 id ) const = 0;

            /**
             * @brief Get the root scene node of the scene.
             * @return A SmartPtr to the root scene node.
             */
            virtual SmartPtr<ISceneNode> getRootSceneNode() const = 0;

            /**
             * @brief Add a graphics mesh to the scene.
             * @param name The name of the mesh to add.
             * @param meshName The name of the source mesh.
             * @return A SmartPtr to the newly added graphics mesh.
             */
            virtual SmartPtr<IGraphicsMesh> addMesh( const String &name, const String &meshName ) = 0;

            /**
             * @brief Add a graphics mesh to the scene using the given mesh name.
             * @param meshName The name of the source mesh.
             * @return A SmartPtr to the newly added graphics mesh.
             */
            virtual SmartPtr<IGraphicsMesh> addMesh( const String &meshName ) = 0;

            /**
             * @brief Get a graphics mesh from the scene by its name.
             * @param name The name of the graphics mesh to retrieve.
             * @return A SmartPtr to the retrieved graphics mesh.
             */
            virtual SmartPtr<IGraphicsMesh> getMesh( const String &name ) const = 0;

            /**
             * @brief Add a billboard set to the scene with the given name and pool size.
             * @param name The name of the billboard set.
             * @param poolSize The size of the billboard pool (default is 20).
             * @return A SmartPtr to the newly added billboard set.
             */
            virtual SmartPtr<IBillboardSet> addBillboardSet( const String &name, u32 poolSize = 20 ) = 0;

            /**
             * @brief Get a billboard set from the scene by its name.
             * @param name The name of the billboard set to retrieve.
             * @return A SmartPtr to the retrieved billboard set.
             */
            virtual SmartPtr<IBillboardSet> getBillboardSet( const String &name ) = 0;

            /**
             * @brief Get an array of all billboard sets present in the scene.
             * @return An array of SmartPtrs to billboard sets.
             */
            virtual Array<SmartPtr<IBillboardSet>> getBillboardSets() const = 0;

            /**
             * @brief Add a particle system to the scene with the given name and template name.
             * @param name The name of the particle system.
             * @param templateName The name of the particle system template.
             * @return A SmartPtr to the newly added particle system.
             */
            virtual SmartPtr<IParticleSystem> addParticleSystem( const String &name,
                                                                 const String &templateName ) = 0;

            /**
             * @brief Get a particle system from the scene by its name.
             * @param name The name of the particle system to retrieve.
             * @return A SmartPtr to the retrieved particle system.
             */
            virtual SmartPtr<IParticleSystem> getParticleSystem( const String &name ) const = 0;

            /**
             * @brief Add a scene node to the scene.
             * @return A SmartPtr to the newly added scene node.
             */
            virtual SmartPtr<ISceneNode> addSceneNode() = 0;

            /**
             * @brief Add a static scene node to the scene.
             * @return A SmartPtr to the newly added static scene node.
             */
            virtual SmartPtr<ISceneNode> addStaticNode() = 0;

            /**
             * @brief Add a named scene node to the scene.
             * @param name The name of the scene node to add.
             * @return A SmartPtr to the newly added scene node.
             */
            virtual SmartPtr<ISceneNode> addSceneNode( const String &name ) = 0;

            /**
             * @brief Remove a scene node from the scene.
             * @param sceneNode A SmartPtr to the scene node to remove.
             * @return True if the removal was successful, false otherwise.
             */
            virtual bool removeSceneNode( SmartPtr<ISceneNode> sceneNode ) = 0;

            /**
             * @brief Create an animation state controller.
             * @return A SmartPtr to the newly created animation state controller.
             */
            virtual SmartPtr<IAnimationStateController> createAnimationStateController() = 0;

            /**
             * @brief Create an animation texture control for the specified texture unit.
             * @param textureUnit A SmartPtr to the target material texture.
             * @param clone Whether to clone the material.
             * @param clonedMaterialName The name of the cloned material.
             * @return A SmartPtr to the newly created animation texture control.
             */
            virtual SmartPtr<IAnimationTextureControl> createAnimationTextureCtrl(
                SmartPtr<IMaterialTexture> textureUnit, bool clone = false,
                const String &clonedMaterialName = StringUtil::EmptyString ) = 0;

            /**
             * @brief Set the skybox settings.
             * @param enable Whether to enable the skybox.
             * @param material A SmartPtr to the material used for the skybox.
             * @param distance The distance of the skybox from the camera.
             * @param drawFirst Whether to draw the skybox first.
             */
            virtual void setSkyBox( bool enable, SmartPtr<IMaterial> material, f32 distance = 5000,
                                    bool drawFirst = true ) = 0;

            /**
             * @brief Set the fog settings for the scene.
             * @param fogMode The fog mode to apply.
             * @param colour The color of the fog.
             * @param expDensity The exponential fog density.
             * @param linearStart The starting point of linear fog.
             * @param linearEnd The ending point of linear fog.
             */
            virtual void setFog( u32 fogMode, const ColourF &colour = ColourF::White,
                                 f32 expDensity = 0.001f, f32 linearStart = 0.0f,
                                 f32 linearEnd = 1.0f ) = 0;

            /**
             * @brief Get whether the skybox is currently enabled in the scene.
             * @return True if the skybox is enabled, false otherwise.
             */
            virtual bool getEnableSkybox() const = 0;

            /**
             * @brief Get whether shadows are currently enabled in the scene.
             * @return True if shadows are enabled, false otherwise.
             */
            virtual bool getEnableShadows() const = 0;

            /**
             * @brief Set whether shadows should be enabled in the scene.
             * @param enableShadows Whether to enable shadows.
             * @param depthShadows Whether to enable depth shadows (default is true).
             */
            virtual void setEnableShadows( bool enableShadows, bool depthShadows = true ) = 0;

            /**
             * @brief Create a terrain object and add it to the scene.
             * @return A SmartPtr to the newly created terrain object.
             */
            virtual SmartPtr<ITerrain> createTerrain() = 0;

            /**
             * @brief Destroy a terrain object from the scene.
             * @param terrain A SmartPtr to the terrain object to destroy.
             */
            virtual void destroyTerrain( SmartPtr<ITerrain> terrain ) = 0;

            /**
             * @brief Get a terrain object from the scene by its ID.
             * @param id The ID of the terrain object to retrieve.
             * @return A SmartPtr to the retrieved terrain object.
             */
            virtual SmartPtr<ITerrain> getTerrain( u32 id ) const = 0;

            /**
             * @brief Add a decal cursor to the scene.
             * @param terrainMaterial The material for the terrain.
             * @param decalTextureName The name of the decal texture.
             * @param size The size of the decal.
             * @return A SmartPtr to the newly added decal cursor.
             */
            virtual SmartPtr<IDecalCursor> addDecalCursor( const String &terrainMaterial,
                                                           const String &decalTextureName,
                                                           const Vector2<real_Num> &size ) = 0;

            /**
             * @brief Cast a ray into the scene and retrieve the intersection point.
             * @param ray The ray to cast.
             * @param result The intersection point if the ray hits an object.
             * @return True if the ray hits an object, false otherwise.
             */
            virtual bool castRay( const Ray3F &ray, Vector3<real_Num> &result ) = 0;

            /**
             * @brief Split a graphics mesh into multiple sub-meshes based on properties.
             *
             * This function divides a given graphics mesh into sub-meshes according to provided properties.
             *
             * @param mesh The source graphics mesh to split.
             * @param properties SmartPtr to the properties used to split the mesh.
             * @return An array of SmartPtrs to the resulting sub-meshes.
             */
            virtual Array<SmartPtr<IGraphicsMesh>> splitMesh(
                SmartPtr<IGraphicsMesh> mesh, const SmartPtr<Properties> &properties ) = 0;

            /**
             * @brief Create an instance manager for rendering instances of a mesh.
             *
             * This function creates an instance manager responsible for rendering multiple instances of a mesh.
             *
             * @param customName The custom name of the instance manager.
             * @param meshName The name of the mesh.
             * @param groupName The name of the group containing the mesh.
             * @param technique The rendering technique for the instances.
             * @param numInstancesPerBatch The number of instances per batch.
             * @param flags Flags for the instance manager (optional, default is 0).
             * @param subMeshIdx The index of the sub-mesh (optional, default is 0).
             * @return A SmartPtr to the created instance manager.
             */
            virtual SmartPtr<IInstanceManager> createInstanceManager(
                const String &customName, const String &meshName, const String &groupName, u32 technique,
                u32 numInstancesPerBatch, u16 flags = 0, u16 subMeshIdx = 0 ) = 0;

            /**
             * @brief Create an InstancedEntity based on an existing InstanceManager.
             *
             * This function creates an InstancedEntity using an already existing InstanceManager.
             *
             * @param materialName The name of the material for the InstancedEntity.
             * @param managerName The name of the InstanceManager to base the InstancedEntity on.
             * @return A SmartPtr to the created InstancedEntity.
             */
            virtual SmartPtr<IInstancedObject> createInstancedObject( const String &materialName,
                const String &managerName ) = 0;

            /** Removes an InstancedEntity. */
            virtual void destroyInstancedObject( SmartPtr<IInstancedObject> instancedObject ) = 0;

            /** Gets a pointer to the underlying object. This is dependent on the graphics library used.
             */
            virtual void _getObject( void **ppObject ) const = 0;

            /** Adds a graphics object.
             * @paramt T The type of the graphics object.
             */
            template <class T>
            SmartPtr<T> addGraphicsObjectByType();

            FB_CLASS_REGISTER_DECL;
        };

        template <class T>
        SmartPtr<T> IGraphicsScene::addGraphicsObjectByType()
        {
            auto typeInfo = T::typeInfo();
            return addGraphicsObjectByTypeId( typeInfo );
        }

        template <class T>
        Array<SmartPtr<T>> IGraphicsScene::getGraphicsObjectsByType() const
        {
            auto objectsByType = Array<SmartPtr<T>>();
            objectsByType.reserve( 8 );

            auto objects = getGraphicsObjects();
            for(auto obj : objects)
            {
                if(obj)
                {
                    if(obj->isDerived<T>())
                    {
                        objectsByType.push_back( obj );
                    }
                }
            }

            return objectsByType;
        }

        template <class T>
        SmartPtr<T> IGraphicsScene::getGraphicsObjectByType() const
        {
            auto objects = getGraphicsObjects();
            for(auto obj : objects)
            {
                if(obj->isDerived<T>())
                {
                    return obj;
                }
            }

            return nullptr;
        }
    } // end namespace render
}     // end namespace fb

#endif
