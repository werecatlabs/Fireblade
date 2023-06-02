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

        /** Interface for a graphics scene manager. */
        class IGraphicsScene : public ISharedObject
        {
        public:
            /** Defines the frame buffer types. */
            enum FrameBufferType
            {
                FBT_COLOUR = 0x1,
                FBT_DEPTH = 0x2,
                FBT_STENCIL = 0x4
            };

            /** Fog modes. */
            enum FogMode
            {
                /// No fog. 
                FOG_NONE,
                /// Fog density increases  exponentially from the camera (fog = 1/e^(distance * density))
                FOG_EXP,
                /// Fog density increases at the square of FOG_EXP, i.e. even quicker (fog =
                /// 1/e^(distance * density)^2)
                FOG_EXP2,
                /// Fog density increases linearly between the start and end distances
                FOG_LINEAR
            };

            enum RenderQueueGroupID
            {
                /// Use this queue for objects which must be rendered first e.g. backgrounds
                RENDER_QUEUE_BACKGROUND = 0,
                /// First queue (after backgrounds), used for skyboxes if rendered first
                RENDER_QUEUE_SKIES_EARLY = 5,
                RENDER_QUEUE_1 = 10,
                RENDER_QUEUE_2 = 20,
                RENDER_QUEUE_WORLD_GEOMETRY_1 = 25,
                RENDER_QUEUE_3 = 30,
                RENDER_QUEUE_4 = 40,
                /// The default render queue
                RENDER_QUEUE_MAIN = 50,
                RENDER_QUEUE_6 = 60,
                RENDER_QUEUE_7 = 70,
                RENDER_QUEUE_WORLD_GEOMETRY_2 = 75,
                RENDER_QUEUE_8 = 80,
                RENDER_QUEUE_9 = 90,
                /// Penultimate queue(before overlays), used for skyboxes if rendered last
                RENDER_QUEUE_SKIES_LATE = 95,
                /// Use this queue for objects which must be rendered last e.g. overlays
                RENDER_QUEUE_OVERLAY = 100,
                /// Final possible render queue, don't exceed this
                RENDER_QUEUE_MAX = 105
            };

            static const u32 VIEWPORT_MASK_TERRAIN;
            static const u32 VIEWPORT_MASK_OCCLUDER;
            static const u32 VIEWPORT_MASK_USER;
            static const u32 VIEWPORT_MASK_SHADOW;

            /** Virtual destructor. */
            ~IGraphicsScene() override = default;

            /** Gets the scene manager name.
            @return A string containing the scene manager name.
            */
            virtual String getName() const = 0;

            /** Sets the scene manager name.
            @param name A string containing the scene manager name.
            */
            virtual void setName( const String &name ) = 0;

            /** Clears the scene contents. */
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
            Array<SmartPtr<T>> getGraphicsObjectsByType() const
            {
                auto objectsByType = Array<SmartPtr<T>>();
                objectsByType.reserve( 8 );

                auto objects = getGraphicsObjects();
                for( auto obj : objects )
                {
                    if( obj )
                    {
                        if( obj->isDerived<T>() )
                        {
                            objectsByType.push_back( obj );
                        }
                    }
                }

                return objectsByType;
            }

            /** Gets a graphics objects. */
            template <class T>
            SmartPtr<T> getGraphicsObjectByType() const
            {
                auto objects = getGraphicsObjects();
                for( auto obj : objects )
                {
                    if( obj->isDerived<T>() )
                    {
                        return obj;
                    }
                }

                return nullptr;
            }

            /** Sets the ambient light colour.
             * @param colour The colour of the ambient light.
             */
            virtual void setAmbientLight( const ColourF &colour ) = 0;

            /** Adds a camera to the scene manager. */
            virtual SmartPtr<ICamera> addCamera( const String &name ) = 0;

            /** Gets a camera by name. */
            virtual SmartPtr<ICamera> getCamera( const String &name ) = 0;

            /** Gets the default camera. */
            virtual SmartPtr<ICamera> getCamera() const = 0;

            /** */
            virtual SmartPtr<ICamera> getActiveCamera() const = 0;

            /** */
            virtual void setActiveCamera( SmartPtr<ICamera> camera ) = 0;

            /** */
            virtual bool hasCamera( const String &name ) = 0;

            /** */
            virtual SmartPtr<ILight> addLight( const String &name ) = 0;

            /** */
            virtual SmartPtr<ISceneNode> getSceneNode( const String &name ) const = 0;

            /** */
            virtual SmartPtr<ISceneNode> getSceneNodeById( hash32 id ) const = 0;

            /** */
            virtual SmartPtr<ISceneNode> getRootSceneNode() const = 0;

            /** */
            virtual SmartPtr<IGraphicsMesh> addMesh( const String &name, const String &meshName ) = 0;

            /** */
            virtual SmartPtr<IGraphicsMesh> addMesh( const String &meshName ) = 0;

            /** */
            virtual SmartPtr<IGraphicsMesh> getMesh( const String &name ) const = 0;

            /** */
            virtual SmartPtr<IBillboardSet> addBillboardSet( const String &name, u32 poolSize = 20 ) = 0;

            /** */
            virtual SmartPtr<IBillboardSet> getBillboardSet( const String &name ) = 0;

            /** */
            virtual Array<SmartPtr<IBillboardSet>> getBillboardSets() const = 0;

            /** */
            virtual SmartPtr<IParticleSystem> addParticleSystem( const String &name,
                                                                 const String &templateName ) = 0;

            /** */
            virtual SmartPtr<IParticleSystem> getParticleSystem( const String &name ) const = 0;

            /** */
            virtual SmartPtr<ISceneNode> addSceneNode() = 0;

            /** */
            virtual SmartPtr<ISceneNode> addStaticNode() = 0;

            /** */
            virtual SmartPtr<ISceneNode> addSceneNode( const String &name ) = 0;

            /** */
            virtual bool removeSceneNode( SmartPtr<ISceneNode> sceneNode ) = 0;

            /** */
            virtual SmartPtr<IAnimationStateController> createAnimationStateController() = 0;

            /** */
            virtual SmartPtr<IAnimationTextureControl> createAnimationTextureCtrl(
                SmartPtr<IMaterialTexture> textureUnit, bool clone = false,
                const String &clonedMaterialName = StringUtil::EmptyString ) = 0;

            /** */
            virtual void setSkyBox( bool enable, SmartPtr<IMaterial> material, f32 distance = 5000,
                                    bool drawFirst = true ) = 0;

            /** */
            virtual void setFog( u32 fogMode, const ColourF &colour = ColourF::White,
                                 f32 expDensity = 0.001f, f32 linearStart = 0.0f,
                                 f32 linearEnd = 1.0f ) = 0;

            /** */
            virtual bool getEnableSkybox() const = 0;

            /** */
            virtual bool getEnableShadows() const = 0;

            /** */
            virtual void setEnableShadows( bool enableShadows, bool depthShadows = true ) = 0;

            /** */
            virtual SmartPtr<ITerrain> createTerrain() = 0;

            /** */
            virtual void destroyTerrain( SmartPtr<ITerrain> terrain ) = 0;

            /** */
            virtual SmartPtr<ITerrain> getTerrain( u32 id ) const = 0;

            /** */
            virtual SmartPtr<IDecalCursor> addDecalCursor( const String &terrainMaterial,
                                                           const String &decalTextureName,
                                                           const Vector2F &size ) = 0;

            /** */
            virtual bool castRay( const Ray3F &ray, Vector3F &result ) = 0;

            /** */
            virtual Array<SmartPtr<IGraphicsMesh>> splitMesh(
                SmartPtr<IGraphicsMesh> mesh, const SmartPtr<Properties> &properties ) = 0;

            /** */
            virtual SmartPtr<IInstanceManager> createInstanceManager(
                const String &customName, const String &meshName, const String &groupName, u32 technique,
                u32 numInstancesPerBatch, u16 flags = 0, u16 subMeshIdx = 0 ) = 0;

            /** Creates an InstancedEntity based on an existing InstanceManager. */
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
            template<class T>
            SmartPtr<T> addGraphicsObjectByType();

            FB_CLASS_REGISTER_DECL;
        };

        template <class T>
        SmartPtr<T> IGraphicsScene::addGraphicsObjectByType()
        {
            auto typeInfo = T::typeInfo();
            return addGraphicsObjectByTypeId( typeInfo );
        }

    }  // end namespace render
}  // end namespace fb

#endif
