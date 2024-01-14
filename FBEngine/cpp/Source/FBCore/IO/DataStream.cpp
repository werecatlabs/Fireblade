#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/DataStream.h>
#include <FBCore/FBCore.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, DataStream, IStream );

    template <typename T>
    auto DataStream::operator>>( T &val ) -> DataStream &
    {
        read( static_cast<void *>( &val ), sizeof( T ) );
        return *this;
    }

    auto DataStream::getLine( bool trimAfter ) -> String
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

    auto DataStream::isOpen() const -> bool
    {
        return true;
    }

    DataStream::DataStream( const String &name, u16 accessMode /*= READ*/ ) :
        m_name( name ),

        m_access( accessMode )
    {
    }

    DataStream::DataStream( u16 accessMode /*= READ*/ ) : m_access( accessMode )
    {
    }

    DataStream::~DataStream() = default;

    auto DataStream::getFileName() const -> String
    {
        return m_name;
    }

    auto DataStream::getFileName() -> String
    {
        return m_name;
    }

    void DataStream::setFileName( const String &fileName )
    {
        m_name = fileName;
    }

    auto DataStream::getAccessMode() const -> u16
    {
        return m_access;
    }

    auto DataStream::isReadable() const -> bool
    {
        return ( m_access & static_cast<s32>( AccessMode::Read ) ) != 0;
    }

    auto DataStream::isWriteable() const -> bool
    {
        return ( m_access & static_cast<s32>( AccessMode::Write ) ) != 0;
    }

    auto DataStream::write( const void *buf, size_t count ) -> size_t
    {
        (void)buf;
        (void)count;
        // default to not supported
        return 0;
    }

    auto DataStream::readLine( char *buf, size_t maxCount, const String &delim ) -> size_t
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

    auto DataStream::skipLine( const std::string &delim ) -> size_t
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

    auto DataStream::size() const -> size_t
    {
        return m_size;
    }

    auto DataStream::getAsString() -> String
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

    auto DataStream::getFileInfo() const -> FileInfo
    {
        return m_fileInfo;
    }

    void DataStream::setFileInfo( const FileInfo &fileInfo )
    {
        m_fileInfo = fileInfo;
    }

}  // end namespace fb
