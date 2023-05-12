#ifndef MemoryPool_h__
#define MemoryPool_h__

#include <cstddef>
#include <iostream>

namespace fb
{

    class MemoryPool
    {
    public:
        explicit MemoryPool( std::size_t block_size, std::size_t block_count );
        ~MemoryPool();

        void *allocate();

        void deallocate( void *block );

    private:
        std::size_t block_size_;
        std::size_t block_count_;
        char *pool_;
        char *head_;
    };

}  // namespace fb

#endif  // MemoryPool_h__
