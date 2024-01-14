#include <FBPhysx/FBPhysxPCH.h>
#include "FBPhysx/PhysxMemoryInputStream.h"
#include "PxPhysicsAPI.h"
#include "PxExtensionsAPI.h"

namespace fb::physics
{
    ///////////////////////////////////////////////////////////////////////////////

    MemoryInputData::MemoryInputData( physx::PxU8 *data, u32 length ) :
        mSize( length ),
        mData( data ),
        mPos( 0 )
    {
    }

    auto MemoryInputData::read( void *dest, u32 count ) -> u32
    {
        u32 length = physx::PxMin<u32>( count, mSize - mPos );
        memcpy( dest, mData + mPos, length );
        mPos += length;
        return length;
    }

    auto MemoryInputData::getLength() const -> u32
    {
        return mSize;
    }

    void MemoryInputData::seek( u32 offset )
    {
        mPos = physx::PxMin<u32>( mSize, offset );
    }

    auto MemoryInputData::tell() const -> u32
    {
        return mPos;
    }
}  // namespace fb::physics
