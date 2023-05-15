#include <FBPhysx/FBPhysxPCH.h>
#include "FBPhysx/PhysxMemoryInputStream.h"
#include "PxPhysicsAPI.h"
#include "PxExtensionsAPI.h"

namespace fb
{
    namespace physics
    {
        ///////////////////////////////////////////////////////////////////////////////

        MemoryInputData::MemoryInputData( physx::PxU8 *data, u32 length ) :
            mSize( length ),
            mData( data ),
            mPos( 0 )
        {
        }

        u32 MemoryInputData::read( void *dest, u32 count )
        {
            u32 length = physx::PxMin<u32>( count, mSize - mPos );
            memcpy( dest, mData + mPos, length );
            mPos += length;
            return length;
        }

        u32 MemoryInputData::getLength() const
        {
            return mSize;
        }

        void MemoryInputData::seek( u32 offset )
        {
            mPos = physx::PxMin<u32>( mSize, offset );
        }

        u32 MemoryInputData::tell() const
        {
            return mPos;
        }
    } // end namespace physics
}     // end namespace fb
