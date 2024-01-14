#ifndef __ImGuiMenuBar_h__
#define __ImGuiMenuBar_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIMenubar.h>

namespace fb
{
    namespace ui
    {

        class ImGuiMenuBar : public CImGuiElement<IUIMenubar>
        {
        public:
            ImGuiMenuBar();
            ~ImGuiMenuBar() override;

            void update() override;

            /** */
            void addMenu( SmartPtr<IUIMenu> menu ) override;

            /** */
            void removeMenu( SmartPtr<IUIMenu> menu ) override;

            Array<SmartPtr<IUIMenu>> getMenus() const override;
            void setMenus( Array<SmartPtr<IUIMenu>> menus ) override;

        protected:
            Array<SmartPtr<IUIMenu>> m_menus;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // CEGUIText_h__
