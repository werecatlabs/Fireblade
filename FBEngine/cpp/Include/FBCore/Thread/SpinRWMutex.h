#ifndef SpinRWMutex_h__
#define SpinRWMutex_h__

#include <FBCore/FBCoreTypes.h>
#include <atomic>

namespace fb
{
    /**
     * A read-write mutex implemented using spin locks.
     */
    class SpinRWMutex
    {
    public:
        /**
         * A scoped lock for the SpinRWMutex.
         */
        class ScopedLock
        {
        public:
            /**
             * Constructs a lock for the specified SpinRWMutex.
             *
             * @param m The SpinRWMutex to lock.
             * @param write True if the lock should be exclusive for writing; false if it's a shared read lock.
             */
            ScopedLock( SpinRWMutex &m, bool write = true );

            /**
             * Releases the lock.
             */
            ~ScopedLock();

            /**
             * Returns true if the lock was successfully acquired; false otherwise.
             */
            operator bool();

        protected:
            SpinRWMutex &m_mutex;
            bool m_write = true;
        };

        /**
         * Constructs a SpinRWMutex.
         */
        SpinRWMutex();

        /**
         * Locks the mutex for shared read access.
         */
        void lock_read();

        /**
         * Unlocks the shared read lock.
         */
        void unlock_read();

        /**
         * Locks the mutex for exclusive write access.
         */
        void lock_write();

        /**
         * Unlocks the exclusive write lock.
         */
        void unlock_write();

    protected:
        ///< The number of readers currently holding a shared lock.
        std::atomic<s32> readers;         

        ///< The number of writers currently holding an exclusive lock.
        std::atomic<s32> writers;         

        ///< True if there are pending write lock requests.
        std::atomic<bool> write_request;  
    };

}  // end namespace fb

#endif  // SpinRWMutex_h__
