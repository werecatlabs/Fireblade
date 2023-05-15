#ifndef ImGuiColourPicker_h__
#define ImGuiColourPicker_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIColourPicker.h>

namespace fb
{
    namespace ui
    {

        class ImGuiColourPicker : public CImGuiElement<IUIColourPicker>
        {
        public:
            ImGuiColourPicker();
            ~ImGuiColourPicker() override;

            void update() override;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiColourPicker_h__
