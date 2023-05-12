#include <FBCore/FBCorePCH.h>
#include <FBCore/Thread/SpinMutex.h>
#include <thread>

namespace fb
{

    void SpinMutex::lock()
    {
        while( locked.test_and_set( std::memory_order_acquire ) )
        {
            std::this_thread::yield();
        }
    }

    void SpinMutex::unlock()
    {
        locked.clear( std::memory_order_release );
    }

    SpinMutex::ScopedLock::ScopedLock( SpinMutex &m ) : m_mutex( m )
    {
        m_mutex.lock();
    }

    SpinMutex::ScopedLock::~ScopedLock()
    {
        m_mutex.unlock();
    }

}  // namespace fb
