#include <GameEditorPCH.h>
#include <jobs/ReloadScriptsJob.h>
#include <editor/EditorManager.h>
#include <FBCore/FBCore.h>
#include "ui/MaterialWindow.h"
#include "ui/ObjectWindow.h"
#include "ui/TerrainWindow.h"
#include "ui/UIManager.h"

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

            auto threadPool = applicationManager->getThreadPool();

            auto renderLock = taskManager->lockTask( Thread::Task::Render );
            //auto applicationLock = taskManager->lockTask( Thread::Task::Application );
            auto physicsLock = taskManager->lockTask( Thread::Task::Physics );

            auto scriptManager = applicationManager->getScriptManager();
            FB_ASSERT( scriptManager );

            scriptManager->reloadScripts();

            if( threadPool->getNumThreads() > 0 )
            {
                while( scriptManager->reloadPending() )
                {
                    Thread::sleep( 3.0 );
                }
            }

            // hack
            auto editorManager = EditorManager::getSingletonPtr();
            auto ui = editorManager->getUI();
            if( auto materialWindow = ui->getObjectWindow() )
            {
                materialWindow->reload( nullptr );
            }

            if( auto terrainWindow = ui->getTerrainWindow() )
            {
                terrainWindow->reload( nullptr );
            }
        }

    }  // namespace editor
}  // namespace fb
