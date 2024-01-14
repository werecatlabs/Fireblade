#include <GameEditorPCH.h>
#include <commands/Command.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

namespace fb::editor
{

    void Command::undo()
    {
    }

    void Command::redo()
    {
    }

    void Command::execute()
    {
    }

    auto Command::getState() const -> ICommand::State
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_commandState;
    }

    void Command::setState( ICommand::State state )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_commandState = state;
    }

}  // namespace fb::editor
