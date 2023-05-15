#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTabBar.h>
#include <imgui.h>

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
                ImGui::EndTabBar();
            }
        }

    }  // end namespace ui
}  // end namespace fb
