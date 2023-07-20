#ifndef __IUISearchBar_h__
#define __IUISearchBar_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        /** Interface for a search bar. */
        class IUISearchBar : public IUIElement
        {
        public:
            /** Virtual destructor. */
            ~IUISearchBar() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // __IUISearchBar_h__
