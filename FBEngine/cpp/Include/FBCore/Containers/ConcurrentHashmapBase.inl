namespace fb
{

    template <class Key, class T, class Compare, class A>
    ConcurrentHashmapBase<Key, T, Compare, A>::ConcurrentHashmapBase()
    {

    }

    template <class Key, class T, class Compare, class A>
    ConcurrentHashmapBase<Key, T, Compare, A>::ConcurrentHashmapBase( const ConcurrentHashmapBase<Key, T, Compare, A> &other )
    {

    }

    template <class Key, class T, class Compare, class A>
    ConcurrentHashmapBase<Key, T, Compare, A>::~ConcurrentHashmapBase()
    {

    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::iterator ConcurrentHashmapBase<Key, T, Compare, A>::begin()
    {
        return iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::const_iterator ConcurrentHashmapBase<Key, T, Compare, A>::begin() const
    {
        return const_iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::const_iterator ConcurrentHashmapBase<Key, T, Compare, A>::cbegin() const
    {
        return const_iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::iterator ConcurrentHashmapBase<Key, T, Compare, A>::end()
    {
        return iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::const_iterator ConcurrentHashmapBase<Key, T, Compare, A>::end() const
    {
        return const_iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::const_iterator ConcurrentHashmapBase<Key, T, Compare, A>::cend() const
    {
        return const_iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::reverse_iterator ConcurrentHashmapBase<Key, T, Compare, A>::rbegin()
    {
        return reverse_iterator();
    }

}  // end namespace fb


