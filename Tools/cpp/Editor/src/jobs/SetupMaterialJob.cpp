#include <GameEditorPCH.h>
#include <jobs/SetupMaterialJob.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        SetupMaterialJob::SetupMaterialJob()
        {
        }

        SetupMaterialJob::~SetupMaterialJob()
        {
        }

        void SetupMaterialJob::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto meshLoader = applicationManager->getMeshLoader();

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto editorManager = EditorManager::getSingletonPtr();
                auto uiManager = editorManager->getUI();

                auto meshFiles = fileSystem->getFileNamesWithExtension( ".fbx" );
                for( auto &file : meshFiles )
                {
                }

                uiManager->rebuildResourceTree();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // end namespace editor
}  // end namespace fb
