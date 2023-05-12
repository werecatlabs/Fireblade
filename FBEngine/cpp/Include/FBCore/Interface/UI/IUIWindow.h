#ifndef __IUIWindow_h__
#define __IUIWindow_h__

#include <FBCore/Interface/UI/IUIElement.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIWindow
         * @brief Interface for a UI window element, responsible for managing window elements in the user interface
         */
        class IUIWindow : public IUIElement
        {
        public:
            /**
             * @brief Destructor
             */
            ~IUIWindow() override = default;

            /**
             * @brief Sets the window's label text
             * @param label The new label text for the window
             */
            virtual void setLabel( const String &label ) = 0;

            /**
             * @brief Gets the window's label text
             * @return The label text as a string
             */
            virtual String getLabel() const = 0;

            /**
             * @brief Sets the window's context menu
             * @param menu A SmartPtr to an IUIMenu object representing the context menu
             */
            virtual void setContextMenu( SmartPtr<IUIMenu> menu ) = 0;

            /**
             * @brief Gets the window's context menu
             * @return A SmartPtr to an IUIMenu object representing the context menu
             */
            virtual SmartPtr<IUIMenu> getContextMenu() const = 0;

            /**
             * @brief Checks if the window has a border
             * @return A boolean value, true if the window has a border, false otherwise
             */
            virtual bool hasBorder() const = 0;

            /**
             * @brief Sets whether the window has a border or not
             * @param border A boolean value, true to enable border, false to disable
             */
            virtual void setHasBorder( bool border ) = 0;

            /**
             * @brief Checks if the window is docked
             * @return A boolean value, true if the window is docked, false otherwise
             */
            virtual bool isDocked() const = 0;

            /**
             * @brief Sets whether the window is docked or not
             * @param docked A boolean value, true to enable docking, false to disable
             */
            virtual void setDocked( bool docked ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // end namespace ui
}  // end namespace fb

#endif  // IUIWindow_h__
