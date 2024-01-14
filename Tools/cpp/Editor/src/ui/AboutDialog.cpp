#include <GameEditorPCH.h>
#include <ui/AboutDialog.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    void AboutDialog::load( SmartPtr<ISharedObject> data )
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
            parentWindow->setLabel( "AboutDialog" );

            if( parent )
            {
                parent->addChild( parentWindow );
            }

            if( parentWindow )
            {
                parentWindow->setSize( Vector2F( 400, 300 ) );
            }

            auto about = ui->addElementByType<ui::IUIAbout>();
            parentWindow->addChild( about );
            m_aboutWindow = about;

            FB_ASSERT( parentWindow->isValid() );
            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void AboutDialog::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

}  // namespace fb::editor
