#ifndef ImGuiToolbar_h__
#define ImGuiToolbar_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIToolbar.h>

namespace fb
{
    namespace ui
    {
        
        class ImGuiToolbar : public CImGuiElement<IUIToolbar>
        {
        public:
            ImGuiToolbar();
            ~ImGuiToolbar() override;

            FB_CLASS_REGISTER_DECL;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiToolbar_h__
