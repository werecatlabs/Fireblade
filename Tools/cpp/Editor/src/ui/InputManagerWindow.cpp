#include <GameEditorPCH.h>
#include <ui/InputManagerWindow.h>
#include "editor/EditorManager.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{
    FB_CLASS_REGISTER_DERIVED( fb, InputManagerWindow, EditorWindow );

    InputManagerWindow::InputManagerWindow() = default;

    InputManagerWindow::~InputManagerWindow()
    {
        unload( nullptr );
    }

    void InputManagerWindow::load( SmartPtr<ISharedObject> data )
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
            parentWindow->setLabel( "TerrainWindowChild" );

            if( parent )
            {
                parent->addChild( parentWindow );
            }

            auto inputManager = ui->addElementByType<ui::IUIInputManager>();
            parentWindow->addChild( inputManager );
            m_inputManager = inputManager;

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void InputManagerWindow::unload( SmartPtr<ISharedObject> data )
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
                    if( m_inputManager )
                    {
                        ui->removeElement( m_inputManager );
                        m_inputManager = nullptr;
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

    auto InputManagerWindow::getInputManager() const -> SmartPtr<ui::IUIInputManager>
    {
        return m_inputManager;
    }

    void InputManagerWindow::setInputManager( SmartPtr<ui::IUIInputManager> inputManager )
    {
        m_inputManager = inputManager;
    }

}  // namespace fb::editor
