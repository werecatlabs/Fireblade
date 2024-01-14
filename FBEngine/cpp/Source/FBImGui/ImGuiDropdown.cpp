#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiDropdown.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiDropdown, CImGuiElement<IUIDropdown> );

    ImGuiDropdown::ImGuiDropdown() = default;

    ImGuiDropdown::~ImGuiDropdown() = default;

    auto ImGuiDropdown::getOptions() const -> Array<String>
    {
        return m_options;
    }

    void ImGuiDropdown::setOptions( const Array<String> &options )
    {
        m_options = options;
    }

    auto ImGuiDropdown::getSelectedOption() const -> u32
    {
        return m_selectedOption;
    }

    void ImGuiDropdown::setSelectedOption( u32 selectedOption )
    {
        m_selectedOption = selectedOption;
    }
}  // namespace fb::ui
