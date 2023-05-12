#ifndef _IGUICharacterSelect_H
#define _IGUICharacterSelect_H

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUICharacterSelect
         * @brief Interface for a game character select menu UI element
         */
        class IUICharacterSelect : public IUIElement
        {
        public:
            /**
             * @brief Destructor
             */
            ~IUICharacterSelect() override = default;

            /**
             * @brief Sets the position of the cursor for a particular menu item
             * @param cursorIdx The index of the menu item
             * @param cursorPosition The position to set for the cursor
             */
            virtual void setCursorPosition( u32 cursorIdx, u32 cursorPosition ) = 0;

            /**
             * @brief Gets the position of the cursor for a particular menu item
             * @param cursorIdx The index of the menu item
             * @return The position of the cursor for the specified menu item
             */
            virtual u32 getCursorPosition( u32 cursorIdx ) const = 0;

            /**
             * @brief Sets the number of items in the list
             * @param numListItems The number of items in the list
             */
            virtual void setNumListItems( u32 numListItems ) = 0;

            /**
             * @brief Gets the number of items in the list
             * @return The number of items in the list
             */
            virtual u32 getNumListItems() const = 0;

            /**
             * @brief Sets the index of the currently selected item in the list
             * @param index The index of the currently selected item
             */
            virtual void setCurrentItemIndex( u32 index ) = 0;

            /**
             * @brief Gets the index of the currently selected item in the list
             * @return The index of the currently selected item
             */
            virtual u32 getCurrentItemIndex() const = 0;

            /**
             * @brief Moves the cursor to the next menu item
             * @param cursorIndex The index of the cursor to move
             */
            virtual void increamentCursor( s32 cursorIndex ) = 0;

            /**
             * @brief Moves the cursor to the previous menu item
             * @param cursorIndex The index of the cursor to move
             */
            virtual void decrementCursor( s32 cursorIndex ) = 0;

            /**
             * @brief Gets the total number of menu items
             * @return The total number of menu items
             */
            virtual int getNumMenuItems() const = 0;

            /**
             * @brief Gets the index of the current cursor
             * @return The index of the current cursor
             */
            virtual u32 getCurrentCursorIndex() const = 0;
        };

    }  // namespace ui
}  // end namespace fb

#endif
