#include <GameEditorPCH.h>
#include "jobs/UnloadPluginJob.h"
#include "editor/EditorManager.h"
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

DECLARE_FUNCTION_ARG1( unloadPlugin, void, fb::core::IApplicationManager * );

namespace fb
{
    namespace editor
    {
        FB_CLASS_REGISTER_DERIVED( fb, UnloadPluginJob, Job );

        UnloadPluginJob::UnloadPluginJob()
        {
        }

        UnloadPluginJob::~UnloadPluginJob()
        {
        }

        void UnloadPluginJob::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
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

        SmartPtr<IPlugin> UnloadPluginJob::getPlugin() const
        {
            return m_plugin;
        }

        void UnloadPluginJob::setPlugin( SmartPtr<IPlugin> plugin )
        {
            m_plugin = plugin;
        }
    }  // namespace editor
}  // namespace fb
