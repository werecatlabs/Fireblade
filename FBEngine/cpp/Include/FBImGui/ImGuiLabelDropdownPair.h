#ifndef ImGuiLabelDropdownPair_h__
#define ImGuiLabelDropdownPair_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBCore/Interface/UI/IUILabelDropdownPair.h>
#include <FBImGui/CImGuiElement.h>

namespace fb
{
    namespace ui
    {
        class ImGuiLabelDropdownPair : public CImGuiElement<IUILabelDropdownPair>
        {
        public:
            ImGuiLabelDropdownPair();
            ~ImGuiLabelDropdownPair() override;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiLabelDropdownPair_h__
