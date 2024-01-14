#ifndef SharedMutex_h__
#define SharedMutex_h__

#include <FBCore/FBCoreTypes.h>
#include <iostream>
#include <mutex>
#include <condition_variable>

namespace fb
{

    /** A shared mutex that allows multiple readers or one writer. */
    class SharedMutex
    {
    public:
        class ScopedLock
        {
        public:
            ScopedLock( SharedMutex &m, bool write = true );

            ~ScopedLock();

        protected:
            SharedMutex &m_mutex; 
        };

        void lock_shared();

        void unlock_shared();

        void lock();

        void unlock();

    private:
        std::mutex mutex_;
        std::condition_variable readCondition_;
        std::condition_variable writeCondition_;
        bool writeLocked_ = false;
        s32 readCount_ = 0;
    };

}  // namespace fb

#endif  // SharedMutex_h__
