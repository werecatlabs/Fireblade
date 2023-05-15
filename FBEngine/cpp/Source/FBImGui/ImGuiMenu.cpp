#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiMenu.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        ImGuiMenu::ImGuiMenu()
        {
        }

        ImGuiMenu::~ImGuiMenu()
        {
        }

        void ImGuiMenu::load( SmartPtr<ISharedObject> data )
        {
            
        }

        void ImGuiMenu::unload( SmartPtr<ISharedObject> data )
        {
            m_menuItems.clear();
            CImGuiElement<IUIMenu>::unload( data );
        }

        void ImGuiMenu::update()
        {
            for(auto menuItem : m_menuItems)
            {
                menuItem->update();
            }
        }

        void ImGuiMenu::addMenuItem( SmartPtr<IUIElement> item )
        {
            m_menuItems.push_back( item );
        }

        void ImGuiMenu::removeMenuItem( SmartPtr<IUIElement> item )
        {
            auto it = std::find( m_menuItems.begin(), m_menuItems.end(), item );
            if(it != m_menuItems.end())
            {
                m_menuItems.erase( it );
            }
        }

        Array<SmartPtr<IUIElement>> ImGuiMenu::getMenuItems() const
        {
            return m_menuItems;
        }

        void ImGuiMenu::setMenuItems( Array<SmartPtr<IUIElement>> val )
        {
            m_menuItems = val;
        }

        void ImGuiMenu::setCursorPosition( u32 cursorIdx )
        {
            m_cursorPosition = cursorIdx;
        }

        u32 ImGuiMenu::getCursorPosition() const
        {
            return m_cursorPosition;
        }

        void ImGuiMenu::setNumListItems( u32 numListItems )
        {
        }

        u32 ImGuiMenu::getNumListItems() const
        {
            return 0;
        }

        void ImGuiMenu::setCurrentItemIndex( u32 index )
        {
            m_currentItemIndex = index;
        }

        u32 ImGuiMenu::getCurrentItemIndex() const
        {
            return m_currentItemIndex;
        }

        void ImGuiMenu::incrementCursor()
        {
        }

        void ImGuiMenu::decrementCursor()
        {
        }

        s32 ImGuiMenu::getNumMenuItems() const
        {
            return static_cast<s32>(m_menuItems.size());
        }

        String ImGuiMenu::getLabel() const
        {
            return m_label;
        }

        void ImGuiMenu::setLabel( const String &label )
        {
            m_label = label;
        }
    } // end namespace ui
}     // end namespace fb
