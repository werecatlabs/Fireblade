#include <GameEditorPCH.h>
#include "jobs/FileSelectedJob.h"
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        FileSelectedJob::FileSelectedJob()
        {
        }

        FileSelectedJob::~FileSelectedJob()
        {
        }

        void FileSelectedJob::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto taskManager = applicationManager->getTaskManager();
            FB_ASSERT( taskManager );

            auto editorManager = EditorManager::getSingletonPtr();
            FB_ASSERT( editorManager );

            auto project = editorManager->getProject();
            FB_ASSERT( project );

            auto uiManager = editorManager->getUI();
            FB_ASSERT( uiManager );

            if( !applicationManager->isPlaying() )
            {
                auto renderLock = taskManager->lockTask( Thread::Task::Render );
                auto physicsLock = taskManager->lockTask( Thread::Task::Physics );

                auto path = getFilePath();

                auto projectPath = applicationManager->getProjectPath();
                if( StringUtil::isNullOrEmpty( projectPath ) )
                {
                    projectPath = Path::getWorkingDirectory();
                }

                auto scenePath = Path::lexically_normal( projectPath, path );
                scenePath = StringUtil::cleanupPath( scenePath );
                project->setCurrentScenePath( scenePath );

                auto sceneManager = applicationManager->getSceneManager();

                if( auto scene = sceneManager->getCurrentScene() )
                {
                    scene->clear();
                    scene->loadScene( scenePath );
                }

                sceneManager->edit();

                uiManager->rebuildSceneTree();
            }
        }

        String FileSelectedJob::getFilePath() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_filePath;
        }

        void FileSelectedJob::setFilePath( const String &filePath )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_filePath = filePath;
        }

    }  // namespace editor
}  // namespace fb
