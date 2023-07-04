#include <GameEditorPCH.h>
#include <jobs/OptimiseDatabasesJob.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        OptimiseDatabasesJob::OptimiseDatabasesJob()
        {
        }

        OptimiseDatabasesJob::~OptimiseDatabasesJob()
        {
        }

        void OptimiseDatabasesJob::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

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
