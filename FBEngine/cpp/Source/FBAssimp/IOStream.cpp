#include <FBAssimp/FBAssimpPCH.h>
#include <FBAssimp/IOStream.h>

#include <utility>

namespace fb
{

    IOStream::IOStream( SmartPtr<IStream> _stream ) : stream( std::move( _stream ) )
    {
    }

    auto IOStream::Read( void *pvBuffer, size_t pSize, size_t pCount ) -> size_t
    {
        size_t bytes = stream->read( pvBuffer, pSize * pCount );
        return bytes / pSize;
    }

    auto IOStream::Tell() const -> size_t
    {
        return stream->tell();
    }

    auto IOStream::FileSize() const -> size_t
    {
        return stream->size();
    }

    auto IOStream::Write( const void *pvBuffer, size_t pSize, size_t pCount ) -> size_t
    {
        return 0;
    }

    auto IOStream::Seek( size_t pOffset, aiOrigin pOrigin ) -> aiReturn
    {
        if( pOrigin != aiOrigin_SET )
        {
            return AI_FAILURE;
        }

        stream->seek( pOffset );
        return AI_SUCCESS;
    }

    void IOStream::Flush()
    {
    }

}  // namespace fb
