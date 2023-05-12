#ifndef IUIHorizontalLayout_h__
#define IUIHorizontalLayout_h__

#include <FBCore/Interface/UI/IUIContainer.h>

namespace fb
{
    namespace ui
    {

        class IUIHorizontalLayout : public IUIContainer
        {
        public:
            /** Virtual destructor. */
            ~IUIHorizontalLayout() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace ui
}  // namespace fb

#endif  // IUIHorizontalLayout_h__
