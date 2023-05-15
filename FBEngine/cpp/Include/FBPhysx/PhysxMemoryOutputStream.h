#ifndef MemoryOutputStream_h__
#define MemoryOutputStream_h__

#include "FBPhysx/FBPhysxPrerequisites.h"
#include "foundation/PxIO.h"
#include <PxPhysicsAPI.h>

namespace fb
{
    namespace physics
    {
        class MemoryOutputStream : public physx::PxOutputStream
        {
        public:
            MemoryOutputStream();
            ~MemoryOutputStream() override;

            u32 write( const void *src, u32 count ) override;

            u32 getSize() const;
            physx::PxU8 *getData() const;

        private:
            physx::PxU8 *mData;
            u32 mSize;
            u32 mCapacity;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // PhysxMemoryStream_h__
