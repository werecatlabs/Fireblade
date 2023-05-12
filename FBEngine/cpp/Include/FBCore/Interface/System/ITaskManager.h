#ifndef __ITaskManager_H_
#define __ITaskManager_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Thread/ThreadTypes.h>

namespace fb
{
    /**
     * @brief Interface for a task manager.
     */
    class ITaskManager : public ISharedObject
    {
    public:
        /**
         * @brief Enumeration of task manager states.
         */
        enum class State
        {
            None,
            FreeStep,
            Parallel,
            LockStep,
            FixedStep,
            Sequential,

            Count
        };

        /** Virtual destructor. */
        ~ITaskManager() override = default;

        /**
         * @brief Retrieves a task with the specified task ID.
         *
         * @param taskId The ID of the task to retrieve.
         * @return A smart pointer to the ITask with the given task ID, or null if no task exists.
         */
        virtual SmartPtr<ITask> getTask( Thread::Task taskId ) const = 0;

        /**
         * @brief Retrieves an array of all tasks managed by the task manager.
         *
         * @return An array of smart pointers to ITask objects.
         */
        virtual Array<SmartPtr<ITask>> getTasks() const = 0;

        /**
         * @brief Sets the state of the task manager.
         *
         * @param state The desired state for the task manager.
         */
        virtual void setState( State state ) = 0;

        /**
         * @brief Retrieves the current state of the task manager.
         *
         * @return The current state of the task manager.
         */
        virtual State getState() const = 0;

        /**
         * @brief Retrieves the number of tasks managed by the task manager.
         *
         * @return The number of tasks.
         */
        virtual u32 getNumTasks() const = 0;

        /**
         * @brief Waits for all tasks managed by the task manager to complete.
         */
        virtual void wait() = 0;

        /**
         * @brief Stops the execution of all tasks managed by the task manager.
         */
        virtual void stop() = 0;

        /**
         * @brief Resets the task manager, effectively clearing its state and tasks.
         */
        virtual void reset() = 0;

        /**
         * @brief Locks a task with the specified task ID.
         *
         * @param taskId The ID of the task to lock.
         * @return A smart pointer to an ITaskLock for the locked task.
         */
        virtual SmartPtr<ITaskLock> lockTask( Thread::Task taskId ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif
