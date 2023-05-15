#ifndef __IUIElement_h__
#define __IUIElement_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/ColourF.h>
#include <FBCore/Base/ConcurrentArray.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Interface/IPrototype.h>

namespace fb
{
    namespace ui
    {

        /**
         * @class IUIElement
         * @brief Interface for a UI element.
         * @extends core::IPrototype
         */
        class IUIElement : public core::IPrototype
        {
        public:
            /// @brief Hash type used for state message add child.
            static const hash_type STATE_MESSAGE_ADD_CHILD;
            /// @brief Hash type used for state message remove child.
            static const hash_type STATE_MESSAGE_REMOVE_CHILD;

            /**
             * @brief Virtual destructor.
             */
            ~IUIElement() override = default;

            virtual bool handleEvent( const SmartPtr<IInputEvent> &event ) = 0;

            /**
             * @brief Sets the ID of the UI item.
             * @param name The new name of the UI item.
             */
            virtual void setName( const String &name ) = 0;

            /**
             * @brief Gets the ID of the UI item.
             * @return Returns the name of the UI item.
             */
            virtual String getName() const = 0;

            /**
             * @brief Gets the element ID.
             * @return Returns the element ID as s32.
             */
            virtual s32 getElementId() const = 0;
            /**
             * @brief Sets the element ID.
             * @param elementId The new element ID.
             */
            virtual void setElementId( s32 elementId ) = 0;

            /**
             * @brief Sets the position of the GUI item.
             * @param position The new position of the GUI item in 2D space.
             */
            virtual void setPosition( const Vector2F &position ) = 0;

            /**
             * @brief Returns the position of the GUI item.
             * @return Returns the position of the GUI item in 2D space.
             */
            virtual Vector2F getPosition() const = 0;

            /**
             * @brief Returns the absolute position of the GUI item.
             * @return Returns the absolute position of the GUI item in 2D space.
             */
            virtual Vector2F getAbsolutePosition() const = 0;

            /**
             * @brief Sets the size of the GUI item.
             * @param size The new size of the GUI item in 2D space.
             */
            virtual void setSize( const Vector2F &size ) = 0;

            /**
             * @brief Gets the size of the GUI item.
             * @return Returns the size of the GUI item in 2D space.
             */
            virtual Vector2F getSize() const = 0;

            /**
             * @brief Sets whether the GUI item is visible or not.
             * @param visible The new visibility state of the GUI item.
             * @param cascade Optional parameter to cascade the visibility change to children. Default is true.
             */
            virtual void setVisible( bool visible, bool cascade = true ) = 0;

            /**
             * @brief Returns a boolean value indicating whether or not the UI item is visible.
             * @return Returns true if the UI item is visible, false otherwise
             */
            virtual bool isVisible() const = 0;

            /** Sets whether this ui item is enabled. */
            virtual void setEnabled( bool enabled, bool cascade = true ) = 0;

            /** Sets whether this ui item is enabled. */
            virtual bool isEnabled() const = 0;

            /** Gets the parent element. */
            virtual SmartPtr<IUIElement> getParent() const = 0;

            /** Sets the parent element. */
            virtual void setParent( SmartPtr<IUIElement> parent ) = 0;

            /** Gets the children of the gui item. */
            virtual SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> getChildren() const = 0;

            /** Adds a child to this ui item. */
            virtual void addChild( SmartPtr<IUIElement> child ) = 0;

            /** Removes a child of the ui item. */
            virtual bool removeChild( SmartPtr<IUIElement> child ) = 0;

            /** Removes this element. */
            virtual void remove() = 0;

            /** Removes all the children of this ui item. */
            virtual void removeAllChildren() = 0;

            /** Destroys all the children of this ui item. */
            virtual void destroyAllChildren() = 0;

            /** Check if the check item has a child. */
            virtual bool hasChildById( const String &id ) const = 0;

            /** Find a child by id. */
            virtual SmartPtr<IUIElement> findChildById( const String &id ) const = 0;

            /** Finds a component by id. */
            virtual SmartPtr<IUIElement> findChildByComponentId( const String &componentId ) const = 0;

            /** Gets the ui layout this belongs to. */
            virtual SmartPtr<IUIElement> getLayout() const = 0;

