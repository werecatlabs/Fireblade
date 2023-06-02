#ifndef ConcurrentQueueBase_h__
#define ConcurrentQueueBase_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <deque>

namespace fb
{

    /** Custom concurrent queue. */
    template <class T, class A = std::allocator<T>>
    class ConcurrentQueueBase
    {
    public:
        typedef A allocator_type;
        typedef T value_type;
        typedef T &reference;
        typedef const T &const_reference;
        typedef typename A::difference_type difference_type;
        typedef typename A::size_type size_type;

        ConcurrentQueueBase();
        ConcurrentQueueBase( const ConcurrentQueueBase &other );
        ~ConcurrentQueueBase();

        void push( const T &e );

        bool try_pop( T &e );

        void clear();

        bool empty() const;

    protected:
        std::deque<T> m_queue;
    };

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

}  // namespace fb

#endif  // ConcurrentQueueBase_h__
