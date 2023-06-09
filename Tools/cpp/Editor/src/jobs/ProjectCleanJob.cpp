#include <GameEditorPCH.h>
#include "jobs/ProjectCleanJob.h"
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        ProjectCleanJob::ProjectCleanJob()
        {
        }

        ProjectCleanJob::~ProjectCleanJob()
        {
        }

        void ProjectCleanJob::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto fileSystem = applicationManager->getFileSystem();
                FB_ASSERT( fileSystem );

                auto editorManager = EditorManager::getSingletonPtr();
                auto uiManager = editorManager->getUI();

                auto metaFiles = fileSystem->getFileNamesWithExtension( ".meta" );
                for( auto &file : metaFiles )
                {
                    fileSystem->deleteFile( file );
                }

                auto lightingFiles = fileSystem->getFileNamesWithExtension( ".lighting" );
                for( auto &file : lightingFiles )
                {
                    fileSystem->deleteFile( file );
                }

                auto unityFiles = fileSystem->getFileNamesWithExtension( ".unity" );
                for( auto &file : unityFiles )
                {
                    fileSystem->deleteFile( file );
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
