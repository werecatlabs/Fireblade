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

}  // namespace fb

#include <FBCore/Containers/ConcurrentQueueBase.inl>

#endif  // ConcurrentQueueBase_h__
