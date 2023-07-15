#include <GameEditorPCH.h>
#include <jobs/SaveSceneJob.h>
#include <editor/EditorManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        SaveSceneJob::SaveSceneJob()
        {
        }

        SaveSceneJob::~SaveSceneJob()
        {
        }

        void SaveSceneJob::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto editorManager = EditorManager::getSingletonPtr();
            FB_ASSERT( editorManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            if( auto scene = sceneManager->getCurrentScene() )
            {
                auto filePath = scene->getFilePath();
                if (!StringUtil::isNullOrEmpty(filePath))
                {
                    if( Path::isExistingFile( filePath ) )
                    {
                        scene->saveScene();
                    }
                    else
                    {
                        saveScene( filePath );
                    }
                }

                filePath = getFilePath();
                if ( !StringUtil::isNullOrEmpty(filePath) )
                {
                    saveScene( filePath );
                }
            }
        }

        void SaveSceneJob::saveScene( const String &filePath )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            FB_ASSERT( fileSystem );

            auto editorManager = EditorManager::getSingletonPtr();
            FB_ASSERT( editorManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto scene = sceneManager->getCurrentScene();

            if( auto fileDialog = fileSystem->openFileDialog() )
            {
                auto projectPath = Path::getWorkingDirectory();
                if( !fileSystem->isExistingFolder( projectPath ) )
                {
                    projectPath = "";
                }

                fileDialog->setDialogMode( INativeFileDialog::DialogMode::Save );
                fileDialog->setFileExtension( ".fbscene" );
                fileDialog->setFilePath( projectPath );

                auto result = fileDialog->openDialog();
                if( result == INativeFileDialog::Result::Dialog_Okay )
                {
                    auto sceneFilePath = fileDialog->getFilePath();
                    scene->saveScene( sceneFilePath );
                }
            }
        }

        String SaveSceneJob::getFilePath() const
        {
            return m_filePath;
        }

        void SaveSceneJob::setFilePath( const String &filePath )
        {
            m_filePath = filePath;
        }

    }  // namespace editor
}  // namespace fb
