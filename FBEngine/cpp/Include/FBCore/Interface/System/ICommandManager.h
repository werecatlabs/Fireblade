#ifndef ICommandManager_H
#define ICommandManager_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    
    /** Interface for a command manager class.
     */
    class ICommandManager : public ISharedObject
    {
    public:
        enum class CommandType
        {
            COMMAND_MANAGER_NONE,
            COMMAND_MANAGER_UNDO,
            COMMAND_MANAGER_REDO,
            COMMAND_MANAGER_ADD,
            Count
        };

        /** Virtual destructor. */
        ~ICommandManager() override = default;

        /** Adds a command to the command history.
        @param command The command instance.
        */
        virtual void addCommand( SmartPtr<ICommand> command ) = 0;

        virtual void removeCommand( SmartPtr<ICommand> command ) = 0;

        virtual bool hasCommand( SmartPtr<ICommand> command ) = 0;
        virtual bool isCommandQueued( SmartPtr<ICommand> command ) = 0;

        /** Retrieves the next command returns null=0 if
        there is not a next command.
        */
        virtual SmartPtr<ICommand> getNextCommand() = 0;

        /** Retrieves the previous command returns null=0 if
        there is not a next command.
        */
        virtual SmartPtr<ICommand> getPreviousCommand() = 0;

        /** Clears all the commands. */
        virtual void clearAll() = 0;

        /** Adds a listener.
        @param listener The listener instance.
        */
        virtual void addListener( SmartPtr<ICommandManagerListener> listener ) = 0;

        /** Removes a listener.
        @param listener The listener instance.
        @return Returns true if the listener was removed.
        */
        virtual bool removeListener( SmartPtr<ICommandManagerListener> listener ) = 0;
    };
}  // end namespace fb

#endif
