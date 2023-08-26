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

            virtual SmartPtr<IUITabItem> addTabItem() = 0;
            virtual void removeTabItem(SmartPtr<IUITabItem> tabItem) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUITabBar_h__
