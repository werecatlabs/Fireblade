#include <GameEditorPCH.h>
#include "ui/EventListenerWindow.h"
#include "ui/EventWindow.h"
#include "ui/PropertiesWindow.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{
    EventListenerWindow::EventListenerWindow() = default;

    EventListenerWindow::~EventListenerWindow() = default;

    void EventListenerWindow::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto parent = getParent();

            setParentWindow( parent );

            m_propertiesWindow = fb::make_ptr<PropertiesWindow>();
            m_propertiesWindow->setParent( parent );
            m_propertiesWindow->load( data );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void EventListenerWindow::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            if( m_propertiesWindow )
            {
                m_propertiesWindow->setWindowVisible( false );
                m_propertiesWindow->unload( nullptr );
                m_propertiesWindow = nullptr;
            }

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void EventListenerWindow::updateSelection()
    {
        auto listener = getEventListener();
        m_propertiesWindow->setSelected( listener );

        m_propertiesWindow->updateSelection();
    }

    auto EventListenerWindow::getComponentEventListener() const
        -> SmartPtr<scene::IComponentEventListener>
    {
        return m_componentEventListener;
    }

    void EventListenerWindow::setComponentEventListener(
        SmartPtr<scene::IComponentEventListener> eventListener )
    {
        m_componentEventListener = eventListener;
    }
}  // namespace fb::editor
