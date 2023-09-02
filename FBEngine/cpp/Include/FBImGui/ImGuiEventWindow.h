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
            ~ImGuiEventWindow();

            /** @copydoc IUIEventWindow::getEvents */
            virtual Array<SmartPtr<IEvent>> getEvents() const;

            /** @copydoc IUIEventWindow::setEvents */
            virtual void setEvents( const Array<SmartPtr<IEvent>> &events );

            void update();

            FB_CLASS_REGISTER_DECL;

        protected:
            Array<SmartPtr<IEvent>> m_events;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // ImGuiWindow_h__
