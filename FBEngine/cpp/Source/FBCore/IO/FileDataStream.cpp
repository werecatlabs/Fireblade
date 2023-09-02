#include <FBCore/FBCorePCH.h>
#include <FBCore/IO/FileDataStream.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/LogManager.h>
#include <fstream>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, FileDataStream, DataStream );

    FileDataStream::FileDataStream( std::ifstream *s, bool freeOnClose ) :
        DataStream(),
        mInStream( s ),
        mFStreamRO( s ),
        mFStream( nullptr ),
        mFreeOnClose( freeOnClose )
    {
        // calculate the size
        mInStream->seekg( 0, std::ios_base::end );
        m_size = static_cast<size_t>( mInStream->tellg() );
        mInStream->seekg( 0, std::ios_base::beg );

        FB_ASSERT( size() < static_cast<size_t>( 2e+9 ) );

        determineAccess();
    }

    FileDataStream::FileDataStream( const std::string &name, std::ifstream *s, bool freeOnClose ) :
        DataStream( name ),
        mInStream( s ),
        mFStreamRO( s ),
        mFStream( nullptr ),
        mFreeOnClose( freeOnClose )
    {
        // calculate the size
        mInStream->seekg( 0, std::ios_base::end );
        m_size = static_cast<size_t>( mInStream->tellg() );
        mInStream->seekg( 0, std::ios_base::beg );

        FB_ASSERT( size() < static_cast<size_t>( 2e+9 ) );

        determineAccess();
    }

    FileDataStream::FileDataStream( const std::string &name, std::ifstream *s, size_t inSize,
                                    bool freeOnClose ) :
        DataStream( name ),
        mInStream( s ),
        mFStreamRO( s ),
        mFStream( nullptr ),
        mFreeOnClose( freeOnClose )
    {
        // Size is passed in
        m_size = inSize;

        FB_ASSERT( size() < static_cast<size_t>( 2e+9 ) );

        determineAccess();
    }

    FileDataStream::FileDataStream( std::fstream *s, bool freeOnClose ) :
        DataStream( false ),
        mInStream( s ),
        mFStreamRO( nullptr ),
        mFStream( s ),
        mFreeOnClose( freeOnClose )
    {
        // writeable!
        // calculate the size
        mInStream->seekg( 0, std::ios_base::end );
        m_size = static_cast<size_t>( mInStream->tellg() );
        mInStream->seekg( 0, std::ios_base::beg );

        FB_ASSERT( size() < static_cast<size_t>( 2e+9 ) );

        determineAccess();
    }

    FileDataStream::FileDataStream( const std::string &name, std::fstream *s, bool freeOnClose ) :
        DataStream( name, false ),
        mInStream( s ),
        mFStreamRO( nullptr ),
        mFStream( s ),
        mFreeOnClose( freeOnClose )
    {
        // writeable!
        // calculate the size
        mInStream->seekg( 0, std::ios_base::end );
        m_size = static_cast<size_t>( mInStream->tellg() );
        mInStream->seekg( 0, std::ios_base::beg );

        FB_ASSERT( size() < static_cast<size_t>( 2e+9 ) );

        determineAccess();
    }

    FileDataStream::FileDataStream( const std::string &name, std::fstream *s, size_t inSize,
                                    bool freeOnClose ) :
        DataStream( name, false ),
        mInStream( s ),
        mFStreamRO( nullptr ),
        mFStream( s ),
        mFreeOnClose( freeOnClose )
    {
        // writeable!
        // Size is passed in
        m_size = inSize;

        FB_ASSERT( size() < static_cast<size_t>( 2e+9 ) );

        determineAccess();
    }

    void FileDataStream::determineAccess()
    {
        m_access = 0;

        if( mInStream )
        {
            m_access |= static_cast<u16>( AccessMode::Read );
        }

        if( mFStream )
        {
            m_access |= static_cast<u16>( AccessMode::Write );
        }
    }

    FileDataStream::~FileDataStream()
    {
        close();
    }

    size_t FileDataStream::read( void *buf, size_t count )
    {
        mInStream->read( static_cast<char *>( buf ), static_cast<std::streamsize>( count ) );
        return static_cast<size_t>( mInStream->gcount() );
    }

    size_t FileDataStream::write( const void *buf, size_t count )
    {
        size_t written = 0;
        if( isWriteable() && mFStream )
        {
            mFStream->write( static_cast<const char *>( buf ), static_cast<std::streamsize>( count ) );
            written = count;
        }
        return written;
    }

    size_t FileDataStream::readLine( char *buf, size_t maxCount, const std::string &delim )
    {
        if( delim.empty() )
        {
            throw Exception( "No delimiter provided FileStreamDataStream::readLine" );
        }

        if( delim.size() > 1 )
        {
            FB_LOG_ERROR( "WARNING: FileStreamDataStream::readLine - using only first delimeter" );
        }

        // Deal with both Unix & Windows LFs
        bool trimCR = false;
        if( delim.at( 0 ) == '\n' )
        {
            trimCR = true;
        }
        // maxCount + 1 since count excludes terminator in getline
        mInStream->getline( buf, static_cast<std::streamsize>( maxCount + 1 ), delim.at( 0 ) );
        size_t ret = static_cast<size_t>( mInStream->gcount() );
        // three options
        // 1) we had an eof before we read a whole line
        // 2) we ran out of buffer space
        // 3) we read a whole line - in this case the delim character is taken from the stream but not
        // written in the buffer so the read data is of length ret-1 and thus ends at index ret-2 in all
        // cases the buffer will be null terminated for us

        if( mInStream->eof() )
        {
            // no problem
        }
        else if( mInStream->fail() )
        {
            // Did we fail because of maxCount hit? No - no terminating character
            // in included in the count in this case
            if( ret == maxCount )
            {
                // clear failbit for next time
                mInStream->clear();
            }
            else
            {
                throw Exception( "Streaming error occurred FileStreamDataStream::readLine" );
            }
        }
        else
        {
            // we need to adjust ret because we want to use it as a
            // pointer to the terminating null character and it is
            // currently the length of the data read from the stream
            // i.e. 1 more than the length of the data in the buffer and
            // hence 1 more than the _index_ of the NULL character
            --ret;
        }

        // trim off CR if we found CR/LF
        if( trimCR && buf[ret - 1] == '\r' )
        {
            --ret;
            buf[ret] = '\0';
        }
        return ret;
    }

    void FileDataStream::skip( long count )
    {
#if defined( STLPORT )
        // Workaround for STLport issues: After reached eof of file stream,
        // it's seems the stream was putted in intermediate state, and will be
        // fail if try to repositioning relative to current position.
        // Note: tellg() fail in this case too.
        if( mInStream->eof() )
        {
            mInStream->clear();
            // Use seek relative to either begin or end to bring the stream
            // back to normal state.
            mInStream->seekg( 0, std::ios::end );
        }
#endif
        mInStream->clear();  // Clear fail status in case eof was set
        mInStream->seekg( count, std::ios::cur );
    }

    bool FileDataStream::seek( long pos )
    {
        mInStream->clear();  // Clear fail status in case eof was set
        mInStream->seekg( pos, std::ios::beg );
        return true;
    }

    size_t FileDataStream::tell( void ) const
    {
        mInStream->clear();  // Clear fail status in case eof was set
        return mInStream->tellg();
    }

    bool FileDataStream::eof( void ) const
    {
        return mInStream->eof();
    }

    void FileDataStream::close( void )
    {
        if( mInStream )
        {
            // Unfortunately, there is no file-specific shared class hierarchy between fstream and
            // ifstream (!!)
            if( mFStreamRO )
                mFStreamRO->close();
            if( mFStream )
            {
                mFStream->flush();
                mFStream->close();
            }

            if( mFreeOnClose )
            {
                // delete the stream too
                delete mFStreamRO;
                delete mFStream;

                mInStream = nullptr;
                mFStreamRO = nullptr;
                mFStream = nullptr;
            }
        }
    }

    bool FileDataStream::isOpen() const
    {
        if( mInStream )
        {
            auto s = static_cast<std::ifstream *>( mInStream );
            return s->is_open();
        }

        if( mFStreamRO )
        {
            return mFStreamRO->is_open();
        }

        if( mFStream )
        {
            return mFStream->is_open();
        }

        return false;
    }

    bool FileDataStream::isValid() const
    {
        return isOpen() && ( size() > 0 ) && ( size() < static_cast<size_t>( 2e+9 ) );
    }

    std::istream *FileDataStream::getInStream() const
    {
        return mInStream;
    }

    void FileDataStream::setInStream( std::istream *stream )
    {
        mInStream = stream;
        mFStreamRO = (std::ifstream *)stream;

        mInStream->seekg( 0, std::ios_base::end );
        m_size = static_cast<size_t>( mInStream->tellg() );
        mInStream->seekg( 0, std::ios_base::beg );

        FB_ASSERT( size() < static_cast<size_t>( 2e+9 ) );

        determineAccess();
    }

    std::ifstream *FileDataStream::getFStreamRO() const
    {
        return mFStreamRO;
    }

    void FileDataStream::setFStreamRO( std::ifstream *stream )
    {
        mInStream = stream;
        mFStreamRO = stream;

        mInStream->seekg( 0, std::ios_base::end );
        m_size = static_cast<size_t>( mInStream->tellg() );
        mInStream->seekg( 0, std::ios_base::beg );

        FB_ASSERT( size() < static_cast<size_t>( 2e+9 ) );

        determineAccess();
    }

    std::fstream *FileDataStream::getFStream() const
    {
        return mFStream;
    }

    void FileDataStream::setFStream( std::fstream *stream )
    {
        mInStream = stream;
        mFStream = stream;

        // calculate the size
        mInStream->seekg( 0, std::ios_base::end );
        m_size = static_cast<size_t>( mInStream->tellg() );
        mInStream->seekg( 0, std::ios_base::beg );

        FB_ASSERT( size() < static_cast<size_t>( 2e+9 ) );

        determineAccess();
    }

}  // end namespace fb
