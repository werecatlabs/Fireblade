#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTabBar.h>
#include <FBCore/FBCore.h>
#include <imgui.h>

#include "ImGuiTabItem.h"

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiTabBar, CImGuiElement<IUITabBar> );

        ImGuiTabBar::ImGuiTabBar()
        {
        }

        ImGuiTabBar::~ImGuiTabBar()
        {
        }

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

        SmartPtr<IUITabItem> ImGuiTabBar::addTabItem()
        {
            auto tabItem = fb::make_ptr<ImGuiTabItem>();
            m_tabItems.push_back( tabItem );
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
    }  // end namespace ui
}  // end namespace fb
