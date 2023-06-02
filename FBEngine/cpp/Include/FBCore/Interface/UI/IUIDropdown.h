#ifndef IUIDropdown_h__
#define IUIDropdown_h__

#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIDropdown
         * @brief Interface for a dropdown UI element that displays a list of options and allows the user to select one
         */
        class IUIDropdown : public IUIElement
        {
        public:
            /**
             * @brief Destructor
             */
            ~IUIDropdown() override = default;

            /**
             * @brief Gets the list of available options
             * @return The list of available options
             */
            virtual Array<String> getOptions() const = 0;

            /**
             * @brief Sets the list of available options
             * @param options The list of available options to set
             */
            virtual void setOptions( const Array<String> &options ) = 0;

            /**
             * @brief Gets the index of the currently selected option
             * @return The index of the currently selected option
             */
            virtual u32 getSelectedOption() const = 0;

            /**
             * @brief Sets the index of the currently selected option
             * @param selectedOption The index of the option to select
             */
            virtual void setSelectedOption( u32 selectedOption ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIDropdown_h__
