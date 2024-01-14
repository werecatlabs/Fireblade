#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiMenu.h>
#include <FBCore/FBCore.h>

namespace fb::ui
{
    ImGuiMenu::ImGuiMenu() = default;

    ImGuiMenu::~ImGuiMenu() = default;

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
        for( auto menuItem : m_menuItems )
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
        if( it != m_menuItems.end() )
        {
            m_menuItems.erase( it );
        }
    }

    auto ImGuiMenu::getMenuItems() const -> Array<SmartPtr<IUIElement>>
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

    auto ImGuiMenu::getCursorPosition() const -> u32
    {
        return m_cursorPosition;
    }

    void ImGuiMenu::setNumListItems( u32 numListItems )
    {
    }

    auto ImGuiMenu::getNumListItems() const -> u32
    {
        return 0;
    }

    void ImGuiMenu::setCurrentItemIndex( u32 index )
    {
        m_currentItemIndex = index;
    }

    auto ImGuiMenu::getCurrentItemIndex() const -> u32
    {
        return m_currentItemIndex;
    }

    void ImGuiMenu::incrementCursor()
    {
    }

    void ImGuiMenu::decrementCursor()
    {
    }

    auto ImGuiMenu::getNumMenuItems() const -> s32
    {
        return static_cast<s32>( m_menuItems.size() );
    }

    auto ImGuiMenu::getLabel() const -> String
    {
        return m_label;
    }

    void ImGuiMenu::setLabel( const String &label )
    {
        m_label = label;
    }
}  // namespace fb::ui
