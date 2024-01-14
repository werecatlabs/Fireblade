#ifndef ImageComponent_h__
#define ImageComponent_h__

#include <FBCore/Scene/Components/UI/UIComponent.h>

namespace fb
{
    namespace scene
    {
        /** Image component.
         *
         *  This component is used to display an image on the screen.
         *
         *  @note This component is not yet implemented.
         *
         *  @see UIComponent
         */
        class Image : public UIComponent
        {
        public:
            static const String TextureStr;

            /** Constructor. */
            Image();

            /** Destructor. */
            ~Image() override;

            /** @copydoc UIComponent::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc UIComponent::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc UIComponent::updateMaterials */
            void updateMaterials() override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IObject::isValid */
            bool isValid() const override;

            SmartPtr<ui::IUIImage> getImage() const;
            void setImage( SmartPtr<ui::IUIImage> image );

            SmartPtr<render::ITexture> getTexture() const;
            void setTexture( SmartPtr<render::ITexture> texture );

            String getTextureName() const;
            void setTextureName( const String &textureName );

            FB_CLASS_REGISTER_DECL;

        protected:
            /** Creates the image. */
            void createUI() override;

            /** Sets up the material. */
            void setupMaterial();

            SmartPtr<ui::IUIImage> m_image;
            SmartPtr<render::ITexture> m_texture;
            SmartPtr<render::IMaterial> m_material;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // ImageComponent_h__
