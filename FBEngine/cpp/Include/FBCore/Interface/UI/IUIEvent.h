#ifndef IUIEvent_h__
#define IUIEvent_h__

#include <FBCore/Interface/System/IEvent.h>

namespace fb
{
    namespace ui
    {

        class IUIEvent : public IEvent
        {
        public:
            /** Destructor. */
            ~IUIEvent() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace ui
}  // namespace fb

#endif  // IUIEvent_h__
