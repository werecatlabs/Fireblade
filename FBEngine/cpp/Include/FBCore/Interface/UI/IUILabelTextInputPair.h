#ifndef IUILabelTextInputPair_h__
#define IUILabelTextInputPair_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        /**
         * @class IUILabelTextInputPair
         * @brief Interface for a UI element that consists of a label and a text input box
         */
        class IUILabelTextInputPair : public IUIElement
        {
        public:
            /**
             * @brief Destructor
             */
            ~IUILabelTextInputPair() override = default;

            /**
             * @brief Gets the label associated with the input box
             * @return The label associated with the input box
             */
            virtual String getLabel() const = 0;

            /**
             * @brief Sets the label associated with the input box
             * @param label The label to associate with the input box
             */
            virtual void setLabel( const String &label ) = 0;

            /**
             * @brief Gets the current value of the text input box
             * @return The current value of the text input box
             */
            virtual String getValue() const = 0;

            /**
             * @brief Sets the value of the text input box
             * @param value The value to set for the text input box
             */
            virtual void setValue( const String &value ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUILabelTextInputPair_h__
