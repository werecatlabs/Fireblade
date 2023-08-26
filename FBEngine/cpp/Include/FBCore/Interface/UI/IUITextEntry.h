#ifndef __IUITextEntry_h__
#define __IUITextEntry_h__

#include <FBCore/Interface/UI/IUIText.h>

namespace fb
{
    namespace ui
    {

        class IUITextEntry : public IUIText
        {
        public:
            /** Destructor. */
            ~IUITextEntry() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IGUITextEntry_h__
