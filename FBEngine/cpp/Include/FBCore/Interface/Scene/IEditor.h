#ifndef IEditor_h__
#define IEditor_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Parameter.h>
#include <FBCore/Interface/System/IEvent.h>

namespace fb
{
    namespace scene
    {

        /**
        @brief Interface for a window object in the user interface.
        */
        class IEditor : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IEditor() override = default;

            /**
             * @brief Gets the parent of the window.
             *
             * @return SmartPtr<ui::IUIWindow> The parent of the window.
             */
            virtual SmartPtr<ui::IUIWindow> getParent() const = 0;

            /**
             * @brief Sets the parent of the window.
             *
             * @param parent The parent of the window.
             */
            virtual void setParent( SmartPtr<ui::IUIWindow> parent ) = 0;

            /**
             * @brief Gets the parent window of the window.
             *
             * @return SmartPtr<ui::IUIWindow> The parent window of the window.
             */
            virtual SmartPtr<ui::IUIWindow> getParentWindow() const = 0;

            /**
             * @brief Sets the parent window of the window.
             *
             * @param parentWindow The parent window of the window.
             */
            virtual void setParentWindow( SmartPtr<ui::IUIWindow> parentWindow ) = 0;

            /**
             * @brief Checks if the window is visible.
             *
             * @return bool True if the window is visible, false otherwise.
             */
            virtual bool isWindowVisible() const = 0;

            /**
             * @brief Sets the window visible or hidden.
             *
             * @param visible True to make the window visible, false to hide it.
             */
            virtual void setWindowVisible( bool visible ) = 0;

            /**
             * @brief Updates the selection of the window.
             *
             */
            virtual void updateSelection() = 0;

            /** Gets the event listener of the window.
             *@return The event listener of the window. */
            virtual SmartPtr<IEventListener> getEventListener() const = 0;

            /** Sets the event listener of the window.
             * @param eventListener The event listener of the window. */
            virtual void setEventListener( SmartPtr<IEventListener> eventListener ) = 0;

            /** Gets the drag source of the window.
             *@return The drag source of the window. */
            virtual SmartPtr<ui::IUIDragSource> getWindowDragSource() const = 0;

            /** Sets the drag source of the window.
             * @param windowDragSource The drag source of the window. */
            virtual void setWindowDragSource( SmartPtr<ui::IUIDragSource> windowDragSource ) = 0;

            /** Gets the drop target of the window.
             * @return The drop target of the window. */
            virtual SmartPtr<ui::IUIDropTarget> getWindowDropTarget() const = 0;

            /** Sets the drop target of the window.
             * @param windowDropTarget The drop target of the window. */
            virtual void setWindowDropTarget( SmartPtr<ui::IUIDropTarget> windowDropTarget ) = 0;

            /** Sets the draggable state of the element.
             *             * @param element The element to set.
             *             * @param draggable True to make the element draggable, false otherwise. */
            virtual void setDraggable( SmartPtr<ui::IUIElement> element, bool draggable ) = 0;

            /** Checks if the element is draggable.
             *  @param element The element to check.
             *  @return True if the element is draggable, false otherwise. */
            virtual bool isDraggable( SmartPtr<ui::IUIElement> element ) const = 0;

            /** Sets the droppable state of the element.
             * @param element The element to set.
             * @param droppable True to make the element droppable, false otherwise. */
            virtual void setDroppable( SmartPtr<ui::IUIElement> element, bool droppable ) = 0;

            /** Checks if the element is droppable.
             * @param element The element to check.
             * @return True if the element is droppable, false otherwise. */
            virtual bool isDroppable( SmartPtr<ui::IUIElement> element ) const = 0;

            /** Sets the handle events state of the element.
             * @param element The element to set.
             * @param handleEvents True to make the element handle events, false otherwise. */
            virtual void setHandleEvents( SmartPtr<ui::IUIElement> element, bool handleEvents ) = 0;

            /** Checks if the element handles events.
             * @param element The element to check.
             * @return True if the element handles events, false otherwise. */
            virtual bool getHandleEvents( SmartPtr<ui::IUIElement> element ) const = 0;

            /** Handles an event.
             @param eventType The event type.
             @param eventValue The event value.
             @param sender The object triggering the event. This can be null.
             @param event The event data. This can be null.
             @return Contains a return parameter. */
            virtual Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                           const Array<Parameter> &arguments,
                                           SmartPtr<ISharedObject> sender,
                                           SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) = 0;

            /** Handles a drag event.
             * @param position The position of the drag event.
             * @param element The element that is being dragged.
             * @return The data to be dropped. */
            virtual String handleDrag( const Vector2I &position, SmartPtr<ui::IUIElement> element ) = 0;

            /** Handles a drop event.
             * @param position The position of the drop event.
             * @param element The element that is being dropped.
             * @param data The data to be dropped. */
            virtual void handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> element, const String &data ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace scene
}  // namespace fb

#endif  // IEditor_h__
