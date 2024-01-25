#ifndef CTerrainOgre_h__
#define CTerrainOgre_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/Terrain.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <OgreVector3.h>

namespace fb
{
    namespace render
    {

        /** @brief Terrain implementation for OgreNext */
        class CTerrainOgreNext : public Terrain
        {
        public:
            CTerrainOgreNext();
            ~CTerrainOgreNext() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::reload */
            void reload( SmartPtr<ISharedObject> data ) override;

            void postUpdate() override;

            Vector3F getPosition() const override;
            void setPosition( const Vector3F &position ) override;

            f32 getHeightAtWorldPosition( const Vector3F &position ) const override;

            u16 getSize() const override;

            Vector3F getTerrainSpacePosition( const Vector3F &worldSpace ) const override;

            Array<f32> getHeightData() const override;

            bool isVisible() const override;
            void setVisible( bool visible ) override;

            String getMaterialName() const override;
            void setMaterialName( const String &materialName ) override;

            SmartPtr<ITerrainBlendMap> getBlendMap( u32 index ) override;
            u16 getLayerBlendMapSize() const override;

            SmartPtr<ITerrainRayResult> intersects( const Ray3F &ray ) const override;

            SmartPtr<IMesh> getMesh() const override;

            f32 getHeightScale() const override;
            void setHeightScale( f32 val ) override;

            void _getObject( void **ppObject ) const override;

            SmartPtr<ITexture> getHeightMap() const;

            void setHeightMap( SmartPtr<ITexture> heightMap );

            SmartPtr<IGraphicsScene> getSceneManager() const override;
            void setSceneManager( SmartPtr<IGraphicsScene> sceneManger ) override;

            void setTextureLayer( s32 layer, const String &textureName ) override;

            Ogre::SceneNode *getSceneNode() const;

            void setSceneNode( Ogre::SceneNode *sceneNode );

            FB_CLASS_REGISTER_DECL;

        protected:
            class StateListener : public IStateListener
            {
            public:
                StateListener() = default;
                ~StateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;

                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CTerrainOgreNext *getOwner() const;

                void setOwner( CTerrainOgreNext *owner );

            protected:
                CTerrainOgreNext *m_owner = nullptr;
            };

            void setupMaterial();

            void createTerraInstance();

            Array<f32> m_heightData;

            SmartPtr<ITexture> m_heightMap;
            SmartPtr<IGraphicsScene> m_sceneManager;

            Ogre::SceneNode *m_sceneNode = nullptr;

            Ogre::Terra *m_terra = nullptr;

            /// Listener to make PBS objects also be affected by terrain's shadows
            Ogre::HlmsPbsTerraShadows *m_hlmsPbsTerraShadows = nullptr;

            Ogre::Vector3 m_center = Ogre::Vector3( 0.0f, 0, 0.0f );
            Ogre::Vector3 m_dimensions = Ogre::Vector3( 256, 83.26562, 256 );

            Ogre::Vector3 m_lightDirection = Ogre::Vector3( 0.01f, -0.9958f, 0.3f );

            time_interval m_nextUpdateTime = 0.0;
            f32 m_lightEpsilon = 1e-6f;

            String m_heightMapPath;
            String m_materialName;

            atomic_bool m_isVisible = true;
        };
    } // end namespace render
}     // end namespace fb

#endif  // CTerrainOgre_h__
