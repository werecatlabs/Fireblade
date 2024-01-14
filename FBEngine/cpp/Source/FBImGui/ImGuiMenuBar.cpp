#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiMenuBar.h>
#include <FBCore/Interface/UI/IUIMenu.h>

namespace fb::ui
{
    ImGuiMenuBar::ImGuiMenuBar() = default;

    ImGuiMenuBar::~ImGuiMenuBar() = default;

    void ImGuiMenuBar::update()
    {
        for( auto menu : m_menus )
        {
            menu->update();
        }
    }

    void ImGuiMenuBar::addMenu( SmartPtr<IUIMenu> menu )
    {
        addChild( menu );

        m_menus.push_back( menu );
    }

    void ImGuiMenuBar::removeMenu( SmartPtr<IUIMenu> menu )
    {
        removeChild( menu );

        auto it = std::find( m_menus.begin(), m_menus.end(), menu );
        if( it != m_menus.end() )
        {
            m_menus.erase( it );
        }
    }

    auto ImGuiMenuBar::getMenus() const -> Array<SmartPtr<IUIMenu>>
    {
        return m_menus;
    }

    void ImGuiMenuBar::setMenus( Array<SmartPtr<IUIMenu>> menus )
    {
        m_menus = menus;
    }

}  // namespace fb::ui
