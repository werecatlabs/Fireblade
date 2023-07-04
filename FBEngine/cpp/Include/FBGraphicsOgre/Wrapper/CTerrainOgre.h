#ifndef __CTerrainOgre_h__
#define __CTerrainOgre_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBGraphics/Wrapper/CTerrain.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/Graphics/ITerrain.h>
#include <FBCore/Memory/SharedObject.h>
#include <OgreVector3.h>
#include <OgreRenderTargetListener.h>
#include <OgreTexture.h>

namespace fb
{
    namespace render
    {

        /**
         * Represents a terrain object that can be rendered.
         */
        class CTerrainOgre : public CTerrain
        {
        public:
            /**
             * Default constructor.
             */
            CTerrainOgre();

            /**
             * Destructor.
             */
            ~CTerrainOgre() override;

            /**
             * Updates the terrain.
             */
            void update() override;

            /**
             * Loads terrain data.
             * @param data The data to load.
             */
            void load( SmartPtr<ISharedObject> data ) override;

            /**
             * Unloads terrain data.
             * @param data The data to unload.
             */
            void unload( SmartPtr<ISharedObject> data ) override;

            virtual Transform3<real_Num> getWorldTransform() const;

            virtual void setWorldTransform( const Transform3<real_Num> &worldTransform );

            /**
             * Gets the position of the terrain.
             * @return The position of the terrain.
             */
            Vector3F getPosition() const override;

            /**
             * Sets the position of the terrain.
             * @param position The new position of the terrain.
             */
            void setPosition( const Vector3F &position ) override;

            /**
             * Gets the height of the terrain at a specified world position.
             * @param position The position at which to find the height.
             * @return The height of the terrain at the specified position.
             */
            f32 getHeightAtWorldPosition( const Vector3F &position ) const override;

            /**
             * Gets the size of the terrain.
             * @return The size of the terrain.
             */
            u16 getSize() const override;

            /**
             * Gets a terrain space position from a world space position.
             * @param worldSpace The world space position.
             * @return The terrain space position.
             */
            Vector3F getTerrainSpacePosition( const Vector3F &worldSpace ) const override;

            /**
             * Gets an array of height data.
             * @return The height data.
             */
            Array<f32> getHeightData() const override;

            /**
             * Gets the name of the terrain's material.
             * @return The name of the terrain's material.
             */
            String getMaterialName() const override;

            /**
             * Sets the name of the terrain's material.
             * @param materialName The new name of the terrain's material.
             */
            void setMaterialName( const String &materialName ) override;

            /**
             * Gets a blend map for a specific terrain layer.
             * @param index The index of the layer for which to get the blend map.
             * @return The blend map for the specified layer.
             */
            SmartPtr<ITerrainBlendMap> getBlendMap( u32 index ) override;

            /**
             * Casts a ray against the terrain.
             * @param ray The ray to cast.
             * @return The result of the ray cast.
             */
            SmartPtr<ITerrainRayResult> intersects( const Ray3F &ray ) const override;

            /**
             * Gets the size of a layer blend map.
             * @return The size of a layer blend map.
             */
            u16 getLayerBlendMapSize() const override;

            /**
             * Gets the terrain represented as a mesh.
             * @return An object containing mesh data.
             */
            SmartPtr<IMesh> getMesh() const override;

            f32 getHeightScale() const override;
            void setHeightScale( f32 heightScale ) override;

            void configureTerrainDefaults( Ogre::Light *l );
            void defineTerrain( long x, long y, bool flat = false );
            bool getTerrainImage( bool flipX, bool flipY, Ogre::Image &img );
            void initBlendMaps( Ogre::Terrain *terrain );

            Ogre::TerrainGroup *getTerrainGroup() const;
            void setTerrainGroup( Ogre::TerrainGroup *terrainGroup );

            void addTextureShadowDebugOverlay( int loc, size_t num );
            void addTextureDebugOverlay( int loc, const Ogre::String &texname, size_t i );

            void _getObject( void **ppObject ) const override;

            SmartPtr<IGraphicsScene> getSceneManager() const override;

            void setSceneManager( SmartPtr<IGraphicsScene> sceneManager ) override;

            Ogre::SceneManager *getOgreSceneManager() const;

            Ogre::Camera *getTerrainCamera() const;
            void setTerrainCamera( Ogre::Camera *terrainCamera );

            Ogre::SceneNode *getTerrainCameraSceneNode() const;
            void setTerrainCameraSceneNode( Ogre::SceneNode *terrainCameraSceneNode );

            Forests::PagedGeometry *getTrees() const;
            void setTrees( Forests::PagedGeometry *trees );

            u16 getTerrainSize() const;
            void setTerrainSize( u16 terrainSize );

            f32 getTerrainWorldSize() const;
            void setTerrainWorldSize( f32 terrainWorldSize );

            void setTextureLayer( s32 layer, const String &textureName );

            FB_CLASS_REGISTER_DECL;

        protected:
            class TerrainStateListener : public SharedObject<IStateListener>
            {
            public:
                TerrainStateListener() = default;
                ~TerrainStateListener() = default;

                void unload( SmartPtr<ISharedObject> data ) override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<CTerrainOgre> getOwner() const;

                void setOwner( SmartPtr<CTerrainOgre> owner );

            private:
                AtomicWeakPtr<CTerrainOgre> m_owner;
            };

            // hack to move
            class ShadowRenderTargetListener : public Ogre::RenderTargetListener
            {
            public:
                ShadowRenderTargetListener( Ogre::TexturePtr tex );
                ~ShadowRenderTargetListener() override;

                void preRenderTargetUpdate( const Ogre::RenderTargetEvent &evt ) override;

                void preViewportUpdate( const Ogre::RenderTargetViewportEvent &evt ) override;

            protected:
                Ogre::TexturePtr m_tex;
                Ogre::PixelBox *box = nullptr;
                float *data = nullptr;
                u32 m_nextUpdate = 0;
            };

            void initialise();
            void initialise2();
            void loadTrees();

            void setTreeMask( Ogre::SceneNode *node );

            Ogre::Camera *m_terrainCamera = nullptr;
            Ogre::SceneNode *m_terrainCameraSceneNode = nullptr;

            // Pointers to PagedGeometry class instances:
            Forests::PagedGeometry *m_trees = nullptr;

            Ogre::TerrainGroup *mTerrainGroup = nullptr;
            Ogre::TerrainPaging *mTerrainPaging = nullptr;
            Ogre::PageManager *mPageManager = nullptr;

            AtomicSmartPtr<IGraphicsScene> m_sceneManager;

            f32 mHeightUpdateCountDown = 0.0f;
            f32 mHeightUpdateRate = 0.0f;
            Ogre::Vector3 mTerrainPos = Ogre::Vector3::ZERO;

            u16 m_terrainSize = 129;
            f32 m_terrainWorldSize = 512.0f;

            bool mTerrainsImported = false;
            bool mPaging = false;

            bool m_isVisible = false;

            Array<SmartPtr<ITerrainBlendMap>> m_blendLayers;

            static u32 m_ext;
        };
    }  // end namespace render
}  // end namespace fb

#endif  // CTerrain_h__
