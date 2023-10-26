#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/OgreMemoryStream.h>
#include <tbb/scalable_allocator.h>

namespace fb
{

    Ogre::uchar *MemoryStream::getCurrentPtr( void )
    {
        return mPos;
    }

    //-----------------------------------------------------------------------
    size_t MemoryStream::read( void *buf, size_t count )
    {
        size_t cnt = count;
        // Read over end of memory?
        if( mPos + cnt > mEnd )
            cnt = mEnd - mPos;
        if( cnt == 0 )
            return 0;

        assert( cnt <= count );

        memcpy( buf, mPos, cnt );
        mPos += cnt;
        return cnt;
    }
    //---------------------------------------------------------------------
    size_t MemoryStream::write( const void *buf, size_t count )
    {
        size_t written = 0;
        if( isWriteable() )
        {
            written = count;
            // we only allow writing within the extents of allocated memory
            // check for buffer overrun & disallow
            if( mPos + written > mEnd )
                written = mEnd - mPos;
            if( written == 0 )
                return 0;

            memcpy( mPos, buf, written );
            mPos += written;
        }
        return written;
    }
    //-----------------------------------------------------------------------
    size_t MemoryStream::readLine( char *buf, size_t maxCount, const Ogre::String &delim )
    {
        // Deal with both Unix & Windows LFs
        bool trimCR = false;
        if( delim.find_first_of( '\n' ) != Ogre::String::npos )
        {
            trimCR = true;
        }

        size_t pos = 0;

        // Make sure pos can never go past the end of the data
        while( pos < maxCount && mPos < mEnd )
        {
            if( delim.find( *mPos ) != Ogre::String::npos )
            {
                // Trim off trailing CR if this was a CR/LF entry
                if( trimCR && pos && buf[pos - 1] == '\r' )
                {
                    // terminate 1 character early
                    --pos;
                }

                // Found terminator, skip and break out
                ++mPos;
                break;
            }

            buf[pos++] = *mPos++;
        }

        // terminate
        buf[pos] = '\0';

        return pos;
    }
    //-----------------------------------------------------------------------
    size_t MemoryStream::skipLine( const Ogre::String &delim )
    {
        size_t pos = 0;

        // Make sure pos can never go past the end of the data
        while( mPos < mEnd )
        {
            ++pos;
            if( delim.find( *mPos++ ) != Ogre::String::npos )
            {
                // Found terminator, break out
                break;
            }
        }

        return pos;
    }
    //-----------------------------------------------------------------------
    void MemoryStream::skip( long count )
    {
        size_t newpos = (size_t)( ( mPos - mData ) + count );
        assert( mData + newpos <= mEnd );

        mPos = mData + newpos;
    }
    //-----------------------------------------------------------------------
    void MemoryStream::seek( size_t pos )
    {
        assert( mData + pos <= mEnd );
        mPos = mData + pos;
    }
    //-----------------------------------------------------------------------
    size_t MemoryStream::tell( void ) const
    {
        // mData is start, mPos is current location
        return mPos - mData;
    }
    //-----------------------------------------------------------------------
    bool MemoryStream::eof( void ) const
    {
        return mPos >= mEnd;
    }
    //-----------------------------------------------------------------------
    void MemoryStream::close( void )
    {
        if( mData )
        {
            delete[] mData;
            mData = 0;
        }
    }

    MemoryStream::MemoryStream( void *pMem, size_t size, bool freeOnClose /*= false*/,
                                bool readOnly /*= false*/ ) :
        DataStream( static_cast<Ogre::uint16>( readOnly ? READ : ( READ | WRITE ) ) )
    {
        mData = mPos = static_cast<u8 *>( pMem );
        mSize = size;
        mEnd = mData + mSize;
        FB_ASSERT( mEnd >= mPos );
    }

    MemoryStream::~MemoryStream()
    {
        close();
    }

    Ogre::uchar *MemoryStream::getPtr( void )
    {
        return mData;
    }

}  // namespace fb
