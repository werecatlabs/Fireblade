#include <FBCore/FBCorePCH.h>
#include <FBCore/Thread/SharedMutex.h>

namespace fb
{

    void SharedMutex::lock_shared()
    {
        std::unique_lock<std::mutex> lock( mutex_ );
        while( writeLocked_ )
        {
            readCondition_.wait( lock );
        }
        ++readCount_;
    }

    void SharedMutex::unlock_shared()
    {
        std::lock_guard<std::mutex> lock( mutex_ );
        --readCount_;
        if( readCount_ == 0 )
        {
            writeCondition_.notify_one();
        }
    }

    void SharedMutex::lock()
    {
        std::unique_lock<std::mutex> lock( mutex_ );
        while( writeLocked_ || readCount_ > 0 )
        {
            writeCondition_.wait( lock );
        }
        writeLocked_ = true;
    }

    void SharedMutex::unlock()
    {
        std::lock_guard<std::mutex> lock( mutex_ );
        writeLocked_ = false;
        readCondition_.notify_all();
        writeCondition_.notify_one();
    }

    SharedMutex::ScopedLock::ScopedLock( SharedMutex &m, bool write ) : m_mutex( m )
    {
        if( write )
        {
            m_mutex.lock();
        }
        else
        {
            m_mutex.lock_shared();
        }
    }

    SharedMutex::ScopedLock::~ScopedLock()
    {
        if( m_mutex.writeLocked_ )
        {
            m_mutex.unlock();
        }
        else
        {
            m_mutex.unlock_shared();
        }
    }

}  // namespace fb
