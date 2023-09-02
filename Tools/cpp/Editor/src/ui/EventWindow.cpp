#include <GameEditorPCH.h>
#include "ui/EventWindow.h"
#include "ui/EventListenerWindow.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {
        EventWindow::EventWindow()
        {
        }

        EventWindow::~EventWindow()
        {
            unload( nullptr );
        }

        void EventWindow::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto parent = getParent();
                setParentWindow( parent );

                auto uiListener = fb::make_ptr<UIElementListener>();
                uiListener->setOwner( this );
                m_uiListener = uiListener;

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void EventWindow::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                m_event = nullptr;

                for( auto l : m_eventWindows )
                {
                    l->unload( nullptr );
                }

                m_eventWindows.clear();

                for( auto l : m_eventListenerWindows )
                {
                    l->unload( nullptr );
                }

                m_eventListenerWindows.clear();

                if( m_addComponentButton )
                {
                    ui->removeElement( m_addComponentButton );
                    m_addComponentButton = nullptr;
                }

                if( m_removeComponentButton )
                {
                    ui->removeElement( m_removeComponentButton );
                    m_removeComponentButton = nullptr;
                }

                m_uiListener = nullptr;

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void EventWindow::updateSelection()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto selectionManager = applicationManager->getSelectionManager();
                FB_ASSERT( selectionManager );

                if( m_addComponentButton )
                {
                    ui->removeElement( m_addComponentButton );
                    m_addComponentButton = nullptr;
                }

                if( m_removeComponentButton )
                {
                    ui->removeElement( m_removeComponentButton );
                    m_removeComponentButton = nullptr;
                }

                for( auto eventListenerWindow : m_eventListenerWindows )
                {
                    eventListenerWindow->unload( nullptr );
                }

                m_eventListenerWindows.clear();

                //auto parentWindow = getParentWindow();

                //auto addComponentButton = ui->addElementByType<ui::IUIButton>();
                //FB_ASSERT( addComponentButton );

                //addComponentButton->setElementId( static_cast<s32>( WidgetId::AddComponent ) );
                //addComponentButton->setLabel( "Add" );
                //parentWindow->addChild( addComponentButton );
                //m_addComponentButton = addComponentButton;
                //m_addComponentButton->addObjectListener( m_uiListener );

                //auto removeComponentButton = ui->addElementByType<ui::IUIButton>();
                //FB_ASSERT( removeComponentButton );

                //removeComponentButton->setElementId( static_cast<s32>( WidgetId::RemoveComponent ) );
                //removeComponentButton->setLabel( "Remove" );
                //parentWindow->addChild( removeComponentButton );
                //m_removeComponentButton = removeComponentButton;
                //m_removeComponentButton->addObjectListener( m_uiListener );

                if( auto event = getEvent() )
                {
                    auto listeners = event->getListeners();
                    for( auto &listener : listeners )
                    {
                        auto eventListenerWindow = fb::make_ptr<EventListenerWindow>();
                        eventListenerWindow->setParent( getParentWindow() );
                        eventListenerWindow->load( nullptr );
                        m_eventListenerWindows.push_back( eventListenerWindow );

                        eventListenerWindow->setEventListener( listener );
                        eventListenerWindow->updateSelection();
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<scene::IComponentEvent> EventWindow::getEvent() const
        {
            return m_event;
        }

        void EventWindow::setEvent( SmartPtr<scene::IComponentEvent> event )
        {
            m_event = event;
        }

        EventWindow::UIElementListener::UIElementListener()
        {
        }

        EventWindow::UIElementListener::~UIElementListener()
        {
        }

        Parameter EventWindow::UIElementListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            return Parameter();
        }

        EventWindow *EventWindow::UIElementListener::getOwner() const
        {
            return m_owner;
        }

        void EventWindow::UIElementListener::setOwner( EventWindow *owner )
        {
            m_owner = owner;
        }
    }  // namespace editor
}  // namespace fb
