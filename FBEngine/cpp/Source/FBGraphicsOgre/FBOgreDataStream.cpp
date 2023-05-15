#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/FBOgreDataStream.h>
#include <FBCore/Interface/IO/IStream.h>

namespace fb
{

    FBOgreDataStream::FBOgreDataStream( SmartPtr<IStream> stream ) : DataStream(), m_stream( stream )
    {
        mSize = m_stream->size();
    }

    FBOgreDataStream::~FBOgreDataStream()
    {
        m_stream = nullptr;
    }

    size_t FBOgreDataStream::read( void *buf, size_t count )
    {
        FB_ASSERT( m_stream );
        if( m_stream->isValid() )
        {
            return m_stream->read( buf, count );
        }

        return 0;
    }

    Ogre::String FBOgreDataStream::getLine( bool trimAfter )
    {
        if( m_stream )
        {
            return m_stream->getLine( trimAfter );
        }

        return "";
    }

    void FBOgreDataStream::skip( long count )
    {
        FB_ASSERT( m_stream );
        m_stream->skip( count );
    }

    void FBOgreDataStream::seek( size_t pos )
    {
        FB_ASSERT( m_stream );
        m_stream->seek( pos );
    }

    size_t FBOgreDataStream::tell( void ) const
    {
        FB_ASSERT( m_stream );
        return m_stream->tell();
    }

    bool FBOgreDataStream::eof( void ) const
    {
        FB_ASSERT( m_stream );
        if( m_stream->isValid() )
        {
            return m_stream->eof();
        }

        return true;
    }

    void FBOgreDataStream::close( void )
    {
        FB_ASSERT( m_stream );
        m_stream->close();
    }

    SmartPtr<IStream> FBOgreDataStream::getStream() const
    {
        return m_stream;
    }

    void FBOgreDataStream::setStream( SmartPtr<IStream> stream )
    {
        m_stream = stream;
    }
}  // end namespace fb
