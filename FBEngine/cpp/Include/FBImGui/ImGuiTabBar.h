#ifndef ImGuiTabBar_h__
#define ImGuiTabBar_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUITabBar.h>

namespace fb
{
    namespace ui
    {

        class ImGuiTabBar : public CImGuiElement<IUITabBar>
        {
        public:
            ImGuiTabBar();
            ~ImGuiTabBar() override;

            void update();

            virtual SmartPtr<IUITabItem> addTabItem();
            virtual void removeTabItem( SmartPtr<IUITabItem> tabItem );

            FB_CLASS_REGISTER_DECL;

        protected:
            Array<SmartPtr<IUITabItem>> m_tabItems;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiTabBar_h__
