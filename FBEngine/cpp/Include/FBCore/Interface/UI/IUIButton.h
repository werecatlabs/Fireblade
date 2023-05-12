#ifndef __IUIButton_H
#define __IUIButton_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIButton
         * @brief Interface for a button, responsible for managing button elements in the user interface
         */
        class IUIButton : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIButton() override = default;

            /**
             * @brief Gets the label text of the button
             * @return The label text as a string
             */
            virtual String getLabel() const = 0;

            /**
             * @brief Sets the label text of the button
             * @param label The new label text for the button
             */
            virtual void setLabel( const String &label ) = 0;

            /**
             * @brief Sets the text size of the button label
             * @param textSize The new text size for the button label
             */
            virtual void setTextSize( f32 textSize ) = 0;

            /**
             * @brief Gets the text size of the button label
             * @return The text size of the button label as a float
             */
            virtual f32 getTextSize() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
