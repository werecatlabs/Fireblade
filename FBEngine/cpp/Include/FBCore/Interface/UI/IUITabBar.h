#ifndef IUITabBar_h__
#define IUITabBar_h__


#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        class IUITabBar : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUITabBar() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif // IUITabBar_h__
