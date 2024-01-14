#include <GameEditorPCH.h>
#include "jobs/LoadPluginJob.h"
#include "editor/EditorManager.h"
#include "editor/Project.h"
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

DECLARE_FUNCTION_ARG1( loadPlugin, void, fb::core::ApplicationManager * );
DECLARE_FUNCTION_ARG3( getFirebladeVersion, void, int *, int *, int * );

namespace fb::editor
{
    FB_CLASS_REGISTER_DERIVED( fb, LoadPluginJob, Job );

    LoadPluginJob::LoadPluginJob() = default;

    LoadPluginJob::~LoadPluginJob() = default;

    void LoadPluginJob::execute()
    {
        try
        {
            int exeMajor = FB_VERSION_MAJOR;
            int exeMinor = FB_VERSION_MINOR;
            int exePatch = FB_VERSION_PATCH;

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto editorManager = EditorManager::getSingletonPtr();
            auto project = editorManager->getProject();

            auto pluginManager = applicationManager->getPluginManager();
            if( pluginManager )
            {
                auto pluginPath = getPluginPath();
                auto plugin = pluginManager->loadPlugin( pluginPath );
                if( plugin )
                {
                    plugin->load( nullptr );

                    auto libraryHandle = plugin->getLibraryHandle();

                    init_getFirebladeVersion( libraryHandle );

                    int dllMajor = 0;
                    int dllMinor = 0;
                    int dllPatch = 0;

                    if( hgetFirebladeVersion )
                    {
                        hgetFirebladeVersion( &dllMajor, &dllMinor, &dllPatch );
                    }

                    // Compare versions
                    if( exeMajor != dllMajor || exeMinor != dllMinor || exePatch != dllPatch )
                    {
                        std::cerr << "Error: DLL version mismatch. Recompile required." << std::endl;
                        // Perform actions for version mismatch, e.g., recompiling
                    }
                    else
                    {
                        //std::cout << "DLL version matches the host executable version." << std::endl;

                        init_loadPlugin( libraryHandle );

                        if( hloadPlugin )
                        {
                            auto fLoadPlugin = hloadPlugin;
                            fLoadPlugin( applicationManager.get() );
                        }

                        project->setPlugin( plugin );
                    }
                }
            }
        }
        catch( Exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto LoadPluginJob::getPluginPath() const -> String
    {
        return m_pluginPath;
    }

    void LoadPluginJob::setPluginPath( const String &pluginPath )
    {
        m_pluginPath = pluginPath;
    }
}  // namespace fb::editor
