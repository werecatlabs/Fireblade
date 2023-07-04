#ifndef EventListenerWindow_h__
#define EventListenerWindow_h__

#include "ui/BaseWindow.h"
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace editor
    {

        class EventListenerWindow : public BaseWindow
        {
        public:
            EventListenerWindow();
            ~EventListenerWindow() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void updateSelection();

            SmartPtr<scene::IComponentEventListener> getComponentEventListener() const;
            void setComponentEventListener( SmartPtr<scene::IComponentEventListener> eventListener );

        protected:
            SmartPtr<PropertiesWindow> m_propertiesWindow;

            SmartPtr<scene::IComponentEventListener> m_componentEventListener;
        };

    }  // namespace editor
}  // namespace fb

#endif  // EventListenerWindow_h__
