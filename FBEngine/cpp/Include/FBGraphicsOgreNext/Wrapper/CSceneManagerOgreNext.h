#ifndef __CSceneManagerOgreNext_H_
#define __CSceneManagerOgreNext_H_

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Graphics/IGraphicsScene.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Pool.h>
#include <FBCore/Core/Set.h>
#include <OgreVector3.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneNodeOgreNext.h>

namespace fb
{
    namespace render
    {

        class CSceneManagerOgreNext : public IGraphicsScene
        {
        public:
            CSceneManagerOgreNext();
            CSceneManagerOgreNext( Ogre::SceneManager *sceneManager );
            ~CSceneManagerOgreNext() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;
            void postUpdate() override;

            SmartPtr<IGraphicsObject> addGraphicsObject( const String &name,
                                                         const String &type ) override;
            SmartPtr<IGraphicsObject> addGraphicsObject( const String &type ) override;
            bool removeGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject ) override;

            Array<SmartPtr<IGraphicsObject>> getGraphicsObjects() const override;

            void clearScene() override;

            bool hasAnimation( const String &animationName ) override;
            bool destroyAnimation( const String &animationName ) override;

            void setAmbientLight( const ColourF &colour ) override;

            SmartPtr<ICamera> addCamera( const String &name ) override;
            SmartPtr<ICamera> getCamera( const String &name ) override;
            SmartPtr<ICamera> getCamera() const override;

            /** */
            SmartPtr<ICamera> getActiveCamera() const override;

            /** */
            void setActiveCamera( SmartPtr<ICamera> camera ) override;

            bool hasCamera( const String &name ) override;

            SmartPtr<ILight> addLight( const String &name ) override;
            SmartPtr<ILight> getLight( const String &name ) const;

            SmartPtr<ISceneNode> getSceneNode( const String &name ) const override;
            SmartPtr<ISceneNode> getSceneNodeById( hash32 id ) const override;
            SmartPtr<ISceneNode> getRootSceneNode() const override;

            SmartPtr<IGraphicsMesh> addMesh( const String &name, const String &meshName ) override;
            SmartPtr<IGraphicsMesh> addMesh( const String &meshName ) override;

            SmartPtr<IGraphicsMesh> createV1Mesh( const String &meshName );

            SmartPtr<IGraphicsMesh> getMesh( const String &name ) const override;

            SmartPtr<IBillboardSet> addBillboardSet( const String &name, u32 poolSize = 20 ) override;
            SmartPtr<IBillboardSet> getBillboardSet( const String &name ) override;
            Array<SmartPtr<IBillboardSet>> getBillboardSets() const override;

            SmartPtr<IParticleSystem> addParticleSystem( const String &name,
                                                         const String &templateName ) override;
            SmartPtr<IParticleSystem> addParticleSystem( const String &name,
                                                         SmartPtr<IParticleSystemBuilder> &builder );
            SmartPtr<IParticleSystem> getParticleSystem( const String &name ) const override;

            SmartPtr<IAnimationStateController> createAnimationStateController() override;
            SmartPtr<IAnimationTextureControl> createAnimationTextureCtrl(
                SmartPtr<IMaterialTexture> textureUnit, bool clone = false,
                const String &clonedMaterialName = StringUtil::EmptyString ) override;

            void setSkyBox( bool enable, SmartPtr<IMaterial> material, f32 distance = 5000,
                            bool drawFirst = true ) override;
            void setFog( u32 fogMode, const ColourF &colour = ColourF::White, f32 expDensity = 0.001f,
                         f32 linearStart = 0.0f, f32 linearEnd = 1.0f ) override;

            bool getEnableSkybox() const override;

            SmartPtr<ISceneNode> addSceneNode() override;
            SmartPtr<ISceneNode> addStaticNode() override;
            SmartPtr<ISceneNode> addSceneNode( const String &name ) override;
            bool removeSceneNode( SmartPtr<ISceneNode> sceneNode ) override;

            void registerSceneNodeForUpdates( SmartPtr<ISceneNode> sceneNode );
            bool unregisteredForUpdates( SmartPtr<ISceneNode> sceneNode );
            bool unregisteredForUpdates( ISceneNode *sceneNode );

            void registerForUpdates( SmartPtr<IGraphicsObject> gfxObject );
            bool unregisteredForUpdates( SmartPtr<IGraphicsObject> gfxObject );
            bool unregisteredForUpdates( IGraphicsObject *gfxObject );

            String getAnimationNamePrefix() const;
            void setAnimationNamePrefix( const String &val );

            String getAnimationNameSuffix() const;
            void setAnimationNameSuffix( const String &val );

            bool getEnableShadows() const override;
            void setEnableShadows( bool enableShadows, bool depthShadows = true ) override;

            SmartPtr<ITerrain> createTerrain() override;
            void destroyTerrain( SmartPtr<ITerrain> terrain ) override;

