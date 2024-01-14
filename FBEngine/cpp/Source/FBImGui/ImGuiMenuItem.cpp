#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiMenuItem.h>

namespace fb::ui
{
    FB_CLASS_REGISTER_DERIVED( fb, ImGuiMenuItem, CImGuiElement<IUIMenuItem> );

    ImGuiMenuItem::ImGuiMenuItem() = default;

    ImGuiMenuItem::~ImGuiMenuItem() = default;

    void ImGuiMenuItem::update()
    {
    }

    auto ImGuiMenuItem::getMenuItemType() const -> IUIMenuItem::Type
    {
        return m_type;
    }

    void ImGuiMenuItem::setMenuItemType( Type type )
    {
        m_type = type;
    }

    auto ImGuiMenuItem::getText() const -> String
    {
        return m_text;
    }

    void ImGuiMenuItem::setText( const String &text )
    {
        m_text = text;
    }

    auto ImGuiMenuItem::getHelp() const -> String
    {
        return m_help;
    }

    void ImGuiMenuItem::setHelp( const String &help )
    {
        m_help = help;
    }
}  // namespace fb::ui
