#ifndef IUIToolbar_h__
#define IUIToolbar_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
        @brief Interface for a toolbar.
        */
        class IUIToolbar : public IUIElement
        {
        public:
            /*
            @brief Virtual destructor.
            */
            ~IUIToolbar() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IUIToolbar_h__
