#ifndef UIImage_h__
#define UIImage_h__

#include <FBGraphicsOgreNext/UI/UIElement.h>
#include <FBCore/Interface/UI/IUIImage.h>

namespace fb
{
    namespace ui
    {

        class UIImage : public UIElement<ui::IUIImage>
        {
        public:
            UIImage();
            ~UIImage();

            void load( SmartPtr<ISharedObject> data ) override;

            void unload( SmartPtr<ISharedObject> data ) override;

            void setTexture( SmartPtr<render::ITexture> texture ) override;

            SmartPtr<render::ITexture> getTexture() const override;

            void setMaterialName( const String &materialName ) override;

            String getMaterialName() const override;

            void setMaterial( SmartPtr<render::IMaterial> material ) override;

            SmartPtr<render::IMaterial> getMaterial() const override;

            void handleStateChanged( SmartPtr<IState> &state );

        protected:
            class MaterialStateListener : public IStateListener
            {
            public:
                MaterialStateListener() = default;
                ~MaterialStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<UIImage> getOwner() const;
                void setOwner( SmartPtr<UIImage> owner );

            protected:
                AtomicSmartPtr<UIImage> m_owner;
            };

            void createDatablock();

            void setupMaterial( SmartPtr<render::IMaterial> material );

            void updateMaterial();

            String m_datablockName;
            SmartPtr<render::ITexture> m_texture;
            Ogre::HlmsColibriDatablock *m_datablock = nullptr;
            SmartPtr<IStateListener> m_materialStateListener;
            SmartPtr<render::IMaterial> m_material;
            Colibri::CustomShape *m_renderable = nullptr;
        };

    }  // namespace ui
}  // namespace fb

#endif  // UIImage_h__
