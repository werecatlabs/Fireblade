#ifndef RecursiveMutex_h__
#define RecursiveMutex_h__

#include <FBCore/FBCoreTypes.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

namespace fb
{
    /**
     * A recursive mutex.
     */
    class RecursiveMutex
    {
    public:
        /**
         * A scoped lock for a recursive mutex.
         */
        class ScopedLock
        {
        public:
            /**
             * Constructor that locks the mutex.
             *
             * @param mutex The recursive mutex to lock.
             */
            ScopedLock( RecursiveMutex &mutex );

            /**
             * Destructor that unlocks the mutex.
             */
            ~ScopedLock();

            /**
             * Conversion operator to check if the lock was successfully acquired.
             *
             * @return True if the lock was successfully acquired, false otherwise.
             */
            operator bool();

        protected:
            RecursiveMutex &m_mutex; /**< The recursive mutex being locked. */
        };

        /**
         * Locks the mutex.
         */
        void lock();

        /**
         * Unlocks the mutex.
         */
        void unlock();

    protected:
        /**< The underlying mutex. */
        std::mutex m_mutex;                  

        /**< The condition variable used for waiting. */
        std::condition_variable m_condition; 

        /**< The thread ID of the owner thread. */
        std::thread::id m_owner_thread = std::thread::id(); 

        /**< The recursion count for the mutex. */
        s32 m_count = 0;                                    
    };

}  // namespace fb

#endif  // RecursiveMutex_h__
