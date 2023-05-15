#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/ImGuiMenuBar.h>
#include <FBCore/Interface/UI/IUIMenu.h>

namespace fb
{
    namespace ui
    {
        ImGuiMenubar::ImGuiMenubar()
        {
        }

        ImGuiMenubar::~ImGuiMenubar()
        {
        }

        void ImGuiMenubar::update()
        {
            for(auto menu : m_menus)
            {
                menu->update();
            }
        }

        void ImGuiMenubar::addMenu( SmartPtr<IUIMenu> menu )
        {
            addChild( menu );

            m_menus.push_back( menu );
        }

        void ImGuiMenubar::removeMenu( SmartPtr<IUIMenu> menu )
        {
            removeChild( menu );

            auto it = std::find( m_menus.begin(), m_menus.end(), menu );
            if(it != m_menus.end())
            {
                m_menus.erase( it );
            }
        }

        Array<SmartPtr<IUIMenu>> ImGuiMenubar::getMenus() const
        {
            return m_menus;
        }

        void ImGuiMenubar::setMenus( Array<SmartPtr<IUIMenu>> menus )
        {
            m_menus = menus;
        }

    } // end namespace ui
}     // end namespace fb
