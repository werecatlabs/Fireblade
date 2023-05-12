#ifndef IUILabelSliderPair_h__
#define IUILabelSliderPair_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        class IUILabelSliderPair : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUILabelSliderPair() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IUILabelSliderPair_h__
