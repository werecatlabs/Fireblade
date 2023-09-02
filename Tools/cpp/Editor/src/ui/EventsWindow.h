#ifndef EventsWindow_h__
#define EventsWindow_h__

#include "ui/EditorWindow.h"
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace editor
    {

        class EventsWindow : public EditorWindow
        {
        public:
            EventsWindow();
            ~EventsWindow() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void updateSelection();

        protected:
            Array<SmartPtr<EventWindow>> m_eventWindows;
        };

    }  // namespace editor
}  // namespace fb

#endif  // EventsWindow_h__
