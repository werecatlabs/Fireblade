#ifndef __IUIMenuItem_h__
#define __IUIMenuItem_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * An interface for a UI menu item.
         */
        class IUIMenuItem : public IUIElement
        {
        public:
            /**
             * Enumerates the types of menu items.
             */
            enum class Type
            {
                Normal,     ///< A normal menu item.
                Check,      ///< A checkable menu item.
                Radio,      ///< A radio button menu item.
                Dropdown,   ///< A dropdown menu item.
                Separator,  ///< A separator menu item.
                Count       ///< The number of menu item types.
            };

            /** Virtual destructor. */
            ~IUIMenuItem() override = default;

            /**
             * Gets the type of the menu item.
             *
             * @return The type of the menu item.
             */
            virtual Type getMenuItemType() const = 0;

            /**
             * Sets the type of the menu item.
             *
             * @param[in] type The type of the menu item.
             */
            virtual void setMenuItemType( Type type ) = 0;

            /**
             * Gets the text of the menu item.
             *
             * @return The text of the menu item.
             */
            virtual String getText() const = 0;

            /**
             * Sets the text of the menu item.
             *
             * @param[in] text The text of the menu item.
             */
            virtual void setText( const String &text ) = 0;

            /**
             * Gets the help text of the menu item.
             *
             * @return The help text of the menu item.
             */
            virtual String getHelp() const = 0;

            /**
             * Sets the help text of the menu item.
             *
             * @param[in] help The help text of the menu item.
             */
            virtual void setHelp( const String &help ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IGUIMenuItem_h__
