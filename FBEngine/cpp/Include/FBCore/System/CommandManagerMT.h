#ifndef CommandManagerMT_H
#define CommandManagerMT_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/System/ICommandManager.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    
    /** A multi threaded implementation of the command manager. */
    class CommandManagerMT : public SharedObject<ICommandManager>
    {
    public:
        CommandManagerMT();
        ~CommandManagerMT() override;

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
        RecursiveMutex m_mutex;

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
