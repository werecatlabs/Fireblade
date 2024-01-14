#include <GameEditorPCH.h>
#include <jobs/SaveSceneJob.h>
#include <editor/EditorManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    SaveSceneJob::SaveSceneJob() = default;

    SaveSceneJob::~SaveSceneJob() = default;

    void SaveSceneJob::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
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
            if( !StringUtil::isNullOrEmpty( filePath ) )
            {
                if( !getSaveAs() )
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
                else
                {
                    saveScene( filePath );
                }

                filePath = getFilePath();
                if( !StringUtil::isNullOrEmpty( filePath ) )
                {
                    saveScene( filePath );
                }
            }
            else
            {
                auto filePath = applicationManager->getProjectPath();
                saveScene( filePath );
            }
        }
    }

    void SaveSceneJob::saveScene( const String &filePath )
    {
        auto applicationManager = core::ApplicationManager::instance();
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
            auto projectPath = filePath;

            if( StringUtil::isNullOrEmpty( projectPath ) )
            {
                projectPath = applicationManager->getProjectPath();
            }

            if( !fileSystem->isExistingFolder( projectPath ) )
            {
                projectPath = Path::getWorkingDirectory();
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

    auto SaveSceneJob::getFilePath() const -> String
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_filePath;
    }

    void SaveSceneJob::setFilePath( const String &filePath )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_filePath = filePath;
    }

    auto SaveSceneJob::getSaveAs() const -> bool
    {
        SpinRWMutex::ScopedLock lock( m_mutex, false );
        return m_saveAs;
    }

    void SaveSceneJob::setSaveAs( bool saveAs )
    {
        SpinRWMutex::ScopedLock lock( m_mutex, true );
        m_saveAs = saveAs;
    }

}  // namespace fb::editor
