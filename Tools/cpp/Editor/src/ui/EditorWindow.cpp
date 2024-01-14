#include <GameEditorPCH.h>
#include "ui/EditorWindow.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{
    FB_CLASS_REGISTER_DERIVED( fb, EditorWindow, scene::IEditor );

    EditorWindow::EditorWindow() = default;

    EditorWindow::~EditorWindow() = default;

    void EditorWindow::load( SmartPtr<ISharedObject> data )
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );
        FB_ASSERT( factoryManager->isValid() );

        auto dragSource = fb::make_ptr<UIDragSource>();
        dragSource->setOwner( this );
        //setWindowDropTarget( dragSource );

        auto dropTarget = fb::make_ptr<UIDropTarget>();
        dropTarget->setOwner( this );
        setWindowDropTarget( dropTarget );

        auto invoker = fb::make_ptr<ScriptInvoker>( this );
        setInvoker( invoker );

        //auto receiver = fb::make_ptr<ScriptReceiver>( this );
        //setReceiver( receiver );

        //auto updateEvent = fb::make_ptr<ScriptEvent>();
        //updateEvent->setFunction( "update" );
        //invoker->setEventFunction( UPDATE_HASH, updateEvent );

        auto scriptManager = applicationManager->getScriptManager();
        FB_ASSERT( scriptManager );
        FB_ASSERT( scriptManager->isValid() );

        if( !StringUtil::isNullOrEmpty( m_className ) )
        {
            scriptManager->createObject( m_className, this );
        }

        auto listener = fb::make_ptr<UIListener>();
        listener->setOwner( this );
        setEventListener( listener );
    }

    void EditorWindow::reload( SmartPtr<ISharedObject> data )
    {
        if( auto invoker = getInvoker() )
        {
            invoker->callObjectMember( "unload" );
            invoker->callObjectMember( "load" );
        }
    }

    void EditorWindow::unload( SmartPtr<ISharedObject> data )
    {
        if( auto invoker = getInvoker() )
        {
            invoker->callObjectMember( "unload" );
        }
    }

    auto EditorWindow::getParent() const -> SmartPtr<ui::IUIWindow>
    {
        return m_parent;
    }

    void EditorWindow::setParent( SmartPtr<ui::IUIWindow> parent )
    {
        m_parent = parent;
    }

    auto EditorWindow::getParentWindow() const -> SmartPtr<ui::IUIWindow>
    {
        return m_parentWindow;
    }

    void EditorWindow::setParentWindow( SmartPtr<ui::IUIWindow> parentWindow )
    {
        m_parentWindow = parentWindow;
    }

    auto EditorWindow::getDebugWindow() const -> SmartPtr<ui::IUIWindow>
    {
        return m_debugWindow;
    }

    void EditorWindow::setDebugWindow( SmartPtr<ui::IUIWindow> debugWindow )
    {
        m_debugWindow = debugWindow;
    }

    auto EditorWindow::isWindowVisible() const -> bool
    {
        return m_windowVisible;
    }

    void EditorWindow::setWindowVisible( bool visible )
    {
        try
        {
            m_windowVisible = visible;

            if( m_parentWindow )
            {
                m_parentWindow->setVisible( visible, false );
            }

            //if( m_debugWindow )
            //{
            //    m_debugWindow->setVisible( visible, false );
            //}

            if( visible )
            {
                if( auto invoker = getInvoker() )
                {
                    invoker->callObjectMember( "show" );
                }
            }
            else
            {
                if( auto invoker = getInvoker() )
                {
                    invoker->callObjectMember( "hide" );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void EditorWindow::updateSelection()
    {
        if( auto invoker = getInvoker() )
        {
            invoker->callObjectMember( "updateSelection" );
        }
    }

    auto EditorWindow::getEventListener() const -> SmartPtr<IEventListener>
    {
        return m_eventListener;
    }

    void EditorWindow::setEventListener( SmartPtr<IEventListener> eventListener )
    {
        m_eventListener = eventListener;
    }

    auto EditorWindow::getWindowDragSource() const -> SmartPtr<ui::IUIDragSource>
    {
        return m_windowDragSource;
    }

    void EditorWindow::setWindowDragSource( SmartPtr<ui::IUIDragSource> windowDragSource )
    {
        m_windowDragSource = windowDragSource;
    }

    auto EditorWindow::getWindowDropTarget() const -> SmartPtr<ui::IUIDropTarget>
    {
        return m_windowDropTarget;
    }

    void EditorWindow::setWindowDropTarget( SmartPtr<ui::IUIDropTarget> windowDropTarget )
    {
        m_windowDropTarget = windowDropTarget;
    }

    auto EditorWindow::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                    const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                    SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) -> Parameter
    {
        if( auto invoker = getInvoker() )
        {
            Array<Parameter> params;
            params.reserve( 6 );

            params.emplace_back( static_cast<u32>( eventType ) );
            params.emplace_back( static_cast<u32>( eventValue ) );
            params.emplace_back( arguments );
            params.emplace_back( sender );
            params.emplace_back( object );
            params.emplace_back( fb::static_pointer_cast<ISharedObject>( event ) );

            Array<Parameter> results;

            invoker->callObjectMember( "handleEvent", params, results );
        }

        return {};
    }

    auto EditorWindow::handleDrag( const Vector2I &position, SmartPtr<ui::IUIElement> element ) -> String
    {
        return "";
    }

    void EditorWindow::handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> element,
                                   const String &data )
    {
    }

    auto EditorWindow::getClassName() const -> String
    {
        return m_className;
    }

    void EditorWindow::setClassName( const String &className )
    {
        m_className = className;
    }

    auto EditorWindow::getInvoker() const -> SmartPtr<IScriptInvoker>
    {
        return m_invoker;
    }

    void EditorWindow::setInvoker( SmartPtr<IScriptInvoker> invoker )
    {
        m_invoker = invoker;
    }

    auto EditorWindow::getReceiver() const -> SmartPtr<IScriptReceiver>
    {
        return m_receiver;
    }

    void EditorWindow::setReceiver( SmartPtr<IScriptReceiver> receiver )
    {
        m_receiver = receiver;
    }

    auto EditorWindow::getScriptClass() const -> SmartPtr<IScriptClass>
    {
        return m_scriptClass;
    }

    void EditorWindow::setScriptClass( SmartPtr<IScriptClass> scriptClass )
    {
        m_scriptClass = scriptClass;
    }

    void EditorWindow::addData( SmartPtr<ISharedObject> data )
    {
        if( isLoaded() )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_dataArray.push_back( data );
        }
    }

    void EditorWindow::removeData( SmartPtr<ISharedObject> data )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        auto it = std::find( m_dataArray.begin(), m_dataArray.end(), data );
        if( it != m_dataArray.end() )
        {
            m_dataArray.erase( it );
        }
    }

    void EditorWindow::clearData()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_dataArray.clear();
    }

    auto EditorWindow::getData() const -> Array<SmartPtr<ISharedObject>>
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_dataArray;
    }

    void EditorWindow::setDraggable( SmartPtr<ui::IUIElement> element, bool draggable )
    {
    }

    auto EditorWindow::isDraggable( SmartPtr<ui::IUIElement> element ) const -> bool
    {
        return false;
    }

    void EditorWindow::setDroppable( SmartPtr<ui::IUIElement> element, bool droppable )
    {
        auto dropTarget = getWindowDropTarget();

        if( droppable )
        {
            element->setDropTarget( dropTarget );
        }
        else
        {
            element->setDropTarget( nullptr );
        }
    }

    auto EditorWindow::isDroppable( SmartPtr<ui::IUIElement> element ) const -> bool
    {
        return element->getDropTarget() == getWindowDropTarget();
    }

    void EditorWindow::setHandleEvents( SmartPtr<ui::IUIElement> element, bool handleEvents )
    {
        auto listener = getEventListener();

        if( handleEvents )
        {
            element->addObjectListener( listener );
        }
        else
        {
            element->removeObjectListener( listener );
        }
    }

    auto EditorWindow::getHandleEvents( SmartPtr<ui::IUIElement> element ) const -> bool
    {
        return false;
    }

    auto EditorWindow::UIListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                const Array<Parameter> &arguments,
                                                SmartPtr<ISharedObject> sender,
                                                SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        if( auto owner = getOwner() )
        {
            owner->handleEvent( eventType, eventValue, arguments, sender, object, event );
        }

        return {};
    }

    auto EditorWindow::UIListener::getOwner() const -> EditorWindow *
    {
        return m_owner;
    }

    void EditorWindow::UIListener::setOwner( EditorWindow *owner )
    {
        m_owner = owner;
    }

    auto EditorWindow::UIDragSource::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                  const Array<Parameter> &arguments,
                                                  SmartPtr<ISharedObject> sender,
                                                  SmartPtr<ISharedObject> object,
                                                  SmartPtr<IEvent> event ) -> Parameter
    {
        if( eventValue == IEvent::handleDrag )
        {
            if( auto owner = getOwner() )
            {
                owner->handleDrag( Vector2I::zero(), sender );
            }
        }

        return {};
    }

    auto EditorWindow::UIDragSource::handleDrag( const Vector2I &position,
                                                 SmartPtr<ui::IUIElement> element ) -> String
    {
        if( auto owner = getOwner() )
        {
            return owner->handleDrag( position, element );
        }

        return "";
    }

    auto EditorWindow::UIDragSource::getOwner() const -> EditorWindow *
    {
        return m_owner;
    }

    void EditorWindow::UIDragSource::setOwner( EditorWindow *owner )
    {
        m_owner = owner;
    }

    auto EditorWindow::UIDropTarget::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                  const Array<Parameter> &arguments,
                                                  SmartPtr<ISharedObject> sender,
                                                  SmartPtr<ISharedObject> object,
                                                  SmartPtr<IEvent> event ) -> Parameter
    {
        if( auto owner = getOwner() )
        {
            owner->handleEvent( eventType, eventValue, arguments, sender, object, event );
        }

        return {};
    }

    auto EditorWindow::UIDropTarget::handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> src,
                                                 SmartPtr<ui::IUIElement> dst, const String &data )
        -> bool
    {
        if( auto owner = getOwner() )
        {
            owner->handleDrop( position, src, data );
        }

        return false;
    }

    auto EditorWindow::UIDropTarget::getOwner() const -> EditorWindow *
    {
        return m_owner;
    }

    void EditorWindow::UIDropTarget::setOwner( EditorWindow *owner )
    {
        m_owner = owner;
    }
}  // namespace fb::editor
