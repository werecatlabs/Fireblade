#ifndef ImGuiWindow_h__
#define ImGuiWindow_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/ImGuiWindowT.h>
#include <FBCore/Interface/UI/IUIWindow.h>

namespace fb
{
    namespace ui
    {

        class ImGuiWindow : public ImGuiWindowT<IUIWindow>
        {
        public:
            ImGuiWindow();
            ~ImGuiWindow();

            virtual void update();

            FB_CLASS_REGISTER_DECL;
        };
    }  // namespace ui
}  // namespace fb

#endif  // ImGuiWindow_h__
