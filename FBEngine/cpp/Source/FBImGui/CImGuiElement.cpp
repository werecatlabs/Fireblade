#include <FBImGui/FBImGuiPCH.h>
#include <FBImGui/CImGuiElement.h>
#include <FBCore/FBCore.h>

#include "ImGuiProfilerWindow.h"
#include "ImGuiProfileWindow.h"

namespace fb
{
    namespace ui
    {
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
                auto applicationManager = core::IApplicationManager::instance();
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

                if( auto stateObject = getStateObject() )
                {
                    if( auto stateListener = getStateListener() )
                    {
                        stateObject->removeStateListener( stateListener );
                        setStateListener( nullptr );
                    }

                    if( auto stateManager = applicationManager->getStateManager() )
                    {
                        stateManager->removeStateObject( stateObject );
                    }

                    setStateObject( nullptr );
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
            auto applicationManager = core::IApplicationManager::instance();
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
            auto applicationManager = core::IApplicationManager::instance();
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

        template class CImGuiElement<IUIElement>;
        template class CImGuiElement<IUIAbout>;
        template class CImGuiElement<IUIAnimatedMaterial>;
        template class CImGuiElement<IUIAnimatorPosition>;
        template class CImGuiElement<IUIAnimatorScale>;
        template class CImGuiElement<IUIBar>;
        template class CImGuiElement<IUIButton>;
        template class CImGuiElement<IUIDropdown>;
        template class CImGuiElement<IUIEventWindow>;
        template class CImGuiElement<IUIFileBrowser>;
        template class CImGuiElement<IUIImage>;
        template class CImGuiElement<IUIImageArray>;
        template class CImGuiElement<IUIInputManager>;
        template class CImGuiElement<IUIMenu>;
        template class CImGuiElement<IUIMenubar>;
        template class CImGuiElement<IUIMenuItem>;
        template class CImGuiElement<IUILayout>;
        template class CImGuiElement<IUISpinner>;
        template class CImGuiElement<IUIScrollingText>;
        template class CImGuiElement<IUIText>;
        template class CImGuiElement<IUITextEntry>;
        template class CImGuiElement<IUIToggleButton>;
        template class CImGuiElement<IUIToggleGroup>;
        template class CImGuiElement<IUICharacterSelect>;
        template class CImGuiElement<IUIContainer>;
        template class CImGuiElement<IUIRenderWindow>;
        template class CImGuiElement<IUITreeCtrl>;
        template class CImGuiElement<IUITreeNode>;
        template class CImGuiElement<IUIWindow>;

        template class CImGuiElement<IUILabelCheckboxPair>;
        template class CImGuiElement<IUILabelDropdownPair>;
        template class CImGuiElement<IUILabelSliderPair>;
        template class CImGuiElement<IUILabelTextInputPair>;

        template class CImGuiElement<IUIPropertyGrid>;

        template class CImGuiElement<IUIVector2>;
        template class CImGuiElement<IUIVector3>;
        template class CImGuiElement<IUIVector4>;

        template class CImGuiElement<IUIToolbar>;
        template class CImGuiElement<IUITerrainEditor>;

        template class CImGuiElement<ImGuiProfileWindow>;
        template class CImGuiElement<ImGuiProfilerWindow>;

    }  // end namespace ui
}  // end namespace fb
