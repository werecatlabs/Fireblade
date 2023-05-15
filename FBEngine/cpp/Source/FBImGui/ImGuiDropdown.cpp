#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiDropdown.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiDropdown, CImGuiElement<IUIDropdown> );

        ImGuiDropdown::ImGuiDropdown()
        {
        }

        ImGuiDropdown::~ImGuiDropdown()
        {
        }

        Array<String> ImGuiDropdown::getOptions() const
        {
            return m_options;
        }

        void ImGuiDropdown::setOptions( const Array<String> &options )
        {
            m_options = options;
        }

        u32 ImGuiDropdown::getSelectedOption() const
        {
            return m_selectedOption;
        }

        void ImGuiDropdown::setSelectedOption( u32 selectedOption )
        {
            m_selectedOption = selectedOption;
        }
    } // end namespace ui
}     // end namespace fb
