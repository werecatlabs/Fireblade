#include <GameEditorPCH.h>
#include <jobs/ReloadScriptsJob.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        ReloadScriptsJob::ReloadScriptsJob()
        {
        }

        ReloadScriptsJob::~ReloadScriptsJob()
        {
        }

        void ReloadScriptsJob::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto taskManager = applicationManager->getTaskManager();
            FB_ASSERT( taskManager );

            auto renderLock = taskManager->lockTask( Thread::Task::Render );
            auto applicationLock = taskManager->lockTask( Thread::Task::Application );
            auto physicsLock = taskManager->lockTask( Thread::Task::Physics );

            auto scriptManager = applicationManager->getScriptManager();
            FB_ASSERT( scriptManager );

            scriptManager->reloadScripts();
        }

    }  // namespace editor
}  // namespace fb
