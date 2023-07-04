#ifndef _IOverlayElementText_H
#define _IOverlayElementText_H

#include <FBCore/Interface/Graphics/IOverlayElement.h>

namespace fb
{
    namespace render
    {

        /**
         * Interface for an overlay element that displays text.
         */
        class IOverlayElementText : public IOverlayElement
        {
        public:
            /** Alignment options for the text element. */
            enum class Alignment
            {
                Left,  /**< Align text to the left. */
                Right, /**< Align text to the right. */
                Center /**< Center text. */
            };

            /** Virtual destructor. */
            ~IOverlayElementText() override = default;

            /**
             * Sets the name of the font to use for rendering the text.
             * @param fontName The name of the font.
             */
            virtual void setFontName( const String &fontName ) = 0;

            /**
             * Sets the height of each character in the text.
             * @param charHeight The character height.
             */
            virtual void setCharHeight( f32 charHeight ) = 0;

            /**
             * Sets the alignment of the text within the element.
             * @param alignment The alignment value to set.
             */
            virtual void setVerticalAlignment( u8 alignment ) = 0;

            /**
             * Gets the alignment of the text within the element.
             * @return The alignment value.
             */
            virtual u8 getVerticalAlignment() const = 0;

            /**
             * Sets the alignment of the text within the element.
             * @param alignment The alignment value to set.
             */
            virtual void setHorizontalAlignment( u8 alignment ) = 0;

            /**
             * Gets the alignment of the text within the element.
             * @return The alignment value.
             */
            virtual u8 getHorizontalAlignment() const = 0;

            /**
             * Sets the width of the space character.
             * @param width The width of the space character.
             */
            virtual void setSpaceWidth( f32 width ) = 0;

            /**
             * Gets the width of the space character.
             * @return The width of the space character.
             */
            virtual f32 getSpaceWidth() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace render
}  // end namespace fb

#endif
