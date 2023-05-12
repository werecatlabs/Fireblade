#ifndef _ICOMMAND_H
#define _ICOMMAND_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief Interface for a command, providing support for the undo/redo functionality.
     * Inherits from ISharedObject.
     */
    class ICommand : public ISharedObject
    {
    public:
        enum class State
        {
            Allocated,
            Queued,
            Executing,
            Finished,

            Count
        };

        /**
         * @brief Destroy the ICommand object.
         */
        ~ICommand() override = default;

        /**
         * @brief Undo the previously executed command.
         */
        virtual void undo() = 0;

        /**
         * @brief Redo the previously undone command.
         */
        virtual void redo() = 0;

        /**
         * @brief Execute the command.
         */
        virtual void execute() = 0;

        virtual State getState() const = 0;
        virtual void setState(State state) = 0;
    };

}  // end namespace fb

#endif
