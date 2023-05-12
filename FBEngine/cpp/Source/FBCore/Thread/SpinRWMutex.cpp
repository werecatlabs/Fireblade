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
        while( write_request.load( std::memory_order_acquire ) )
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
        write_request.store( true, std::memory_order_release );

        while( readers.load( std::memory_order_acquire ) != 0 )
        {
            std::this_thread::yield();
        }

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
