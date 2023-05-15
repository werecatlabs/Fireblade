#ifndef ImGuiProfiler_h__
#define ImGuiProfiler_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/Interface/UI/IUIProfilerWindow.h>

namespace fb
{
    namespace ui
    {
        
        class ImGuiProfiler : public CImGuiElement<IUIProfilerWindow>
        {
        public:
            ImGuiProfiler();
            ~ImGuiProfiler() override;
        };

    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiProfiler_h__
