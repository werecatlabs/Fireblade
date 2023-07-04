#ifndef _FB_GUIElement_H
#define _FB_GUIElement_H

#include <FBRenderUI/FBRenderUIPrerequisites.h>
#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Interface/UI/IUIDropTarget.h>
#include <FBCore/Interface/UI/IUIDragSource.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/SharedObject.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Core/Parameter.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/State/Messages/StateMessageObject.h>
#include <FBCore/State/States/UIElementState.h>
#include <FBCore/System/Prototype.h>

namespace fb
{
    namespace ui
    {
        /** A base gui element.
         */
        template <class T>
        class CUIElement : public core::Prototype<T>
        {
        public:
            /** Default constructor. */
            CUIElement();

            /** Virtual destructor. */
            ~CUIElement() override;

            void unload( SmartPtr<ISharedObject> data ) override;

            /** Used to initialise the gui element with default values. */
            virtual void initialise( SmartPtr<IUIElement> &parent );

            /** Used to initialise the gui element. */
            virtual void initialise( SmartPtr<IUIElement> &parent, const Properties &properties );

            /** Called the update the gui item. */
            void update() override;

            /** Called when an input event has occurred. */
            virtual bool handleEvent( const SmartPtr<IInputEvent> &event );

            /** Sets the id of the gui item. */
            void setName( const String &name );

            /** Gets the id of the gui item. */
            String getName() const;

            virtual s32 getElementId() const;

            virtual void setElementId( s32 val );

            /** Gets the hash id of the gui element. */
            u32 getHashId() const;

            /** Sets the component id. */
            void setComponentID( const String &componentId );

            /** Gets the component id. */
            const String &getComponentID() const;

            /** Gets the type of the gui item. */
            const String &getType() const;

            /** Sets the position of the gui item. */
            virtual void setPosition( const Vector2F &position );

            /** Returns the position of the gui item. */
            Vector2F getPosition() const;

            /** Gets the absolute position of the gui item. */
            Vector2F getAbsolutePosition() const;

            /** Sets the size of the gui item. */
            virtual void setSize( const Vector2F &size );

            /** Gets the size of the gui item. */
            Vector2F getSize() const;

            /** Sets whether this gui item is enabled. */
            void setEnabled( bool enabled, bool cascade = true );

            /** Sets whether this gui item is enabled. */
            bool isEnabled() const;

            /** Sets whether this gui item is selected. */
            void setSelected( bool selected );

            /** Checks if this gui item is selected. */
            bool isSelected() const;

            /** Sets whether the gui item is visible or not. */
            virtual void setVisible( bool isVisible, bool cascade = true );

            /** Returns a boolean value indicating whether or not the ui item is visible. */
            bool isVisible() const;

            /** Sets whether this gui item is in focus. */
            virtual void setFocus( bool hasFocus );

            /** Checks if the this gui item is in focus. */
            bool isInFocus() const;

            /** Sets this gui item as being highlighted. */
            void setHighlighted( bool isHighlighted, bool cascade = true );

            /** Check if this gui item is highlighted. */
            bool isHighlighted() const;

            SmartPtr<IUIElement> getParent() const;

            void setParent( SmartPtr<IUIElement> parent );

            /** Gets the children of the gui item. */
            SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> getChildren() const;

            void setChildren( SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> p );

            /** Adds a child to this gui item. */
            void addChild( SmartPtr<IUIElement> child ) override;

            /** Removes a child of the gui item. */
            bool removeChild( SmartPtr<IUIElement> child ) override;

            /** Removes this element. */
            void remove();

            /** Removes a the children of this gui item. */
            void removeAllChildren();

            void destroyAllChildren();

            /** Check if the check item has a child. */
            virtual bool hasChildById( const String &id ) const;

            /** Find a child by id. */
            virtual SmartPtr<IUIElement> findChildById( const String &id ) const;

            /** Finds a component by id. */
            SmartPtr<IUIElement> findChildByComponentId( const String &componentId ) const;

            SmartPtr<IUIElement> getLayout() const;
            void setLayout( SmartPtr<IUIElement> layout );

            /** Sets the attached user data.*/
            void setUserData( void *userData ) override;

            /** Gets the user data attached. */
            void *getUserData() const override;

            /** Finds a gui item listener. */
            SmartPtr<IEventListener> findObjectListener( const String &id ) const;

            /** Adds an animator to the container. */
            void addAnimator( SmartPtr<IAnimator> &animator );

            /** Removes an animator from the container. */
            bool removeAnimator( SmartPtr<IAnimator> &animator );

            SmartPtr<IUIContainer> getContainer() const;
            void setContainer( SmartPtr<IUIContainer> container );

            /** @copydoc IComponent::getOwner */
            virtual SmartPtr<ISharedObject> getOwner() const;

            /** @copydoc IComponent::setOwner */
            virtual void setOwner( SmartPtr<ISharedObject> owner );

            void _getObject( void **ppObject ) const;

            /** @copydoc IComponent::getProperties */
            SmartPtr<Properties> getProperties() const override;

            /** @copydoc IComponent::setProperties */
            void setProperties( SmartPtr<Properties> properties ) override;

