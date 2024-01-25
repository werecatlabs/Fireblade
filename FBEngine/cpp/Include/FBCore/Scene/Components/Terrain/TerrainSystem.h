#ifndef TerrainSystem_h__
#define TerrainSystem_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace scene
    {
        class TerrainSystem : public Component
        {
        public:
            /** Creates a TerrainSystem object. */
            TerrainSystem();

            /** Copy constructor is deleted. */
            TerrainSystem( const TerrainSystem &other ) = delete;

            /** Destructor. */
            ~TerrainSystem() override;

            /** @copydoc Component::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc Component::updateDirty */
            void updateFlags( u32 flags, u32 oldFlags ) override;

            /** @copydoc Component::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc Component::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::updateTransform */
            void updateTransform() override;

            s32 calculateNumLayers() const;
            s32 getNumLayers() const;

            SmartPtr<TerrainLayer> addLayer();

            void removeLayer( s32 index );

            void removeLayer( SmartPtr<TerrainLayer> layer );

            void setNumLayers( s32 numLayers );

            void resizeLayermap();

            void updateLayers();

            SmartPtr<render::ITexture> getHeightMap() const;

            void setHeightMap( SmartPtr<render::ITexture> heightMap );

            FB_CLASS_REGISTER_DECL;

        protected:
            class TerrainSharedObjectListener : public IEventListener
            {
            public:
                TerrainSharedObjectListener();

                ~TerrainSharedObjectListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                void loadingStateChanged( ISharedObject *sharedObject, LoadingState oldState,
                                          LoadingState newState );

                bool destroy( void *ptr );

                TerrainSystem *getOwner() const;

                void setOwner( TerrainSystem *owner );

            protected:
                TerrainSystem *m_owner = nullptr;
            };

            /**
             * Creates a terrain object.
             *
             * This function creates a new terrain object and stores it in the
             * `m_terrain` member variable. The terrain is rendered to the screen
             * using the `m_node` member variable.
             *
             * @note This function must be called before the terrain can be
             * rendered to the screen.
             */
            void createTerrain();
            void updateVisibility() override;

            SmartPtr<TerrainSharedObjectListener> m_terrainListener;

            /** Handles events that affect the state of the TerrainSystem component. */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            SmartPtr<render::ITexture> m_heightMap;

            /** Pointer to a render::ITerrain object. */
            SmartPtr<render::ITerrain> m_terrain;

            s32 m_numLayers = 0;

            std::vector<SmartPtr<render::ITexture>> m_layers;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // TerrainSystem_h__
