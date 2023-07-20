#ifndef IUIGrid_h__
#define IUIGrid_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /** Interface for a ui frame. */
        class IUIGrid : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUIGrid() override = default;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace ui
}  // end namespace fb

#endif  // IUIGrid_h__
