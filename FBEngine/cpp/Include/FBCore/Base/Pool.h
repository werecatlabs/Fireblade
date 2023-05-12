#ifndef FB_POOL_H
#define FB_POOL_H

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/ConcurrentArray.h>
#include <FBCore/Base/ConcurrentQueue.h>
#include <FBCore/Memory/PoolAllocatorAligned.h>
#include <FBCore/Base/PoolData.h>

namespace fb
{
    /** Pool class to allocate memory. */
    template <class T, class A = std::allocator<T>>
    class Pool : public CSharedObject<ISharedObject>
    {
    public:
        /** Default constructor. */
        Pool();

        /** Constructor with grow size. */
        Pool( size_t nextSize );

        /** Destructor. */
        ~Pool() override;

        /** Allocates data. */
        RawPtr<T> malloc();

        /** Frees data. */
        void free( T *ptr );

        /** Clears the pool. */
        void clear();

        /** Gets the next size. */
        size_t getNextSize() const;

        /** Sets the next size. */
        void setNextSize( size_t nextSize );

    private:
        /// The Array storing the data.
        ConcurrentArray<SmartPtr<PoolData<T, A>>, A> m_elements;

        /// The free elements.
        ConcurrentQueue<RawPtr<T>> m_freeElements;

        /// Num of elements.
        size_t m_nextSize = 32;
    };
}  // namespace fb

#include <FBCore/Base/Pool.inl>

#endif
