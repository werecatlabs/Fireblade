#include <GameEditorPCH.h>
#include "ui/ProfilerWindow.h"
#include "editor/EditorManager.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{
    ProfilerWindow::ProfilerWindow() = default;

    ProfilerWindow::~ProfilerWindow()
    {
        unload( nullptr );
    }

    void ProfilerWindow::load( SmartPtr<ISharedObject> data )
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
            parentWindow->setLabel( "ProfilerWindowChild" );

            if( parent )
            {
                parent->addChild( parentWindow );
            }

            auto inputManager = ui->addElementByType<ui::IUIProfilerWindow>();
            parentWindow->addChild( inputManager );
            m_profilerWindow = inputManager;

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ProfilerWindow::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( getLoadingState() == LoadingState::Loaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                if( auto ui = applicationManager->getUI() )
                {
                    if( m_profilerWindow )
                    {
                        ui->removeElement( m_profilerWindow );
                        m_profilerWindow = nullptr;
                    }
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ProfilerWindow::update()
    {
    }

    auto ProfilerWindow::getProfilerWindow() const -> SmartPtr<ui::IUIProfilerWindow>
    {
        return m_profilerWindow;
    }

    void ProfilerWindow::setProfilerWindow( SmartPtr<ui::IUIProfilerWindow> profilerWindow )
    {
        m_profilerWindow = profilerWindow;
    }
}  // namespace fb::editor
