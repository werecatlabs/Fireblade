#ifndef __ImGuiMenu_h__
#define __ImGuiMenu_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIMenu.h>

namespace fb
{
    namespace ui
    {
        
        class ImGuiMenu : public CImGuiElement<IUIMenu>
        {
        public:
            ImGuiMenu();
            ~ImGuiMenu() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void update() override;

            void addMenuItem( SmartPtr<IUIElement> item ) override;

            void removeMenuItem( SmartPtr<IUIElement> item ) override;

            Array<SmartPtr<IUIElement>> getMenuItems() const override;

            void setMenuItems( Array<SmartPtr<IUIElement>> val ) override;

            void setCursorPosition( u32 cursorIdx ) override;

            u32 getCursorPosition() const override;

            void setNumListItems( u32 numListItems ) override;

            u32 getNumListItems() const override;

            void setCurrentItemIndex( u32 index ) override;

            u32 getCurrentItemIndex() const override;

            void incrementCursor() override;

            void decrementCursor() override;

            s32 getNumMenuItems() const override;

            String getLabel() const override;
            void setLabel( const String &label ) override;

        protected:
            s32 m_currentItemIndex = 0;
            s32 m_cursorPosition = 0;
            String m_label;
            Array<SmartPtr<IUIElement>> m_menuItems;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // CEGUIText_h__
