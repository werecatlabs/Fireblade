#ifndef _IUIImage_H
#define _IUIImage_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /** Interface for a ui image. */
        class IUIImage : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUIImage() override = default;

            /** Sets the texture. */
            virtual void setTexture( SmartPtr<render::ITexture> texture ) = 0;

            /** Gets the texture. */
            virtual SmartPtr<render::ITexture> getTexture() const = 0;

            /** Sets the image material name. */
            virtual void setMaterialName( const String &materialName ) = 0;

            /** Gets the image material name. */
            virtual String getMaterialName() const = 0;

            /** Sets the image material. */
            virtual void setMaterial( SmartPtr<render::IMaterial> material ) = 0;

            /** Gets the image material. */
            virtual SmartPtr<render::IMaterial> getMaterial() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif
