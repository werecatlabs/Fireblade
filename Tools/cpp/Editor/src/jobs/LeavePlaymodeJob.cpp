#include <GameEditorPCH.h>
#include "jobs/LeavePlaymodeJob.h"
#include "editor/EditorManager.h"
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        LeavePlaymodeJob::LeavePlaymodeJob()
        {
        }

        LeavePlaymodeJob::~LeavePlaymodeJob()
        {
        }

        void LeavePlaymodeJob::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto taskManager = applicationManager->getTaskManager();
                FB_ASSERT( taskManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto renderLock = taskManager->lockTask( Thread::Task::Render );
                auto physicsLock = taskManager->lockTask( Thread::Task::Physics );
                auto applicationLock = taskManager->lockTask( Thread::Task::Application );

                if( applicationManager->isPlaying() )
                {
                    applicationManager->setPlaying( false );
                    applicationManager->setEditorCamera( true );

                    auto sceneManager = applicationManager->getSceneManager();
                    FB_ASSERT( sceneManager );

                    auto scene = sceneManager->getCurrentScene();
                    FB_ASSERT( scene );

                    auto cachePath = applicationManager->getCachePath();
                    FB_ASSERT( !StringUtil::isNullOrEmpty( cachePath ) );

                    static const auto filePath = "/tmp.fbscene";
                    auto tempScenePath = cachePath + filePath;

                    auto projectPath = applicationManager->getProjectPath();
                    if( StringUtil::isNullOrEmpty( projectPath ) )
                    {
                        projectPath = Path::getWorkingDirectory();
                    }

                    tempScenePath = Path::getRelativePath( projectPath, tempScenePath );

                    FB_ASSERT( !StringUtil::isNullOrEmpty( tempScenePath ) );
                    FB_ASSERT( !Path::isPathAbsolute( tempScenePath ) );

                    auto sceneFilePath = scene->getFilePath();
                    scene->clear();
                    scene->loadScene( sceneFilePath );

                    auto editorManager = EditorManager::getSingletonPtr();
                    FB_ASSERT( editorManager );

                    auto uiManager = editorManager->getUI();
                    FB_ASSERT( uiManager );

                    uiManager->rebuildSceneTree();
                }

                if( auto sceneManager = applicationManager->getSceneManager() )
                {
                    sceneManager->edit();
                }

                if( auto cameraManager = applicationManager->getCameraManager() )
                {
                    cameraManager->edit();
                }
            }
            catch( Exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // namespace editor
}  // namespace fb
