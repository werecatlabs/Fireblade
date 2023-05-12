#ifndef _IGUICONTAINER_H
#define _IGUICONTAINER_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        class IUIContainer : public IUIElement
        {
        public:
            /** Virtual destructor. */
            ~IUIContainer() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
