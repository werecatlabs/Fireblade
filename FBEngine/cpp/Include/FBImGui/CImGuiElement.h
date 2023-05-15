#ifndef _FB_GUIElement_H
#define _FB_GUIElement_H

#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Interface/UI/IUIDropTarget.h>
#include <FBCore/Interface/UI/IUIDragSource.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Base/Parameter.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/System/Prototype.h>

namespace fb
{
    namespace ui
    {
        /** A base gui element.
         */
        template <class T>
        class CImGuiElement : public core::Prototype<T>
        {
        public:
            using element_type = T;

            /** Default constructor. */
            CImGuiElement();

            /** Virtual destructor. */
            ~CImGuiElement() override;

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

            virtual void setElementId( s32 itemId );

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
            virtual void setVisible( bool visible, bool cascade = true );

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

            /** Adds a child to this gui item. */
            void addChild( SmartPtr<IUIElement> pGUIItem ) override;

            /** Removes a child of the gui item. */
            bool removeChild( SmartPtr<IUIElement> pGUIItem ) override;

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
            void setLayout( SmartPtr<IUIElement> val );

            /** Sets the attached user data.*/
            void setUserData( void *userData ) override;

            /** Gets the user data attached. */
            void *getUserData() const override;

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

            //
            // IScriptObjects
            //

            /** Gets an object call script functions. */
            virtual SmartPtr<IScriptInvoker> &getInvoker();

            /** Gets an object call script functions. */
            virtual const SmartPtr<IScriptInvoker> &getInvoker() const;

            /** Sets an object call script functions. */
            virtual void setInvoker( SmartPtr<IScriptInvoker> invoker );

            /** Gets an object to receive script calls. */
            virtual SmartPtr<IScriptReceiver> &getReceiver();

            /** Gets an object to receive script calls. */
            virtual const SmartPtr<IScriptReceiver> &getReceiver() const;

            /** Sets an object to receive script calls. */
            virtual void setReceiver( SmartPtr<IScriptReceiver> receiver );

            /** Internal function used by the script system. */
            virtual void _setData( SmartPtr<IScriptData> data );

            /** Internal function used by the script system. */
            virtual SmartPtr<IScriptData> _getData() const;

            /** Sets a property. */
            virtual s32 setProperty( hash32 hash, const String &value );

            /** Gets a property. */
            virtual s32 getProperty( hash32 hash, String &value ) const;

            /** Sets a property. */
            virtual s32 setProperty( hash32 hash, const Parameter &param );

            /** Sets a property. */
            virtual s32 setProperty( hash32 hash, const Parameters &params );

            /** Sets a property. */
            virtual s32 setProperty( hash32 hash, void *param );

            /** Gets a property. */
            virtual s32 getProperty( hash32 hash, Parameter &param );

            /** Gets a property. */
            virtual s32 getProperty( hash32 hash, Parameters &params ) const;

            /** Gets a property. */
            virtual s32 getProperty( hash32 hash, void *param ) const;

            /** Overridden from IScriptObject*/
            virtual s32 getObject( u32 hash, SmartPtr<ISharedObject> &object ) const;

            /** Calls a function. */
            virtual s32 callFunction( u32 hash, const Parameters &params, Parameters &results );

            /** Calls a function. */
            virtual s32 callFunction( u32 hash, SmartPtr<ISharedObject> object, Parameters &results );

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

            SmartPtr<IStateContext> getStateObject() const;

            void setStateObject( SmartPtr<IStateContext> stateObject );

            SmartPtr<IStateListener> getStateListener() const;

            void setStateListener( SmartPtr<IStateListener> stateListener );

            bool getSameLine() const;

            void setSameLine( bool sameLine );

            ColourF getColour() const;

            void setColour( const ColourF &colour );

            bool getHandleInputEvents() const;

            void setHandleInputEvents( bool handleInputEvents );

            void updateZOrder();

            FB_CLASS_REGISTER_TEMPLATE_DECL( CImGuiElement, T );

        protected:
            class BaseStateListener : public CSharedObject<IStateListener>
            {
            public:
                BaseStateListener() = default;
                ~BaseStateListener() = default;

                void handleStateChanged( const SmartPtr<IStateMessage> &message )
                {
                }

                void handleStateChanged( SmartPtr<IState> &state )
                {
                }

                void handleQuery( SmartPtr<IStateQuery> &query )
                {
                }

                CImGuiElement *getOwner() const
                {
                    return m_owner;
                }

                void setOwner( CImGuiElement *owner )
                {
                    m_owner = owner;
                }

            protected:
                CImGuiElement *m_owner = nullptr;
            };

            void setChildren( SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> p );

            // general event
            virtual void onEvent( const String &eventType );

            void setType( const String &type );

            RecursiveMutex m_childrenMutex;

            AtomicSmartPtr<ISharedObject> m_owner;

            AtomicSmartPtr<IStateContext> m_stateObject;
            AtomicSmartPtr<IStateListener> m_stateListener;

            SmartPtr<IUIDragSource> m_dragSource;
            SmartPtr<IUIDropTarget> m_dropTarget;

            SmartPtr<IUIContainer> m_container;

            SmartPtr<IScriptInvoker> m_scriptInvoker;

            SmartPtr<IScriptReceiver> m_scriptReceiver;

            /// The data used by the script system.
            SmartPtr<IScriptData> m_scriptData;

            /// The position of the gui element.
            Vector2F m_position;

            /// The size of the gui element.
            Vector2F m_size;

            /// The parent gui element.
            SmartPtr<IUIElement> m_parent;

            /// The attached user data.
            void *m_userData;

            SmartPtr<IUIElement> m_layout;

            /// Used to store user properties.
            mutable SmartPtr<ISharedObject> m_userProperties;

            /// Used to know if the element is enabled.
            bool m_isEnabled;

            /// To know if the element is visible.
            bool m_isVisible;

            /// To know if the element has focus.
            bool m_hasFocus;

            /// To know if the element is highlighted.
            bool m_isHighlighted;

            /// To know if the element is selected.
            bool m_isSelected;

            bool m_dragDropSource = false;

            bool m_sameLine = false;

            /// The id of the element.
            String m_name;

            s32 m_itemId = -1;

            /// The hashed id of the element.
            hash_type m_hashId;

            /// The id of the component.
            String m_componentId;

            /// The type of ui element.
            String m_type;

            /// The children of the ui element.
            AtomicSharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> m_children;

            bool m_handleInputEvents = true;

            /// The number of the next name extension.
            static u32 m_nextGeneratedNameExt;
        };

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb, CImGuiElement, T, CSharedObject<T> );
    }  // end namespace ui
}  // end namespace fb

#include <FBImGui/CImGuiElement.inl>

#endif
