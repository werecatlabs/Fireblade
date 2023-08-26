#include <FBCore/FBCorePCH.h>
#include <FBCore/Thread/SpinRWMutex.h>
#include <thread>

namespace fb
{

    SpinRWMutex::SpinRWMutex() : readers( 0 ), writers( 0 ), write_request( false )
    {
    }

    void SpinRWMutex::lock_read()
    {
        // Spin until there are no active writers
        while( writers.load( std::memory_order_acquire ) > 0 )
        {
            std::this_thread::yield();
        }

        readers.fetch_add( 1, std::memory_order_acquire );
    }

    void SpinRWMutex::unlock_read()
    {
        readers.fetch_sub( 1, std::memory_order_release );
    }

    void SpinRWMutex::lock_write()
    {
        // Set the write request flag and acquire the lock
        while( write_request.exchange( true, std::memory_order_acquire ) )
        {
            // Spin while the write request flag was already set
            std::this_thread::yield();
        }

        // Wait for active readers to finish
        while( readers.load( std::memory_order_acquire ) != 0 )
        {
            std::this_thread::yield();
        }

        // Increment the writers count
        writers.fetch_add( 1, std::memory_order_acquire );
        write_request.store( false, std::memory_order_release );
    }

    void SpinRWMutex::unlock_write()
    {
        writers.fetch_sub( 1, std::memory_order_release );
    }

    SpinRWMutex::ScopedLock::ScopedLock( SpinRWMutex &m, bool write /*= true */ ) :
        m_mutex( m ),
        m_write( write )
    {
        if( m_write )
        {
            m_mutex.lock_write();
        }
        else
        {
            m_mutex.lock_read();
        }
    }

    SpinRWMutex::ScopedLock::~ScopedLock()
    {
        if( m_write )
        {
            m_mutex.unlock_write();
        }
        else
        {
            m_mutex.unlock_read();
        }
    }

    SpinRWMutex::ScopedLock::operator bool()
    {
        return true;
    }

}  // namespace fb
