#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiCollapsingHeader.h>
#include <FBImGui/ImGuiApplication.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiCollapsingHeader, CImGuiElement<IUICollapsingHeader> );

    ImGuiCollapsingHeader::ImGuiCollapsingHeader() = default;

    ImGuiCollapsingHeader::~ImGuiCollapsingHeader() = default;

    void ImGuiCollapsingHeader::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto ui = applicationManager->getUI();
        auto uiApplication = fb::static_pointer_cast<ImGuiApplication>( ui->getApplication() );

        auto name = getName();
        if( ImGui::CollapsingHeader( name.c_str() ) )
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    uiApplication->createElement( child );
                }
            }
        }
    }

}  // namespace fb::ui
