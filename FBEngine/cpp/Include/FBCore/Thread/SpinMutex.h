#ifndef SpinMutex_h__
#define SpinMutex_h__

#include <FBCore/FBCoreTypes.h>
#include <atomic>

namespace fb
{

    /**
     * A spinlock mutex implementation that uses atomic_flag to achieve synchronization.
     */
    class SpinMutex
    {
    public:
        /**
         * A lock guard that automatically locks and unlocks the SpinMutex.
         */
        class ScopedLock
        {
        public:
            /**
             * Constructs a ScopeLock and acquires the SpinMutex lock.
             *
             * @param m The SpinMutex to lock.
             */
            ScopedLock( SpinMutex &m );

            /**
             * Releases the SpinMutex lock.
             */
            ~ScopedLock();

        protected:
            SpinMutex &m_mutex; /**< The SpinMutex that is locked by the ScopeLock. */
        };

        /**
         * Locks the SpinMutex.
         */
        void lock();

        /**
         * Unlocks the SpinMutex.
         */
        void unlock();

    private:
        /**< The atomic flag used to synchronize access to the SpinMutex. */
        std::atomic_flag locked = ATOMIC_FLAG_INIT;
    };

}  // namespace fb

#endif  // SpinMutex_h__
