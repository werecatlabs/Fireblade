#include <GameEditorPCH.h>
#include <commands/Command.h>
#include <editor/EditorManager.h>

#include <editor/Project.h>
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>

namespace fb
{
    namespace editor
    {

        Command::Command()
        {
        }

        Command::~Command()
        {
        }

        void Command::undo()
        {
        }

        void Command::redo()
        {
        }

        void Command::execute()
        {
        }

        ICommand::State Command::getState() const
        {
            return m_commandState;
        }

        void Command::setState( ICommand::State state )
        {
            m_commandState = state;
        }

    }  // namespace editor
}  // namespace fb
