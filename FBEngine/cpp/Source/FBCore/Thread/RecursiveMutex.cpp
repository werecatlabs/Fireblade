#include <FBCore/FBCorePCH.h>
#include <FBCore/Thread/RecursiveMutex.h>

namespace fb
{

    void RecursiveMutex::lock()
    {
        std::unique_lock<std::mutex> lock( m_mutex );
        if( m_owner_thread == std::this_thread::get_id() )
        {
            ++m_count;
            return;
        }

        m_condition.wait( lock, [this] { return m_count == 0; } );
        m_owner_thread = std::this_thread::get_id();
        m_count = 1;
    }

    void RecursiveMutex::unlock()
    {
        std::unique_lock<std::mutex> lock( m_mutex );
        if( std::this_thread::get_id() == m_owner_thread )
        {
            --m_count;
            if( m_count == 0 )
            {
                m_owner_thread = std::thread::id();
                m_condition.notify_one();
            }
        }
    }

    RecursiveMutex::ScopedLock::ScopedLock( RecursiveMutex &mutex ) : m_mutex( mutex )
    {
        m_mutex.lock();
    }

    RecursiveMutex::ScopedLock::~ScopedLock()
    {
        m_mutex.unlock();
    }

    RecursiveMutex::ScopedLock::operator bool()
    {
        return true;
    }

}  // namespace fb