            /** Sets the ui layout this belongs to. */
            virtual void setLayout( SmartPtr<IUIElement> layout ) = 0;

            /** Gets the ui container. */
            virtual SmartPtr<IUIContainer> getContainer() const = 0;

            /** Sets the ui container. */
            virtual void setContainer( SmartPtr<IUIContainer> container ) = 0;

            /** Gets the owner. */
            virtual SmartPtr<ISharedObject> getOwner() const = 0;

            /** Sets the owner. */
            virtual void setOwner( SmartPtr<ISharedObject> owner ) = 0;

            /** Gets the drag source. */
            virtual SmartPtr<IUIDragSource> getDragSource() const = 0;

            /** Sets the drag source. */
            virtual void setDragSource( SmartPtr<IUIDragSource> dragSource ) = 0;

            /** Gets the drop target. */
            virtual SmartPtr<IUIDropTarget> getDropTarget() const = 0;

            /** Sets the drop target. */
            virtual void setDropTarget( SmartPtr<IUIDropTarget> dropTarget ) = 0;

            /**
             * @brief Gets the order of the UI element.
             * @return Returns the order as a u32 value.
             */
            virtual u32 getOrder() const = 0;

            /**
             * @brief Sets the order of the UI element.
             * @param order The new order value for the UI element.
             */
            virtual void setOrder( u32 order ) = 0;

            /**
             * @brief Gets the same line state of the UI element.
             * @return Returns true if the UI element is on the same line, false otherwise.
             */
            virtual bool getSameLine() const = 0;

            /**
             * @brief Sets the same line state of the UI element.
             * @param sameLine The new same line state for the UI element.
             */
            virtual void setSameLine( bool sameLine ) = 0;

            /**
             * @brief Gets the color of the UI element.
             * @return Returns the color of the UI element as a ColourF instance.
             */
            virtual ColourF getColour() const = 0;

            /**
             * @brief Sets the color of the UI element.
             * @param colour The new color for the UI element as a ColourF instance.
             */
            virtual void setColour( const ColourF &colour ) = 0;

            /**
             * @brief Gets the input event handling state of the UI element.
             * @return Returns true if the UI element handles input events, false otherwise.
             */
            virtual bool getHandleInputEvents() const = 0;

            /**
             * @brief Sets the input event handling state of the UI element.
             * @param handleInputEvents The new input event handling state for the UI element.
             */
            virtual void setHandleInputEvents( bool handleInputEvents ) = 0;

            /**
             * @brief Updates the Z-order of the UI element.
             */
            virtual void updateZOrder() = 0;

            /**
             * @brief Gets the state object associated with the UI element.
             * @return Returns a smart pointer to the IStateContext instance.
             */
            virtual SmartPtr<IStateContext> getStateObject() const = 0;

            /**
             * @brief Sets the state object associated with the UI element.
             * @param stateObject A smart pointer to the new IStateContext instance.
             */
            virtual void setStateObject( SmartPtr<IStateContext> stateObject ) = 0;

            /**
             * @brief Gets the state listener associated with the UI element.
             * @return Returns a smart pointer to the IStateListener instance.
             */
            virtual SmartPtr<IStateListener> getStateListener() const = 0;

            /**
             * @brief Sets the state listener associated with the UI element.
             * @param stateListener A smart pointer to the new IStateListener instance.
             */
            virtual void setStateListener( SmartPtr<IStateListener> stateListener ) = 0;

            /** Gets a pointer to the underlying scene node object. This is dependent on the graphics
             * library used. */
            virtual void _getObject( void **ppObject ) const = 0;

            template <class T>
            Array<SmartPtr<T>> getChildrenByType() const;

            FB_CLASS_REGISTER_DECL;
        };

        template <class T>
        Array<SmartPtr<T>> IUIElement::getChildrenByType() const
        {
            Array<SmartPtr<T>> childrenByType;

            if( auto p = getChildren() )
            {
                auto &children = *p;
                childrenByType.reserve( children.size() );

                for( auto &child : children )
                {
                    if( child->isDerived<T>() )
                    {
                        childrenByType.push_back( child );
                    }
                }
            }

            return childrenByType;
        }

    }  // end namespace ui
}  // end namespace fb

#endif  // IIGUIElement_h__
