#include <GameEditorPCH.h>
#include "ui/EditorWindow.h"
#include <FBCore/FBCore.h>


namespace fb
{
    namespace editor
    {
        FB_CLASS_REGISTER_DERIVED( fb, EditorWindow, scene::IEditor );

        EditorWindow::EditorWindow()
        {
        }

        EditorWindow::~EditorWindow()
        {
        }

        void EditorWindow::load( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
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

        SmartPtr<ui::IUIWindow> EditorWindow::getParent() const
        {
            return m_parent;
        }

        void EditorWindow::setParent( SmartPtr<ui::IUIWindow> parent )
        {
            m_parent = parent;
        }

        SmartPtr<ui::IUIWindow> EditorWindow::getParentWindow() const
        {
            return m_parentWindow;
        }

        void EditorWindow::setParentWindow( SmartPtr<ui::IUIWindow> parentWindow )
        {
            m_parentWindow = parentWindow;
        }

        SmartPtr<ui::IUIWindow> EditorWindow::getDebugWindow() const
        {
            return m_debugWindow;
        }

        void EditorWindow::setDebugWindow( SmartPtr<ui::IUIWindow> debugWindow )
        {
            m_debugWindow = debugWindow;
        }

        bool EditorWindow::isWindowVisible() const
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

        SmartPtr<IEventListener> EditorWindow::getEventListener() const
        {
            return m_eventListener;
        }

        void EditorWindow::setEventListener( SmartPtr<IEventListener> eventListener )
        {
            m_eventListener = eventListener;
        }

        SmartPtr<ui::IUIDragSource> EditorWindow::getWindowDragSource() const
        {
            return m_windowDragSource;
        }

        void EditorWindow::setWindowDragSource( SmartPtr<ui::IUIDragSource> windowDragSource )
        {
            m_windowDragSource = windowDragSource;
        }

        SmartPtr<ui::IUIDropTarget> EditorWindow::getWindowDropTarget() const
        {
            return m_windowDropTarget;
        }

        void EditorWindow::setWindowDropTarget( SmartPtr<ui::IUIDropTarget> windowDropTarget )
        {
            m_windowDropTarget = windowDropTarget;
        }

        Parameter EditorWindow::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                             const Array<Parameter> &arguments,
                                             SmartPtr<ISharedObject> sender,
                                             SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            if( auto invoker = getInvoker() )
            {
                Array<Parameter> params;
                params.reserve( 6 );

                params.push_back( Parameter( (u32)eventType ) );
                params.push_back( Parameter( (u32)eventValue ) );
                params.push_back( Parameter( arguments ) );
                params.push_back( Parameter( sender ) );
                params.push_back( Parameter( object ) );
                params.push_back( Parameter( fb::static_pointer_cast<ISharedObject>( event ) ) );

                Array<Parameter> results;

                invoker->callObjectMember( "handleEvent", params, results );
            }

            return Parameter();
        }

        String EditorWindow::handleDrag( const Vector2I &position, SmartPtr<ui::IUIElement> element )
        {
            return "";
        }

        void EditorWindow::handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> element,
                                       const String &data )
        {
        }

        String EditorWindow::getClassName() const
        {
            return m_className;
        }

        void EditorWindow::setClassName( const String &className )
        {
            m_className = className;
        }

        SmartPtr<IScriptInvoker> EditorWindow::getInvoker() const
        {
            return m_invoker;
        }

        void EditorWindow::setInvoker( SmartPtr<IScriptInvoker> invoker )
        {
            m_invoker = invoker;
        }

        SmartPtr<IScriptReceiver> EditorWindow::getReceiver() const
        {
            return m_receiver;
        }

        void EditorWindow::setReceiver( SmartPtr<IScriptReceiver> receiver )
        {
            m_receiver = receiver;
        }

        SmartPtr<IScriptClass> EditorWindow::getScriptClass() const
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

        Array<SmartPtr<ISharedObject>> EditorWindow::getData() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_dataArray;
        }

        void EditorWindow::setDraggable( SmartPtr<ui::IUIElement> element, bool draggable )
        {
        }

        bool EditorWindow::isDraggable( SmartPtr<ui::IUIElement> element ) const
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

        bool EditorWindow::isDroppable( SmartPtr<ui::IUIElement> element ) const
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

        bool EditorWindow::getHandleEvents( SmartPtr<ui::IUIElement> element ) const
        {
            return false;
        }

        Parameter EditorWindow::UIListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                         const Array<Parameter> &arguments,
                                                         SmartPtr<ISharedObject> sender,
                                                         SmartPtr<ISharedObject> object,
                                                         SmartPtr<IEvent> event )
        {
            if( auto owner = getOwner() )
            {
                owner->handleEvent( eventType, eventValue, arguments, sender, object, event );
            }

            return Parameter();
        }

        EditorWindow *EditorWindow::UIListener::getOwner() const
        {
            return m_owner;
        }

        void EditorWindow::UIListener::setOwner( EditorWindow *owner )
        {
            m_owner = owner;
        }

        Parameter EditorWindow::UIDragSource::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                           const Array<Parameter> &arguments,
                                                           SmartPtr<ISharedObject> sender,
                                                           SmartPtr<ISharedObject> object,
                                                           SmartPtr<IEvent> event )
        {
            if( eventValue == IEvent::handleDrag )
            {
                if( auto owner = getOwner() )
                {
                    owner->handleDrag( Vector2I::zero(), sender );
                }
            }

            return Parameter();
        }

        String EditorWindow::UIDragSource::handleDrag( const Vector2I &position,
                                                       SmartPtr<ui::IUIElement> element )
        {
            if( auto owner = getOwner() )
            {
                return owner->handleDrag( position, element );
            }

            return "";
        }

        EditorWindow *EditorWindow::UIDragSource::getOwner() const
        {
            return m_owner;
        }

        void EditorWindow::UIDragSource::setOwner( EditorWindow *owner )
        {
            m_owner = owner;
        }

        Parameter EditorWindow::UIDropTarget::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                           const Array<Parameter> &arguments,
                                                           SmartPtr<ISharedObject> sender,
                                                           SmartPtr<ISharedObject> object,
                                                           SmartPtr<IEvent> event )
        {
            if( auto owner = getOwner() )
            {
                owner->handleEvent( eventType, eventValue, arguments, sender, object, event );
            }

            return Parameter();
        }

        bool EditorWindow::UIDropTarget::handleDrop( const Vector2I &position,
                                                     SmartPtr<ui::IUIElement> src,
                                                     SmartPtr<ui::IUIElement> dst, const String &data )
        {
            if( auto owner = getOwner() )
            {
                owner->handleDrop( position, src, data );
            }

            return false;
        }

        EditorWindow *EditorWindow::UIDropTarget::getOwner() const
        {
            return m_owner;
        }

        void EditorWindow::UIDropTarget::setOwner( EditorWindow *owner )
        {
            m_owner = owner;
        }
    }  // end namespace editor
}  // end namespace fb
