#include <GameEditorPCH.h>
#include "jobs/OpenSceneJob.h"
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <ui/UIManager.h>
#include "ui/ProjectWindow.h"

#include <FBCore/FBCore.h>

namespace fb::editor
{

    OpenSceneJob::OpenSceneJob() = default;

    OpenSceneJob::~OpenSceneJob() = default;

    void OpenSceneJob::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto editorManager = EditorManager::getSingletonPtr();
        FB_ASSERT( editorManager );

        auto uiManager = editorManager->getUI();
        FB_ASSERT( uiManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        if( auto fileDialog = fileSystem->openFileDialog() )
        {
            auto projectPath = applicationManager->getProjectPath();
            if( !fileSystem->isExistingFolder( projectPath ) )
            {
                projectPath = "";
            }

            fileDialog->setDialogMode( INativeFileDialog::DialogMode::Open );
            fileDialog->setFileExtension( ".fbscene" );
            fileDialog->setFilePath( projectPath );

            //auto result = fileDialog->openDialog();
            //if( result == INativeFileDialog::Result::Dialog_Okay )
            {
                auto project = editorManager->getProject();
                FB_ASSERT( project );

                auto filePath = getFilePath();
                if( !StringUtil::isNullOrEmpty( filePath ) )
                {
                    auto fileSystem = applicationManager->getFileSystem();
                    FB_ASSERT( fileSystem );

                    auto path = Path::getFilePath( filePath );
                    path = StringUtil::cleanupPath( path );

                    auto projectPath = applicationManager->getProjectPath();
                    if( StringUtil::isNullOrEmpty( projectPath ) )
                    {
                        projectPath = Path::getWorkingDirectory();
                    }

                    auto scenePath = Path::getRelativePath( projectPath, filePath );
                    scenePath = StringUtil::cleanupPath( scenePath );

                    project->setCurrentScenePath( filePath );

                    auto sceneManager = applicationManager->getSceneManager();
                    if( auto scene = sceneManager->getCurrentScene() )
                    {
                        scene->clear();
                        scene->loadScene( scenePath );
                    }

                    resourceDatabase->refresh();
                    uiManager->rebuildSceneTree();

                    if( auto projectWindow = uiManager->getProjectWindow() )
                    {
                        projectWindow->buildTree();
                    }
                }
            }
        }
    }

    auto OpenSceneJob::getFilePath() const -> String
    {
        return m_filePath;
    }

    void OpenSceneJob::setFilePath( const String &filePath )
    {
        m_filePath = filePath;
    }

}  // namespace fb::editor
