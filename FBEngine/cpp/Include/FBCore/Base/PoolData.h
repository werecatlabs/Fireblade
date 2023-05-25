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

    template <class T, class A>
    PoolData<T, A>::PoolData() : m_data( nullptr ), m_numAllocated( 0 )
    {
        //uninitialized
    }

    template <class T, class A>
    PoolData<T, A>::PoolData( size_t numElements ) : m_numAllocated( numElements )
    {
    }

    template <class T, class A>
    PoolData<T, A>::~PoolData()
    {
        unload( nullptr );
    }

    template <class T, class A>
    void PoolData<T, A>::load( SmartPtr<ISharedObject> data )
    {
        auto numElements = getNumAllocated();
        m_data = m_alloc.allocate( numElements );
    }

    template <class T, class A>
    void PoolData<T, A>::unload( SmartPtr<ISharedObject> data )
    {
        if( m_data )
        {
            m_alloc.deallocate( m_data, m_numAllocated );
            m_data = nullptr;
        }
    }

    template <class T, class A>
    void PoolData<T, A>::setNumElements( size_t numElements )
    {
        T *old_data = m_data;

        if( numElements > 0 )
        {
            m_data = m_alloc.allocate( numElements );

            //auto numElementCopy = m_numAllocated < numElements ? m_numAllocated : numElements;

            ////copy old m_data
            //for(size_t i = 0; i < numElementCopy; ++i)
            //{
            //    m_data[i] = old_data[i];
            //}
        }
        else
        {
            m_data = nullptr;
        }

        if( old_data )
        {
            m_alloc.deallocate( old_data, m_numAllocated );
        }

        m_numAllocated = numElements;

        auto element = getData();
        for( size_t i = 0; i < m_numAllocated; ++i )
        {
            m_freeElements.push_back( element++ );
        }
    }

    template <class T, class A>
    void PoolData<T, A>::freeAllElements()
    {
        m_freeElements.clear();

        auto element = getData();
        for( size_t i = 0; i < m_numAllocated; ++i )
        {
            m_freeElements.push_back( element++ );
        }
    }

    template <class T, class A>
    T *PoolData<T, A>::getFreeElement()
    {
        if( !m_freeElements.empty() )
        {
            auto element = m_freeElements.front();
            m_freeElements.pop_front();
            return element;
        }

        return nullptr;
    }

    template <class T, class A>
    void PoolData<T, A>::setElementFree( T *element )
    {
        m_freeElements.push_back( element );
    }

    template <class T, class A>
    T *PoolData<T, A>::getData()
    {
        return m_data;
    }

    template <class T, class A>
    size_t PoolData<T, A>::getNumAllocated() const
    {
        return m_numAllocated;
    }

    template <class T, class A>
    size_t PoolData<T, A>::getNumFree() const
    {
        return m_freeElements.size();
    }

    template <class T, class A>
    T &PoolData<T, A>::operator[]( size_t index )
    {
        FB_ASSERT( index < m_numAllocated );
        return m_data[index];
    }

    template <class T, class A>
    const T &PoolData<T, A>::operator[]( size_t index ) const
    {
        FB_ASSERT( index < m_numAllocated );
        return m_data[index];
    }

    template <class T, class A>
    A &PoolData<T, A>::getAlloc()
    {
        return m_alloc;
    }

    template <class T, class A>
    const A &PoolData<T, A>::getAlloc() const
    {
        return m_alloc;
    }

}  // namespace fb

#endif  // PoolData_h__
