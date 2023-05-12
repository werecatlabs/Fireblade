namespace fb
{
/*
    template <typename Container, typename Value>
    template <typename C, typename T>
    ConcurrentArrayIterator<C, T> ConcurrentArrayIterator<Container, Value>::operator+(
        difference_type offset,
        const ConcurrentArrayIterator<C, T> &v )
    {
        return ConcurrentArrayIterator<Container, Value>();
    }

    template <typename Container, typename Value>
    template <typename C, typename T, typename U>
    bool ConcurrentArrayIterator<Container, Value>::operator==( const ConcurrentArrayIterator<Container, Value> &i,
                            const ConcurrentArrayIterator<Container, Value> &j )
    {
        return false;
    }

    template <typename Container, typename Value>
    template <typename C, typename T, typename U>
    bool ConcurrentArrayIterator<Container, Value>::operator<( const ConcurrentArrayIterator<Container, Value> &i,
                           const ConcurrentArrayIterator<Container, Value> &j )
    {
        return false;
    }

    template <typename Container, typename Value>
    template <typename C, typename T, typename U>
    ConcurrentArrayIterator<Container, Value>::difference_type operator-( const ConcurrentArrayIterator<Container, Value> &i,
                                      const ConcurrentArrayIterator<Container, Value> &j )
    {
        return 0;
    }
    */

    template <typename Container, typename Value>
    ConcurrentArrayIterator<Container, Value>::ConcurrentArrayIterator()
        : my_vector( NULL ), my_index( ~size_t( 0 ) ), my_item( NULL )
    {
    }

    template <typename Container, typename Value>
    ConcurrentArrayIterator<Container, Value>::ConcurrentArrayIterator( const ConcurrentArrayIterator &other ) :
        my_vector( other.my_vector ),
        my_index( other.my_index ),
        my_item( other.my_item )
    {
    }

    template <typename Container, typename Value>
    ConcurrentArrayIterator<Container, Value>::ConcurrentArrayIterator( const Container &vector,
                                                                        size_t index,
                                                                        void *ptr /*= 0*/ ) :
        my_vector( const_cast<Container *>( &vector ) ),
        my_index( index ),
        my_item( static_cast<Value *>( ptr ) )
    {
    }

    template <typename Container, typename Value>
    ConcurrentArrayIterator<Container, Value>::~ConcurrentArrayIterator()
    {
    }

}  // namespace fb
