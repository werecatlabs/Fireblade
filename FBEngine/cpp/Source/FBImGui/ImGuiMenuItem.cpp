#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiMenuItem.h>

namespace fb
{
    namespace ui
    {
        FB_CLASS_REGISTER_DERIVED( fb, ImGuiMenuItem, CImGuiElement<IUIMenuItem> );

        ImGuiMenuItem::ImGuiMenuItem()
        {
        }

        ImGuiMenuItem::~ImGuiMenuItem()
        {
        }

        void ImGuiMenuItem::update()
        {
        }

        IUIMenuItem::Type ImGuiMenuItem::getMenuItemType() const
        {
            return m_type;
        }

        void ImGuiMenuItem::setMenuItemType( Type type )
        {
            m_type = type;
        }

        String ImGuiMenuItem::getText() const
        {
            return m_text;
        }

        void ImGuiMenuItem::setText( const String &text )
        {
            m_text = text;
        }

        String ImGuiMenuItem::getHelp() const
        {
            return m_help;
        }

        void ImGuiMenuItem::setHelp( const String &help )
        {
            m_help = help;
        }
    } // end namespace ui
}     // end namespace fb
