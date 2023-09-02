#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxAllocator.h>
#include <FBCore/FBCore.h>
#include <foundation/PxAssert.h>
#include <foundation/PxErrorCallback.h>
#include <stdio.h>
#include <assert.h>



#if FB_USE_TBB
#    include <tbb/scalable_allocator.h>
#endif

namespace fb
{
    namespace physics
    {
        PhysxAllocator::PhysxAllocator()
        {
        }

        PhysxAllocator::~PhysxAllocator()
        {
        }

        void *PhysxAllocator::allocate( size_t size, const char *typeName, const char *filename,
                                        int line )
        {
            void *ptr = Memory::ScalableAlignedMalloc( size, 16 );
            if(!ptr)
            {
                std::terminate();
            }

#if FB_ENABLE_MEMORY_TRACKER
            auto &memoryTracker = MemoryTracker::get();
            memoryTracker._recordAlloc( ptr, size, 0, filename, line, typeName );
#endif

            return ptr;
        }

        void PhysxAllocator::deallocate( void *ptr )
        {
            Memory::ScalableAlignedFree( ptr );

#if FB_ENABLE_MEMORY_TRACKER
            auto &memoryTracker = MemoryTracker::get();
            memoryTracker._recordDealloc( ptr );
#endif
        }
    } // end namespace physics
}     // end namespace fb
