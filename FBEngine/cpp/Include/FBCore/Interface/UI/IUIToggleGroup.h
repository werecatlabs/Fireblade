#ifndef _IGUIToggleGroup_H
#define _IGUIToggleGroup_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @brief Interface for a toggle group of UI toggle buttons.
         */
        class IUIToggleGroup : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUIToggleGroup() override = default;

            /**
             * @brief Adds a toggle button to the toggle group.
             *
             * @param button The toggle button to add.
             */
            virtual void addToggleButton( SmartPtr<IUIToggleButton> button ) = 0;

            /**
             * @brief Removes a toggle button from the toggle group.
             *
             * @param button The toggle button to remove.
             *
             * @return True if the toggle button was removed successfully, false otherwise.
             */
            virtual bool removeToggleButton( SmartPtr<IUIToggleButton> button ) = 0;

            /**
             * @brief Called when a toggle button in the group is toggled.
             *
             * @param button The toggle button that was toggled.
             */
            virtual void OnSetButtonToggled( SmartPtr<IUIToggleButton> button ) = 0;

            /**
             * @brief Gets the currently toggled button in the group.
             *
             * @return The currently toggled button in the group, or null if no button is toggled.
             */
            virtual SmartPtr<IUIToggleButton> getCurrentToggledButton() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
