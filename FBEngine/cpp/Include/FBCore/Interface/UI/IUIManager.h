#ifndef _IUIMANAGER_H
#define _IUIMANAGER_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    namespace ui
    {
        /**
         * @class IUIManager
         * @brief Interface for a user interface manager, responsible for managing UI elements and their interactions
         */
        class IUIManager : public ISharedObject
        {
        public:
            /**
             * @brief Destructor
             */
            ~IUIManager() override = default;

            /**
             * @brief Processes messages
             * @param data The message data to process
             * @return The number of messages processed
             */
            virtual size_t messagePump( SmartPtr<ISharedObject> data ) = 0;

            /**
             * @brief Adds a new application to the user interface manager
             * @return A smart pointer to the newly added application
             */
            virtual SmartPtr<IUIApplication> addApplication() = 0;

            /**
             * @brief Removes an application from the user interface manager
             * @param application The application to be removed
             */
            virtual void removeApplication( SmartPtr<IUIApplication> application ) = 0;

            /**
             * @brief Gets the current application
             * @return A smart pointer to the current application
             */
            virtual SmartPtr<IUIApplication> getApplication() const = 0;

            /**
             * @brief Sets the current application
             * @param application A smart pointer to the application to be set as the current application
             */
            virtual void setApplication( SmartPtr<IUIApplication> application ) = 0;

            /**
             * @brief Adds a UI element by type
             * @tparam T The type of UI element to add
             * @return A smart pointer to the newly added UI element
             */
            template <class T>
            SmartPtr<T> addElementByType();

            /**
             * @brief Adds a UI element
             * @param type The type of element
             * @return A smart pointer to the newly added UI element
             */
            virtual SmartPtr<IUIElement> addElement( hash64 type ) = 0;

            /**
             * @brief Removes a UI element
             * @param element The element to be removed
             */
            virtual void removeElement( SmartPtr<IUIElement> element ) = 0;

            /**
             * @brief Removes all the UI elements
             */
            virtual void clear() = 0;

            /**
             * @brief Gets the UI cursor object
             * @return A smart pointer to the UI cursor
             */
            virtual SmartPtr<IUICursor> getCursor() const = 0;

            /**
             * @brief Finds a UI element with the specified ID
             * @param id The ID of the UI element to find
             * @return A smart pointer to the found UI element or nullptr if not found
             */
            virtual SmartPtr<IUIElement> findElement( const String &id ) const = 0;

            /**
             * @brief Checks if an element is being dragged
             * @return true if an element is being dragged, false otherwise
             */
            virtual bool isDragging() const = 0;

            /**
             * @brief Sets whether an element is being dragged or not
             * @param dragging true if an element is being dragged, false otherwise
             */
            virtual void setDragging( bool dragging ) = 0;

            virtual SmartPtr<IUIWindow> getMainWindow() const = 0;

            virtual void setMainWindow( SmartPtr<IUIWindow> uiWindow ) = 0;

            virtual void _getObject( void **ppObject ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

        template <class T>
        SmartPtr<T> IUIManager::addElementByType()
        {
            auto typeInfo = T::typeInfo();
            FB_ASSERT( typeInfo != 0 );

            auto element = addElement( typeInfo );
            FB_ASSERT( fb::dynamic_pointer_cast<T>( element ) );
            return fb::static_pointer_cast<T>( element );
        }

    }  // end namespace ui
}  // end namespace fb

#endif
