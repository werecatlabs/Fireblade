#ifndef IUICollapsingHeader_h__
#define IUICollapsingHeader_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        class IUICollapsingHeader : public IUIElement
        {
        public:
            /** Virtual destructor. */
            ~IUICollapsingHeader() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb


#endif // IUICollapsingHeader_h__


