#include <GameEditorPCH.h>
#include "jobs/JobOpenScript.h"
#include "commands/AddNewScriptCmd.h"
#include "editor/EditorManager.h"
#include "editor/Project.h"
#include "ui/ProjectWindow.h"
#include "ui/UIManager.h"

#include <FBCore/FBCore.h>
#include <fstream>

namespace fb::editor
{

    JobOpenScript::JobOpenScript() = default;

    JobOpenScript::~JobOpenScript() = default;

    void JobOpenScript::execute()
    {
        /*
        auto applicationManager = core::ApplicationManager::instance();

        auto appRoot = EditorManager::getSingletonPtr();
        SmartPtr<Project> project = appRoot->getProject();
        SmartPtr<UIManager> guiMgr = appRoot->getUI();

        ApplicationFrame *appFrame = guiMgr->getApplicationFrame();

        appFrame->openScript( getScriptTemplate() );
         */
    }

}  // namespace fb::editor
