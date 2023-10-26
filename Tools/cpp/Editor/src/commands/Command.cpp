#include <GameEditorPCH.h>
#include <commands/Command.h>
#include <editor/EditorManager.h>
#include <editor/Project.h>
#include "ui/UIManager.h"
#include <FBCore/FBCore.h>

namespace fb
{
    namespace editor
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

        ICommand::State Command::getState() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_commandState;
        }

        void Command::setState( ICommand::State state )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_commandState = state;
        }

    }  // namespace editor
}  // namespace fb
