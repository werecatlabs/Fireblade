#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiLabelTextInputPair.h>

namespace fb::ui
{
    ImGuiLabelTextInputPair::ImGuiLabelTextInputPair() = default;

    ImGuiLabelTextInputPair::~ImGuiLabelTextInputPair() = default;

    auto ImGuiLabelTextInputPair::getLabel() const -> String
    {
        return m_label;
    }

    void ImGuiLabelTextInputPair::setLabel( const String &label )
    {
        m_label = label;
    }

    auto ImGuiLabelTextInputPair::getValue() const -> String
    {
        return m_value;
    }

    void ImGuiLabelTextInputPair::setValue( const String &value )
    {
        m_value = value;
    }
}  // namespace fb::ui
