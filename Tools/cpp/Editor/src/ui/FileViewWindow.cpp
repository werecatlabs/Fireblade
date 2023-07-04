#include <GameEditorPCH.h>
#include <ui/FileViewWindow.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>

namespace fb
{
    namespace editor
    {

        FileViewWindow::FileViewWindow( SmartPtr<ui::IUIWindow> parent )
        {
            setParent( parent );
        }

        FileViewWindow::~FileViewWindow()
        {
        }

        void FileViewWindow::load( SmartPtr<ISharedObject> data )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            auto parent = getParent();

            auto parentWindow = ui->addElementByType<ui::IUIWindow>();
            setParentWindow( parentWindow );

            if( parent )
            {
                parent->addChild( parentWindow );
            }

            m_text = ui->addElementByType<ui::IUIText>();
            FB_ASSERT( m_text );

            parentWindow->addChild( m_text );
        }

        void FileViewWindow::unload( SmartPtr<ISharedObject> data )
        {
        }

        void FileViewWindow::updateSelection()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto selection = selectionManager->getSelection();
            if( !selection.empty() )
            {
                auto selected = selection.front();
                if( selected->isDerived<FileSelection>() )
                {
                    auto fileSelection = fb::static_pointer_cast<FileSelection>( selected );
                    auto filePath = fileSelection->getFilePath();

                    auto data = fileSystem->readAllText( filePath );

                    m_text->setText( data );
                }
            }
        }

    }  // end namespace editor
}  // end namespace fb
