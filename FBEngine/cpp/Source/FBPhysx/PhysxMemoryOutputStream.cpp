#include <FBPhysx/FBPhysxPCH.h>
#include "FBPhysx/PhysxMemoryOutputStream.h"
#include "PxPhysicsAPI.h"
#include "PxExtensionsAPI.h"

namespace fb
{
    namespace physics
    {
        MemoryOutputStream::MemoryOutputStream() :
            mData( nullptr ),
            mSize( 0 ),
            mCapacity( 0 )
        {
        }

        MemoryOutputStream::~MemoryOutputStream()
        {
            if(mData)
                delete[] mData;
        }

        u32 MemoryOutputStream::write( const void *src, u32 size )
        {
            u32 expectedSize = mSize + size;
            if(expectedSize > mCapacity)
            {
                mCapacity = expectedSize + 4096;

                auto newData = new physx::PxU8[mCapacity];
                PX_ASSERT( newData != NULL );

                if(newData)
                {
                    memcpy( newData, mData, mSize );
                    delete[] mData;
                }
                mData = newData;
            }
            memcpy( mData + mSize, src, size );
            mSize += size;
            return size;
        }

        u32 MemoryOutputStream::getSize() const
        {
            return mSize;
        }

        physx::PxU8 *MemoryOutputStream::getData() const
        {
            return mData;
        }
    } // end namespace physics
}     // end namespace fb
