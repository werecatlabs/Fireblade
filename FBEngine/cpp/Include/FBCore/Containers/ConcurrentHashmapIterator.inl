namespace fb
{

    template <typename Container, typename Value>
    std::pair<typename Container::key_type, Value>* ConcurrentHashmapIterator<Container, Value>::operator->()
    {
        return &m_value;
    }

    template <typename Container, typename Value>
    const std::pair<typename Container::key_type, Value>* ConcurrentHashmapIterator<Container, Value>::operator->() const
    {
        return &m_value;
    }

/*
    template <typename Container, typename Value>
    template <typename C, typename T>
    ConcurrentHashmapIterator<C, T> ConcurrentHashmapIterator<Container, Value>::operator+(
        difference_type offset,
        const ConcurrentHashmapIterator<C, T> &v )
    {
        return ConcurrentHashmapIterator<Container, Value>();
    }

    template <typename Container, typename Value>
    template <typename C, typename T, typename U>
    bool ConcurrentHashmapIterator<Container, Value>::operator==( const ConcurrentHashmapIterator<Container, Value> &i,
                            const ConcurrentHashmapIterator<Container, Value> &j )
    {
        return false;
    }

    template <typename Container, typename Value>
    template <typename C, typename T, typename U>
    bool ConcurrentHashmapIterator<Container, Value>::operator<( const ConcurrentHashmapIterator<Container, Value> &i,
                           const ConcurrentHashmapIterator<Container, Value> &j )
    {
        return false;
    }

    template <typename Container, typename Value>
    template <typename C, typename T, typename U>
    ConcurrentHashmapIterator<Container, Value>::difference_type operator-( const ConcurrentHashmapIterator<Container, Value> &i,
                                      const ConcurrentHashmapIterator<Container, Value> &j )
    {
        return 0;
    }
    */

    template <typename Container, typename Value>
    ConcurrentHashmapIterator<Container, Value>::ConcurrentHashmapIterator()
        : my_vector( NULL ), my_index( ~size_t( 0 ) ), my_item( NULL )
    {
    }

    template <typename Container, typename Value>
    ConcurrentHashmapIterator<Container, Value>::ConcurrentHashmapIterator( const ConcurrentHashmapIterator &other ) :
        my_vector( other.my_vector ),
        my_index( other.my_index ),
        my_item( other.my_item )
    {
    }

    template <typename Container, typename Value>
    ConcurrentHashmapIterator<Container, Value>::ConcurrentHashmapIterator( const Container &vector,
                                                                        size_t index,
                                                                        void *ptr /*= 0*/ ) :
        my_vector( const_cast<Container *>( &vector ) ),
        my_index( index ),
        my_item( static_cast<Value *>( ptr ) )
    {
    }

    template <typename Container, typename Value>
    ConcurrentHashmapIterator<Container, Value>::~ConcurrentHashmapIterator()
    {
    }

}  // namespace fb
