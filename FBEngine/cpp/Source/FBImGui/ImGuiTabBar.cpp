#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTabBar.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

#include "ImGuiTabItem.h"

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiTabBar, CImGuiElement<IUITabBar> );

    ImGuiTabBar::ImGuiTabBar() = default;

    ImGuiTabBar::~ImGuiTabBar() = default;

    void ImGuiTabBar::update()
    {
        auto name = getName();
        if( ImGui::BeginTabBar( name.c_str() ) )
        {
            for( auto &tabItem : m_tabItems )
            {
                tabItem->update();
            }

            ImGui::EndTabBar();
        }
    }

    auto ImGuiTabBar::addTabItem() -> SmartPtr<IUITabItem>
    {
        auto tabItem = fb::make_ptr<ImGuiTabItem>();
        m_tabItems.emplace_back( tabItem );
        return tabItem;
    }

    void ImGuiTabBar::removeTabItem( SmartPtr<IUITabItem> tabItem )
    {
        auto it = std::find( m_tabItems.begin(), m_tabItems.end(), tabItem );
        if( it != m_tabItems.end() )
        {
            m_tabItems.erase( it );
        }
    }
}  // namespace fb::ui
