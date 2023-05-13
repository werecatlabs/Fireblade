#include <FBFileSystem/FBFileSystemPCH.h>
#include <FBFileSystem/CMemoryFile.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, CMemoryFile, DataStream );

    CMemoryFile::CMemoryFile( void *memory, long len, const String &fileName, bool d ) :
        m_buffer( static_cast<u8 *>( memory ) ),
        m_position( nullptr ),
        m_length( len ),
        m_freeMemory( d )
    {
        m_access = static_cast<u16>( AccessMode::Read );
        m_position = m_buffer;
        m_end = &m_buffer[len - 1];
    }

    CMemoryFile::~CMemoryFile()
    {
        if( m_freeMemory )
        {
            delete[](c8 *)m_buffer;
        }
    }

    size_t CMemoryFile::read( void *buffer, size_t sizeToRead )
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

    size_t CMemoryFile::write( const void *buffer, size_t sizeToWrite )
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

    bool CMemoryFile::seek( long finalPos )
    {
        FB_ASSERT( m_position + finalPos < m_end );
        m_position = m_buffer + finalPos;
        return true;
    }

    size_t CMemoryFile::size() const
    {
        return m_length;
    }

    size_t CMemoryFile::tell() const
    {
        return m_position - m_buffer;
    }

    void *CMemoryFile::getBuffer() const
    {
        return m_buffer;
    }

    void CMemoryFile::setBuffer( void *val )
    {
        m_buffer = static_cast<u8 *>( val );
    }

    long CMemoryFile::getLength() const
    {
        return m_length;
    }

    void CMemoryFile::setLength( long val )
    {
        m_length = val;
    }

    bool CMemoryFile::getFreeMemory() const
    {
        return m_freeMemory;
    }

    void CMemoryFile::setFreeMemory( bool freeMemory )
    {
        m_freeMemory = freeMemory;
    }

    void *CMemoryFile::getData() const
    {
        return m_buffer;
    }

    const c8 *CMemoryFile::getCharPtr() const
    {
        return (char *)m_buffer;
    }

    bool CMemoryFile::eof( void ) const
    {
        return m_position >= m_end;
    }

    bool CMemoryFile::isOpen() const
    {
        return true;
    }

    void CMemoryFile::close()
    {
    }

    size_t CMemoryFile::skipLine( const String &delim )
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

    void CMemoryFile::skip( long count )
    {
        auto newpos = static_cast<size_t>( ( m_position - m_buffer ) + count );
        FB_ASSERT( m_buffer + newpos <= m_end );

        m_position = m_buffer + newpos;
    }
}  // end namespace fb
