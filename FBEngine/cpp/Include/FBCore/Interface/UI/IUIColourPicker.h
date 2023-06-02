#ifndef IUIColourPicker_h__
#define IUIColourPicker_h__

#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Core/ColourF.h>
#include <FBCore/Core/UtilityTypes.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIColourPicker
         * @brief Interface for a colour picker user interface element, which allows users to choose colours from a gradient.
         */
        class IUIColourPicker : public IUIElement
        {
        public:
            /**
             * @brief Enumeration for the colour format options.
             */
            enum class ColourFormat
            {
                RGB, /**< RGB format */
                HSL, /**< HSL format */
                HEX  /**< HEX format */
            };

            /** Virtual destructor. */
            ~IUIColourPicker() override = default;

            /**
             * @brief Sets the currently selected colour.
             * @param colour The new colour to set
             */
            virtual void setColour( const ColourF &colour ) = 0;

            /**
             * @brief Gets the currently selected colour.
             * @return The currently selected colour
             */
            virtual ColourF getColour() const = 0;

            /**
             * @brief Sets the gradient for the colour picker.
             * @param startColour The starting colour of the gradient
             * @param endColour The ending colour of the gradient
             */
            virtual void setGradient( const ColourF &startColour, const ColourF &endColour ) = 0;

            /**
             * @brief Gets the gradient for the colour picker.
             * @return A pair of the starting and ending colours of the gradient
             */
            virtual Pair<ColourF, ColourF> getGradient() const = 0;

            /**
             * @brief Sets the colour format for the colour picker.
             * @param format The colour format to set
             */
            virtual void setColourFormat( ColourFormat format ) = 0;

            /**
             * @brief Gets the colour format for the colour picker.
             * @return The colour format for the colour picker
             */
            virtual ColourFormat getColourFormat() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIColourPicker_h__
