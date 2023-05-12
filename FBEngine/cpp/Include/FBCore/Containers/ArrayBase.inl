namespace fb
{

    template <class T, class A>
    ArrayBase<T, A>::ArrayBase()
    {
    }

    template <class T, class A>
    ArrayBase<T, A>::ArrayBase( size_t capacity )
    {
        reserve( capacity );
    }

    template <class T, class A>
    ArrayBase<T, A>::ArrayBase( size_t n, const T &other )
    {
        resize( n );

        for( size_t i = 0; i < n; ++i )
        {
            m_array[i] = other;
        }
    }

    template <class T, class A>
    ArrayBase<T, A>::ArrayBase( std::initializer_list<value_type> il )
    {
        resize( il.size() );

        auto count = 0;
        for( auto &v : il )
        {
            m_array[count++] = v;
        }
    }

    template <class T, class A>
    ArrayBase<T, A>::ArrayBase( const ArrayBase<T, A> &other )
    {
        *this = other;
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class I>
    ArrayBase<T, A>::ArrayBase( I first, I last, const allocator_type &a /*= allocator_type()*/ )
    {
        auto count = 0;
        auto it = first;
        for( ; it != last; ++it )
        {
            //auto value = *it;
            //m_array[count++] = value;
        }
    }

    template <class T, class A>
    ArrayBase<T, A>::~ArrayBase()
    {
        if( m_array )
        {
            m_allocator.deallocate( m_array, m_capacity );
            m_array = nullptr;
        }
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::begin()
    {
        return iterator( *this, 0 );
    }

    template <class T, class A>
    typename ArrayBase<T, A>::const_iterator ArrayBase<T, A>::begin() const
    {
        return ArrayBase<T, A>::const_iterator( *this, 0 );
    }

    template <class T, class A>
    typename ArrayBase<T, A>::const_iterator ArrayBase<T, A>::cbegin() const
    {
        return ArrayBase<T, A>::const_iterator( *this, 0 );
    }

    template <class T, class A>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::end()
    {
        return ArrayBase<T, A>::iterator( *this, size() );
    }

    template <class T, class A>
    typename ArrayBase<T, A>::const_iterator ArrayBase<T, A>::end() const
    {
        return ArrayBase<T, A>::const_iterator( *this, size() );
    }

    template <class T, class A>
    typename ArrayBase<T, A>::const_iterator ArrayBase<T, A>::cend() const
    {
        return ArrayBase<T, A>::const_iterator( *this, size() );
    }

    template <class T, class A>
    void ArrayBase<T, A>::push_front( const T &val )
    {
        FB_ASSERT( m_array );
        m_array[0] = val;
    }

    template <class T, class A>
    void ArrayBase<T, A>::push_front( T &&val )
    {
        FB_ASSERT( m_array );
        m_array[0] = val;
    }

    template <class T, class A>
    void ArrayBase<T, A>::push_back( const T &val )
    {
        if( !m_array )
        {
            auto sz = ( capacity() + 1 ) * 2;
            reserve( sz );
        }

        FB_ASSERT( m_array );
        m_array[m_size++] = val;
    }

    template <class T, class A>
    void ArrayBase<T, A>::push_back( T &&val )
    {
        if( !m_array )
        {
            auto sz = ( capacity() + 1 ) * 2;
            reserve( sz );
        }

        FB_ASSERT( m_array );
        m_array[m_size++] = val;
    }

    template <class T, class A>
    void ArrayBase<T, A>::clear()
    {
        if( m_array )
        {
            Memory::ScalableFree( m_array );
            m_array = nullptr;
        }

        m_capacity = 0;
        m_size = 0;
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::resize( size_type n )
    {
        if( m_capacity < n )
        {
            reserve( n );
        }

        m_size = n;
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::resize( size_type n, T value )
    {
        if( m_capacity < n )
        {
            reserve( n );
        }

        m_size = n;
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::reserve( size_type n )
    {
        auto old = m_array;
        auto oldCapacity = m_capacity;
        m_array = m_allocator.allocate( n );
        memcpy( m_array, old, m_size );
        m_allocator.deallocate( old, oldCapacity );
    }

    template <class T, class A /*= std::allocator<T> */>
    ArrayBase<T, A> &ArrayBase<T, A>::operator=( const ArrayBase &other )
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
    bool ArrayBase<T, A>::operator!=( const ArrayBase &other ) const
    {
        return m_array != other.m_array;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ArrayBase<T, A>::operator<( const ArrayBase &other ) const
    {
        return m_array < other.m_array;
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::const_reverse_iterator ArrayBase<T, A>::crbegin() const
    {
        return const_reverse_iterator( end() );
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::const_reverse_iterator ArrayBase<T, A>::crend() const
    {
        return const_reverse_iterator( this->cbegin() );
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class... Args>
    void ArrayBase<T, A>::emplace_front( Args &&...val )
    {
        if( m_size == m_capacity )
        {
            reserve( m_size == 0 ? 1 : m_size * 2 );
        }

        auto front = reinterpret_cast<T *>( m_array );
        new( front - 1 ) T( std::forward<Args>( val )... );

        ++m_size;
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class... Args>
    void ArrayBase<T, A>::emplace_back( Args &&...args )
    {
        if( m_size == m_capacity )
        {
            reserve( m_capacity ? m_capacity * 2 : 1 );
        }

        m_allocator.construct( m_array + m_size, std::forward<Args>( args )... );
        ++m_size;
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::pop_front()
    {
        if( empty() )
            return;

        // Call the destructor of the first element
        std::allocator_traits<allocator_type>::destroy( m_allocator, m_array );

        // Move all elements one position to the left
        for( size_type i = 1; i < m_size; ++i )
        {
            m_allocator.construct( m_array + i - 1, std::move( m_array[i] ) );
            std::allocator_traits<allocator_type>::destroy( m_allocator, m_array + i );
        }

        // Decrement the size and update the capacity if necessary
        --m_size;
        //if( m_size < m_capacity / 2 && m_capacity > 2 * m_minCapacity )
        //    reallocate( m_capacity / 2 );
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::pop_back()
    {
        FB_ASSERT( !empty() );
        //allocator_type allocator;
        //dataAllocator_t( data ).destroy( data + m_size - 1 );
        m_size--;
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class... Args>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::emplace( const_iterator pos, Args &&...args )
    {
        const size_type index = pos - m_array;
        if( size() == capacity() )
            reserve( capacity() * 2 );
        const iterator new_pos = begin() + index;
        new( end() ) T();
        std::move_backward( new_pos, end() - 1, end() );
        ++m_size;
        *new_pos = T( std::forward<Args>( args )... );
        return new_pos;
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::swap( ArrayBase &other )
    {
        // Swap the contents of the arrays
        std::swap( m_array, other.m_array );
        std::swap( m_capacity, other.m_capacity );
        std::swap( m_size, other.m_size );
        std::swap( m_allocator, other.m_allocator );
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::size_type fb::ArrayBase<T, A>::size() const
    {
        return m_size;
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::size_type fb::ArrayBase<T, A>::capacity() const
    {
        return m_capacity;
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::size_type fb::ArrayBase<T, A>::max_size() const
    {
        return m_capacity;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ArrayBase<T, A>::empty() const
    {
        return size() == 0;
    }

    template <class T, class A /*= std::allocator<T> */>
    A ArrayBase<T, A>::get_allocator() const
    {
        return A();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::erase(
        typename ArrayBase<T, A>::const_iterator begin, typename ArrayBase<T, A>::const_iterator end )
    {
        auto i = begin.base();
        auto j = end.base();
        auto n = j - i;
        auto p = std::copy( j, cend(), i );

        for( auto q = p; q != cend(); ++q )
        {
            //std::allocator_traits::destroy( m_allocator, q );
        }

        m_size -= n;
        return begin.base();
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::erase( typename ArrayBase<T, A>::iterator begin,
                                                               typename ArrayBase<T, A>::iterator end )
    {
        if( begin == end )
            return begin;

        ptrdiff_t numToErase = end - begin;

        iterator newEnd = begin;
        for( iterator i = end; i != this->end(); ++i, ++newEnd )
        {
            *newEnd = std::move( *i );
        }

        m_size -= numToErase;

        destroy( newEnd, end );
        return begin;
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::iterator fb::ArrayBase<T, A>::erase(
        typename ArrayBase<T, A>::const_iterator it )
    {
        // Use const_cast to call the non-const version of erase
        return erase( const_cast<typename ArrayBase<T, A>::iterator>( it ) );
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::iterator fb::ArrayBase<T, A>::erase(
        typename ArrayBase<T, A>::iterator it )
    {
        /**
         * This implementation checks if the provided iterator is valid by comparing it to begin() and end(). If the iterator is invalid, it returns end(). If it is valid, it computes the index of the element to be erased and either uses std::memmove to move the subsequent elements and decrease the size, or calls the destructors, moves the subsequent elements with placement new, and finally decrements the size.
         */
        if( it < begin() || it >= end() )
        {
            return end();
        }

        size_type index = it - begin();

        if constexpr( std::is_trivially_destructible_v<T> )
        {
            std::memmove( m_array + index, m_array + index + 1, sizeof( T ) * ( m_size - index - 1 ) );
        }
        else
        {
            std::destroy_at( m_array + index );
            for( size_type i = index + 1; i < m_size; ++i )
            {
                std::destroy_at( m_array + i );
                new( m_array + i - 1 ) T( std::move( m_array[i] ) );
            }
        }

        --m_size;
        return begin() + index;
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::assign( size_type count, const T &value )
    {
        /** This implementation sets the new size of the array to count, and then fills the entire range of the array with value. If the new size is greater than the current capacity, reallocate() is called to resize the array. */
        if( count > capacity() )
        {
            resize( count );
        }

        m_size = count;
        std::fill( begin(), end(), value );
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class iter>
    void ArrayBase<T, A>::assign( iter first, iter last )
    {
        /**
         * This implementation takes a pair of iterators specifying a range of elements to copy into the array. It first calculates the number of elements in the range using std::distance, then checks if the range is longer than the current capacity of the array. If it is, the reallocate function is called to allocate a new, larger array. The current array is then cleared using clear, and the new range is copied into the array using push_back.
         */

        // Calculate the number of elements in the range
        size_type count = std::distance( first, last );

        // If the new range is longer than the current capacity, reallocate the array
        if( count > capacity() )
        {
            reallocate( count );
        }

        // Clear the current array
        clear();

        // Copy the new range into the array
        for( auto it = first; it != last; ++it )
        {
            push_back( *it );
        }
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::assign( std::initializer_list<T> ilist )
    {
        /**
         * This implementation clears the existing elements in the array and allocates a new block of memory to hold the elements from the initializer list. It then copies the elements from the initializer list to the newly allocated block using std::uninitialized_copy.
         */
        clear();

        const size_type newCapacity = ilist.size();
        if( newCapacity > 0 )
        {
            T *newData = allocate( newCapacity );
            m_capacity = newCapacity;
            m_array = newData;

            std::uninitialized_copy( ilist.begin(), ilist.end(), newData );
            m_size = newCapacity;
        }
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::insert(
        typename ArrayBase<T, A>::const_iterator pos, T &&value )
    {
        /**
         * This function inserts a single element value at the position pos in the array. The function first checks if the array has enough capacity to add a new element, and if not, it resizes the array. Then, the function moves all elements in the range [pos, end) one position to the right to make room for the new element, and constructs the new element at position pos. Finally, the function returns an iterator to the newly inserted element.
         */
        size_type index = pos - cbegin();

        if( m_size == m_capacity )
        {
            reserve( 2 * m_capacity + 1 );
        }

        // Move elements in range [pos, end) one position to the right
        for( size_type i = m_size; i > index; --i )
        {
            m_allocator.construct( m_array + i, std::move_if_noexcept( m_array[i - 1] ) );
        }

        m_allocator.construct( m_array + index, std::forward<T>( value ) );
        ++m_size;

        return iterator( m_array + index );
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::insert(
        typename ArrayBase<T, A>::const_iterator pos, const T &value )
    {
        /*This implementation inserts a new element at the given position
         *in the array, resizing the array if necessary to accommodate
         *the new element. It first calculates the index of the
         *position in the array, reserves enough space to insert the
         *new element if necessary, and then moves existing elements
         *to make room for the new element. Finally, it inserts the new
         *element at the appropriate position and increments the size of the array.*/
        auto newPos = pos - m_array;
        reserve( size() + 1 );

        // Move elements to make room for the new element
        for( size_type i = size(); i > newPos; --i )
        {
            m_array[i] = m_array[i - 1];
        }

        // Insert the new element and increment size
        m_array[newPos] = value;
        ++m_size;

        return m_array + newPos;
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::insert(
        typename ArrayBase<T, A>::const_iterator pos, std::initializer_list<T> ilist )
    {
        const size_type count = ilist.size();

        if( count == 0 )
        {
            return iterator( pos );
        }

        const size_type index = static_cast<size_type>( pos - begin() );

        const size_type newSize = m_size + count;
        const size_type newCapacity = computeCapacity( newSize );
        T *newData = allocate( newCapacity );

        // copy elements before insertion point
        uninitialized_copy_n( m_array, index, newData );

        // copy inserted elements
        uninitialized_copy_n( ilist.begin(), count, newData + index );

        // copy elements after insertion point
        uninitialized_copy_n( m_array + index, m_size - index, newData + index + count );

        deallocate( m_array, m_size );
        m_array = newData;
        m_size = newSize;
        m_capacity = newCapacity;

        return iterator( m_array + index );
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::insert(
        typename ArrayBase<T, A>::const_iterator pos, typename ArrayBase<T, A>::size_type count,
        T &value )
    {
        /** This implementation inserts count copies of the given value starting at the specified pos iterator. If the array is too small to hold the additional elements, it will be resized to accommodate them. Note that this implementation assumes that the value parameter is copyable. If this is not the case, you may need to modify the implementation to use a move constructor or a different insertion strategy.*/
        const size_t index = pos - begin();
        if( count == 0 )
        {
            return begin() + index;
        }

        if( size() + count > capacity() )
        {
            resize( size() + count );
        }

        // Move the existing elements to make room for the new ones
        for( size_t i = size(); i >= index + count; --i )
        {
            m_array[i] = m_array[i - count];
        }

        // Insert the new elements
        for( size_t i = index; i < index + count; ++i )
        {
            m_array[i] = value;
        }

        m_size += count;

        return begin() + index;
    }

    template <class T, class A /*= std::allocator<T> */>
    T *ArrayBase<T, A>::allocate( size_t newCapacity )
    {
        return m_allocator.allocate( newCapacity );
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::deallocate( T *data, size_t sz )
    {
        m_allocator.deallocate( data, sz );
    }

    template <class T, class A /*= std::allocator<T> */>
    void ArrayBase<T, A>::destructElements( T *data, size_t sz )
    {
        if( std::is_trivially_destructible<T>::value )
        {
            return;
        }

        for( size_t i = 0; i < sz; ++i )
        {
            data[i].~T();
        }
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class iter>
    typename ArrayBase<T, A>::iterator ArrayBase<T, A>::insert(
        typename ArrayBase<T, A>::const_iterator position, iter first, iter last )
    {
        /** This implementation inserts a range of elements specified by the iterators first and last into the array at the position specified by the position iterator. If the new elements will not fit in the existing capacity of the array, the array is reallocated with a larger capacity. The existing elements are moved or copied as necessary to make room for the new elements, and the new elements are inserted into the array. Finally, an iterator to the first new element is returned.
         *
         */
        size_t numElements = std::distance( first, last );
        if( numElements == 0 )
            return iterator( position );

        size_t insertPosition = position - begin();

        size_t newSize = m_size + numElements;
        if( newSize > m_capacity )
        {
            size_t newCapacity = std::max( newSize, m_capacity * 2 );
            T *newData = allocate( newCapacity );

            // Copy the elements before the insert position
            std::uninitialized_copy( m_array, m_array + insertPosition, newData );

            // Copy the new elements
            std::uninitialized_copy( first, last, newData + insertPosition );

            // Copy the elements after the insert position
            std::uninitialized_copy( m_array + insertPosition, m_array + m_size,
                                     newData + insertPosition + numElements );

            // Call destructors and free old memory
            destructElements( m_array, m_size );
            deallocate( m_array, m_capacity );

            m_array = newData;
            m_capacity = newCapacity;
        }
        else
        {
            // Move elements to make room for the new ones
            std::move_backward( m_array, m_array + m_size, m_array + newSize );

            // Copy the new elements
            std::uninitialized_copy( first, last, m_array + insertPosition );
        }

        m_size = newSize;

        return iterator( begin() + insertPosition );
    }

    template <class T, class A /*= std::allocator<T> */>
    template <class I>
    void ArrayBase<T, A>::insert( I pos, I first, I last )
    {
        auto it = first;
        for( ; it != last; ++it )
        {
            pos++ = it;
        }
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::const_reference ArrayBase<T, A>::at(
        typename ArrayBase<T, A>::size_type index ) const
    {
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::reference ArrayBase<T, A>::at( typename ArrayBase<T, A>::size_type index )
    {
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::const_reference ArrayBase<T, A>::operator[](
        typename ArrayBase<T, A>::size_type index ) const
    {
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::reference ArrayBase<T, A>::operator[](
        typename ArrayBase<T, A>::size_type index )
    {
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::const_reverse_iterator ArrayBase<T, A>::rbegin() const
    {
        return const_reverse_iterator( end() );
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::reverse_iterator ArrayBase<T, A>::rbegin()
    {
        return reverse_iterator( end() );
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::const_reverse_iterator ArrayBase<T, A>::rend() const
    {
        return const_reverse_iterator( begin() );
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::reverse_iterator ArrayBase<T, A>::rend()
    {
        return reverse_iterator( begin() );
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::const_reference ArrayBase<T, A>::front() const
    {
        FB_ASSERT( m_array );
        return m_array[0];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::reference ArrayBase<T, A>::front()
    {
        FB_ASSERT( m_array );
        return m_array[0];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::const_reference ArrayBase<T, A>::back() const
    {
        FB_ASSERT( m_array );
        auto index = size() - 1;
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    typename ArrayBase<T, A>::reference ArrayBase<T, A>::back()
    {
        FB_ASSERT( m_array );
        auto index = size() - 1;
        return m_array[index];
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ArrayBase<T, A>::operator<=( const ArrayBase<T, A> &other ) const
    {
        if( size() != other.size() )
        {
            return size() <= other.size();
        }

        for( std::size_t i = 0; i < size(); i++ )
        {
            if( m_array[i] != other.m_array[i] )
            {
                return m_array[i] <= other.m_array[i];
            }
        }

        return true;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ArrayBase<T, A>::operator>( const ArrayBase<T, A> &other ) const
    {
        return other < *this;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ArrayBase<T, A>::operator==( const ArrayBase &other ) const
    {
        // Compare sizes
        if( size() != other.size() )
        {
            return false;
        }

        // Compare elements
        for( size_type i = 0; i < size(); ++i )
        {
            if( ( *this )[i] != other[i] )
            {
                return false;
            }
        }

        return true;
    }

    template <class T, class A /*= std::allocator<T> */>
    bool ArrayBase<T, A>::operator>=( const ArrayBase &other ) const
    {
        return !( *this < other );
    }

}  // end namespace fb
