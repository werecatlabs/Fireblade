#ifndef IUIEventResize_h__
#define IUIEventResize_h__

#include <FBCore/Interface/UI/IUIEvent.h>

namespace fb
{
    namespace ui
    {

        class IUIEventResize : public IUIEvent
        {
        public:
            /** Destructor. */
            ~IUIEventResize() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace ui
}  // namespace fb

#endif  // IUIEventResize_h__
