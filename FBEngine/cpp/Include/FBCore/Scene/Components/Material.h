#ifndef MaterialComponent_h__
#define MaterialComponent_h__

#include <FBCore/Scene/Components/Component.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace scene
    {
        class Material : public Component
        {
        public:
            static const String mainTextureStr;
            static const String materialStr;
            static const String indexStr;

            Material();
            ~Material() override;

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

            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            String getMaterialPath() const;
            void setMaterialPath( const String &path );

            SmartPtr<render::IMaterial> getMaterial() const;
            void setMaterial( SmartPtr<render::IMaterial> material );

            void updateMaterial();

            void updateDependentComponents();

            void updateImageComponent();

            u32 getIndex() const;

            void setIndex( u32 index );

            SmartPtr<IEventListener> getMaterialObjectListener() const;

            void setMaterialObjectListener( SmartPtr<IEventListener> materialObjectListener );

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialStateObjectListener : public IEventListener
            {
            public:
                MaterialStateObjectListener();
                ~MaterialStateObjectListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                SmartPtr<Material> getOwner() const;

                void setOwner( SmartPtr<Material> owner );

            protected:
                AtomicSmartPtr<Material> m_owner;
            };

            class MaterialStateListener : public IStateListener
            {
            public:
                MaterialStateListener() = default;
                explicit MaterialStateListener( Material *owner );
                ~MaterialStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<Material> getOwner() const;
                void setOwner( SmartPtr<Material> owner );

            protected:
                AtomicSmartPtr<Material> m_owner;
            };

            void updateFlags();

            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            AtomicSmartPtr<IEventListener> m_materialObjectListener;
            SmartPtr<IStateListener> m_materialListener;
            SmartPtr<render::IMaterial> m_material;
            s32 m_index = 0;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // MaterialComponent_h__
