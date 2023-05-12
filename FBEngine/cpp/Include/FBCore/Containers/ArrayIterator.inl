namespace fb
{

    template <typename ContainerType, typename ValueType>
    ArrayIterator<ContainerType, ValueType>::ArrayIterator() :
        my_vector( NULL ),
        my_index( ~size_t( 0 ) ),
        my_item( NULL )
    {
    }

    template <typename ContainerType, typename ValueType>
    ArrayIterator<ContainerType, ValueType>::ArrayIterator( const ArrayIterator &other ) :
        my_vector( other.my_vector ),
        my_index( other.my_index ),
        my_item( other.my_item )
    {
    }

    template <typename ContainerType, typename ValueType>
    template <typename CType, typename ConstValue>
    ArrayIterator<ContainerType, ValueType>::ArrayIterator(
        const ArrayIterator<CType, ConstValue> &other )
    {
    }

    template <typename ContainerType, typename ValueType>
    ArrayIterator<ContainerType, ValueType>::ArrayIterator( const ContainerType &vector, size_t index,
                                                            void *ptr /*= 0*/ ) :
        my_vector( const_cast<ContainerType *>( &vector ) ),
        my_index( index ),
        my_item( static_cast<value_type *>( ptr ) )
    {
    }

    template <typename ContainerType, typename ValueType>
    ArrayIterator<ContainerType, ValueType>::~ArrayIterator()
    {
    }

    /*
    template <typename ContainerType, typename ValueType>
    ArrayIterator<ContainerType, ValueType> ArrayIterator<ContainerType, ValueType>::operator--( int )
    {
        auto result = *this;
        operator--();
        return *result;
    }

    template <typename ContainerType, typename ValueType>
    ArrayIterator<ContainerType, ValueType>::difference_type ArrayIterator<ContainerType, ValueType>::operator-( ArrayIterator it ) const
    {
        return my_index - it.my_index;
    }
    */

    template <typename ContainerType, typename ValueType>
    typename ArrayIterator<ContainerType, ValueType>::value_type &
    ArrayIterator<ContainerType, ValueType>::operator*()
    {
        if( !my_item )
        {
            if( my_index < my_vector->size() )
            {
                auto &val = ( *my_vector )[my_index];
                my_item = &val;
            }
        }

        return *my_item;
    }

    template <typename ContainerType, typename ValueType>
    typename ArrayIterator<ContainerType, ValueType>::value_type &
    ArrayIterator<ContainerType, ValueType>::operator*() const
    {
        if( !my_item )
        {
            if( my_index < my_vector->size() )
            {
                auto &val = ( *my_vector )[my_index];
                my_item = &val;
            }
        }

        return *my_item;
    }

    template <typename ContainerType, typename ValueType>
    typename ArrayIterator<ContainerType, ValueType>::pointer
    ArrayIterator<ContainerType, ValueType>::operator->() const
    {
        if( !my_item )
        {
            if( my_index < my_vector->size() )
            {
                my_item = (value_type *)&( *my_vector )[my_index];
            }
        }

        return my_item;
    }

    template <typename ContainerType, typename ValueType>
    typename ArrayIterator<ContainerType, ValueType>::reference
    ArrayIterator<ContainerType, ValueType>::operator[](
        ArrayIterator<ContainerType, ValueType>::difference_type k ) const
    {
        return my_vector[my_index + k];
    }

}  // namespace fb
