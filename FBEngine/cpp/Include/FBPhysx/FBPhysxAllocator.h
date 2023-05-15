#ifndef FBPhysxAllocator_H
#define FBPhysxAllocator_H

#include "foundation/PxAllocatorCallback.h"
//#include "common/PxPhysXCommon.h"

namespace fb
{
    namespace physics
    {
        class PhysxAllocator : public physx::PxAllocatorCallback
        {
        public:
            PhysxAllocator();
            ~PhysxAllocator() override;

            void *allocate( size_t size, const char *typeName, const char *filename, int line ) override;
            void deallocate( void *ptr ) override;
        };
    } // end namespace physics
}     // end namespace fb

#endif
