#include <GameEditorPCH.h>
#include <jobs/CompileProjectJob.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>

namespace fb::editor
{

    CompileProjectJob::CompileProjectJob() = default;

    CompileProjectJob::~CompileProjectJob() = default;

    void CompileProjectJob::execute()
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto editorManager = EditorManager::getSingletonPtr();
        auto project = editorManager->getProject();

        project->compile();
    }

}  // namespace fb::editor
