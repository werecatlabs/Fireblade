#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiTabItem.h>
#include <imgui.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiTabItem, CImGuiElement<IUITabItem> );

        ImGuiTabItem::ImGuiTabItem()
        {
        }

        ImGuiTabItem::~ImGuiTabItem()
        {
        }

        void ImGuiTabItem::update()
        {
            auto name = getName();
            if( ImGui::BeginTabItem( name.c_str() ) )
            {
                ImGui::EndTabItem();
            }
        }

    }  // end namespace ui
}  // end namespace fb
