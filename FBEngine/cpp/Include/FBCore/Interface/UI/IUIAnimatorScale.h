#ifndef IGUIAnimatorScale_h__
#define IGUIAnimatorScale_h__

#include <FBCore/Interface/UI/IUIAnimator.h>

namespace fb
{
    namespace ui
    {

        class IUIAnimatorScale : public IUIAnimator
        {
        public:
            /** Virtual destructor. */
            ~IUIAnimatorScale() override = default;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IGUIAnimatorScale_h__
