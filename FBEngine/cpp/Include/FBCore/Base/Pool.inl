namespace fb
{
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
        for(auto &element : m_elements)
        {
            element->unload( nullptr );
            element = nullptr;
        }

        m_elements.clear();
    }

    template <class T, class A>
    RawPtr<T> Pool<T, A>::malloc()
    {
        if( m_freeElements.empty() )
        {
            auto nextSize = getNextSize();

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

}  // end namespace fb
