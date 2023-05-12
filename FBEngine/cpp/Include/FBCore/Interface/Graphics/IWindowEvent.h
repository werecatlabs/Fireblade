#ifndef _IWindowEvent_H
#define _IWindowEvent_H

#include <FBCore/Interface/System/IEvent.h>

namespace fb
{
    namespace render
    {

        /**
         * @brief Interface for a window event.
         */
        class IWindowEvent : public IEvent
        {
        public:
            /** Virtual destructor. */
            ~IWindowEvent() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
