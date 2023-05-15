#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiButton.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiButton, CImGuiElement<IUIButton> );

        ImGuiButton::ImGuiButton()
        {
        }

        ImGuiButton::~ImGuiButton()
        {
        }

        String ImGuiButton::getLabel() const
        {
            return m_label;
        }

        void ImGuiButton::setLabel( const String &label )
        {
            m_label = label;
        }

        void ImGuiButton::setTextSize( f32 textSize )
        {
        }

        f32 ImGuiButton::getTextSize() const
        {
            return 0.0f;
        }
    } // end namespace ui
}     // end namespace fb
