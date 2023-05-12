#ifndef IUIResourceBrowser_h__
#define IUIResourceBrowser_h__

#include <FBCore/Interface/UI/IUIDialogBox.h>

namespace fb
{
    namespace ui
    {
        /** Interface for a resource browser. */
        class IUIResourceBrowser : public IUIDialogBox
        {
        public:
            /** Virtual destructor. */
            ~IUIResourceBrowser() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IUIResourceBrowser_h__
