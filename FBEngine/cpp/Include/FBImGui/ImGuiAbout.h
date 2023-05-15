#ifndef ImGuiAbout_h__
#define ImGuiAbout_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIAbout.h>

namespace fb
{
    namespace ui
    {

        class ImGuiAbout : public CImGuiElement<IUIAbout>
        {
        public:
            ImGuiAbout();
            ~ImGuiAbout() override;

             void update() override;

            FB_CLASS_REGISTER_DECL;

        protected:
        };

    }  // namespace ui
}  // namespace fb

#endif  // ImGuiAbout_h__
