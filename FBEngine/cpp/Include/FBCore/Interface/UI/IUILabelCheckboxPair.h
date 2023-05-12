#ifndef IUILabelCheckboxPair_h__
#define IUILabelCheckboxPair_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUILabelCheckboxPair
         * @brief Interface for a UI Label-Checkbox Pair element, extending IUIElement and providing functionality to manage and display a label and a checkbox
         */
        class IUILabelCheckboxPair : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUILabelCheckboxPair() override = default;

            /**
             * @brief Gets the label's text
             * @return A string representing the label's text
             */
            virtual String getLabel() const = 0;

            /**
             * @brief Sets the label's text
             * @param label A string representing the label's text
             */
            virtual void setLabel( const String &label ) = 0;

            /**
             * @brief Gets the checkbox's value
             * @return A boolean representing the checkbox's value (true if checked, false otherwise)
             */
            virtual bool getValue() const = 0;

            /**
             * @brief Sets the checkbox's value
             * @param value A boolean representing the checkbox's value (true if checked, false otherwise)
             */
            virtual void setValue( bool value ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUILabelCheckboxPair_h__
