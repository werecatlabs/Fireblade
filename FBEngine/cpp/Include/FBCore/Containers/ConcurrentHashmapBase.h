#ifndef ConcurrentHashmapBase_h__
#define ConcurrentHashmapBase_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Containers/ConcurrentHashmapIterator.h>
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
            FB_SPIN_LOCK_WRITE( m_mutex );
            return m_map[key];
        }

        T &operator[]( Key &&key )
        {
            FB_SPIN_LOCK_WRITE( m_mutex );
            return m_map[key];
        }

        void clear()
        {
            FB_SPIN_LOCK_WRITE( m_mutex );
            m_map.clear();
        }

        bool empty() const
        {
            FB_SPIN_LOCK_READ( m_mutex );
            return m_map.empty() == 0;
        }

        size_t size() const
        {
            return m_map.size();
        }

    protected:
        FB_SPIN_MUTEX_MUTABLE( m_mutex );
        std::unordered_map<Key, T> m_map;
    };

}  // namespace fb

#include <FBCore/Containers/ConcurrentHashmapBase.inl>

#endif  // ConcurrentQueueBase_h__
