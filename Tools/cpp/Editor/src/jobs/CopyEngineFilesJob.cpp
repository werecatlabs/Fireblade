#include <GameEditorPCH.h>
#include <jobs/CopyEngineFilesJob.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        CopyEngineFilesJob::CopyEngineFilesJob()
        {
        }

        CopyEngineFilesJob::~CopyEngineFilesJob()
        {
        }

        void CopyEngineFilesJob::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto editorManager = EditorManager::getSingletonPtr();
                auto projectManager = editorManager->getProjectManager();

                auto srcFolder = applicationManager->getMediaPath() + "/Engine";
                auto dstFolder = applicationManager->getProjectPath() + "/Engine";

                if(Path::isExistingFolder(dstFolder))
                {
                    Path::deleteFolder(dstFolder);
                }

                Path::copyFolder(srcFolder, dstFolder);
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // end namespace editor
}  // end namespace fb
