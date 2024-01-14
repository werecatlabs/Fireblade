#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiToolbar.h>

namespace fb::ui
{

    FB_CLASS_REGISTER_DERIVED( fb, ImGuiToolbar, CImGuiElement<IUIToolbar> );

    ImGuiToolbar::ImGuiToolbar() = default;

    ImGuiToolbar::~ImGuiToolbar() = default;

}  // namespace fb::ui
