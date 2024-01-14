#ifndef FB_POOL_H
#define FB_POOL_H

#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Core/PoolData.h>
#include <FBCore/Memory/PoolAllocatorAligned.h>
#include <FBCore/System/RttiClass.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    /** Pool class to allocate memory. */
    template <class T, class A = std::allocator<T>>
    class Pool : public ISharedObject
    {
    public:
        /** Default constructor. */
        Pool();

        /** Constructor with grow size. */
        explicit Pool( size_t nextSize );

        /** Destructor. */
        ~Pool() override;

        /** Allocates pool data. */
        void allocateData();

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

        /** Gets the size. */
        u32 getSize() const;

        FB_CLASS_REGISTER_TEMPLATE_PAIR_DECL( Pool, T, A );

    private:
        /// The Array storing the data.
        ConcurrentArray<SmartPtr<PoolData<T, A>>, A> m_elements;

        /// The free elements.
        ConcurrentQueue<RawPtr<T>> m_freeElements;

        /// Num of elements.
        size_t m_nextSize = 32;
    };

    FB_CLASS_REGISTER_DERIVED_TEMPLATE_PAIR( fb, Pool, T, A, ISharedObject );

    template <class T, class A>
    Pool<T, A>::Pool()
    {
    }

    template <class T, class A>
    Pool<T, A>::Pool( size_t nextSize ) : m_nextSize( nextSize )
    {
    }

    template <class T, class A>
    Pool<T, A>::~Pool()
    {
        for( auto &element : m_elements )
        {
            element->unload( nullptr );
        }

        m_elements.clear();
    }

    template <class T, class A>
    void Pool<T, A>::allocateData()
    {
        auto nextSize = getNextSize();
        if( nextSize > 0 )
        {
            auto element = fb::make_ptr<PoolData<T, A>>();
            element->setNumElements( nextSize );
            element->load( nullptr );

            m_elements.push_back( element );

            auto numAllocated = element->getNumAllocated();
            for( size_t i = 0; i < numAllocated; ++i )
            {
                T *ptr = &( ( *element )[i] );
                m_freeElements.push( ptr );
            }
        }
    }

    template <class T, class A>
    RawPtr<T> Pool<T, A>::malloc()
    {
        if( m_freeElements.empty() )
        {
            allocateData();
        }

        if( !m_freeElements.empty() )
        {
            RawPtr<T> ptr;
            while( !m_freeElements.try_pop( ptr ) )
            {
                Thread::yield();
            }

            return ptr;
        }

        return nullptr;
    }

    template <class T, class A>
    void Pool<T, A>::free( T *ptr )
    {
        m_freeElements.push( ptr );
    }

    template <class T, class A>
    void Pool<T, A>::clear()
    {
        while( !m_freeElements.empty() )
        {
            RawPtr<T> ptr;
            if( m_freeElements.try_pop( ptr ) )
            {
                Thread::yield();
            }
        }

        for( size_t i = 0; i < m_elements.size(); ++i )
        {
            m_elements[i] = nullptr;
        }
    }

    template <class T, class A>
    size_t Pool<T, A>::getNextSize() const
    {
        return m_nextSize;
    }

    template <class T, class A>
    void Pool<T, A>::setNextSize( size_t nextSize )
    {
        m_nextSize = nextSize;
    }

    template <class T, class A>
    u32 Pool<T, A>::getSize() const
    {
        return static_cast<u32>( m_elements.size() );
    }

}  // namespace fb

#endif
