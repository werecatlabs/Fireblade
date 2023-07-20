#ifndef IUISystemWindow_h__
#define IUISystemWindow_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        class IUISystemWindow : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUISystemWindow() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUISystemWindow_h__
