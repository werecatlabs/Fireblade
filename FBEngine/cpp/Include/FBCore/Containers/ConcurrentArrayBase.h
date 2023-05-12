#ifndef ConcurrentArrayBase_h__
#define ConcurrentArrayBase_h__

#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Thread/Threading.h>
#include <FBCore/Thread/RecursiveMutex.h>
#include <FBCore/Containers/ConcurrentArrayIterator.h>

namespace fb
{

    /** Custom concurrent array. */
    template <class T, class A = std::allocator<T>>
    class ConcurrentArrayBase
    {
    public:
        typedef A allocator_type;
        typedef T value_type;
        typedef T &reference;
        typedef const T &const_reference;
        typedef typename A::difference_type difference_type;
        typedef typename A::size_type size_type;

        typedef ConcurrentArrayIterator<ConcurrentArrayBase, T> iterator;
        typedef ConcurrentArrayIterator<ConcurrentArrayBase, const T> const_iterator;

        typedef std::reverse_iterator<iterator> reverse_iterator;              // optional
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;  // optional

        ConcurrentArrayBase();
        ConcurrentArrayBase( std::initializer_list<value_type> il );
        ConcurrentArrayBase( const ConcurrentArrayBase & );
        ~ConcurrentArrayBase();

        template <class I>
        ConcurrentArrayBase( I first, I last, const allocator_type &a = allocator_type() );

        void resize( size_type n );

        void reserve( size_type n );

        ConcurrentArrayBase &operator=( const ConcurrentArrayBase & );

        bool operator==( const ConcurrentArrayBase & ) const;

        bool operator!=( const ConcurrentArrayBase & ) const;
        bool operator<( const ConcurrentArrayBase & ) const;

        bool operator<=( const ConcurrentArrayBase & ) const;
        bool operator>=( const ConcurrentArrayBase & ) const;

        bool operator>( const ConcurrentArrayBase<T, A> & ) const;

        iterator begin();
        const_iterator begin() const;
        const_iterator cbegin() const;
        iterator end();
        const_iterator end() const;
        const_iterator cend() const;
        reverse_iterator rbegin();

        const_reverse_iterator rbegin() const;
        const_reverse_iterator crbegin() const;
        reverse_iterator rend();

        const_reverse_iterator rend() const;
        const_reverse_iterator crend() const;

        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

        template <class... Args>
        void emplace_front( Args &&... );

        template <class... Args>
        void emplace_back( Args &&... );

        void push_front( const T & );
        void push_front( T && );
        void push_back( const T & );
        void push_back( T && );

        void pop_front();
        void pop_back();

        reference operator[]( size_type );

        const_reference operator[]( size_type ) const;
        reference at( size_type );

        const_reference at( size_type ) const;

        template <class... Args>
        iterator emplace( const_iterator, Args &&... );

        template <class iter>
        iterator insert( const_iterator, iter, iter );

        iterator insert( const_iterator, const T & );
        iterator insert( const_iterator, T && );
        iterator insert( const_iterator, std::initializer_list<T> );
        iterator insert( const_iterator, size_type, T & );

        template <class I>
        void insert( I pos, I first, I last );

        iterator erase( iterator it );
        iterator erase( iterator start, iterator end );

        iterator erase( const_iterator it );
        iterator erase( const_iterator start, const_iterator end );

        void clear();

        template <class iter>
        void assign( iter, iter );

        void assign( std::initializer_list<T> );
        void assign( size_type, const T & );

        void swap( ConcurrentArrayBase &other );

        void shrink_to_fit();

        size_type size() const;

        size_type max_size() const;

        bool empty() const;

        A get_allocator() const;

    protected:
        mutable RecursiveMutex m_mutex;
        T *m_array = nullptr;
        size_type m_size = 0;
        size_type m_capacity = 0;
    };

}  // namespace fb

#include <FBCore/Containers/ConcurrentArrayBase.inl>

#endif  // ConcurrentArrayBase_h__
