#ifndef SkyboxComponent_h__
#define SkyboxComponent_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Interface/System/IStateListener.h>

namespace fb
{
    namespace scene
    {
        class Skybox : public Component
        {
        public:
            Skybox();
            ~Skybox() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void updateDirty( u32 flags, u32 oldFlags ) override;

            SmartPtr<Properties> getProperties() const override;
            void setProperties( SmartPtr<Properties> properties ) override;

            SmartPtr<render::IMaterial> getMaterial() const;

            void updateMaterials();

            f32 getDistance() const;
            void setDistance( f32 distance );

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialSharedListener : public SharedObject<IEventListener>
            {
            public:
                MaterialSharedListener() = default;
                ~MaterialSharedListener() = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                void loadingStateChanged( ISharedObject *sharedObject, LoadingState oldState,
                                          LoadingState newState );

                bool destroy( void *ptr );

                SmartPtr<Skybox> getOwner() const;
                void setOwner( SmartPtr<Skybox> owner );

            private:
                WeakPtr<Skybox> m_owner = nullptr;
            };

            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            void setupMaterial();

            Array<SmartPtr<render::ITexture>> m_textures;
            SmartPtr<MaterialSharedListener> m_materialSharedListener;
            SmartPtr<render::IMaterial> m_material;

            f32 m_distance = 50000.0f;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // SkyboxComponent_h__
