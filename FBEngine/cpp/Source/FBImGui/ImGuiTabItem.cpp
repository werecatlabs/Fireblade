#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTabItem.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

#include "ImGuiApplication.h"

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiTabItem, CImGuiElement<IUITabItem> );

    ImGuiTabItem::ImGuiTabItem() = default;

    ImGuiTabItem::~ImGuiTabItem() = default;

    void ImGuiTabItem::update()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto ui = applicationManager->getUI();
        auto uiApplication = fb::static_pointer_cast<ImGuiApplication>( ui->getApplication() );

        auto name = getLabel();
        if( ImGui::BeginTabItem( name.c_str() ) )
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    uiApplication->createElement( child );
                }
            }

            ImGui::EndTabItem();
        }
    }

    auto ImGuiTabItem::getLabel() const -> String
    {
        return m_label;
    }

    void ImGuiTabItem::setLabel( const String &label )
    {
        m_label = label;
    }
}  // namespace fb::ui
