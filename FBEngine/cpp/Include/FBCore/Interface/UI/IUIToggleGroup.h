#ifndef _IUIToggleGroup_H
#define _IUIToggleGroup_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /** @brief Interface for a toggle group of UI toggle buttons.
         */
        class IUIToggleGroup : public IUIElement
        {
        public:
            /** Destructor. */
            ~IUIToggleGroup() override = default;

            /** @brief Adds a toggle button to the toggle group.
             * @param button The toggle button to add.
             */
            virtual void addToggleButton( SmartPtr<IUIToggle> button ) = 0;

            /**
             * @brief Removes a toggle button from the toggle group.
             * @param button The toggle button to remove.
             * @return True if the toggle button was removed successfully, false otherwise.
             */
            virtual bool removeToggleButton( SmartPtr<IUIToggle> button ) = 0;

            /** @brief Called when a toggle button in the group is toggled.
             * @param button The toggle button that was toggled.
             */
            virtual void handleSetToggled( SmartPtr<IUIToggle> button ) = 0;

            /** @brief Gets the currently toggled button in the group.
             * @return The currently toggled button in the group, or null if no button is toggled.
             */
            virtual SmartPtr<IUIToggle> getToggledButton() const = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
