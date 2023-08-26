#ifndef _IUITEXT_H
#define _IUITEXT_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @brief Abstract interface for a UI text element.
         */
        class IUIText : public IUIElement
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
            ~IUIText() override = default;

            /**
             * @brief Sets the text of the UI element.
             * @param text The new text for the element.
             */
            virtual void setText( const String &text ) = 0;

            /**
             * @brief Gets the text of the UI element.
             * @return The current text of the element.
             */
            virtual String getText() const = 0;

            /**
             * @brief Sets the size of the text.
             * @param textSize The new size for the text.
             */
            virtual void setTextSize( f32 textSize ) = 0;

            /**
             * @brief Gets the size of the text.
             * @return The current size of the text.
             */
            virtual f32 getTextSize() const = 0;

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

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
