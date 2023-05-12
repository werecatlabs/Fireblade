#ifndef _IGUIToggleButton_H
#define _IGUIToggleButton_H

#include <FBCore/Interface/UI/IUIButton.h>

namespace fb
{
    namespace ui
    {

        /**
         * @brief Interface for a toggle button UI element.
         */
        class IUIToggleButton : public IUIButton
        {
        public:
            /** Destructor. */
            ~IUIToggleButton() override = default;

            /** Sets the toggled state of the toggle button.
             *
             * @param toggled The toggled state to set.
             */
            virtual void setToggled( bool toggled ) = 0;

            /** Gets the toggled state of the toggle button.
             *
             * @return The toggled state of the toggle button.
             */
            virtual bool isToggled() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
