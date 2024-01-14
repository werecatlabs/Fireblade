#ifndef _FB_GUIElement_H
#define _FB_GUIElement_H

#include <FBCore/Interface/UI/IUIElement.h>
#include <FBCore/Interface/UI/IUIContainer.h>
#include <FBCore/Interface/UI/IUIDropTarget.h>
#include <FBCore/Interface/UI/IUIDragSource.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/Script/IScriptInvoker.h>
#include <FBCore/Interface/Script/IScriptReceiver.h>
#include <FBCore/Interface/Script/IScriptData.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Core/Parameter.h>
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

            void setHovered( bool hovered )
            {
                m_isHovered = hovered;
            }

            bool isHovered() const
            {
                return m_isHovered;
            }

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

            s32 getSiblingIndex() const
            {
                if( m_parent )
                {
                    auto pThis = CImGuiElement<T>::template getSharedFromThis<IUIElement>();
                    auto p = m_parent->getChildren();
                    if( p )
                    {
                        auto children = Array<SmartPtr<IUIElement>>( p->begin(), p->end() );
                        for( u32 i = 0; i < children.size(); ++i )
                        {
                            if( children.at( i ) == pThis )
                            {
                                return i;
                            }
                        }
                    }
                }

                return -1;
            }

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

            SmartPtr<IUIDragSource> &getDragSource();
            const SmartPtr<IUIDragSource> &getDragSource() const;

            void setDragSource( SmartPtr<IUIDragSource> dragSource );

            SmartPtr<IUIDropTarget> &getDropTarget();
            const SmartPtr<IUIDropTarget> &getDropTarget() const;

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

            SmartPtr<IStateContext> getStateContext() const;

            void setStateContext( SmartPtr<IStateContext> stateContext );

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
            class BaseStateListener : public IStateListener
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

            mutable RecursiveMutex m_childrenMutex;

            AtomicSmartPtr<ISharedObject> m_owner;

            AtomicSmartPtr<IStateContext> m_stateContext;
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

            bool m_isHovered = false;

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

        FB_CLASS_REGISTER_DERIVED_TEMPLATE( fb::ui, CImGuiElement, T, core::Prototype<T> );

        template <class T>
        u32 CImGuiElement<T>::m_nextGeneratedNameExt = 0;

        template <class T>
        CImGuiElement<T>::CImGuiElement() :
            m_parent( nullptr ),
            m_userData( nullptr ),
            m_isEnabled( true ),
            m_isVisible( true ),
            m_hasFocus( false ),
            m_isHighlighted( false ),
            m_isSelected( false )
        {
            m_name = String( "GUIElement" ) + StringUtil::toString( m_nextGeneratedNameExt++ );

            m_layout = nullptr;

            m_size = Vector2F::zero();

            m_children = fb::make_shared<ConcurrentArray<SmartPtr<IUIElement>>>();
        }

        template <class T>
        CImGuiElement<T>::~CImGuiElement()
        {
            unload( nullptr );
        }

        template <class T>
        void CImGuiElement<T>::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                setContainer( nullptr );

                m_parent = nullptr;

                m_layout = nullptr;
                m_userProperties = nullptr;

                m_dragSource = nullptr;
                m_dropTarget = nullptr;

                m_container = nullptr;

                m_scriptInvoker = nullptr;
                m_scriptReceiver = nullptr;

                m_scriptData = nullptr;

                if( auto p = getChildren() )
                {
                    auto &children = *p;
                    for( auto child : children )
                    {
                        child->unload( nullptr );
                    }

                    children.clear();
                    setChildren( nullptr );
                }

                if( auto stateContext = getStateContext() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateContext->removeStateListener( stateListener );
                        setStateListener( nullptr );
                    }

                    if( auto stateManager = applicationManager->getStateManager() )
                    {
                        stateManager->removeStateObject( stateContext );
                    }

                    setStateContext( nullptr );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CImGuiElement<T>::update()
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            if( task != Thread::Task::Application )
                return;

            if( !m_isVisible )
                return;

            if( !m_isEnabled )
                return;

            //if( !m_listeners.empty() )
            //{
            //    Parameters params;
            //    params[0].setPtr( this );
            //    params[1].setU32( static_cast<u32>( task ) );
            //    params[2].setF64( t );
            //    params[3].setF64( dt );

            //    // for (u32 i = 0; i < m_listeners.size(); ++i)
            //    //{
            //    //	m_listeners[i]->OnEvent(StringUtil::getHash("update"), params);
            //    // }
            //}

            // for(u32 i=0; i<m_children.size(); ++i)
            //{
            //	m_children[i]->update();
            // }
        }

        template <class T>
        bool CImGuiElement<T>::handleEvent( const SmartPtr<IInputEvent> &event )
        {
            // if(!m_isEnabled)
            //	return false;

            // for(u32 i=0; i<m_inputListeners.size(); ++i)
            //{
            //	IGUIItemInputListener* inputListener = m_inputListeners[i];
            //	if(inputListener->onEvent(event))
            //		return true;
            // }

            // for (u32 i = 0; i < m_children.size(); ++i)
            //{
            //	CUIElement* pGUIItem = (CUIElement*)m_children[i];

            //	if (pGUIItem->getName() == ("CreateShip0"))
            //	{
            //		int i = 0;
            //		i = i;
            //	}

            //	if (pGUIItem->onEvent(event))
            //	{
            //		return true;
            //	}
            //}

            return false;
        }

        template <class T>
        Vector2F CImGuiElement<T>::getAbsolutePosition() const
        {
            if( m_parent )
            {
                return m_parent->getAbsolutePosition() + m_position;
            }

            return m_position;
        }

        template <class T>
        void CImGuiElement<T>::setEnabled( bool enabled, bool cascade )
        {
            if( m_isEnabled != enabled )
            {
                m_isEnabled = enabled;

                if( cascade )
                {
                    if( auto p = getChildren() )
                    {
                        auto &children = *p;
                        for( u32 i = 0; i < children.size(); ++i )
                        {
                            children[i]->setEnabled( m_isEnabled );
                        }
                    }
                }

                /*if(!m_isEnabled && m_hasFocus)
                setFocus( false );*/

                onToggleEnabled();
            }
        }

        template <class T>
        void CImGuiElement<T>::setSelected( bool selected )
        {
            bool wasSelected = m_isSelected;
            m_isSelected = selected;

            if( wasSelected && !m_isSelected )
            {
                onDeselect();
            }
            else if( !wasSelected && m_isSelected )
            {
                onSelect();
            }
        }

        template <class T>
        bool CImGuiElement<T>::isSelected() const
        {
            return m_isSelected;
        }

        template <class T>
        void CImGuiElement<T>::setVisible( bool visible, bool cascade )
        {
            if( m_isVisible != visible )
            {
                m_isVisible = visible;

                if( cascade )
                {
                    if( auto p = getChildren() )
                    {
                        auto &children = *p;
                        for( auto &child : children )
                        {
                            if( child )
                            {
                                child->setVisible( m_isVisible, cascade );
                            }
                        }
                    }
                }

                onToggleVisibility();
            }
        }

        template <class T>
        void CImGuiElement<T>::setFocus( bool hasFocus )
        {
            // if (m_hasFocus && !hasFocus)
            //	onLostFocus();

            // m_hasFocus = hasFocus;

            // for (u32 i = 0; i < m_children.size(); ++i)
            //{
            //	((CUIElement*)m_children[i])->setFocus(m_hasFocus);
            // }

            // if (m_hasFocus)
            //	onGainFocus();
        }

        template <class T>
        void CImGuiElement<T>::setHighlighted( bool isHighlighted, bool cascade )
        {
            // if (m_isHighlighted != isHighlighted)
            //{
            //	m_isHighlighted = isHighlighted;

            //	if (cascade)
            //	{
            //		for (u32 i = 0; i < m_children.size(); ++i)
            //		{
            //			((CUIElement*)m_children[i])->setHighlighted(m_isHighlighted);
            //		}
            //	}

            //	onToggleHighlight();
            //}
        }

        template <class T>
        SharedPtr<ConcurrentArray<SmartPtr<IUIElement>>> CImGuiElement<T>::getChildren() const
        {
            return m_children;
        }

        template <class T>
        void CImGuiElement<T>::addChild( SmartPtr<IUIElement> pGUIItem )
        {
            if( pGUIItem )
            {
                pGUIItem->remove();  // remove from old parent
                pGUIItem->setParent( this );

                auto p = getChildren();
                if( !p )
                {
                    p = fb::make_shared<ConcurrentArray<SmartPtr<IUIElement>>>();
                    setChildren( p );
                }

                if( p )
                {
                    RecursiveMutex::ScopedLock lock( m_childrenMutex );

                    auto &children = *p;
                    children.push_back( pGUIItem );
                }

                auto layout = getLayout();
                pGUIItem->setLayout( layout );

                onAddChild( pGUIItem.get() );
            }
        }

        template <class T>
        bool CImGuiElement<T>::removeChild( SmartPtr<IUIElement> element )
        {
            if( element )
            {
                if( auto p = getChildren() )
                {
                    RecursiveMutex::ScopedLock lock( m_childrenMutex );

                    auto &children = *p;

                    auto childrenArray = Array<SmartPtr<IUIElement>>( children.begin(), children.end() );

                    auto it = std::find( childrenArray.begin(), childrenArray.end(), element );
                    if( it != childrenArray.end() )
                    {
                        element->setParent( nullptr );

                        childrenArray.erase( it );

                        onRemoveChild( element.get() );

                        auto newChildren = fb::make_shared<ConcurrentArray<SmartPtr<IUIElement>>>(
                            childrenArray.begin(), childrenArray.end() );
                        setChildren( newChildren );
                        return true;
                    }
                }
            }

            return false;
        }

        template <class T>
        void CImGuiElement<T>::remove()
        {
            if( m_parent )
            {
                m_parent->removeChild( this );
            }
        }

        template <class T>
        void CImGuiElement<T>::removeAllChildren()
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        child->setParent( nullptr );
                    }
                }

                setChildren( nullptr );
            }
        }

        template <class T>
        void CImGuiElement<T>::destroyAllChildren()
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto ui = applicationManager->getUI();

            if( auto p = getChildren() )
            {
                auto &children = *p;

                for( auto child : children )
                {
                    if( child )
                    {
                        child->destroyAllChildren();
                    }
                }

                for( auto child : children )
                {
                    if( child )
                    {
                        ui->removeElement( child );
                    }
                }

                setChildren( nullptr );
            }
        }

        template <class T>
        bool CImGuiElement<T>::hasChildById( const String &id ) const
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CImGuiElement<T>>( child );
                        if( id == childElement->getName() )
                        {
                            return true;
                        }
                    }
                }

                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CImGuiElement<T>>( child );
                        if( childElement->hasChildById( id ) )
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        template <class T>
        SmartPtr<IUIElement> CImGuiElement<T>::findChildById( const String &id ) const
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CImGuiElement<T>>( child );
                        if( id == childElement->getName() )
                        {
                            return child;
                        }
                    }
                }

                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CImGuiElement<T>>( child );
                        if( childElement->hasChildById( id ) )
                        {
                            return child;
                        }
                    }
                }
            }

            return nullptr;
        }

        template <class T>
        SmartPtr<IUIElement> CImGuiElement<T>::findChildByComponentId( const String &componentId ) const
        {
            // for(u32 i=0; i<m_children.size(); ++i)
            //{
            //	GUIElement* pGUIItem = (GUIElement*)m_children[i];
            //	if(componentId==(pGUIItem->getComponentID()))
            //	{
            //		return pGUIItem;
            //	}
            // }

            // SmartPtr<IGUIElement> pCompGUIItem;
            // for(u32 i=0; i<m_children.size(); ++i)
            //{
            //	GUIElement* pGUIItem = (GUIElement*)m_children[i];
            //	pCompGUIItem = pGUIItem->findChildByComponentId(componentId);
            //	if(!pCompGUIItem.isNull())
            //		return pCompGUIItem;
            // }

            // FB_ASSERT_TRUE(true);
            // String message = String("Could not find gui component: ") + componentId;
            // FB_EXCEPTION(message.c_str());

            return nullptr;
        }

        template <class T>
        void CImGuiElement<T>::setUserData( void *pUserData )
        {
            m_userData = pUserData;
        }

        template <class T>
        void *CImGuiElement<T>::getUserData() const
        {
            return m_userData;
        }

        template <class T>
        void CImGuiElement<T>::addAnimator( SmartPtr<IAnimator> &animator )
        {
        }

        template <class T>
        bool CImGuiElement<T>::removeAnimator( SmartPtr<IAnimator> &animator )
        {
            return false;
        }

        template <class T>
        SmartPtr<IUIContainer> CImGuiElement<T>::getContainer() const
        {
            return m_container;
        }

        template <class T>
        void CImGuiElement<T>::setContainer( SmartPtr<IUIContainer> container )
        {
            m_container = container;
        }

        template <class T>
        void CImGuiElement<T>::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }

        //
        // Callbacks
        //

        template <class T>
        void CImGuiElement<T>::_onInitialiseStart()
        {
        }

        template <class T>
        void CImGuiElement<T>::_onInitialiseEnd()
        {
            //if( !m_listeners.empty() )
            //{
            //    Parameters params;
            //    // params.set_used(2);
            //    params[0].setPtr( this );

            //    // for (u32 i = 0; i < m_listeners.size(); ++i)
            //    //{
            //    //	m_listeners[i]->OnEvent(StringUtil::getHash("initialise"), params);
            //    // }
            //}
        }

        template <class T>
        void CImGuiElement<T>::onAddChild( IUIElement *child )
        {
            //for( u32 i = 0; i < m_listeners.size(); ++i )
            //    m_listeners[i]->handleAddChild( this, child );
        }

        template <class T>
        void CImGuiElement<T>::onRemoveChild( IUIElement *child )
        {
            //for( u32 i = 0; i < m_listeners.size(); ++i )
            //    m_listeners[i]->handleRemoveChild( this, child );
        }

        template <class T>
        void CImGuiElement<T>::onChangedState()
        {
            //for( u32 i = 0; i < m_listeners.size(); ++i )
            //    m_listeners[i]->handleChangedState( this );
        }

        template <class T>
        void CImGuiElement<T>::onChildChangedState( IUIElement *child )
        {
            //for( u32 i = 0; i < m_listeners.size(); ++i )
            //    m_listeners[i]->handleChildChangedState( this, child );
        }

        template <class T>
        void CImGuiElement<T>::onToggleEnabled()
        {
            //for( u32 i = 0; i < m_listeners.size(); ++i )
            //    m_listeners[i]->handleToggleEnabled( this );

            // if (m_parent)
            //	m_parent->onChildChangedState(this);
        }

        template <class T>
        void CImGuiElement<T>::onToggleVisibility()
        {
            //for( u32 i = 0; i < m_listeners.size(); ++i )
            //    m_listeners[i]->handleToggleVisibility( this );

            // if (m_parent)
            //	m_parent->onChildChangedState(this);

            if( isVisible() )
            {
                //if( !m_listeners.empty() )
                //{
                //    Parameters params;
                //    params.resize( 1 );
                //    params[0].setPtr( this );

                //    // for (u32 i = 0; i < m_listeners.size(); ++i)
                //    //	m_listeners[i]->OnEvent(StringUtil::getHash("show"), params);
                //}
            }
            else
            {
                //if( !m_listeners.empty() )
                //{
                //    Parameters params;
                //    params.resize( 1 );
                //    params[0].setPtr( this );

                //    // for (u32 i = 0; i < m_listeners.size(); ++i)
                //    //	m_listeners[i]->OnEvent(StringUtil::getHash("hide"), params);
                //}
            }
        }

        template <class T>
        void CImGuiElement<T>::onToggleHighlight()
        {
            //for( u32 i = 0; i < m_listeners.size(); ++i )
            //    m_listeners[i]->handleToggleHighlight( this );

            // if (m_parent)
            //	m_parent->onChildChangedState(this);
        }

        template <class T>
        void CImGuiElement<T>::onActivate( SmartPtr<IUIElement> element )
        {
            // if (!m_listeners.empty())
            //{
            //	Parameters params;
            //	//params.set_used(1);
            //	params[0].setPtr(element.get());

            //	for (u32 i = 0; i < m_listeners.size(); ++i)
            //		m_listeners[i]->OnEvent(StringUtil::getHash("activate"), params);
            //}

            // for (u32 i = 0; i < m_children.size(); ++i)
            //	((CUIElement*)m_children[i])->onActivate(element);

            // if (m_parent)
            //	((CUIElement*)m_parent)->onChildChangedState(this);
        }

        template <class T>
        void CImGuiElement<T>::onDeactivate()
        {
            // for (u32 i = 0; i < m_listeners.size(); ++i)
            //	m_listeners[i]->onDeactivate(this);

            // for (u32 i = 0; i < m_children.size(); ++i)
            //	((CUIElement*)m_children[i])->onDeactivate();

            // if (m_parent)
            //	((CUIElement*)m_parent)->onChildChangedState(this);
        }

        template <class T>
        void CImGuiElement<T>::onSelect()
        {
            // for (u32 i = 0; i < m_listeners.size(); ++i)
            //	m_listeners[i]->onSelect(this);

            // for (u32 i = 0; i < m_children.size(); ++i)
            //	((CUIElement*)m_children[i])->onSelect();

            // if (m_parent)
            //	((CUIElement*)m_parent)->onChildChangedState(this);
        }

        template <class T>
        void CImGuiElement<T>::onDeselect()
        {
            // for (u32 i = 0; i < m_listeners.size(); ++i)
            //	m_listeners[i]->onDeselect(this);

            // for (u32 i = 0; i < m_children.size(); ++i)
            //	((CUIElement*)m_children[i])->onDeselect();

            // if (m_parent)
            //	((CUIElement*)m_parent)->onChildChangedState(this);
        }

        template <class T>
        void CImGuiElement<T>::onGainFocus()
        {
            //for( u32 i = 0; i < m_listeners.size(); ++i )
            //    m_listeners[i]->handleGainFocus( this );

            // if (m_parent)
            //	m_parent->onChildChangedState(this);
        }

        template <class T>
        void CImGuiElement<T>::onLostFocus()
        {
            //for( u32 i = 0; i < m_listeners.size(); ++i )
            //    m_listeners[i]->handleLostFocus( this );

            // if (m_parent)
            //	m_parent->onChildChangedState(this);
        }

        template <class T>
        void CImGuiElement<T>::onEvent( const String &eventType )
        {
            // for (u32 i = 0; i < m_children.size(); ++i)
            //	((CUIElement*)m_children[i])->onEvent(eventType);

            // for (u32 i = 0; i < m_listeners.size(); ++i)
            //	m_listeners[i]->onEvent(eventType);
        }

        template <class T>
        s32 CImGuiElement<T>::setProperty( hash32 hash, const Parameter &param )
        {
            // if(hash == StringUtil::VISIBLE_HASH)
            //{
            //	setVisible(param.getBool());
            // }

            return 0;
        }

        template <class T>
        s32 CImGuiElement<T>::getObject( u32 hash, SmartPtr<ISharedObject> &object ) const
        {
            if( hash == StringUtil::getHash( "userProperties" ) )
            {
                // if(!m_userProperties)
                //	m_userProperties = SmartPtr<ISharedObject>(new GUIElementProperties, true);

                object = m_userProperties;
            }
            else if( hash == StringUtil::getHash( "animations" ) )
            {
                // object = m_animatorContainer;
            }

            // for(u32 i=0; i<m_children.size(); ++i)
            //{
            //	GUIElement* pGUIItem = (GUIElement*)m_children[i];
            //	if(hash == pGUIItem->getHashId())
            //	{
            //		object = pGUIItem;
            //	}
            // }

            // SmartPtr<IGUIElement> pCompGUIItem;
            // for(u32 i=0; i<m_children.size(); ++i)
            //{
            //	GUIElement* pGUIItem = (GUIElement*)m_children[i];
            //	pGUIItem->getObject(hash, object);
            //	if(!object.isNull())
            //		return 0;
            // }

            return 0;
        }

        template <class T>
        void CImGuiElement<T>::setPosition( const Vector2F &position )
        {
            m_position = position;
        }

        template <class T>
        SmartPtr<IScriptInvoker> &CImGuiElement<T>::getInvoker()
        {
            return m_scriptInvoker;
        }

        template <class T>
        const SmartPtr<IScriptInvoker> &CImGuiElement<T>::getInvoker() const
        {
            return m_scriptInvoker;
        }

        template <class T>
        void CImGuiElement<T>::setInvoker( SmartPtr<IScriptInvoker> invoker )
        {
            m_scriptInvoker = invoker;
        }

        template <class T>
        SmartPtr<IScriptReceiver> &CImGuiElement<T>::getReceiver()
        {
            return m_scriptReceiver;
        }

        template <class T>
        const SmartPtr<IScriptReceiver> &CImGuiElement<T>::getReceiver() const
        {
            return m_scriptReceiver;
        }

        template <class T>
        void CImGuiElement<T>::setReceiver( SmartPtr<IScriptReceiver> receiver )
        {
            m_scriptReceiver = receiver;
        }

        template <class T>
        void CImGuiElement<T>::_setData( SmartPtr<IScriptData> data )
        {
            m_scriptData = data;
        }

        template <class T>
        SmartPtr<IScriptData> CImGuiElement<T>::_getData() const
        {
            return m_scriptData;
        }

        template <class T>
        void CImGuiElement<T>::initialise( SmartPtr<IUIElement> &parent )
        {
        }

        template <class T>
        void CImGuiElement<T>::initialise( SmartPtr<IUIElement> &parent, const Properties &properties )
        {
        }

        template <class T>
        void CImGuiElement<T>::setName( const String &name )
        {
            m_name = name;
            m_hashId = StringUtil::getHash( m_name );
        }

        template <class T>
        String CImGuiElement<T>::getName() const
        {
            return m_name;
        }

        template <class T>
        void CImGuiElement<T>::setComponentID( const String &componentId )
        {
            m_componentId = componentId;
        }

        template <class T>
        const String &CImGuiElement<T>::getComponentID() const
        {
            return m_componentId;
        }

        template <class T>
        void CImGuiElement<T>::setType( const String &type )
        {
            m_type = type;
        }

        template <class T>
        const String &CImGuiElement<T>::getType() const
        {
            return m_type;
        }

        template <class T>
        Vector2F CImGuiElement<T>::getPosition() const
        {
            return m_position;
        }

        template <class T>
        void CImGuiElement<T>::setSize( const Vector2F &size )
        {
            m_size = size;
        }

        template <class T>
        Vector2F CImGuiElement<T>::getSize() const
        {
            return m_size;
        }

        template <class T>
        bool CImGuiElement<T>::isEnabled() const
        {
            return m_isEnabled;
        }

        template <class T>
        bool CImGuiElement<T>::isVisible() const
        {
            return m_isVisible;
        }

        template <class T>
        bool CImGuiElement<T>::isInFocus() const
        {
            return m_hasFocus;
        }

        template <class T>
        bool CImGuiElement<T>::isHighlighted() const
        {
            return m_isHighlighted;
        }

        template <class T>
        s32 CImGuiElement<T>::setProperty( hash32 hash, const String &value )
        {
            return 0;
        }

        template <class T>
        s32 CImGuiElement<T>::setProperty( hash32 hash, void *param )
        {
            return 0;
        }

        template <class T>
        u32 CImGuiElement<T>::getHashId() const
        {
            return (u32)m_hashId;
        }

        template <class T>
        SmartPtr<IUIElement> CImGuiElement<T>::getLayout() const
        {
            return m_layout;
        }

        template <class T>
        void CImGuiElement<T>::setLayout( SmartPtr<IUIElement> val )
        {
            m_layout = val;
        }

        template <class T>
        void CImGuiElement<T>::handleEvent( const SmartPtr<IEvent> &event )
        {
        }

        template <class T>
        s32 CImGuiElement<T>::setProperty( hash32 hash, const Parameters &params )
        {
            return 0;
        }

        template <class T>
        s32 CImGuiElement<T>::getProperty( hash32 hash, Parameters &params ) const
        {
            return 0;
        }

        template <class T>
        s32 CImGuiElement<T>::getProperty( hash32 hash, Parameter &param )
        {
            return 0;
        }

        // template <class T>
        // s32 CUIElement<T>::getProperty(hash32 hash, const Parameter& param)
        //{
        //	return 0;
        // }

        template <class T>
        s32 CImGuiElement<T>::getProperty( hash32 hash, String &value ) const
        {
            return 0;
        }

        template <class T>
        s32 CImGuiElement<T>::getProperty( hash32 hash, void *param ) const
        {
            return 0;
        }

        template <class T>
        s32 CImGuiElement<T>::callFunction( u32 hash, SmartPtr<ISharedObject> object,
                                            Parameters &results )
        {
            return 0;
        }

        template <class T>
        s32 CImGuiElement<T>::callFunction( u32 hash, const Parameters &params, Parameters &results )
        {
            return 0;
        }

    }  // end namespace ui
}  // end namespace fb

#include <FBImGui/CImGuiElement.inl>

#endif
