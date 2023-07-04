#include <GameEditorPCH.h>
#include "jobs/JobSaveTree.h"
#include "commands/AddNewScriptCmd.h"
#include <editor/EditorManager.h>
#include "editor/Project.h"
#include "ui/ProjectWindow.h"
#include "ui/ApplicationFrame.h"
#include "ui/UIManager.h"
#include <FBApplication/Script/ScriptGenerator.h>
#include <FBCore/FBCore.h>
#include <fstream>

namespace fb
{
    namespace editor
    {

        JobSaveTree::JobSaveTree()
        {
        }

        JobSaveTree::~JobSaveTree()
        {
        }

        void JobSaveTree::execute()
        {
            // auto applicationManager = core::IApplicationManager::instance();
            // SmartPtr<IFileSystem>& fileSystem = applicationManager->getFileSystem();

            // EditorManager* appRoot = EditorManager::getSingletonPtr();
            // SmartPtr<Project> project = appRoot->getProject();
            // SmartPtr<UIManager> guiMgr = appRoot->getUI();

            // ProjectWindow* projectWindow = guiMgr->getProjectWindow();
            // if(!projectWindow)
            //{
            //	FB_EXCEPTION("Error: projectWindow null! ");
            //	return;
            // }

            // projectWindow->saveTreeState();
        }

    }  // end namespace editor
}  // end namespace fb
