#include <GameEditorPCH.h>
#include <jobs/CompileProjectJob.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
    {

        CompileProjectJob::CompileProjectJob()
        {
        }

        CompileProjectJob::~CompileProjectJob()
        {
        }

        void CompileProjectJob::execute()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto editorManager = EditorManager::getSingletonPtr();
            auto project = editorManager->getProject();

            project->compile();
        }

    }  // namespace editor
}  // namespace fb
