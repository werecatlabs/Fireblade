#ifndef EventWindow_h__
#define EventWindow_h__

#include "ui/BaseWindow.h"
#include <FBCore/Core/Array.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace editor
    {
        class EventWindow : public BaseWindow
        {
        public:
            enum class WidgetId
            {
                Label,

                Enabled,
                Visible,
                Static,

                AddComponent,
                RemoveComponent,

                Count
            };

            EventWindow();
            ~EventWindow() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void updateSelection() override;

            SmartPtr<scene::IComponentEvent> getEvent() const;
            void setEvent( SmartPtr<scene::IComponentEvent> event );

        protected:
            class UIElementListener : public SharedObject<IEventListener>
            {
            public:
                UIElementListener();
                ~UIElementListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                EventWindow *getOwner() const;
                void setOwner( EventWindow *owner );

            private:
                EventWindow *m_owner = nullptr;
            };

            SmartPtr<scene::IComponentEvent> m_event;

            Array<SmartPtr<ui::IUIEventWindow>> m_eventWindows;

            Array<SmartPtr<EventListenerWindow>> m_eventListenerWindows;

            SmartPtr<ui::IUIButton> m_addComponentButton;
            SmartPtr<ui::IUIButton> m_removeComponentButton;

            SmartPtr<IEventListener> m_uiListener;
        };
    }  // namespace editor
}  // namespace fb

#endif  // EventsWindow_h__
