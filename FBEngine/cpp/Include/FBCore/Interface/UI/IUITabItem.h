#ifndef IUITabItem_h__
#define IUITabItem_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        class IUITabItem : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUITabItem() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUITabItem_h__
