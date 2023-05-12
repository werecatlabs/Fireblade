#ifndef _IGUITEXT_H
#define _IGUITEXT_H

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

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
