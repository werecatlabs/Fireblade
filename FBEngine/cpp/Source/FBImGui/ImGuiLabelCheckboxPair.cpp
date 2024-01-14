#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiLabelCheckboxPair.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiLabelCheckboxPair, CImGuiElement<IUILabelTogglePair> );

    ImGuiLabelCheckboxPair::ImGuiLabelCheckboxPair() = default;

    ImGuiLabelCheckboxPair::~ImGuiLabelCheckboxPair()
    {
        unload( nullptr );
    }

    void ImGuiLabelCheckboxPair::load( SmartPtr<ISharedObject> data )
    {
    }

    void ImGuiLabelCheckboxPair::unload( SmartPtr<ISharedObject> data )
    {
    }

    auto ImGuiLabelCheckboxPair::getLabel() const -> String
    {
        return m_label;
    }

    void ImGuiLabelCheckboxPair::setLabel( const String &val )
    {
        m_label = val;
    }

    auto ImGuiLabelCheckboxPair::getValue() const -> bool
    {
        return m_value;
    }

    void ImGuiLabelCheckboxPair::setValue( bool val )
    {
        m_value = val;
    }

    auto ImGuiLabelCheckboxPair::getShowLabel() const -> bool
    {
        return false;
    }

    void ImGuiLabelCheckboxPair::setShowLabel( bool showLabel )
    {
    }

}  // namespace fb::ui
