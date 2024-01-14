#include <GameEditorPCH.h>
#include "jobs/UnloadPluginJob.h"
#include "editor/EditorManager.h"
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

DECLARE_FUNCTION_ARG1( unloadPlugin, void, fb::core::ApplicationManager * );

namespace fb::editor
{
    FB_CLASS_REGISTER_DERIVED( fb, UnloadPluginJob, Job );

    UnloadPluginJob::UnloadPluginJob() = default;

    UnloadPluginJob::~UnloadPluginJob() = default;

    void UnloadPluginJob::execute()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto plugin = getPlugin();

            auto pluginManager = applicationManager->getPluginManager();
            if( pluginManager )
            {
                auto libraryHandle = plugin->getLibraryHandle();
                init_unloadPlugin( libraryHandle );

                if( hunloadPlugin )
                {
                    hunloadPlugin( applicationManager.get() );
                }

                pluginManager->unloadPlugin( plugin );
            }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto UnloadPluginJob::getPlugin() const -> SmartPtr<IPlugin>
    {
        return m_plugin;
    }

    void UnloadPluginJob::setPlugin( SmartPtr<IPlugin> plugin )
    {
        m_plugin = plugin;
    }
}  // namespace fb::editor
