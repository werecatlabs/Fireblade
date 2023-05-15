#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiToolbar.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiToolbar, CImGuiElement<IUIToolbar> );

        ImGuiToolbar::ImGuiToolbar()
        {
        }

        ImGuiToolbar::~ImGuiToolbar()
        {
        }
    } // end namespace ui
}     // end namespace fb