            Array<SmartPtr<ISharedObject>> getChildObjects() const override;

            bool isDragDropSource() const;

            void setDragDropSource( bool dragDropSource );

            SmartPtr<IUIDragSource> getDragSource() const;

            void setDragSource( SmartPtr<IUIDragSource> dragSource );

            SmartPtr<IUIDropTarget> getDropTarget() const;

            void setDropTarget( SmartPtr<IUIDropTarget> dropTarget );

            virtual u32 getOrder() const;

            virtual void setOrder( u32 order );

            /** Internal function. */
            virtual void _onInitialiseStart();

            /** Internal function. */
            virtual void _onInitialiseEnd();

            //
            // Events
            //

            virtual void onAddChild( IUIElement *child );
            virtual void onRemoveChild( IUIElement *child );
            virtual void onChangedState();
            virtual void onChildChangedState( IUIElement *child );
            virtual void onToggleEnabled();
            virtual void onToggleVisibility();
            virtual void onToggleHighlight();
            virtual void onActivate( SmartPtr<IUIElement> element );
            virtual void onDeactivate();
            virtual void onSelect();
            virtual void onDeselect();
            virtual void onGainFocus();
            virtual void onLostFocus();

            virtual void handleEvent( const SmartPtr<IEvent> &event );

            SmartPtr<render::IOverlayElement> getOverlayElement() const;

            void setOverlayElement( SmartPtr<render::IOverlayElement> overlayElement );

            bool getSameLine() const;

            void setSameLine( bool sameLine );

            ColourF getColour() const;

            void setColour( const ColourF &colour );

            SmartPtr<IStateContext> getStateObject() const;

            void setStateObject( SmartPtr<IStateContext> stateObject );

            SmartPtr<IStateListener> getStateListener() const;

            void setStateListener( SmartPtr<IStateListener> stateListener );

            void sortZOrder();

            void updateZOrder();

            virtual void handleStateChanged( SmartPtr<IState> &state );

            bool getHandleInputEvents() const;

            void setHandleInputEvents( bool handleInputEvents );

            bool isThreadSafe() const;

            void addMessage( SmartPtr<IStateMessage> message );

        protected:
            class ElementStateListener : public SharedObject<IStateListener>
            {
            public:
                ElementStateListener() = default;
                ~ElementStateListener() override = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;

                void handleStateChanged( SmartPtr<IState> &state ) override;

                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                SmartPtr<CUIElement<T>> getOwner() const;

                void setOwner( SmartPtr<CUIElement<T>> owner );

            protected:
                AtomicSmartPtr<CUIElement<T>> m_owner;
            };

            // general event
            virtual void onEvent( const String &eventType );

            void setType( const String &type );

            virtual void createStateContext();

            AtomicSmartPtr<IStateContext> m_stateObject;
            AtomicSmartPtr<IStateListener> m_stateListener;

            AtomicSmartPtr<ISharedObject> m_owner;

            SmartPtr<IUIDragSource> m_dragSource;
            SmartPtr<IUIDropTarget> m_dropTarget;

            SmartPtr<render::IOverlayElement> m_overlayElement;

            SmartPtr<IUIContainer> m_container;

            /// The parent gui element.
            SmartPtr<IUIElement> m_parent;

            /// The attached user data.
            void *m_userData;

            SmartPtr<IUIElement> m_layout;

            /// Used to store user properties.
            mutable SmartPtr<ISharedObject> m_userProperties;

            s32 m_itemId = -1;

            /// Used to know if the element is enabled.
            bool m_isEnabled = true;

            /// To know if the element has focus.
            bool m_hasFocus = false;

            /// To know if the element is highlighted.
            bool m_isHighlighted = false;

            /// To know if the element is selected.
            bool m_isSelected = false;

            bool m_dragDropSource = false;

            bool m_sameLine = false;

            bool m_handleInputEvents = true;

            /// The id of the element.
            String m_name;

            /// The hashed id of the element.
            hash_type m_hashId;

            /// The id of the component.
            String m_componentId;

            /// The type of gui element.
            String m_type;

            /// The children of the gui element.
            SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> m_children;

            /// The number of the next name extension.
            static u32 m_nextGeneratedNameExt;
        };

        template <class T>
        bool CUIElement<T>::isThreadSafe() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto renderTask = graphicsSystem->getRenderTask();

            auto task = Thread::getCurrentTask();

            const auto &loadingState = SharedObject<T>::getLoadingState();

            return loadingState == LoadingState::Loaded && task == renderTask;
        }

        template <class T>
        void CUIElement<T>::addMessage( SmartPtr<IStateMessage> message )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();

            if( auto stateObject = CUIElement<T>::getStateObject() )
            {
                const auto stateTask = graphicsSystem->getStateTask();
                stateObject->addMessage( stateTask, message );
            }
        }

        template <class T>
        SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> CUIElement<T>::getChildren() const
        {
            return m_children;
        }

        template <class T>
        void CUIElement<T>::setChildren( SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> p )
        {
            m_children = p;
        }

    }  // end namespace ui
}  // end namespace fb

#endif
