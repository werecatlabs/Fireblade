#ifndef ImGuiTabItem_h__
#define ImGuiTabItem_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUITabItem.h>

namespace fb
{
    namespace ui
    {

        class ImGuiTabItem : public CImGuiElement<IUITabItem>
        {
        public:
            ImGuiTabItem();
            ~ImGuiTabItem() override;

            void update();

            FB_CLASS_REGISTER_DECL;
            
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiTabItem_h__