            SmartPtr<ITerrain> getTerrain( u32 id ) const override;

            SmartPtr<IDecalCursor> addDecalCursor( const String &terrainMaterial,
                                                   const String &decalTextureName,
                                                   const Vector2F &size ) override;

            bool castRay( const Ray3F &ray, Vector3F &result ) override;

            Array<SmartPtr<IGraphicsMesh>> splitMesh( SmartPtr<IGraphicsMesh> mesh,
                                                      const SmartPtr<Properties> &properties ) override;

            Ogre::SceneManager *getSceneManager() const;

            void _getObject( void **ppObject ) const override;

            void addExistingGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject );

            void addExistingSceneNode( SmartPtr<ISceneNode> sceneNode );

            SmartPtr<IInstanceManager> createInstanceManager( const String &customName,
                                                              const String &meshName,
                                                              const String &groupName, u32 technique,
                                                              u32 numInstancesPerBatch, u16 flags = 0,
                                                              u16 subMeshIdx = 0 ) override;

            SmartPtr<IInstancedObject> createInstancedObject( const String &materialName,
                                                              const String &managerName ) override;
            void destroyInstancedObject( SmartPtr<IInstancedObject> instancedObject ) override;

            String getType() const;
            void setType( const String &val );

            String getName() const override;
            void setName( const String &val ) override;

            SmartPtr<IStateContext> getStateContext() const;
            void setStateContext( SmartPtr<IStateContext> stateContext );

            SmartPtr<IGraphicsObject> addGraphicsObjectByTypeId( u32 id ) override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class SceneManagerStateListener : public IStateListener
            {
            public:
                SceneManagerStateListener();
                ~SceneManagerStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CSceneManagerOgreNext *getOwner() const;
                void setOwner( CSceneManagerOgreNext *val );

            protected:
                CSceneManagerOgreNext *m_owner = nullptr;
            };

            class RootSceneNode : public CSceneNodeOgreNext
            {
            public:
                RootSceneNode() = default;
                ~RootSceneNode() override;

                void load( SmartPtr<ISharedObject> data ) override;
                void unload( SmartPtr<ISharedObject> data ) override;
            };

            void _addGraphicsObject( SmartPtr<IGraphicsObject> graphicsObject );

            void clearQueues();

            bool validateGfxObjName( const String &name ) const;

            String getUniqueName( const String &baseName ) const;

            SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> getSceneNodes() const;
            void setSceneNodes( SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> sceneNodes );

            SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> getRegisteredSceneNodes() const;
            void setRegisteredSceneNodes( SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> sceneNodes );

            SmartPtr<ISkybox> m_skybox;

            SmartPtr<ICamera> m_camera;
            SmartPtr<ICamera> m_activeCamera;

            Ogre::SceneManager *m_sceneManager = nullptr;

            Ogre::RaySceneQuery *m_pRaySceneQuery = nullptr;

            AssimpLoader *m_loader = nullptr;

            SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> m_sceneNodes;
            SharedPtr<ConcurrentArray<SmartPtr<ISceneNode>>> m_registeredSceneNodes;

            Array<SmartPtr<IGraphicsObject>> m_graphicsObjects;

            using EntityDeleteQueue = Array<Ogre::Entity *>;
            EntityDeleteQueue m_entityDeleteQueue;

            using SceneNodeDeleteQueue = Array<Ogre::SceneNode *>;
            SceneNodeDeleteQueue m_sceneNodeDeleteQueue;

            using ParticleSysDeleteQueue = Array<Ogre::ParticleSystem *>;
            ParticleSysDeleteQueue m_particleSysDeleteQueue;

            SharedMutex ParticleSysDeleteQueueMutex;
            SharedMutex EntityDeleteQueueMutex;
            SharedMutex SceneNodeDeleteQueueMutex;
            SharedMutex SceneManagerMutex;
            SpinRWMutex ListMutex;
            SpinRWMutex GraphicsObjectMutex;

            SmartPtr<ISceneNode> m_rootSceneNode;

            String m_animationNamePrefix;
            String m_animationNameSuffix;

            String m_type;
            String m_name;

            u32 nextRenderQueueUpdate;

            bool m_isClearing;

            // Set<ISceneNode*> m_registeredSceneNodes;
            Set<IGraphicsObject *> m_registeredGfxObjects;

            Array<SmartPtr<IInstanceManager>> m_instanceManagers;

            Array<SmartPtr<ITerrain>> m_terrains;

            SmartPtr<IStateContext> m_stateContext;
            SmartPtr<IStateListener> m_stateListener;
            SmartPtr<SceneManagerState> m_state;

            /// Value used to generate a unique entity name.
            static u32 m_nextGeneratedNameExt;
        };
    }  // end namespace render
}  // end namespace fb

#endif
