#ifndef __IUIMenuBar_h__
#define __IUIMenuBar_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {
        /**
         * @class IUIMenubar
         * @brief Interface for a menu bar, responsible for managing menus within the user interface
         */
        class IUIMenubar : public IUIElement
        {
        public:
            /**
             * @brief Destructor
             */
            ~IUIMenubar() override = default;

            /**
             * @brief Adds a menu to the menu bar
             * @param menu The menu to add
             */
            virtual void addMenu( SmartPtr<IUIMenu> menu ) = 0;

            /**
             * @brief Removes a menu from the menu bar
             * @param menu The menu to remove
             */
            virtual void removeMenu( SmartPtr<IUIMenu> menu ) = 0;

            /**
             * @brief Gets the menus in the menu bar
             * @return An array of smart pointers to the menus
             */
            virtual Array<SmartPtr<IUIMenu>> getMenus() const = 0;

            /**
             * @brief Sets the menus in the menu bar
             * @param menus An array of smart pointers to the menus
             */
            virtual void setMenus( Array<SmartPtr<IUIMenu>> menus ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIMenuBar_h__
