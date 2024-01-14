#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiImage.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

#include "ImGuiApplication.h"

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiImage, CImGuiElement<IUIImage> );

    ImGuiImage::ImGuiImage() = default;

    ImGuiImage::~ImGuiImage() = default;

    void ImGuiImage::update()
    {
        auto size = getSize();

        if( auto texture = getTexture() )
        {
            if( !texture->isLoaded() )
            {
                texture->load( nullptr );
            }

            void *iTexture = nullptr;
            texture->getTextureFinal( &iTexture );
            m_textureHandle = reinterpret_cast<size_t>( iTexture );

            //m_textureHandle = texture->getTextureHandle();

            if( iTexture != nullptr )
            {
                ImGui::Image( static_cast<ImTextureID>( iTexture ), ImVec2( size.x, size.y ) );

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
                            args.resize( 1 );

                            args[0].setStr( data );

                            dropTarget->handleEvent( IEvent::Type::UI, IEvent::handleDrop, args, this,
                                                     this, nullptr );
                        }

                        ImGui::EndDragDropTarget();
                    }
                }
            }
        }
        else
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            SmartPtr<ImGuiApplication> application = ui->getApplication();
            auto emptyTexture = application->getEmptyTexture();

            ImGui::Image( static_cast<ImTextureID>( emptyTexture ), ImVec2( size.x, size.y ) );

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
                        args.resize( 1 );

                        args[0].setStr( data );

                        dropTarget->handleEvent( IEvent::Type::UI, IEvent::handleDrop, args, this, this,
                                                 nullptr );
                    }

                    ImGui::EndDragDropTarget();
                }
            }
        }
    }

    auto ImGuiImage::getLabel() const -> String
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

    auto ImGuiImage::getMaterialName() const -> String
    {
        return "";
    }

    void ImGuiImage::setMaterial( SmartPtr<render::IMaterial> material )
    {
        m_material = material;
    }

    auto ImGuiImage::getMaterial() const -> SmartPtr<render::IMaterial>
    {
        return m_material;
    }

    void ImGuiImage::setTexture( SmartPtr<render::ITexture> texture )
    {
        m_texture = texture;
    }

    auto ImGuiImage::getTexture() const -> SmartPtr<render::ITexture>
    {
        return m_texture;
    }
}  // namespace fb::ui
