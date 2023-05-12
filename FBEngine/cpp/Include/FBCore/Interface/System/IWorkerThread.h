#ifndef __IWorkerThread_H_
#define __IWorkerThread_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Thread/ThreadTypes.h>

namespace fb
{

    /**
     * @brief IWorkerThread represents a thread that belongs to a thread pool. Inherits from ISharedObject.
     */
    class IWorkerThread : public ISharedObject
    {
    public:
        /**
         * @brief Enumeration representing the possible states of a worker thread.
         */
        enum class State
        {
            None,
            Start,
            Stop,

            Count
        };

        /** Virtual destructor. */
        ~IWorkerThread() override = default;

        /**
         * @brief Executes the main functionality of the worker thread.
         */
        virtual void run() = 0;

        /**
         * @brief Retrieves the ID of the thread.
         *
         * @return The ID of the thread.
         */
        virtual Thread::ThreadId getThreadId() const = 0;

        /** Sets the thread id.
        @param threadId The id of the thread.
        */
        virtual void setThreadId( Thread::ThreadId threadId ) = 0;

        /**
         * @brief Retrieves the target frame rate of the worker thread.
         *
         * @return The target frame rate as the number of frames per second.
         */
        virtual time_interval getTargetFPS() const = 0;

        /** Sets the target frame rate.
        @param framesPerSecond The number of frames per second.
        */
        virtual void setTargetFPS( time_interval framesPerSecond ) = 0;

        /** Gets the worker thread state.
        @return The thread state.
        */
        virtual State getState() const = 0;

        /** Sets the worker thread state.
        @param state The thread state.
        */
        virtual void setState( State state ) = 0;

        /**
         * @brief Stops the execution of the worker thread.
         */
        virtual void stop() = 0;

        /**
         * @brief Checks if the worker thread is updating.
         *
         * @return True if the worker thread is updating, false otherwise.
         */
        virtual bool isUpdating() const = 0;

        /**
         * @brief Sets the updating status of the worker thread.
         *
         * @param updating A boolean value representing the updating status of the worker thread.
         */
        virtual void setUpdating( bool updating ) = 0;

        FB_CLASS_REGISTER_DECL;
    };

}  // end namespace fb

#endif
