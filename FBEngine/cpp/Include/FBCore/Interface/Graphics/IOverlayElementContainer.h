#ifndef _IOverlayElementContainer_H
#define _IOverlayElementContainer_H

#include <FBCore/Interface/Graphics/IOverlayElement.h>

namespace fb
{
    namespace render
    {

        /**
         * @brief Interface for an overlay element container.
         */
        class IOverlayElementContainer : public IOverlayElement
        {
        public:
            /** Virtual destructor. */
            ~IOverlayElementContainer() override = default;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
