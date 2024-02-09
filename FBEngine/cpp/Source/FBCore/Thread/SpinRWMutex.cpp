#include <FBCore/FBCorePCH.h>
#include <FBCore/Thread/SpinRWMutex.h>
#include <thread>

namespace fb
{
    SpinRWMutex::SpinRWMutex() : readers( 0 ), writers( 0 ), writeRequest( false )
    {
    }

    void SpinRWMutex::lockRead()
    {
        while( writers.load( std::memory_order_acquire ) > 0 )
        {
            std::this_thread::yield();
        }

        readers.fetch_add( 1, std::memory_order_acquire );
    }

    void SpinRWMutex::unlockRead()
    {
        readers.fetch_sub( 1, std::memory_order_release );
    }

    void SpinRWMutex::lockWrite()
    {
        while( writeRequest.exchange( true, std::memory_order_acquire ) )
        {
            std::this_thread::yield();
        }

        while( readers.load( std::memory_order_acquire ) != 0 )
        {
            std::this_thread::yield();
        }

        writers.fetch_add( 1, std::memory_order_acquire );
        writeRequest.store( false, std::memory_order_release );
    }

    void SpinRWMutex::unlockWrite()
    {
        writers.fetch_sub( 1, std::memory_order_release );
    }

    SpinRWMutex::ScopedLock::ScopedLock( SpinRWMutex &m, bool write /*= true*/ ) :
        m_mutex( m ),
        m_write( write )
    {
        if( m_write )
        {
            m_mutex.lockWrite();
        }
        else
        {
            m_mutex.lockRead();
        }
    }

    SpinRWMutex::ScopedLock::~ScopedLock()
    {
        if( m_write )
        {
            m_mutex.unlockWrite();
        }
        else
        {
            m_mutex.unlockRead();
        }
    }

    SpinRWMutex::ScopedLock::operator bool() const
    {
        // Return true if the lock was successfully acquired
        return m_write ? ( m_mutex.writers.load( std::memory_order_acquire ) > 0 )
                       : ( m_mutex.readers.load( std::memory_order_acquire ) > 0 );
    }
}  // namespace fb
