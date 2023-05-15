#ifndef MaterialComponent_h__
#define MaterialComponent_h__

#include <FBCore/Scene/Components/Component.h>

namespace fb
{
    namespace scene
    {

        class Material : public Component
        {
        public:
            Material();
            ~Material() override;

            /** @copydoc BaseComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc BaseComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            void play();
            void edit();

            /** @copydoc BaseComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc BaseComponent::setProperties */
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

            FB_CLASS_REGISTER_DECL;

        protected:
            class MaterialStateListener : public CSharedObject<IStateListener>
            {
            public:
                MaterialStateListener() = default;
                MaterialStateListener( Material *owner );
                ~MaterialStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<Material> getOwner() const;
                void setOwner( SmartPtr<Material> owner );

            protected:
                SmartPtr<Material> m_owner;
            };

            void updateDirty();

            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            SmartPtr<IStateListener> m_materialListener;
            SmartPtr<render::IMaterial> m_material;
            s32 m_index = 0;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // MaterialComponent_h__
