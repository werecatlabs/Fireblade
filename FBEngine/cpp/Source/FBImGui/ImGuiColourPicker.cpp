#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiColourPicker.h>

#include "ImCurveEdit.h"

namespace fb::ui
{

    FB_CLASS_REGISTER_DERIVED( fb, ImGuiColourPicker, CImGuiElement<IUIColourPicker> );

    ImGuiColourPicker::ImGuiColourPicker() = default;

    ImGuiColourPicker::~ImGuiColourPicker() = default;

    void ImGuiColourPicker::update()
    {
        auto colour = getColour();
        auto name = getName();
        ImGui::ColorEdit3( name.c_str(), &colour.r );
    }

    void ImGuiColourPicker::setGradient( const ColourF &startColour, const ColourF &endColour )
    {
    }

    auto ImGuiColourPicker::getGradient() const -> fb::Pair<fb::ColourF, fb::ColourF>
    {
        return {};
    }

    void ImGuiColourPicker::setColourFormat( ColourFormat format )
    {
    }

    auto ImGuiColourPicker::getColourFormat() const -> fb::ui::IUIColourPicker::ColourFormat
    {
        return ColourFormat::RGB;
    }

}  // namespace fb::ui
