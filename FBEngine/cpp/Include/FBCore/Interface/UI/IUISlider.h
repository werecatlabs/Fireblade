#ifndef IUISlider_h__
#define IUISlider_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /** Interface for a slider. */
        class IUISlider : public IUIElement
        {
        public:
            /** Virtual destructor. */
            ~IUISlider() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUISlider_h__
