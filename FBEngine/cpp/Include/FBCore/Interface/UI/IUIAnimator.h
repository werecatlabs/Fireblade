#ifndef IGUIAnimator_h__
#define IGUIAnimator_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        class IUIAnimator : public IUIElement
        {
        public:
            /** Virtual destructor. */
            ~IUIAnimator() override = default;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IGUIAnimator_h__
