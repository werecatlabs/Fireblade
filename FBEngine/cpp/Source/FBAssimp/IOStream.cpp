#include <FBAssimp/FBAssimpPCH.h>
#include <FBAssimp/IOStream.h>

namespace fb
{

    IOStream::IOStream( SmartPtr<IStream> _stream ) : stream( _stream )
    {
    }

    size_t IOStream::Read( void *pvBuffer, size_t pSize, size_t pCount )
    {
        size_t bytes = stream->read( pvBuffer, pSize * pCount );
        return bytes / pSize;
    }

    size_t IOStream::Tell() const
    {
        return stream->tell();
    }

    size_t IOStream::FileSize() const
    {
        return stream->size();
    }

    size_t IOStream::Write( const void *pvBuffer, size_t pSize, size_t pCount )
    {
        return 0;
    }

    aiReturn IOStream::Seek( size_t pOffset, aiOrigin pOrigin )
    {
        if( pOrigin != aiOrigin_SET )
            return AI_FAILURE;

        stream->seek( pOffset );
        return AI_SUCCESS;
    }

    void IOStream::Flush()
    {
    }

}  // namespace fb
