#include <GameEditorPCH.h>
#include "jobs/LoadPluginJob.h"
#include "editor/EditorManager.h"
#include "editor/Project.h"
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

DECLARE_FUNCTION_ARG1( loadPlugin, void, fb::core::IApplicationManager * );

namespace fb
{
    namespace editor
    {
        FB_CLASS_REGISTER_DERIVED( fb, LoadPluginJob, Job );

        LoadPluginJob::LoadPluginJob()
        {
        }

        LoadPluginJob::~LoadPluginJob()
        {
        }

        void LoadPluginJob::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto editorManager = EditorManager::getSingletonPtr();
                auto project = editorManager->getProject();

                auto pluginManager = applicationManager->getPluginManager();
                if(pluginManager)
                {
                    auto pluginPath = getPluginPath();
                    auto plugin = pluginManager->loadPlugin( pluginPath );
                    if(plugin)
                    {
                        plugin->load( nullptr );

                        auto libraryHandle = plugin->getLibraryHandle();
                        init_loadPlugin( libraryHandle );

                        if(hloadPlugin)
                        {
                            auto fLoadPlugin = hloadPlugin;
                            fLoadPlugin( applicationManager.get() );
                        }

                        project->setPlugin( plugin );
                    }
                }
            }
            catch(Exception &e)
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        String LoadPluginJob::getPluginPath() const
        {
            return m_pluginPath;
        }

        void LoadPluginJob::setPluginPath( const String &pluginPath )
        {
            m_pluginPath = pluginPath;
        }
    } // namespace editor
}     // namespace fb
