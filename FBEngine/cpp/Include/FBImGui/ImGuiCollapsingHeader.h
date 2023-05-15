#ifndef ImGuiCollapsingHeader_h__
#define ImGuiCollapsingHeader_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUICollapsingHeader.h>

namespace fb
{
    namespace ui
    {
        class ImGuiCollapsingHeader : public CImGuiElement<IUICollapsingHeader>
        {
        public:
            ImGuiCollapsingHeader();
            ~ImGuiCollapsingHeader() override;

            void update() override;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif // ImGuiCollapsingHeader_h__
