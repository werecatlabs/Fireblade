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

            FB_CLASS_REGISTER_DECL;
            void update();
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiTabBar_h__
