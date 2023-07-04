#ifndef __PropertiesWindow_h__
#define __PropertiesWindow_h__

#include <GameEditorPrerequisites.h>
#include "ui/BaseWindow.h"
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Interface/Resource/IResource.h>
#include <FBCore/Interface/System/IEventListener.h>

namespace fb
{
    namespace editor
    {

        /**
         * Window for displaying and editing the properties of a selected object.
         * This class extends the BaseWindow class and provides functionality to manage the properties of objects and display them in a property grid.
         */
        class PropertiesWindow : public BaseWindow
        {
        public:
            /** Enumeration for the different IDs that can be assigned to the properties window. */
            enum
            {
                PropertiesId,
            };

            /** Default constructor. */
            PropertiesWindow();

            /** Default destructor. */
            ~PropertiesWindow() override;

            /**
             * Loads the data for the properties window.
             * @param data The data to load.
             */
            void load( SmartPtr<ISharedObject> data ) override;

            /**
             * Unloads the data for the properties window.
             * @param data The data to unload.
             */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** Updates the properties window. */
            void update() override;

            /** Updates the selection. */
            void updateSelection() override;

            /**
             * Updates the selection.
             * @param object The object to select.
             */
            void updateSelection( SmartPtr<ISharedObject> object );

            /** Returns whether the properties window is dirty. */
            bool isDirty() const;

            /**
             * Sets whether the properties window is dirty.
             * @param val True if the window is dirty.
             */
            void setDirty( bool val );

            /** Gets the property grid. */
            SmartPtr<ui::IUIPropertyGrid> getPropertyGrid() const;

            /**
             * Sets the property grid.
             * @param val The property grid to set.
             */
            void setPropertyGrid( SmartPtr<ui::IUIPropertyGrid> val );

            /** Gets the selected object. */
            SmartPtr<ISharedObject> getSelected() const;

            /**
             * Sets the selected object.
             * @param selected The selected object.
             */
            void setSelected( fb::SmartPtr<fb::ISharedObject> selected );

        protected:
            /**
             * @class PropertiesListener
             * @brief Class for listening to property changes.
             * @extends CSharedObject<IEventListener>
             */
            class PropertiesListener : public SharedObject<IEventListener>
            {
            public:
                /**
                 * @brief Default constructor.
                 */
                PropertiesListener() = default;

                /**
                 * @brief Default destructor.
                 */
                ~PropertiesListener() override = default;

                /**
                 * @brief Handles property change events.
                 * @param eventType The type of the event.
                 * @param eventValue The value of the event.
                 * @param arguments An array of event arguments.
                 * @param sender The sender of the event.
                 * @param object The object associated with the event.
                 * @param event A smart pointer to the event.
                 * @return The result of the event handling as a Parameter.
                 */
                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

                /**
                 * @brief Gets the owner of the PropertiesListener.
                 * @return A pointer to the owning PropertiesWindow.
                 */
                PropertiesWindow *getOwner() const;

                /**
                 * @brief Sets the owner of the PropertiesListener.
                 * @param owner A pointer to the PropertiesWindow to set as the owner.
                 */
                void setOwner( PropertiesWindow *owner );

            private:
                /**
                 * @var PropertiesWindow *m_owner
                 * @brief A pointer to the owning PropertiesWindow.
                 */
                PropertiesWindow *m_owner = nullptr;
            };

            /**
             * @brief Handles property change events.
             * @param name The name of the property.
             * @param value The new value of the property.
             * @param isButton Flag indicating if the change is triggered by a button press (default: false).
             */
            void propertyChange( const String &name, const String &value, bool isButton = false );

            /**
             * @brief Handles property change events for a specific object.
             * @param object The shared object associated with the property change.
             * @param name The name of the property.
             * @param value The new value of the property.
             * @param isButton Flag indicating if the change is triggered by a button press (default: false).
             * @return The resource type of the changed property.
             */
            IResource::ResourceType propertyChange( SmartPtr<ISharedObject> object, const String &name,
                                                    const String &value, bool isButton );

            /**
             * @var SmartPtr<ui::IUIPropertyGrid> m_propertyGrid
             * @brief A smart pointer to the UI property grid.
             */
            SmartPtr<ui::IUIPropertyGrid> m_propertyGrid;

            /**
             * @var SmartPtr<ISharedObject> m_selected
             * @brief A smart pointer to the currently selected shared object.
             */
            SmartPtr<ISharedObject> m_selected;

            /**
             * @var bool m_isDirty
             * @brief A flag indicating if the properties have been modified (default: false).
             */
            bool m_isDirty = false;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // PropertiesWindow_h__
