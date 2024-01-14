#include <GameEditorPCH.h>
#include <jobs/CopyEngineFilesJob.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    CopyEngineFilesJob::CopyEngineFilesJob() = default;

    CopyEngineFilesJob::~CopyEngineFilesJob() = default;

    void CopyEngineFilesJob::execute()
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            auto editorManager = EditorManager::getSingletonPtr();
            auto projectManager = editorManager->getProjectManager();

            auto srcFolder = applicationManager->getMediaPath() + "/Engine";
            auto dstFolder = applicationManager->getProjectPath() + "/Engine";

            if( Path::isExistingFolder( dstFolder ) )
            {
                Path::deleteFolder( dstFolder );
            }

            Path::copyFolder( srcFolder, dstFolder );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

}  // namespace fb::editor
