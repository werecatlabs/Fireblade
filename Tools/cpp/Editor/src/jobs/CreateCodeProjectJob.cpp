#include <GameEditorPCH.h>
#include <jobs/CreateCodeProjectJob.h>
#include <editor/EditorManager.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        CreateCodeProjectJob::CreateCodeProjectJob()
        {
        }

        CreateCodeProjectJob::~CreateCodeProjectJob()
        {
        }

        void CreateCodeProjectJob::execute()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto editorManager = EditorManager::getSingletonPtr();
                auto projectManager = editorManager->getProjectManager();

                auto workingDirectory = Path::getWorkingDirectory();
                auto mediaPath = String();

#if FB_FINAL
                mediaPath += applicationManager->getMediaPath() + "Engine";
#else
#    if defined FB_PLATFORM_WIN32
                mediaPath = String( "../../../../../../" );
#    elif defined FB_PLATFORM_APPLE
                mediaPath = String( "../../../" );
#    else
                mediaPath = String( "../../../FBEngine/cpp/Include" );
#    endif
#endif

                auto absoluteMediaPath = Path::getAbsolutePath( workingDirectory, mediaPath );

                projectManager->setEnginePath( absoluteMediaPath );
                projectManager->generateProject();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

    }  // end namespace editor
}  // end namespace fb
