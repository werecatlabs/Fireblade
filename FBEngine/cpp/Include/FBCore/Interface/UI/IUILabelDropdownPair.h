#ifndef IUILabelDropdownPair_h__
#define IUILabelDropdownPair_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        class IUILabelDropdownPair : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUILabelDropdownPair() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IUILabelDropdownPair_h__
