#include <FBFileSystem/FBFileSystemPCH.h>
#include <FBFileSystem/DataStream.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, DataStream, SharedObject<IStream> );

    template <typename T>
    DataStream &DataStream::operator>>( T &val )
    {
        read( static_cast<void *>( &val ), sizeof( T ) );
        return *this;
    }

    String DataStream::getLine( bool trimAfter )
    {
        char tmpBuf[FB_STREAM_TEMP_SIZE];
        std::string retString;
        size_t readCount;
        // Keep looping while not hitting delimiter
        while( ( readCount = read( tmpBuf, FB_STREAM_TEMP_SIZE - 1 ) ) != 0 )
        {
            // Terminate string
            tmpBuf[readCount] = '\0';

            char *p = strchr( tmpBuf, '\n' );
            if( p != nullptr )
            {
                // Reposition backwards
                skip( static_cast<long>( p + 1 - tmpBuf - readCount ) );
                *p = '\0';
            }

            retString += tmpBuf;

            if( p != nullptr )
            {
                // Trim off trailing CR if this was a CR/LF entry
                if( retString.length() && retString[retString.length() - 1] == '\r' )
                {
                    retString.erase( retString.length() - 1, 1 );
                }

                // Found terminator, break out
                break;
            }
        }

        if( trimAfter )
        {
            StringUtil::trim( retString );
        }

        return retString;
    }

    bool DataStream::isOpen() const
    {
        return true;
    }

    DataStream::DataStream( const String &name, u16 accessMode /*= READ*/ ) :
        m_name( name ),
        m_size( 0 ),
        m_access( accessMode )
    {
    }

    DataStream::DataStream( u16 accessMode /*= READ*/ ) : m_size( 0 ), m_access( accessMode )
    {
    }

    DataStream::~DataStream()
    {
    }

    String DataStream::getFileName() const
    {
        return m_name;
    }

    String DataStream::getFileName( void )
    {
        return m_name;
    }

    void DataStream::setFileName( const String &fileName )
    {
        m_name = fileName;
    }

    u16 DataStream::getAccessMode() const
    {
        return m_access;
    }

    bool DataStream::isReadable() const
    {
        return ( m_access & static_cast<s32>( AccessMode::Read ) ) != 0;
    }

    bool DataStream::isWriteable() const
    {
        return ( m_access & static_cast<s32>( AccessMode::Write ) ) != 0;
    }

    size_t DataStream::write( const void *buf, size_t count )
    {
        (void)buf;
        (void)count;
        // default to not supported
        return 0;
    }

    size_t DataStream::readLine( char *buf, size_t maxCount, const String &delim )
    {
        // Deal with both Unix & Windows LFs
        bool trimCR = false;
        if( delim.find_first_of( '\n' ) != std::string::npos )
        {
            trimCR = true;
        }

        char tmpBuf[FB_STREAM_TEMP_SIZE];
        size_t chunkSize = std::min( maxCount, static_cast<size_t>( FB_STREAM_TEMP_SIZE ) - 1 );
        size_t totalCount = 0;
        size_t readCount;
        while( chunkSize && ( readCount = read( tmpBuf, chunkSize ) ) != 0 )
        {
            // Terminate
            tmpBuf[readCount] = '\0';

            // Find first delimiter
            size_t pos = strcspn( tmpBuf, delim.c_str() );

            if( pos < readCount )
            {
                // Found terminator, reposition backwards
                skip( static_cast<long>( pos + 1 - readCount ) );
            }

            // Are we genuinely copying?
            if( buf )
            {
                memcpy( buf + totalCount, tmpBuf, pos );
            }
            totalCount += pos;

            if( pos < readCount )
            {
                // Trim off trailing CR if this was a CR/LF entry
                if( trimCR && totalCount && buf && buf[totalCount - 1] == '\r' )
                {
                    --totalCount;
                }

                // Found terminator, break out
                break;
            }

            // Adjust chunkSize for next time
            chunkSize =
                std::min( maxCount - totalCount, static_cast<size_t>( FB_STREAM_TEMP_SIZE ) - 1 );
        }

        // Terminate
        if( buf )
        {
            buf[totalCount] = '\0';
        }

        return totalCount;
    }

    size_t DataStream::skipLine( const std::string &delim )
    {
        char tmpBuf[FB_STREAM_TEMP_SIZE];
        size_t total = 0;
        size_t readCount;

        // Keep looping while not hitting delimiter
        while( ( readCount = read( tmpBuf, FB_STREAM_TEMP_SIZE - 1 ) ) != 0 )
        {
            // Terminate string
            tmpBuf[readCount] = '\0';

            // Find first delimiter
            size_t pos = strcspn( tmpBuf, delim.c_str() );

            if( pos < readCount )
            {
                // Found terminator, reposition backwards
                skip( static_cast<long>( pos + 1 - readCount ) );

                total += pos + 1;

                // break out
                break;
            }

            total += readCount;
        }

        return total;
    }

    size_t DataStream::size( void ) const
    {
        return m_size;
    }

    String DataStream::getAsString( void )
    {
        try
        {
            // Read the entire buffer - ideally in one read, but if the size of
            // the buffer is unknown, do multiple fixed size reads.
            const size_t bufSize = 4096;
            char pBuf[bufSize];

            // Ensure read from begin of stream
            seek( 0 );

            std::string result;
            result.reserve( size() );

            size_t nr = 1;
            while( !eof() && nr > 0 )
            {
                nr = read( pBuf, bufSize );
                result.append( pBuf, nr );
            }

            return result;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return "";
    }

    void DataStream::setFreeMemory( [[maybe_unused]] bool freeMemory )
    {
    }

    FileInfo DataStream::getFileInfo() const
    {
        return m_fileInfo;
    }

    void DataStream::setFileInfo( const FileInfo &fileInfo )
    {
        m_fileInfo = fileInfo;
    }

}  // end namespace fb
