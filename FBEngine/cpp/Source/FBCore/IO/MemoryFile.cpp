#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/MemoryFile.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, MemoryFile, DataStream );

    MemoryFile::MemoryFile( void *memory, long len, const String &fileName, bool d ) :
        m_buffer( static_cast<u8 *>( memory ) ),
        m_position( nullptr ),
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
            delete[](c8 *)m_buffer;
        }
    }

    size_t MemoryFile::read( void *buffer, size_t sizeToRead )
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

    size_t MemoryFile::write( const void *buffer, size_t sizeToWrite )
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

    bool MemoryFile::seek( size_t finalPos )
    {
        FB_ASSERT( m_position + finalPos < m_end );
        m_position = m_buffer + finalPos;
        return true;
    }

    size_t MemoryFile::size() const
    {
        return m_length;
    }

    size_t MemoryFile::tell() const
    {
        return m_position - m_buffer;
    }

    void *MemoryFile::getBuffer() const
    {
        return m_buffer;
    }

    void MemoryFile::setBuffer( void *val )
    {
        m_buffer = static_cast<u8 *>( val );
    }

    long MemoryFile::getLength() const
    {
        return m_length;
    }

    void MemoryFile::setLength( long val )
    {
        m_length = val;
    }

    bool MemoryFile::getFreeMemory() const
    {
        return m_freeMemory;
    }

    void MemoryFile::setFreeMemory( bool freeMemory )
    {
        m_freeMemory = freeMemory;
    }

    void *MemoryFile::getData() const
    {
        return m_buffer;
    }

    const c8 *MemoryFile::getCharPtr() const
    {
        return (char *)m_buffer;
    }

    bool MemoryFile::eof( void ) const
    {
        return m_position >= m_end;
    }

    bool MemoryFile::isOpen() const
    {
        return true;
    }

    void MemoryFile::close()
    {
    }

    size_t MemoryFile::skipLine( const String &delim )
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
