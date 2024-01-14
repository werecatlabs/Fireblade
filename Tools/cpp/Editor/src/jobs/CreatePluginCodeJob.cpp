#include <GameEditorPCH.h>
#include <jobs/CreatePluginCodeJob.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    CreatePluginCodeJob::CreatePluginCodeJob() = default;

    CreatePluginCodeJob::~CreatePluginCodeJob() = default;

    void CreatePluginCodeJob::execute()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto editorManager = EditorManager::getSingletonPtr();
            auto projectManager = editorManager->getProjectManager();

            auto path = applicationManager->getProjectPath();

            auto pluginPath = path + "/Plugin";
            if( !Path::isExistingFolder( pluginPath ) )
            {
                Path::createDirectories( pluginPath );
            }

            auto project = editorManager->getProject();
            auto pluginHeaderStr = project->getPluginHeader();
            auto pluginSourceStr = project->getPluginSource();

            Path::writeAllText( pluginPath + "/Plugin.h", pluginHeaderStr );
            Path::writeAllText( pluginPath + "/Plugin.cpp", pluginSourceStr );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

}  // namespace fb::editor
