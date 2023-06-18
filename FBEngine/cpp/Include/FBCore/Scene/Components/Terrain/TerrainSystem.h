#ifndef TerrainSystem_h__
#define TerrainSystem_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Scene/Components/Component.h>

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

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IComponent::updateTransform */
            void updateTransform() override;

            s32 calculateNumLayers() const;
            s32 getNumLayers() const;

            void setNumLayers( s32 numLayers );

            void resizeLayermap();

            void updateLayers();

            FB_CLASS_REGISTER_DECL;

        protected:
            class TerrainSharedObjectListener : public SharedObject<IEventListener>
            {
            public:
                TerrainSharedObjectListener();

                ~TerrainSharedObjectListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

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

            SmartPtr<TerrainSharedObjectListener> m_terrainListener;

            /** Handles events that affect the state of the TerrainSystem component. */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            /** Pointer to a render::ITerrain object. */
            SmartPtr<render::ITerrain> m_terrain;

            /** Pointer to a render::ISceneNode object. */
            SmartPtr<render::ISceneNode> m_node;

            s32 m_numLayers = 0;
            std::vector<SmartPtr<render::ITexture>> m_layers;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // TerrainSystem_h__
