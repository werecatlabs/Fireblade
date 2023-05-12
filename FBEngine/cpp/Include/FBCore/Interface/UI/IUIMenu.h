#ifndef __IUIMenu_H
#define __IUIMenu_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        /**
         * @class IUIMenu
         * @brief Interface for a menu, responsible for managing menu items within a user interface menu
         */
        class IUIMenu : public IUIElement
        {
        public:
            /**
             * @brief Virtual destructor
             */
            ~IUIMenu() override = default;

            /**
             * @brief Adds a menu item to the menu
             * @param item The menu item to add
             */
            virtual void addMenuItem( SmartPtr<IUIElement> item ) = 0;

            /**
             * @brief Removes a menu item from the menu
             * @param item The menu item to remove
             */
            virtual void removeMenuItem( SmartPtr<IUIElement> item ) = 0;

            /**
             * @brief Gets the menu items in the menu
             * @return An array of smart pointers to the menu items
             */
            virtual Array<SmartPtr<IUIElement>> getMenuItems() const = 0;

            /**
             * @brief Sets the menu items in the menu
             * @param val An array of smart pointers to the menu items
             */
            virtual void setMenuItems( Array<SmartPtr<IUIElement>> val ) = 0;

            /**
             * @brief Sets the position of the cursor
             * @param cursorIdx The index of the menu item where the cursor should be positioned
             */
            virtual void setCursorPosition( u32 cursorIdx ) = 0;

            /**
             * @brief Gets the position of the cursor
             * @return The index of the menu item where the cursor is currently positioned
             */
            virtual u32 getCursorPosition() const = 0;

            /**
             * @brief Sets the number of list items in the menu
             * @param numListItems The number of list items
             */
            virtual void setNumListItems( u32 numListItems ) = 0;

            /**
             * @brief Gets the number of list items in the menu
             * @return The number of list items
             */
            virtual u32 getNumListItems() const = 0;

            /**
             * @brief Sets the current item index
             * @param index The index of the current item
             */
            virtual void setCurrentItemIndex( u32 index ) = 0;

            /**
             * @brief Gets the current item index
             * @return The index of the current item
             */
            virtual u32 getCurrentItemIndex() const = 0;

            /**
             * @brief Moves the cursor to the next menu item
             */
            virtual void incrementCursor() = 0;

            /**
             * @brief Moves the cursor to the previous menu item
             */
            virtual void decrementCursor() = 0;

            /**
             * @brief Gets the number of menu items in the menu
             * @return The number of menu items
             */
            virtual s32 getNumMenuItems() const = 0;

            /**
             * @brief Gets the label of the menu
             * @return The label of the menu
             */
            virtual String getLabel() const = 0;

            /**
             * @brief Sets the label of the menu
             * @param label The label to set
             */
            virtual void setLabel( const String &label ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif
