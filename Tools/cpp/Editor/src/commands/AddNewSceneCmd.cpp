#include <GameEditorPCH.h>
#include "commands/AddNewSceneCmd.h"
#include <FBCore/Core/Properties.h>

namespace fb::editor
{

    //--------------------------------------------
    AddNewSceneCmd::AddNewSceneCmd( Properties properties ) : m_properties( properties )
    {
    }

    //--------------------------------------------
    AddNewSceneCmd::~AddNewSceneCmd() = default;

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

}  // namespace fb::editor
