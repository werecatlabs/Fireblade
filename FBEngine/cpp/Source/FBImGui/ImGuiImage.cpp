#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiImage.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

#include "ImGuiApplication.h"

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiImage, CImGuiElement<IUIImage> );

        ImGuiImage::ImGuiImage()
        {
        }

        ImGuiImage::~ImGuiImage()
        {
        }

        void ImGuiImage::update()
        {
            auto size = getSize();

            if( auto texture = getTexture() )
            {
                if( !texture->isLoaded() )
                {
                    texture->load(nullptr);
                }

                m_textureHandle = texture->getTextureHandle();

                ImGui::Image( (ImTextureID)&m_textureHandle, ImVec2( size.x, size.y ) );

                auto dropTarget = getDropTarget();
                if( dropTarget )
                {
                    if( ImGui::BeginDragDropTarget() )
                    {
                        auto payload = ImGui::AcceptDragDropPayload( "_TREENODE" );
                        if( payload )
                        {
                            auto pData = static_cast<const char *>( payload->Data );
                            auto dataSize = payload->DataSize;
                            auto data = String( pData, dataSize );

                            auto menuItemId = getElementId();

                            auto args = Array<Parameter>();
                            args.resize(1);

                            args[0].setStr( data );

                            dropTarget->handleEvent( IEvent::Type::UI, IEvent::handleDrop, args,
                                                     this, this, nullptr );
                        }

                        ImGui::EndDragDropTarget();
                    }
                }
            }
            else
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                SmartPtr<ImGuiApplication> application = ui->getApplication();
                auto emptyTexture = application->getEmptyTexture();

                ImGui::Image( (ImTextureID)emptyTexture, ImVec2( size.x, size.y ) );
            }
        }

        String ImGuiImage::getLabel() const
        {
            return m_label;
        }

        void ImGuiImage::setLabel( const String &label )
        {
            m_label = label;
        }

        void ImGuiImage::setMaterialName( const String &materialName )
        {
        }

        String ImGuiImage::getMaterialName() const
        {
            return "";
        }

        void ImGuiImage::setMaterial( SmartPtr<render::IMaterial> material )
        {
            m_material = material;
        }

        SmartPtr<render::IMaterial> ImGuiImage::getMaterial() const
        {
            return m_material;
        }

        void ImGuiImage::setTexture( SmartPtr<render::ITexture> texture )
        {
            m_texture = texture;
        }

        SmartPtr<render::ITexture> ImGuiImage::getTexture() const
        {
            return m_texture;
        }
    }  // end namespace ui
}  // end namespace fb
