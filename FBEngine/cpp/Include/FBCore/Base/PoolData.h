#ifndef PoolData_h__
#define PoolData_h__

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Base/Deque.h>

namespace fb
{
    /** Pool data to allocate memory. */
    template <class T, class A>
    class PoolData : public CSharedObject<ISharedObject>
    {
    public:
        /** Default constructor. */
        PoolData();

        /** Constructor with grow size. */
        PoolData( size_t numElements );

        /** Destructor. */
        ~PoolData() override;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;

        /** Sets the number of elements in the pool. */
        void setNumElements( size_t numElements );

        /** Free the element data. */
        void freeAllElements();

        /** Gets a free element. */
        T *getFreeElement();

        /** Sets a free element. */
        void setElementFree( T *element );

        /** Gets the pool data. */
        T *getData();

        /** Gets the allocated elements. */
        size_t getNumAllocated() const;

        /** Gets the number of free elements. */
        size_t getNumFree() const;

        /** Access operator. */
        T &operator[]( size_t index );

        /** Access operator. */
        const T &operator[]( size_t index ) const;

        /** Gets the allocator. */
        A &getAlloc();

        /** Gets the allocator. */
        const A &getAlloc() const;

    private:
        /// The array of data.
        T *m_data = nullptr;

        /// The number of elements allocated in the array.
        size_t m_numAllocated = 0;

        /// The free elements.
        std::deque<T *> m_freeElements;

        // The allocator.
        A m_alloc;
    };
}  // namespace fb

#include <FBCore/Base/PoolData.inl>

#endif  // PoolData_h__
