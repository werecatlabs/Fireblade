#ifndef PhysxMemoryStream_h__
#define PhysxMemoryStream_h__

#include "FBPhysx/FBPhysxPrerequisites.h"
#include "foundation/PxIO.h"
#include <PxPhysicsAPI.h>

namespace fb
{
    namespace physics
    {
        class MemoryInputData : public physx::PxInputData
        {
        public:
            MemoryInputData( physx::PxU8 *data, u32 length );

            u32 read( void *dest, u32 count ) override;
            u32 getLength() const override;
            void seek( u32 pos ) override;
            u32 tell() const override;

        private:
            u32 mSize;
            const physx::PxU8 *mData;
            u32 mPos;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // PhysxMemoryStream_h__
