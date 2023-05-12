#ifndef IUIVerticalLayout_h__
#define IUIVerticalLayout_h__

#include <FBCore/Interface/UI/IUIContainer.h>

namespace fb
{
    namespace ui
    {

        class IUIVerticalLayout : public IUIContainer
        {
        public:
            /** Virtual destructor. */
            ~IUIVerticalLayout() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace ui
}  // namespace fb

#endif  // IUIVerticalLayout_h__
