
#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiCollapsingHeader.h>
#include <imgui.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiCollapsingHeader, CImGuiElement<IUICollapsingHeader> );

        ImGuiCollapsingHeader::ImGuiCollapsingHeader()
        {
        }

        ImGuiCollapsingHeader::~ImGuiCollapsingHeader()
        {
        }

        void ImGuiCollapsingHeader::update()
        {
                       auto name = getName();
            if (ImGui::CollapsingHeader(name.c_str()))
            {
                           }
        }

    }  // namespace ui
}  // namespace fb
