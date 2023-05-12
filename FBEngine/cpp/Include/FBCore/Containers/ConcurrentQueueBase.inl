namespace fb
{

    template <class T, class A>
    ConcurrentQueueBase<T, A>::ConcurrentQueueBase()
    {
    }

    template <class T, class A>
    ConcurrentQueueBase<T, A>::ConcurrentQueueBase( const ConcurrentQueueBase<T, A> &other )
    {
    }

    template <class T, class A>
    ConcurrentQueueBase<T, A>::~ConcurrentQueueBase()
    {
    }

    template <class T, class A>
    void ConcurrentQueueBase<T, A>::push( const T &e )
    {
        m_queue.push_back( e );
    }

    template <class T, class A>
    bool ConcurrentQueueBase<T, A>::try_pop( T &e )
    {
        auto it = m_queue.begin();
        if( it != m_queue.end() )
        {
            m_queue.pop_front();
            e = *it;
            return true;
        }

        return false;
    }

    template <class T, class A>
    void ConcurrentQueueBase<T, A>::clear()
    {
        m_queue.clear();
    }

    template <class T, class A>
    bool ConcurrentQueueBase<T, A>::empty() const
    {
        return m_queue.empty() == 0;
    }

}  // end namespace fb
