#ifndef CommandManagerStandard_H
#define CommandManagerStandard_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/ICommandManager.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /** A standard implementation of the command manager. Runs the commands from the primary thread. */
    class CommandManager : public CSharedObject<ICommandManager>
    {
    public:
        CommandManager();
        ~CommandManager() override;

        //! adds a command to the command history
        void addCommand( SmartPtr<ICommand> command ) override;

        void removeCommand( SmartPtr<ICommand> command );

        bool hasCommand( SmartPtr<ICommand> command );
        bool isCommandQueued( SmartPtr<ICommand> command );

        //! retrieves the next command returns null=0 if
        //! there is not a next command
        SmartPtr<ICommand> getNextCommand() override;

        //! retrieves the previous command returns null=0 if
        //! there is not a next command
        SmartPtr<ICommand> getPreviousCommand() override;

        void clearAll() override;

        void addListener( SmartPtr<ICommandManagerListener> listener ) override;
        bool removeListener( SmartPtr<ICommandManagerListener> listener ) override;

    private:
        /// The index of the current command.
        s32 m_currentCommand;

        s32 m_lastUndoCommand;
        s32 m_lastRedoCommand;

        ///
        u32 m_numStoredCommands;

        Array<SmartPtr<ICommand>> m_commands;
        Array<SmartPtr<ICommandManagerListener>> m_listeners;

        CommandType m_lastCommand = CommandType::COMMAND_MANAGER_NONE;
    };
}  // end namespace fb

#endif
