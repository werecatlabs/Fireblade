#include <GameEditorPCH.h>
#include "ui/BaseWindow.h"
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>

namespace fb
{
    namespace editor
    {
        FB_CLASS_REGISTER_DERIVED( fb, BaseWindow, SharedObject<scene::IEditor> );

        BaseWindow::BaseWindow()
        {
        }

        BaseWindow::~BaseWindow()
        {
        }

        void BaseWindow::load( SmartPtr<ISharedObject> data )
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

        SmartPtr<ui::IUIWindow> BaseWindow::getParent() const
        {
            return m_parent;
        }

        void BaseWindow::setParent( SmartPtr<ui::IUIWindow> parent )
        {
            m_parent = parent;
        }

        SmartPtr<ui::IUIWindow> BaseWindow::getParentWindow() const
        {
            return m_parentWindow;
        }

        void BaseWindow::setParentWindow( SmartPtr<ui::IUIWindow> parentWindow )
        {
            m_parentWindow = parentWindow;
        }

        SmartPtr<ui::IUIWindow> BaseWindow::getDebugWindow() const
        {
            return m_debugWindow;
        }

        void BaseWindow::setDebugWindow( SmartPtr<ui::IUIWindow> debugWindow )
        {
            m_debugWindow = debugWindow;
        }

        bool BaseWindow::isWindowVisible() const
        {
            return m_windowVisible;
        }

        void BaseWindow::setWindowVisible( bool visible )
        {
            m_windowVisible = visible;

            if( m_parentWindow )
            {
                m_parentWindow->setVisible( visible );
            }

            if( m_debugWindow )
            {
                m_debugWindow->setVisible( visible );
            }

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

        void BaseWindow::updateSelection()
        {
            if( auto invoker = getInvoker() )
            {
                invoker->callObjectMember( "updateSelection" );
            }
        }

        SmartPtr<IEventListener> BaseWindow::getEventListener() const
        {
            return m_eventListener;
        }

        void BaseWindow::setEventListener( SmartPtr<IEventListener> eventListener )
        {
            m_eventListener = eventListener;
        }

        SmartPtr<ui::IUIDragSource> BaseWindow::getWindowDragSource() const
        {
            return m_windowDragSource;
        }

        void BaseWindow::setWindowDragSource( SmartPtr<ui::IUIDragSource> windowDragSource )
        {
            m_windowDragSource = windowDragSource;
        }

        SmartPtr<ui::IUIDropTarget> BaseWindow::getWindowDropTarget() const
        {
            return m_windowDropTarget;
        }

        void BaseWindow::setWindowDropTarget( SmartPtr<ui::IUIDropTarget> windowDropTarget )
        {
            m_windowDropTarget = windowDropTarget;
        }

        Parameter BaseWindow::handleEvent( IEvent::Type eventType, hash_type eventValue,
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

        String BaseWindow::handleDrag( const Vector2I &position, SmartPtr<ui::IUIElement> element )
        {
            return "";
        }

        void BaseWindow::handleDrop( const Vector2I &position, SmartPtr<ui::IUIElement> element,
                                     const String &data )
        {
        }

        String BaseWindow::getClassName() const
        {
            return m_className;
        }

        void BaseWindow::setClassName( const String &className )
        {
            m_className = className;
        }

        SmartPtr<IScriptInvoker> BaseWindow::getInvoker() const
        {
            return m_invoker;
        }

        void BaseWindow::setInvoker( SmartPtr<IScriptInvoker> invoker )
        {
            m_invoker = invoker;
        }

        SmartPtr<IScriptReceiver> BaseWindow::getReceiver() const
        {
            return m_receiver;
        }

        void BaseWindow::setReceiver( SmartPtr<IScriptReceiver> receiver )
        {
            m_receiver = receiver;
        }

        SmartPtr<IScriptClass> BaseWindow::getScriptClass() const
        {
            return m_scriptClass;
        }

        void BaseWindow::setScriptClass( SmartPtr<IScriptClass> scriptClass )
        {
            m_scriptClass = scriptClass;
        }

        void BaseWindow::addData( SmartPtr<ISharedObject> data )
        {
            if( isLoaded() )
            {
                RecursiveMutex::ScopedLock lock( m_mutex );
                m_dataArray.push_back( data );
            }
        }

        void BaseWindow::removeData( SmartPtr<ISharedObject> data )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            auto it = std::find( m_dataArray.begin(), m_dataArray.end(), data );
            if( it != m_dataArray.end() )
            {
                m_dataArray.erase( it );
            }
        }

        void BaseWindow::clearData()
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_dataArray.clear();
        }

        Array<SmartPtr<ISharedObject>> BaseWindow::getData() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_dataArray;
        }

        void BaseWindow::setDraggable( SmartPtr<ui::IUIElement> element, bool draggable )
        {
        }

        bool BaseWindow::isDraggable( SmartPtr<ui::IUIElement> element ) const
        {
            return false;
        }

        void BaseWindow::setDroppable( SmartPtr<ui::IUIElement> element, bool droppable )
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

        bool BaseWindow::isDroppable( SmartPtr<ui::IUIElement> element ) const
        {
            return element->getDropTarget() == getWindowDropTarget();
        }

        void BaseWindow::setHandleEvents( SmartPtr<ui::IUIElement> element, bool handleEvents )
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

        bool BaseWindow::getHandleEvents( SmartPtr<ui::IUIElement> element ) const
        {
            return false;
        }

        Parameter BaseWindow::UIListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
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

        BaseWindow *BaseWindow::UIListener::getOwner() const
        {
            return m_owner;
        }

        void BaseWindow::UIListener::setOwner( BaseWindow *owner )
        {
            m_owner = owner;
        }

        Parameter BaseWindow::UIDragSource::handleEvent( IEvent::Type eventType, hash_type eventValue,
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

        String BaseWindow::UIDragSource::handleDrag( const Vector2I &position,
                                                     SmartPtr<ui::IUIElement> element )
        {
            if( auto owner = getOwner() )
            {
                return owner->handleDrag( position, element );
            }

            return "";
        }

        BaseWindow *BaseWindow::UIDragSource::getOwner() const
        {
            return m_owner;
        }

        void BaseWindow::UIDragSource::setOwner( BaseWindow *owner )
        {
            m_owner = owner;
        }

        Parameter BaseWindow::UIDropTarget::handleEvent( IEvent::Type eventType, hash_type eventValue,
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

        bool BaseWindow::UIDropTarget::handleDrop( const Vector2I &position,
                                                   SmartPtr<ui::IUIElement> src,
                                                   SmartPtr<ui::IUIElement> dst, const String &data )
        {
            if( auto owner = getOwner() )
            {
                owner->handleDrop( position, src, data );
            }

            return false;
        }

        BaseWindow *BaseWindow::UIDropTarget::getOwner() const
        {
            return m_owner;
        }

        void BaseWindow::UIDropTarget::setOwner( BaseWindow *owner )
        {
            m_owner = owner;
        }
    }  // end namespace editor
}  // end namespace fb
