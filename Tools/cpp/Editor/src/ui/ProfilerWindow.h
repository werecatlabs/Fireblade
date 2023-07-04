#ifndef ProfilerWindow_h__
#define ProfilerWindow_h__

#include <GameEditorPrerequisites.h>
#include "ui/BaseWindow.h"

namespace fb
{
    namespace editor
    {
        class ProfilerWindow : public BaseWindow
        {
        public:
            ProfilerWindow();
            ~ProfilerWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void update();

            SmartPtr<ui::IUIProfilerWindow> getProfilerWindow() const;

            void setProfilerWindow( SmartPtr<ui::IUIProfilerWindow> profilerWindow );

        protected:
            SmartPtr<ui::IUIProfilerWindow> m_profilerWindow;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // ProfilerWindow_h__
