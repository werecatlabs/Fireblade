#ifndef IUIInputManager_h__
#define IUIInputManager_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /** Interface for a input manager. */
        class IUIInputManager : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUIInputManager() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace ui
}  // namespace fb

#endif  // IUIInputManager_h__
