#ifndef ImGuiUtil_h__
#define ImGuiUtil_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUITreeNode.h>

namespace fb
{
    namespace ui
    {


        class ImGuiUtil
        {
        public:
            static bool ToggleButton( const char *str_id, bool *v );
        };

    }  // namespace ui
}  // namespace fb


#endif // ImGuiUtil_h__
