#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiColourPicker.h>

#include "ImCurveEdit.h"

namespace fb
{
    namespace ui
    {

        FB_CLASS_REGISTER_DERIVED( fb, ImGuiColourPicker, CImGuiElement<IUIColourPicker> );

        ImGuiColourPicker::ImGuiColourPicker()
        {
        }

        ImGuiColourPicker::~ImGuiColourPicker()
        {
        }

        void ImGuiColourPicker::update()
        {
            auto colour = getColour();
            auto name = getName();
            ImGui::ColorEdit3(name.c_str(), &colour.r);
        }

    }  // namespace ui
}  // namespace fb
