#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/MemoryFile.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, MemoryFile, DataStream );

    MemoryFile::MemoryFile( void *memory, long len, const String &fileName, bool d ) :
        m_buffer( static_cast<u8 *>( memory ) ),

        m_length( len ),
        m_freeMemory( d )
    {
        m_access = static_cast<u16>( AccessMode::Read );
        m_position = m_buffer;
        m_end = &m_buffer[len - 1];
    }

    MemoryFile::~MemoryFile()
    {
        if( m_freeMemory )
        {
            delete[] reinterpret_cast<c8 *>( m_buffer );
        }
    }

    auto MemoryFile::read( void *buffer, size_t sizeToRead ) -> size_t
    {
        size_t cnt = sizeToRead;
        // Read over end of memory?
        if( m_position + cnt > m_end )
        {
            cnt = m_end - m_position;
        }

        if( cnt == 0 )
        {
            return 0;
        }

        assert( cnt <= sizeToRead );

        memcpy( buffer, m_position, cnt );
        m_position += cnt;
        return cnt;
    }

    auto MemoryFile::write( const void *buffer, size_t sizeToWrite ) -> size_t
    {
        size_t written = 0;

        if( isWriteable() )
        {
            written = sizeToWrite;

            // we only allow writing within the extents of allocated memory
            // check for buffer overrun & disallow
            if( m_position + written > m_end )
            {
                written = m_end - m_position;
            }

            if( written == 0 )
            {
                return 0;
            }

            memcpy( m_position, buffer, written );
            m_position += written;
        }

        return written;
    }

    auto MemoryFile::seek( size_t finalPos ) -> bool
    {
        FB_ASSERT( m_position + finalPos < m_end );
        m_position = m_buffer + finalPos;
        return true;
    }

    auto MemoryFile::size() const -> size_t
    {
        return m_length;
    }

    auto MemoryFile::tell() const -> size_t
    {
        return m_position - m_buffer;
    }

    auto MemoryFile::getBuffer() const -> void *
    {
        return m_buffer;
    }

    void MemoryFile::setBuffer( void *val )
    {
        m_buffer = static_cast<u8 *>( val );
    }

    auto MemoryFile::getLength() const -> long
    {
        return m_length;
    }

    void MemoryFile::setLength( long val )
    {
        m_length = val;
    }

    auto MemoryFile::getFreeMemory() const -> bool
    {
        return m_freeMemory;
    }

    void MemoryFile::setFreeMemory( bool freeMemory )
    {
        m_freeMemory = freeMemory;
    }

    auto MemoryFile::getData() const -> void *
    {
        return m_buffer;
    }

    auto MemoryFile::getCharPtr() const -> const c8 *
    {
        return reinterpret_cast<char *>( m_buffer );
    }

    auto MemoryFile::eof() const -> bool
    {
        return m_position >= m_end;
    }

    auto MemoryFile::isOpen() const -> bool
    {
        return true;
    }

    void MemoryFile::close()
    {
    }

    auto MemoryFile::skipLine( const String &delim ) -> size_t
    {
        size_t pos = 0;

        // Make sure pos can never go past the end of the data
        while( m_position < m_end )
        {
            ++pos;
            if( delim.find( *m_position++ ) != String::npos )
            {
                // Found terminator, break out
                break;
            }
        }

        return pos;
    }

    void MemoryFile::skip( long count )
    {
        auto newpos = static_cast<size_t>( ( m_position - m_buffer ) + count );
        FB_ASSERT( m_buffer + newpos <= m_end );

        m_position = m_buffer + newpos;
    }
}  // end namespace fb
