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
            /** Constructor. */
            Image();

            /** Destructor. */
            ~Image() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IComponent::updateDirty */
            void updateDirty( u32 flags, u32 oldFlags ) override;

            /** @copydoc IComponent::parentChanged */
            void parentChanged( SmartPtr<IActor> newParent, SmartPtr<IActor> oldParent ) override;

            /** @copydoc IComponent::hierarchyChanged */
            void hierarchyChanged() override;

            /** @copydoc IComponent::childAdded */
            void childAdded( SmartPtr<IActor> child ) override;

            /** @copydoc IComponent::childRemoved */
            void childRemoved( SmartPtr<IActor> child ) override;

            /** @copydoc UIComponent::updateDimensions */
            void updateDimensions() override;

            /** @copydoc UIComponent::updateMaterials */
            void updateMaterials() override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            /** @copydoc IObject::isValid */
            bool isValid() const override;

            /** @copydoc IObject::getChildObjects */
            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            SmartPtr<ui::IUIImage> getImage() const;
            void setImage( SmartPtr<ui::IUIImage> image );

            SmartPtr<render::ITexture> getTexture() const;
            void setTexture( SmartPtr<render::ITexture> texture );

            String getTextureName() const;
            void setTextureName( const String &textureName );

            FB_CLASS_REGISTER_DECL;

        protected:
            /** @copydoc UIComponent::handleComponentEvent */
            IFSM::ReturnType handleComponentEvent( u32 state, IFSM::Event eventType ) override;

            /** Creates the image. */
            void createImage();

            /** Sets up the material. */
            void setupMaterial();

            SmartPtr<ui::IUIImage> m_image;
            SmartPtr<render::ITexture> m_texture;
            SmartPtr<render::IMaterial> m_material;
        };
    }  // namespace scene
}  // end namespace fb

#endif  // ImageComponent_h__
