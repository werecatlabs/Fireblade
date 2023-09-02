#ifndef IOStream_h__
#define IOStream_h__

#include <FBAssimp/FBAssimpPrerequisites.h>
#include <FBCore/Interface/IO/IStream.h>
#include <assimp/IOStream.hpp>

namespace fb
{
    class IOStream : public Assimp::IOStream
    {
    public:
        IOStream( SmartPtr<IStream> _stream );

        size_t Read( void *pvBuffer, size_t pSize, size_t pCount );
        size_t Tell() const;
        size_t FileSize() const;

        size_t Write( const void *pvBuffer, size_t pSize, size_t pCount );
        aiReturn Seek( size_t pOffset, aiOrigin pOrigin );
        void Flush();

    protected:
        SmartPtr<IStream> stream;
    };

}  // namespace fb

#endif  // IOStream_h__
