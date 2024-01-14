#ifndef ImGuiEventWindow_h__
#define ImGuiEventWindow_h__

#include <FBImGui/FBImGuiPrerequisites.h>
#include <FBImGui/ImGuiWindowT.h>
#include <FBCore/Interface/UI/IUIEventWindow.h>

namespace fb
{
    namespace ui
    {
        class ImGuiEventWindow : public ImGuiWindowT<IUIEventWindow>
        {
        public:
            ImGuiEventWindow();
            ~ImGuiEventWindow() override;

            /** @copydoc IUIEventWindow::getEvents */
            Array<SmartPtr<IEvent>> getEvents() const override;

            /** @copydoc IUIEventWindow::setEvents */
            void setEvents( const Array<SmartPtr<IEvent>> &events ) override;

            void update() override;

            FB_CLASS_REGISTER_DECL;

        protected:
            Array<SmartPtr<IEvent>> m_events;
        };
    } // end namespace ui
}     // end namespace fb

#endif  // ImGuiWindow_h__
