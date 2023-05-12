#ifndef __IThreadPool_H_
#define __IThreadPool_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    /**
     * @brief IThreadPool represents an interface for managing a pool of threads. Inherits from ISharedObject.
     */
    class IThreadPool : public ISharedObject
    {
    public:
        /**
         * @brief Enumeration representing the possible states of the thread pool.
         */
        enum class State
        {
            None,
            Start,
            Stop,

            Count
        };

        /** Virtual destructor. */
        ~IThreadPool() override = default;

        /**
         * @brief Adds a worker thread to the pool.
         *
         * @return A smart pointer to the added IWorkerThread.
         */
        virtual SmartPtr<IWorkerThread> addWorkerThread() = 0;

        /**
         * @brief Retrieves a worker thread by its index.
         *
         * @param index The index of the worker thread to retrieve.
         * @return A smart pointer to the IWorkerThread at the specified index.
         */
        virtual SmartPtr<IWorkerThread> getThread( u32 index ) = 0;

        /**
         * @brief Retrieves the number of threads in the pool.
         *
         * @return The number of threads in the pool.
         */
        virtual u32 getNumThreads() const = 0;

        /**
         * @brief Sets the number of threads in the pool.
         *
         * @param numThreads The desired number of threads in the pool.
         */
        virtual void setNumThreads( u32 numThreads ) = 0;

        /**
         * @brief Retrieves the state of the thread pool.
         *
         * @return The state of the thread pool.
         */
        virtual State getState() const = 0;

        /**
         * @brief Sets the state of the thread pool.
         *
         * @param state The desired state of the thread pool.
         */
        virtual void setState( State state ) = 0;

        /**
         * @brief Stops the execution of all worker threads in the thread pool.
         */
        virtual void stop() = 0;

        FB_CLASS_REGISTER_DECL;
    };
}  // end namespace fb

#endif
