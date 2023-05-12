#ifndef __IUISpinner_H
#define __IUISpinner_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUISpinner
         * @brief Interface for a UI spinner element, responsible for managing a numeric input with increment and decrement buttons
         */
        class IUISpinner : public IUIElement
        {
        public:
            /**
             * @brief Destructor
             */
            ~IUISpinner() override = default;

            /**
             * @brief Sets the spinner's text label
             * @param text The label text as a string
             */
            virtual void setText( const String &text ) = 0;

            /**
             * @brief Gets the spinner's text label
             * @return The label text as a string
             */
            virtual String getText() const = 0;

            /**
             * @brief Increments the spinner's value
             */
            virtual void incrementValue() = 0;

            /**
             * @brief Decrements the spinner's value
             */
            virtual void decrementValue() = 0;

            /**
             * @brief Sets the spinner's value
             * @param value The value as a string
             */
            virtual void setValue( const String &value ) = 0;

            /**
             * @brief Gets the spinner's value
             * @return The value as a string
             */
            virtual String getValue() const = 0;

            /**
             * @brief Sets the spinner's minimum value
             * @param value The minimum value as a string
             */
            virtual void setMinValue( const String &value ) = 0;

            /**
             * @brief Gets the spinner's minimum value
             * @return The minimum value as a string
             */
            virtual String getMinValue() const = 0;

            /**
             * @brief Sets the spinner's maximum value
             * @param value The maximum value as a string
             */
            virtual void setMaxValue( const String &value ) = 0;

            /**
             * @brief Gets the spinner's maximum value
             * @return The maximum value as a string
             */
            virtual String getMaxValue() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // __IUISpinner_H
