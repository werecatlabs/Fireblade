#ifndef FBPhysxPoolAllocator_h__
#define FBPhysxPoolAllocator_h__

#include "foundation/PxAllocatorCallback.h"
#include <list>
#include <mutex>
#include <vector>

namespace fb
{
    namespace physics
    {
        class PhysxPoolAllocator : public physx::PxAllocatorCallback
        {
        public:
            PhysxPoolAllocator();
            PhysxPoolAllocator( size_t poolSize );
            PhysxPoolAllocator( size_t poolSize, size_t alignment );
            ~PhysxPoolAllocator() override;

            void *allocate( size_t size, const char *typeName, const char *filename, int line ) override;
            void deallocate( void *ptr ) override;

        private:
            std::vector<char> mPool;
            size_t mPoolIndex = 0;
            size_t mAlignment = 16;
            std::list<std::pair<size_t, size_t>> mFreeBlocks;
            std::mutex mMutex;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // FBPhysxPoolAllocator_h__
