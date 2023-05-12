#ifndef _IOverlayElementVector_H
#define _IOverlayElementVector_H

#include <FBCore/Interface/Graphics/IOverlayElement.h>

namespace fb
{
    namespace render
    {

        /**
         * An overlay element that displays a vector graphic.
         */
        class IOverlayElementVector : public IOverlayElement
        {
        public:
            /** Virtual destructor. */
            ~IOverlayElementVector() override = default;

            /** Gets the filename of the vector graphic. */
            virtual String getFileName() const = 0;

            /** Sets the filename of the vector graphic. */
            virtual void setFileName( const String &fileName ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
