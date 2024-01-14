#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include "FBGraphicsOgreNext/FBOgreDataStream.h"
#include <FBCore/Interface/IO/IStream.h>

#include <utility>

namespace fb
{

    FBOgreDataStream::FBOgreDataStream( SmartPtr<IStream> stream ) :
        Ogre::DataStream(),
        m_stream( std::move( stream ) )
    {
        mSize = m_stream->size();
    }

    FBOgreDataStream::~FBOgreDataStream()
    {
        m_stream = nullptr;
    }

    auto FBOgreDataStream::read( void *buf, size_t count ) -> size_t
    {
        FB_ASSERT( m_stream );
        if( m_stream->isValid() )
        {
            return m_stream->read( buf, count );
        }

        return 0;
    }

    auto FBOgreDataStream::getLine( bool trimAfter ) -> Ogre::String
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

    auto FBOgreDataStream::tell() const -> size_t
    {
        FB_ASSERT( m_stream );
        return m_stream->tell();
    }

    auto FBOgreDataStream::eof() const -> bool
    {
        FB_ASSERT( m_stream );
        if( m_stream->isValid() )
        {
            return m_stream->eof();
        }

        return true;
    }

    void FBOgreDataStream::close()
    {
        FB_ASSERT( m_stream );
        m_stream->close();
    }

    auto FBOgreDataStream::getStream() const -> SmartPtr<IStream>
    {
        return m_stream;
    }

    void FBOgreDataStream::setStream( SmartPtr<IStream> stream )
    {
        m_stream = stream;
    }

}  // end namespace fb
