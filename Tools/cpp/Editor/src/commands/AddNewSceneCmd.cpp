#include <GameEditorPCH.h>
#include "commands/AddNewSceneCmd.h"
#include <FBCore/Core/Properties.h>

namespace fb
{
    namespace editor
    {

        //--------------------------------------------
        AddNewSceneCmd::AddNewSceneCmd( Properties properties ) : m_properties( properties )
        {
        }

        //--------------------------------------------
        AddNewSceneCmd::~AddNewSceneCmd()
        {
        }

        //--------------------------------------------
        void AddNewSceneCmd::undo()
        {
        }

        //--------------------------------------------
        void AddNewSceneCmd::redo()
        {
        }

        //--------------------------------------------
        void AddNewSceneCmd::execute()
        {
        }

    }  // end namespace editor
}  // end namespace fb
