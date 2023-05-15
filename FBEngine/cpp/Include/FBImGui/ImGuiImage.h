#ifndef ImGuiImage_h__
#define ImGuiImage_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIImage.h>

namespace fb
{
    namespace ui
    {
        class ImGuiImage : public CImGuiElement<IUIImage>
        {
        public:
            ImGuiImage();
            ~ImGuiImage() override;

            void update() override;

            String getLabel() const;
            void setLabel( const String &label );

            /** Sets the image material name. */
            void setMaterialName( const String &materialName );

            /** Gets the image material name. */
            String getMaterialName() const;

            /** Sets the image material. */
            void setMaterial( SmartPtr<render::IMaterial> material );

            /** Gets the image material. */
            SmartPtr<render::IMaterial> getMaterial() const;

            void setTexture( SmartPtr<render::ITexture> texture );
            SmartPtr<render::ITexture> getTexture() const;

            FB_CLASS_REGISTER_DECL;

        protected:
            String m_label;
            SmartPtr<render::IMaterial> m_material;
            SmartPtr<render::ITexture> m_texture;
            size_t m_textureHandle = 0;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiImage_h__
