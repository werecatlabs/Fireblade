#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiLabelTextInputPair.h>

namespace fb
{
    namespace ui
    {
        ImGuiLabelTextInputPair::ImGuiLabelTextInputPair()
        {
        }

        ImGuiLabelTextInputPair::~ImGuiLabelTextInputPair()
        {
        }

        String ImGuiLabelTextInputPair::getLabel() const
        {
            return m_label;
        }

        void ImGuiLabelTextInputPair::setLabel( const String &label )
        {
            m_label = label;
        }

        String ImGuiLabelTextInputPair::getValue() const
        {
            return m_value;
        }

        void ImGuiLabelTextInputPair::setValue( const String &value )
        {
            m_value = value;
        }
    } // end namespace ui
}     // end namespace fb
