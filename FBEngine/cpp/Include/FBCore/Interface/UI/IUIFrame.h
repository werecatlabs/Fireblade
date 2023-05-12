#ifndef FB_IUIFRAME_H
#define FB_IUIFRAME_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        
        /** Interface for a ui frame. */
        class IUIFrame : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUIFrame() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // FB_IUIFRAME_H
