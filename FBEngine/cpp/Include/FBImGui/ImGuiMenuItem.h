#ifndef __ImGuiMenuItem_h__
#define __ImGuiMenuItem_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIMenuItem.h>

namespace fb
{
    namespace ui
    {
        
        class ImGuiMenuItem : public CImGuiElement<IUIMenuItem>
        {
        public:
            ImGuiMenuItem();
            ~ImGuiMenuItem() override;

            void update() override;

            Type getMenuItemType() const override;
            void setMenuItemType( Type type ) override;

            String getText() const override;
            void setText( const String &text ) override;

            String getHelp() const override;
            void setHelp( const String &help ) override;

            FB_CLASS_REGISTER_DECL;

        private:
            Type m_type = Type::Normal;
            String m_text;
            String m_help;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // CEGUIText_h__
