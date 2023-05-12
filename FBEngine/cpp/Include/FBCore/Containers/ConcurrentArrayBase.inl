namespace fb
{

    template <class T, class A>
    ConcurrentArrayBase<T, A>::ConcurrentArrayBase()
    {
    }

    template <class T, class A>
    ConcurrentArrayBase<T, A>::ConcurrentArrayBase( std::initializer_list<value_type> il )
    {
        resize(il.size());

        RecursiveMutex::ScopedLock lock( m_mutex );
        auto count = 0;
        for(auto& v : il)
        {
            m_array[count++] = v;
        }
    }

    template <class T, class A>
    ConcurrentArrayBase<T, A>::ConcurrentArrayBase( const ConcurrentArrayBase<T, A> &other )
    {
        *this = other;
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class I>
    ConcurrentArrayBase<T, A>::ConcurrentArrayBase( I first, I last,
                                                    const allocator_type &a /*= allocator_type()*/ )
    {
        auto count = 0;
        auto it = first;
        for( ; it != last; ++it )
        {
            m_array[count++] = ( *it );
        }
    }

    template <class T, class A>
    ConcurrentArrayBase<T, A>::~ConcurrentArrayBase()
    {
        if( m_array )
        {
            delete[] m_array;
            m_array = nullptr;
        }
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::begin()
    {
        return iterator( *this, 0 );
    }

    template <class T, class A>
    typename ConcurrentArrayBase<T, A>::const_iterator ConcurrentArrayBase<T, A>::begin() const
    {
        return ConcurrentArrayBase<T, A>::const_iterator( *this, 0 );
    }

    template <class T, class A>
    typename ConcurrentArrayBase<T, A>::const_iterator ConcurrentArrayBase<T, A>::cbegin() const
    {
        return ConcurrentArrayBase<T, A>::const_iterator( *this, 0 );
    }

    template <class T, class A>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::end()
    {
        return ConcurrentArrayBase<T, A>::iterator( *this, size() );
    }

    template <class T, class A>
    typename ConcurrentArrayBase<T, A>::const_iterator ConcurrentArrayBase<T, A>::end() const
    {
        return ConcurrentArrayBase<T, A>::const_iterator( *this, size() );
    }

    template <class T, class A>
    typename ConcurrentArrayBase<T, A>::const_iterator ConcurrentArrayBase<T, A>::cend() const
    {
        return ConcurrentArrayBase<T, A>::const_iterator( *this, size() );
    }

    template <class T, class A>
    void ConcurrentArrayBase<T, A>::push_front( const T &val )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_array[0] = val;
    }

    template <class T, class A>
    void ConcurrentArrayBase<T, A>::push_front( T &&val )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_array[0] = val;
    }

    template <class T, class A>
    void ConcurrentArrayBase<T, A>::push_back( const T &val )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_array[m_size++] = val;
    }

    template <class T, class A>
    void ConcurrentArrayBase<T, A>::push_back( T &&val )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_array[m_size++] = val;
    }

    template <class T, class A>
    void ConcurrentArrayBase<T, A>::clear()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        if (m_array)
        {
            delete[] m_array;
            m_array = nullptr;
        }

        m_capacity = 0;
        m_size = 0;
    }

    template <class T, class A /*= std::allocator<T> */>
    void ConcurrentArrayBase<T, A>::resize( size_type n )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        if( m_capacity < n )
        {
            reserve( n );
        }

        m_size = n;
    }

    template <class T, class A /*= std::allocator<T> */>
    void ConcurrentArrayBase<T, A>::reserve( size_type n )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        if( !m_array )
        {
            m_array = new T[n];
        }
    }

    template <class T, class A /*= std::allocator<T> */>
    ConcurrentArrayBase<T, A> &ConcurrentArrayBase<T, A>::operator=( const ConcurrentArrayBase &other )
    {
        auto s = other.size();
        reserve( s );

        for( size_t i = 0; i < s; ++i )
        {
            m_array[i] = other.m_array[i];
        }

        return *this;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ConcurrentArrayBase<T, A>::operator!=( const ConcurrentArrayBase &other ) const
    {
        return m_array != other.m_array;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ConcurrentArrayBase<T, A>::operator<( const ConcurrentArrayBase &other ) const
    {
        return m_array < other.m_array;
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::const_reverse_iterator ConcurrentArrayBase<T, A>::crbegin() const
    {
        return const_reverse_iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::const_reverse_iterator ConcurrentArrayBase<T, A>::crend() const
    {
        return const_reverse_iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class... Args>
    void ConcurrentArrayBase<T, A>::emplace_front( Args &&...val )
    {
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class... Args>
    void ConcurrentArrayBase<T, A>::emplace_back( Args &&... )
    {
        //FB_SPIN_LOCK_WRITE(m_mutex);
        //auto pArray = getArray();
        //pArray->emplace_back(val);
    }

    template <class T, class A /*= std::allocator<T> */>
    void ConcurrentArrayBase<T, A>::pop_front()
    {
    }

    template <class T, class A /*= std::allocator<T> */>
    void ConcurrentArrayBase<T, A>::pop_back()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        //auto pArray = getArray();
        //pArray->pop_back();
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class... Args>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::emplace( const_iterator,
                                                                                     Args &&... )
    {
        return iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    void ConcurrentArrayBase<T, A>::swap( ConcurrentArrayBase &other )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        //auto array = getArray();
        //array->swap( other );
    }

    template <class T, class A /*= std::allocator<T> */>
    void ConcurrentArrayBase<T, A>::shrink_to_fit()
    {

    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::size_type ConcurrentArrayBase<T, A>::size() const
    {
        return m_size;
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::size_type fb::ConcurrentArrayBase<T, A>::max_size() const
    {
        return m_capacity;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ConcurrentArrayBase<T, A>::empty() const
    {
        return size() == 0;
    }

    template <class T, class A /*= std::allocator<T> */>
    A ConcurrentArrayBase<T, A>::get_allocator() const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return A();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::erase(
        typename ConcurrentArrayBase<T, A>::const_iterator begin,
        typename ConcurrentArrayBase<T, A>::const_iterator end )
    {
        /*
        auto pArray = getArray();
        if( pArray )
        {
            RecursiveMutex::ScopeLock lock( m_mutex );
            auto &array = *pArray;

            auto arrayBeginIt = array.begin();
            auto arrayEndIt = array.begin();

            std::advance( arrayBeginIt, begin.my_index );
            std::advance( arrayEndIt, end.my_index );

            auto retIt = array.erase( arrayBeginIt, arrayEndIt );
        }
         */

        return end();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::erase(
        typename ConcurrentArrayBase<T, A>::iterator begin,
        typename ConcurrentArrayBase<T, A>::iterator end )
    {
        /*
        auto pArray = getArray();
        if( pArray )
        {
            RecursiveMutex::ScopeLock lock( m_mutex );
            auto &array = *pArray;

            auto arrayBeginIt = array.begin();
            auto arrayEndIt = array.begin();

            std::advance( arrayBeginIt, begin.my_index );
            std::advance( arrayEndIt, end.my_index );

            auto retIt = array.erase( arrayBeginIt, arrayEndIt );
        }
         */

        return end();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::iterator fb::ConcurrentArrayBase<T, A>::erase(
        typename ConcurrentArrayBase<T, A>::const_iterator it )
    {
        /*
        auto pArray = getArray();
        if( pArray )
        {
            RecursiveMutex::ScopeLock lock( m_mutex );
            auto &array = *pArray;

            auto arrayIt = array.begin();
            std::advance( arrayIt, it.my_index );
            auto retIt = array.erase( arrayIt );
        }
         */

        return end();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::iterator fb::ConcurrentArrayBase<T, A>::erase(
        typename ConcurrentArrayBase<T, A>::iterator it )
    {
        /*
        auto pArray = getArray();
        if( pArray )
        {
            RecursiveMutex::ScopeLock lock( m_mutex );
            auto &array = *pArray;

            auto arrayIt = array.begin();
            std::advance( arrayIt, it.my_index );
            auto retIt = array.erase( arrayIt );
        }
         */

        return end();
    }

    template <class T, class A /*= std::allocator<T> */>
    void ConcurrentArrayBase<T, A>::assign( size_type, const T & )
    {
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class iter>
    void ConcurrentArrayBase<T, A>::assign( iter, iter )
    {
    }

    template <class T, class A /*= std::allocator<T> */>
    void ConcurrentArrayBase<T, A>::assign( std::initializer_list<T> )
    {
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::insert(
        typename ConcurrentArrayBase<T, A>::const_iterator, T && )
    {
        return iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::insert(
        typename ConcurrentArrayBase<T, A>::const_iterator, const T & )
    {
        return iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::insert(
        typename ConcurrentArrayBase<T, A>::const_iterator, std::initializer_list<T> )
    {
        return iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::insert(
        typename ConcurrentArrayBase<T, A>::const_iterator,
        typename ConcurrentArrayBase<T, A>::size_type, T & )
    {
        return iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class iter>
    typename ConcurrentArrayBase<T, A>::iterator ConcurrentArrayBase<T, A>::insert(
        typename ConcurrentArrayBase<T, A>::const_iterator, iter, iter )
    {
        return iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class I>
    void ConcurrentArrayBase<T, A>::insert( I pos, I first, I last )
    {
        auto it = first;
        for( ; it != last; ++it )
        {
            pos++ = it;
        }
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::const_reference ConcurrentArrayBase<T, A>::at(
        typename ConcurrentArrayBase<T, A>::size_type index ) const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::reference ConcurrentArrayBase<T, A>::at(
        typename ConcurrentArrayBase<T, A>::size_type index )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::const_reference ConcurrentArrayBase<T, A>::operator[](
        typename ConcurrentArrayBase<T, A>::size_type index ) const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::reference ConcurrentArrayBase<T, A>::operator[](
        typename ConcurrentArrayBase<T, A>::size_type index )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::const_reverse_iterator ConcurrentArrayBase<T, A>::rbegin() const
    {
        return const_reverse_iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::reverse_iterator ConcurrentArrayBase<T, A>::rbegin()
    {
        return reverse_iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::const_reverse_iterator ConcurrentArrayBase<T, A>::rend() const
    {
        return const_reverse_iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::reverse_iterator ConcurrentArrayBase<T, A>::rend()
    {
        return reverse_iterator();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::const_reference ConcurrentArrayBase<T, A>::front() const
    {
        FB_ASSERT( m_array );
        return m_array[0];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::reference ConcurrentArrayBase<T, A>::front()
    {
        FB_ASSERT( m_array );
        return m_array[0];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::const_reference ConcurrentArrayBase<T, A>::back() const
    {
        FB_ASSERT( m_array );
        auto index = size() - 1;
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ConcurrentArrayBase<T, A>::reference ConcurrentArrayBase<T, A>::back()
    {
        FB_ASSERT( m_array );
        auto index = size() - 1;
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ConcurrentArrayBase<T, A>::operator<=( const ConcurrentArrayBase<T, A> & ) const
    {
        return false;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ConcurrentArrayBase<T, A>::operator>( const ConcurrentArrayBase<T, A> & ) const
    {
        return false;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ConcurrentArrayBase<T, A>::operator==( const ConcurrentArrayBase & ) const
    {
        return false;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ConcurrentArrayBase<T, A>::operator>=( const ConcurrentArrayBase & ) const
    {
        return false;
    }

}  // end namespace fb
