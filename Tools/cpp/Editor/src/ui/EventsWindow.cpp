#include <GameEditorPCH.h>
#include "ui/EventsWindow.h"
#include "ui/EventWindow.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{

    EventsWindow::EventsWindow() = default;

    EventsWindow::~EventsWindow()
    {
        unload( nullptr );
    }

    void EventsWindow::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto parent = getParent();

            auto parentWindow = ui->addElementByType<ui::IUIWindow>();
            FB_ASSERT( parentWindow );

            setParentWindow( parentWindow );
            parentWindow->setLabel( "EventsWindowChild" );
            parentWindow->setSize( Vector2F( 0.0f, 300.0f ) );
            parentWindow->setHasBorder( true );

            if( parent )
            {
                parent->addChild( parentWindow );
            }

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void EventsWindow::unload( SmartPtr<ISharedObject> data )
    {
    }

    void EventsWindow::updateSelection()
    {
        try
        {
            if( isWindowVisible() )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto ui = applicationManager->getUI();
                FB_ASSERT( ui );

                auto selectionManager = applicationManager->getSelectionManager();
                FB_ASSERT( selectionManager );

                //m_eventsWindow->removeAllChildren();

                for( auto window : m_eventWindows )
                {
                    window->unload( nullptr );
                }

                m_eventWindows.clear();

                auto selection = selectionManager->getSelection();
                for( auto object : selection )
                {
                    if( object->isDerived<scene::IComponent>() )
                    {
                        auto component = fb::static_pointer_cast<scene::IComponent>( object );
                        auto events = component->getEvents();
                        for( auto event : events )
                        {
                            auto eventWindow = fb::make_ptr<EventWindow>();
                            eventWindow->setParent( getParentWindow() );
                            eventWindow->load( nullptr );
                            m_eventWindows.push_back( eventWindow );

                            eventWindow->setEvent( event );
                            eventWindow->updateSelection();
                        }
                    }
                }
            }
            else
            {
                for( auto window : m_eventWindows )
                {
                    window->unload( nullptr );
                }

                m_eventWindows.clear();
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

}  // namespace fb::editor
