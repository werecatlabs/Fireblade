#ifndef ImGuiLabelSliderPair_h__
#define ImGuiLabelSliderPair_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBCore/Interface/UI/IUILabelSliderPair.h>
#include <FBImGui/CImGuiElement.h>

namespace fb
{
    namespace ui
    {
        class ImGuiLabelSliderPair : public CImGuiElement<IUILabelSliderPair>
        {
        public:
            ImGuiLabelSliderPair();
            ~ImGuiLabelSliderPair() override;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiLabelSliderPair_h__
