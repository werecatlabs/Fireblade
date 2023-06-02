#ifndef ConcurrentHashmapBase_h__
#define ConcurrentHashmapBase_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/ConcurrentHashmapIterator.h>
#include <unordered_map>

namespace fb
{

    /** Custom concurrent hash map. */
    template <class Key, class T, class Compare = std::less<Key>,
              class A = std::allocator<std::pair<const Key, T>>>
    class ConcurrentHashmapBase
    {
    public:
        typedef A allocator_type;
        typedef typename A::value_type value_type;
        typedef typename A::reference reference;
        typedef typename A::const_reference const_reference;
        typedef typename A::difference_type difference_type;
        typedef typename A::size_type size_type;
        typedef Key key_type;

        typedef ConcurrentHashmapIterator<ConcurrentHashmapBase, T> iterator;
        typedef ConcurrentHashmapIterator<ConcurrentHashmapBase, const T> const_iterator;

        typedef std::reverse_iterator<iterator> reverse_iterator;              // optional
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;  // optional

        ConcurrentHashmapBase();
        ConcurrentHashmapBase( const ConcurrentHashmapBase &other );
        ~ConcurrentHashmapBase();

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;
        reverse_iterator rbegin();

        T &operator[]( const Key &key )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            return m_map[key];
        }

        T &operator[]( Key &&key )
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            return m_map[key];
        }

        void clear()
        {
            SpinRWMutex::ScopedLock lock( m_mutex, true );
            m_map.clear();
        }

        bool empty() const
        {
            SpinRWMutex::ScopedLock lock( m_mutex, false );
            return m_map.empty() == 0;
        }

        size_t size() const
        {
            return m_map.size();
        }

    protected:
        mutable SpinRWMutex m_mutex;
        std::unordered_map<Key, T> m_map;
    };

    template <class Key, class T, class Compare, class A>
    ConcurrentHashmapBase<Key, T, Compare, A>::ConcurrentHashmapBase()
    {
    }

    template <class Key, class T, class Compare, class A>
    ConcurrentHashmapBase<Key, T, Compare, A>::ConcurrentHashmapBase(
        const ConcurrentHashmapBase<Key, T, Compare, A> &other )
    {
    }

    template <class Key, class T, class Compare, class A>
    ConcurrentHashmapBase<Key, T, Compare, A>::~ConcurrentHashmapBase()
    {
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::iterator
    ConcurrentHashmapBase<Key, T, Compare, A>::begin()
    {
        return iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::const_iterator
    ConcurrentHashmapBase<Key, T, Compare, A>::begin() const
    {
        return const_iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::const_iterator
    ConcurrentHashmapBase<Key, T, Compare, A>::cbegin() const
    {
        return const_iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::iterator
    ConcurrentHashmapBase<Key, T, Compare, A>::end()
    {
        return iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::const_iterator
    ConcurrentHashmapBase<Key, T, Compare, A>::end() const
    {
        return const_iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::const_iterator
    ConcurrentHashmapBase<Key, T, Compare, A>::cend() const
    {
        return const_iterator();
    }

    template <class Key, class T, class Compare, class A>
    typename ConcurrentHashmapBase<Key, T, Compare, A>::reverse_iterator
    ConcurrentHashmapBase<Key, T, Compare, A>::rbegin()
    {
        return reverse_iterator();
    }

}  // namespace fb

#endif  // ConcurrentQueueBase_h__
