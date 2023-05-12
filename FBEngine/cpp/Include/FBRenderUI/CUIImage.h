#ifndef _CGUIIMAGE_H
#define _CGUIIMAGE_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIImage.h>
#include <FBRenderUI/CUIElement.h>

namespace fb
{
    namespace ui
    {
        
        class CUIImage : public CUIElement<IUIImage>
        {
        public:
            CUIImage();
            ~CUIImage() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void onToggleVisibility() override;

            void setMaterialName( const String &materialName ) ;
            String getMaterialName() const ;

            /** Sets the texture. */
            void setTexture( SmartPtr<render::ITexture> texture );

            /** Gets the texture. */
            SmartPtr<render::ITexture> getTexture() const;

            void setMaterial( SmartPtr<render::IMaterial> material ) ;
            SmartPtr<render::IMaterial> getMaterial() const ;

            void setPosition( const Vector2F &position ) override;

            void setSize( const Vector2F &size ) override;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            /** @copydoc IObject::isValid */
            bool isValid() const override;

            void _getObject( void **ppObject ) const override;

            SmartPtr<render::IOverlayElementContainer> getContainerObject() const;
            void setContainerObject( SmartPtr<render::IOverlayElementContainer> container );

            void handleStateChanged( SmartPtr<IState> &state );

            FB_CLASS_REGISTER_DECL;

        private:
            SmartPtr<render::IOverlayElementContainer> m_container;
        };
    } // end namespace ui
}     // end namespace fb

#endif
