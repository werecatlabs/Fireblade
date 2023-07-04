#include <FBRenderUI/FBRenderUIPCH.h>
#include <FBRenderUI/CUIElement.h>
#include <FBRenderUI/CUIImage.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace ui
    {
        template <class T>
        u32 CUIElement<T>::m_nextGeneratedNameExt = 0;

        template <class T>
        void CUIElement<T>::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto renderUI = applicationManager->getRenderUI();

                setContainer( nullptr );

                removeAllChildren();

                CUIElement<T>::setObjectListenersPtr( nullptr );

                m_parent = nullptr;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        template <class T>
        void CUIElement<T>::update()
        {
            if( !isVisible() )
            {
                return;
            }

            if( !m_isEnabled )
            {
                return;
            }

            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            if( task == Thread::Task::Application )
            {
                if( auto p = CUIElement<T>::getObjectListenersPtr() )
                {
                    Parameters params;
                    params.resize( 4 );
                    params[0].setPtr( this );
                    params[1].setU32( static_cast<u32>( task ) );
                    params[2].setF64( t );
                    params[3].setF64( dt );

                    auto &listeners = *p;
                    for( auto &listener : listeners )
                    {
                        //listener->handleEvent( IEvent::Type::UI, IUIElementListener::UPDATE_HASH, params,
                        //                       this, nullptr );
                    }
                }

                if( auto p = getChildren() )
                {
                    auto &children = *p;
                    for( auto &child : children )
                    {
                        child->update();
                    }
                }
            }
        }

        template <class T>
        bool CUIElement<T>::handleEvent( const SmartPtr<IInputEvent> &event )
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
        Vector2F CUIElement<T>::getAbsolutePosition() const
        {
            if( m_parent )
            {
                return m_parent->getAbsolutePosition() + getPosition();
            }

            return getPosition();
        }

        template <class T>
        void CUIElement<T>::setEnabled( bool enabled, bool cascade )
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

                if( !m_isEnabled && m_hasFocus )
                {
                    setFocus( false );
                }

                onToggleEnabled();
            }
        }

        template <class T>
        void CUIElement<T>::setSelected( bool selected )
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
        bool CUIElement<T>::isSelected() const
        {
            return m_isSelected;
        }

        template <class T>
        void CUIElement<T>::setVisible( bool visible, bool cascade )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    state->setVisible( visible );
                }
            }

            if( cascade )
            {
                if( auto p = getChildren() )
                {
                    for( auto &child : *p )
                    {
                        child->setVisible( visible, cascade );
                    }
                }
            }

            //if( !visible && element->hasFocus() )
            //{
            //    setFocus( false );
            //}

            onToggleVisibility();
        }

        template <class T>
        void CUIElement<T>::setFocus( bool hasFocus )
        {
            if( m_hasFocus && !hasFocus )
            {
                onLostFocus();
            }

            m_hasFocus = hasFocus;

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );

                    childElement->setFocus( m_hasFocus );
                }

                if( m_hasFocus )
                {
                    onGainFocus();
                }
            }
        }

        template <class T>
        void CUIElement<T>::setHighlighted( bool isHighlighted, bool cascade )
        {
            if( m_isHighlighted != isHighlighted )
            {
                m_isHighlighted = isHighlighted;

                if( cascade )
                {
                    if( auto p = getChildren() )
                    {
                        auto &children = *p;
                        for( auto &child : children )
                        {
                            auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                            childElement->setHighlighted( m_isHighlighted );
                        }
                    }
                }

                onToggleHighlight();
            }
        }

        template <class T>
        CUIElement<T>::CUIElement() :
            m_parent( nullptr ),
            m_userData( nullptr ),
            m_isEnabled( false ),
            m_hasFocus( false ),
            m_isHighlighted( false ),
            m_isSelected( false )
        {
            m_name = String( "GUIElement" ) + StringUtil::toString( m_nextGeneratedNameExt++ );

            m_layout = nullptr;

            m_children = fb::make_shared<ConcurrentArray<SmartPtr<IUIElement>>>();
        }

        template <class T>
        CUIElement<T>::~CUIElement()
        {
            unload( nullptr );
        }

        template <class T>
        SmartPtr<ISharedObject> CUIElement<T>::getOwner() const
        {
            return m_owner;
        }

        template <class T>
        void CUIElement<T>::setOwner( SmartPtr<ISharedObject> owner )
        {
            m_owner = owner;
        }

        template <class T>
        bool CUIElement<T>::getSameLine() const
        {
            return m_sameLine;
        }

        template <class T>
        void CUIElement<T>::setSameLine( bool sameLine )
        {
            m_sameLine = sameLine;
        }

        template <class T>
        void CUIElement<T>::addChild( SmartPtr<IUIElement> child )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto factoryManager = applicationManager->getFactoryManager();
            auto renderTask = graphicsSystem->getRenderTask();

            auto task = Thread::getCurrentTask();

            const auto &loadingState = CUIElement<T>::getLoadingState();
            const auto &childLoadingState = child->getLoadingState();

            if( loadingState == LoadingState::Loaded && childLoadingState == LoadingState::Loaded &&
                task == renderTask )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto renderUI = applicationManager->getRenderUI();
                FB_ASSERT( renderUI );

                IUIManager::ScopedLock lock( renderUI );

                if( child )
                {
                    child->remove();  // remove from old parent
                    child->setParent( this );

                    auto p = getChildren();
                    if( !p )
                    {
                        p = fb::make_shared<ConcurrentArray<SmartPtr<IUIElement>>>();
                        setChildren( p );
                    }

                    if( p )
                    {
                        auto &children = *p;
                        children.push_back( child );
                    }

                    if( !this->template isDerived<ui::IUILayout>() )
                    {
                        auto layout = getLayout();
                        child->setLayout( layout );
                    }

                    onAddChild( child.get() );

                    updateZOrder();
                }

                auto childElement = fb::static_pointer_cast<CUIElement>( child );

                if( auto stateObject = childElement->getStateObject() )
                {
                    stateObject->setDirty( true );
                }
            }
            else
            {
                if( childLoadingState != LoadingState::Loaded )
                {
                    graphicsSystem->loadObject( child );
                }

                auto message = factoryManager->make_ptr<StateMessageObject>();
                message->setType( IUIElement::STATE_MESSAGE_ADD_CHILD );
                message->setObject( child );

                if( auto stateObject = getStateObject() )
                {
                    stateObject->addMessage( renderTask, message );
                }
            }
        }

        template <class T>
        bool CUIElement<T>::removeChild( SmartPtr<IUIElement> child )
        {
            if( child )
            {
                if( auto p = getChildren() )
                {
                    auto &children = *p;

                    auto childrenArray = Array<SmartPtr<IUIElement>>( children.begin(), children.end() );

                    auto it = std::find( childrenArray.begin(), childrenArray.end(), child );
                    if( it != childrenArray.end() )
                    {
                        child->setParent( nullptr );

                        childrenArray.erase( it );

                        onRemoveChild( child.get() );

                        *p = ConcurrentArray<SmartPtr<IUIElement>>( childrenArray.begin(),
                                                                    childrenArray.end() );
                        return true;
                    }
                }
            }

            return false;
        }

        template <class T>
        void CUIElement<T>::remove()
        {
            if( m_parent )
            {
                m_parent->removeChild( this );
            }
        }

        template <class T>
        void CUIElement<T>::removeAllChildren()
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

                children.clear();
            }
        }

        template <class T>
        void CUIElement<T>::destroyAllChildren()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto ui = applicationManager->getUI();

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        child->setParent( nullptr );

                        ui->removeElement( child );
                    }
                }

                setChildren( nullptr );
            }
        }

        template <class T>
        bool CUIElement<T>::hasChildById( const String &id ) const
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
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
                        auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
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
        SmartPtr<IUIElement> CUIElement<T>::findChildById( const String &id ) const
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    if( child )
                    {
                        auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
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
                        auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
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
        SmartPtr<IUIElement> CUIElement<T>::findChildByComponentId( const String &componentId ) const
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                    if( componentId == childElement->getComponentID() )
                    {
                        return childElement;
                    }
                }

                SmartPtr<IUIElement> pCompGUIItem;
                for( auto child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                    pCompGUIItem = childElement->findChildByComponentId( componentId );
                    if( pCompGUIItem )
                    {
                        return pCompGUIItem;
                    }
                }
            }

            return nullptr;
        }

        template <class T>
        void CUIElement<T>::setUserData( void *pUserData )
        {
            m_userData = pUserData;
        }

        template <class T>
        void *CUIElement<T>::getUserData() const
        {
            return m_userData;
        }

        template <class T>
        SmartPtr<IEventListener> CUIElement<T>::findObjectListener( const String &id ) const
        {
            if( auto p = core::Prototype<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto listener : listeners )
                {
                    auto handle = listener->getHandle();
                    if( handle->getName() == id )
                    {
                        return listener;
                    }
                }
            }

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    auto listener = child->findObjectListener( id );
                    if( listener )
                    {
                        return listener;
                    }
                }
            }

            return nullptr;
        }

        template <class T>
        void CUIElement<T>::addAnimator( SmartPtr<IAnimator> &animator )
        {
        }

        template <class T>
        bool CUIElement<T>::removeAnimator( SmartPtr<IAnimator> &animator )
        {
            return false;
        }

        template <class T>
        SmartPtr<IUIContainer> CUIElement<T>::getContainer() const
        {
            return m_container;
        }

        template <class T>
        void CUIElement<T>::setContainer( SmartPtr<IUIContainer> container )
        {
            m_container = container;
        }

        template <class T>
        void CUIElement<T>::_getObject( void **ppObject ) const
        {
            *ppObject = nullptr;
        }

        //
        // Callbacks
        //

        template <class T>
        void CUIElement<T>::_onInitialiseStart()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( this );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::INITIALISE_START_HASH, params, this,
                                           this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::_onInitialiseEnd()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( this );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::INITIALISE_END_HASH, params, this,
                                           this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onAddChild( IUIElement *child )
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( child );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::ADD_CHILD_HASH, params, this, this,
                                           nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onRemoveChild( IUIElement *child )
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( child );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::REMOVE_CHILD_HASH, params, this,
                                           this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onChangedState()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::CHANGED_STATE_HASH, params, this,
                                           this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onChildChangedState( IUIElement *child )
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( child );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::CHILD_CHANGED_STATE_HASH, params,
                                           this, this, nullptr );
                }
            }
        }

        template <class T>
        void CUIElement<T>::onToggleEnabled()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setPtr( this );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::TOGGLE_ENABLED_HASH, params, this,
                                           this, nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                auto parentElement = fb::static_pointer_cast<CUIElement<T>>( parent );
                parentElement->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onToggleVisibility()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                Parameters params;
                params.resize( 1 );
                params[0].setBool( isVisible() );

                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::VISIBLE_HASH, params, this, this,
                                           nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                auto parentElement = fb::static_pointer_cast<CUIElement<T>>( parent );
                parentElement->onChildChangedState( this );
            }

            if( isVisible() )
            {
                if( auto p = CUIElement<T>::getObjectListenersPtr() )
                {
                    auto &listeners = *p;
                    if( !listeners.empty() )
                    {
                        Parameters params;
                        params.resize( 1 );
                        params[0].setBool( isVisible() );

                        for( auto &listener : listeners )
                        {
                            listener->handleEvent( IEvent::Type::UI, IEvent::SHOW_HASH, params, this,
                                                   this, nullptr );
                        }
                    }
                }
            }
            else
            {
                if( auto p = CUIElement<T>::getObjectListenersPtr() )
                {
                    auto &listeners = *p;
                    if( !listeners.empty() )
                    {
                        Parameters params;
                        params.resize( 1 );
                        params[0].setBool( isVisible() );

                        for( auto &listener : listeners )
                        {
                            listener->handleEvent( IEvent::Type::UI, IEvent::HIDE_HASH, params, this,
                                                   this, nullptr );
                        }
                    }
                }
            }
        }

        template <class T>
        void CUIElement<T>::onToggleHighlight()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    Parameters params;

                    listener->handleEvent( IEvent::Type::UI, IEvent::TOGGLE_HIGHLIGHT_HASH, params, this,
                                           this, nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                auto parentElement = fb::static_pointer_cast<CUIElement<T>>( parent );
                parentElement->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onActivate( SmartPtr<IUIElement> element )
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                if( !listeners.empty() )
                {
                    for( auto &listener : listeners )
                    {
                        listener->handleEvent( IEvent::Type::UI, IEvent::ACTIVATE_HASH,
                                               Array<Parameter>(), this, element, nullptr );
                    }
                }
            }

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    if( child )
                    {
                        auto pChild = fb::static_pointer_cast<CUIElement<T>>( child );
                        pChild->onActivate( element );
                    }
                }
            }

            if( auto parent = getParent() )
            {
                auto pParent = fb::static_pointer_cast<CUIElement<T>>( parent );
                pParent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onDeactivate()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::DEACTIVATE_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                    childElement->onDeactivate();
                }
            }

            if( auto parent = getParent() )
            {
                auto pParent = fb::static_pointer_cast<CUIElement<T>>( parent );
                pParent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onSelect()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::SELECT_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                    childElement->onSelect();
                }
            }

            if( auto parent = getParent() )
            {
                auto pParent = fb::static_pointer_cast<CUIElement<T>>( parent );
                pParent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onDeselect()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::DESELECT_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto p = getChildren() )
            {
                auto &children = *p;
                for( auto &child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement<T>>( child );
                    childElement->onDeselect();
                }
            }

            if( auto parent = getParent() )
            {
                auto pParent = fb::static_pointer_cast<CUIElement<T>>( parent );
                pParent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onGainFocus()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::GAIN_FOCUS_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                auto pParent = fb::static_pointer_cast<CUIElement<T>>( parent );
                pParent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onLostFocus()
        {
            if( auto p = CUIElement<T>::getObjectListenersPtr() )
            {
                auto &listeners = *p;
                for( auto &listener : listeners )
                {
                    listener->handleEvent( IEvent::Type::UI, IEvent::LOST_FOCUS_HASH, Array<Parameter>(),
                                           this, this, nullptr );
                }
            }

            if( auto parent = getParent() )
            {
                auto pParent = fb::static_pointer_cast<CUIElement<T>>( parent );
                pParent->onChildChangedState( this );
            }
        }

        template <class T>
        void CUIElement<T>::onEvent( const String &eventType )
        {
            // for (u32 i = 0; i < m_children.size(); ++i)
            //	((CUIElement*)m_children[i])->onEvent(eventType);

            // for (u32 i = 0; i < m_listeners.size(); ++i)
            //	m_listeners[i]->onEvent(eventType);
        }

        template <class T>
        void CUIElement<T>::setPosition( const Vector2F &position )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    state->setPosition( position );
                }
            }
        }

        template <class T>
        SmartPtr<Properties> CUIElement<T>::getProperties() const
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto factoryManager = applicationManager->getFactoryManager();

            auto properties = factoryManager->make_ptr<Properties>();
            properties->setProperty( "size", getSize() );
            properties->setProperty( "position", getPosition() );
            properties->setProperty( "enabled", m_isEnabled );
            properties->setProperty( "visible", isVisible() );
            return properties;
        }

        template <class T>
        void CUIElement<T>::setProperties( SmartPtr<Properties> properties )
        {
            auto enabled = isEnabled();
            auto visible = isVisible();
            //properties->getPropertyValue( "size", m_size );
            //properties->getPropertyValue( "position", m_position );
            properties->getPropertyValue( "enabled", enabled );
            properties->getPropertyValue( "visible", visible );

            setEnabled( enabled );
            setVisible( visible );
        }

        template <class T>
        Array<SmartPtr<ISharedObject>> CUIElement<T>::getChildObjects() const
        {
            auto objects = Array<SmartPtr<ISharedObject>>();
            objects.reserve( 5 );

            objects.push_back( m_dragSource );
            objects.push_back( m_dropTarget );

            if( auto element = getOverlayElement() )
            {
                objects.push_back( element );
            }

            return objects;
        }

        template <class T>
        bool CUIElement<T>::isDragDropSource() const
        {
            return m_dragDropSource;
        }

        template <class T>
        void CUIElement<T>::setDragDropSource( bool dragDropSource )
        {
            m_dragDropSource = dragDropSource;
        }

        template <class T>
        SmartPtr<IUIDragSource> CUIElement<T>::getDragSource() const
        {
            return m_dragSource;
        }

        template <class T>
        void CUIElement<T>::setDragSource( SmartPtr<IUIDragSource> dragSource )
        {
            m_dragSource = dragSource;
        }

        template <class T>
        SmartPtr<IUIDropTarget> CUIElement<T>::getDropTarget() const
        {
            return m_dropTarget;
        }

        template <class T>
        void CUIElement<T>::setDropTarget( SmartPtr<IUIDropTarget> dropTarget )
        {
            m_dropTarget = dropTarget;
        }

        template <class T>
        u32 CUIElement<T>::getOrder() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getZOrder();
                }
            }

            if( auto element = getOverlayElement() )
            {
                return element->getZOrder();
            }

            return 0;
        }

        template <class T>
        void CUIElement<T>::setOrder( u32 order )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    state->setZOrder( order );
                }
            }

            if( auto element = getOverlayElement() )
            {
                element->setZOrder( order );
            }
        }

        template <class T>
        void CUIElement<T>::initialise( SmartPtr<IUIElement> &parent )
        {
        }

        template <class T>
        void CUIElement<T>::initialise( SmartPtr<IUIElement> &parent, const Properties &properties )
        {
        }

        template <class T>
        void CUIElement<T>::setName( const String &name )
        {
            m_name = name;
            m_hashId = StringUtil::getHash( m_name );
        }

        template <class T>
        String CUIElement<T>::getName() const
        {
            return m_name;
        }

        template <class T>
        s32 CUIElement<T>::getElementId() const
        {
            return m_itemId;
        }

        template <class T>
        void CUIElement<T>::setElementId( s32 val )
        {
            m_itemId = val;
        }

        template <class T>
        void CUIElement<T>::setComponentID( const String &componentId )
        {
            m_componentId = componentId;
        }

        template <class T>
        const String &CUIElement<T>::getComponentID() const
        {
            return m_componentId;
        }

        template <class T>
        void CUIElement<T>::setType( const String &type )
        {
            m_type = type;
        }

        template <class T>
        const String &CUIElement<T>::getType() const
        {
            return m_type;
        }

        template <class T>
        Vector2F CUIElement<T>::getPosition() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getPosition();
                }
            }

            return Vector2F::zero();
        }

        template <class T>
        void CUIElement<T>::setSize( const Vector2F &size )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    state->setSize( size );
                }
            }
        }

        template <class T>
        Vector2F CUIElement<T>::getSize() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getSize();
                }
            }

            return Vector2F::zero();
        }

        template <class T>
        bool CUIElement<T>::isEnabled() const
        {
            return m_isEnabled;
        }

        template <class T>
        bool CUIElement<T>::isVisible() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->isVisible();
                }
            }

            return false;
        }

        template <class T>
        bool CUIElement<T>::isInFocus() const
        {
            return m_hasFocus;
        }

        template <class T>
        bool CUIElement<T>::isHighlighted() const
        {
            return m_isHighlighted;
        }

        template <class T>
        SmartPtr<IUIElement> CUIElement<T>::getParent() const
        {
            return m_parent;
        }

        template <class T>
        void CUIElement<T>::setParent( SmartPtr<IUIElement> parent )
        {
            m_parent = parent;
        }

        template <class T>
        u32 CUIElement<T>::getHashId() const
        {
            return (u32)m_hashId;
        }

        template <class T>
        SmartPtr<IUIElement> CUIElement<T>::getLayout() const
        {
            return m_layout;
        }

        template <class T>
        void CUIElement<T>::setLayout( SmartPtr<IUIElement> layout )
        {
            m_layout = layout;
        }

        template <class T>
        void CUIElement<T>::handleEvent( const SmartPtr<IEvent> &event )
        {
        }

        template <class T>
        SmartPtr<render::IOverlayElement> CUIElement<T>::getOverlayElement() const
        {
            return m_overlayElement;
        }

        template <class T>
        void CUIElement<T>::setOverlayElement( SmartPtr<render::IOverlayElement> overlayElement )
        {
            m_overlayElement = overlayElement;
        }

        template <class T>
        void CUIElement<T>::createStateContext()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto stateObject = stateManager->addStateObject();

            auto listener = factoryManager->make_ptr<ElementStateListener>();
            listener->setOwner( this );
            stateObject->addStateListener( listener );

            auto state = factoryManager->make_ptr<UIElementState>();
            stateObject->setState( state );

            stateObject->setOwner( this );

            setStateObject( stateObject );
            setStateListener( listener );

            auto stateTask = graphicsSystem->getStateTask();
            state->setTaskId( stateTask );
        }

        template <class T>
        ColourF CUIElement<T>::getColour() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getColour();
                }
            }

            return ColourF::White;
        }

        template <class T>
        void CUIElement<T>::setColour( const ColourF &colour )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->setColour( colour );
                }
            }
        }

        template <class T>
        SmartPtr<IStateContext> CUIElement<T>::getStateObject() const
        {
            return m_stateObject;
        }

        template <class T>
        void CUIElement<T>::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        template <class T>
        SmartPtr<IStateListener> CUIElement<T>::getStateListener() const
        {
            return m_stateListener;
        }

        template <class T>
        void CUIElement<T>::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        template <class T>
        void CUIElement<T>::sortZOrder()
        {
            if( auto p = getChildren() )
            {
                auto &children = *p;

                std::sort( children.begin(), children.end(),
                           []( auto a, auto b ) { return a->getOrder() < b->getOrder(); } );

                for( auto child : children )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement>( child );
                    childElement->sortZOrder();
                }
            }
        }

        template <class T>
        void CUIElement<T>::updateZOrder()
        {
            sortZOrder();

            if( auto p = getChildren() )
            {
                for( auto &child : *p )
                {
                    auto childElement = fb::static_pointer_cast<CUIElement>( child );
                    childElement->updateZOrder();
                }
            }
        }

        template <class T>
        void CUIElement<T>::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        template <class T>
        bool CUIElement<T>::getHandleInputEvents() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->getHandleInputEvents();
                }
            }

            return false;
        }

        template <class T>
        void CUIElement<T>::setHandleInputEvents( bool handleInputEvents )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->template getStateByType<UIElementState>() )
                {
                    return state->setHandleInputEvents( handleInputEvents );
                }
            }
        }

        template <class T>
        void CUIElement<T>::ElementStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( auto owner = getOwner() )
            {
                if( message->isExactly<StateMessageObject>() )
                {
                    auto objectMessage = fb::static_pointer_cast<StateMessageObject>( message );
                    auto messageType = objectMessage->getType();
                    auto object = objectMessage->getObject();

                    if( messageType == IUIElement::STATE_MESSAGE_ADD_CHILD )
                    {
                        owner->addChild( object );
                    }
                    else if( messageType == IUIElement::STATE_MESSAGE_REMOVE_CHILD )
                    {
                        owner->removeChild( object );
                    }
                }
            }
        }

        template <class T>
        void CUIElement<T>::ElementStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto renderUI = applicationManager->getRenderUI();
            FB_ASSERT( renderUI );

            IUIManager::ScopedLock lock( renderUI );

            if( auto owner = getOwner() )
            {
                owner->handleStateChanged( state );
            }

            auto elementState = fb::static_pointer_cast<UIElementState>( state );

            auto position = elementState->getPosition();
            auto size = elementState->getSize();
            auto zOrder = elementState->getZOrder();
            auto inheritsPick = elementState->getHandleInputEvents();

            auto iPosition = Vector2I::zero();
            auto iSize = Vector2I::zero();

            auto visible = elementState->isVisible();

            if( auto uiWindow = applicationManager->getSceneRenderWindow() )
            {
                auto sceneWindowPosition = uiWindow->getPosition();
                auto sceneWindowSize = uiWindow->getSize();

                sceneWindowSize = Vector2F( 1920, 1080 );

                auto pos = position * sceneWindowSize;
                auto sz = size * sceneWindowSize;

                iPosition = Vector2I( (s32)pos.X(), (s32)pos.Y() );
                iSize = Vector2I( (s32)sz.X(), (s32)sz.Y() );
            }
            else
            {
                if( auto mainWindow = applicationManager->getWindow() )
                {
                    auto mainWindowSize = mainWindow->getSize();
                    auto mainWindowSizeF = Vector2F( (f32)mainWindowSize.x, (f32)mainWindowSize.y );

                    auto pos = position * mainWindowSizeF;
                    auto sz = size * mainWindowSizeF;

                    iPosition = Vector2I( (s32)pos.X(), (s32)pos.Y() );
                    iSize = Vector2I( (s32)sz.X(), (s32)sz.Y() );
                }
            }

            if( auto owner = getOwner() )
            {
                if( auto element = owner->getOverlayElement() )
                {
                    element->setZOrder( zOrder );
                    element->setPosition( position );
                    element->setSize( size );
                    element->setVisible( visible );
                    //element->setInheritsPick(inheritsPick);

                    state->setDirty( false );
                }

                //if( auto widget = owner->getWidget() )
                //{
                //    widget->setDepth( zOrder );
                //    widget->setPosition( MyGUI::IntPoint( iPosition.X(), iPosition.Y() ) );
                //    widget->setSize( MyGUI::IntSize( iSize.X(), iSize.Y() ) );
                //    widget->setVisible( visible );
                //    //widget->setInheritsPick( inheritsPick );

                //    state->setDirty( false );
                //}
            }
        }

        template <class T>
        void CUIElement<T>::ElementStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        template <class T>
        SmartPtr<CUIElement<T>> CUIElement<T>::ElementStateListener::getOwner() const
        {
            return m_owner;
        }

        template <class T>
        void CUIElement<T>::ElementStateListener::setOwner( SmartPtr<CUIElement<T>> owner )
        {
            m_owner = owner;
        }

        template class CUIElement<IUIElement>;
        template class CUIElement<IUIAnimatedMaterial>;
        template class CUIElement<IUIAnimatorPosition>;
        template class CUIElement<IUIAnimatorScale>;
        template class CUIElement<IUIBar>;
        template class CUIElement<IUIButton>;
        template class CUIElement<IUIImage>;
        template class CUIElement<IUIImageArray>;
        template class CUIElement<IUIMenu>;
        template class CUIElement<IUILayout>;
        template class CUIElement<IUISpinner>;
        template class CUIElement<IUIScrollingText>;
        template class CUIElement<IUIText>;
        template class CUIElement<IUITextEntry>;
        template class CUIElement<IUIToggleButton>;
        template class CUIElement<IUIToggleGroup>;
        template class CUIElement<IUICharacterSelect>;
        template class CUIElement<IUIVector3>;
        template class CUIElement<IUIContainer>;
    }  // end namespace ui
}  // end namespace fb
