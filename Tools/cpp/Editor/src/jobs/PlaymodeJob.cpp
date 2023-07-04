#include <GameEditorPCH.h>
#include "jobs/PlaymodeJob.h"
#include "editor/EditorManager.h"
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        PlaymodeJob::PlaymodeJob()
        {
        }

        PlaymodeJob::~PlaymodeJob()
        {
        }

        void PlaymodeJob::execute()
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

                if( !applicationManager->isPlaying() )
                {
                    auto sceneManager = applicationManager->getSceneManager();
                    FB_ASSERT( sceneManager );

                    auto scene = sceneManager->getCurrentScene();
                    FB_ASSERT( scene );

                    applicationManager->setEditorCamera( false );

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

                    //scene->saveScene( tempScenePath );

                    // safety code
                    // clear and load to make sure values are
                    // loaded correctly in actors and components
                    // can refactor to add playing events

                    //scene->clear();
                    //scene->loadScene( tempScenePath );

                    applicationManager->setPlaying( true );

                    auto editorManager = EditorManager::getSingletonPtr();
                    FB_ASSERT( editorManager );

                    auto uiManager = editorManager->getUI();
                    FB_ASSERT( uiManager );

                    uiManager->rebuildSceneTree();
                }

                if( auto sceneManager = applicationManager->getSceneManager() )
                {
                    sceneManager->play();
                }

                if( auto cameraManager = applicationManager->getCameraManager() )
                {
                    cameraManager->play();
                }
            }
            catch( Exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // namespace editor
}  // namespace fb
