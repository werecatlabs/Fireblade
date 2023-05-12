#ifndef IUIAbout_h__
#define IUIAbout_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        class IUIAbout : public IUIElement
        {
        public:
            /** Virtual destructor. */
            ~IUIAbout() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace ui
}  // namespace fb

#endif  // IUIAbout_h__
