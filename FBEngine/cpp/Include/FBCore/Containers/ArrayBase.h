#ifndef ArrayBase_h__
#define ArrayBase_h__

#include <FBCore/Containers/ArrayIterator.h>
#include <FBCore/Memory/Memory.h>
#include <initializer_list>
#include <memory>

namespace fb
{

    /** Custom concurrent array. */
    template <class T, class A = std::allocator<T>>
    class ArrayBase
    {
    public:
        typedef A allocator_type;
        typedef T value_type;
        typedef T &reference;
        typedef const T &const_reference;
        typedef typename A::difference_type difference_type;
        typedef typename A::size_type size_type;

        typedef ArrayIterator<ArrayBase, T> iterator;
        typedef ArrayIterator<ArrayBase, const T> const_iterator;

        typedef std::reverse_iterator<iterator> reverse_iterator;              // optional
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;  // optional

        ArrayBase();
        ArrayBase( size_t capacity );
        ArrayBase( std::initializer_list<value_type> il );
        ArrayBase( size_t n, const T &other );
        ArrayBase( const ArrayBase &other );
        ~ArrayBase();

        template <class I>
        ArrayBase( I first, I last, const allocator_type &a = allocator_type() );

        void resize( size_type n );

        void resize( size_type n, T value );

        void reserve( size_type n );

        ArrayBase &operator=( const ArrayBase & );

        bool operator==( const ArrayBase & ) const;

        bool operator!=( const ArrayBase & ) const;
        bool operator<( const ArrayBase & ) const;

        bool operator<=( const ArrayBase & ) const;
        bool operator>=( const ArrayBase & ) const;

        bool operator>( const ArrayBase<T, A> & ) const;

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

        void swap( ArrayBase &other );

        size_type size() const;

        size_type capacity() const;

        size_type max_size() const;

        bool empty() const;

        A get_allocator() const;

        T *allocate( size_t newCapacity );
        void deallocate(T* data, size_t sz );
        
        void destructElements( T *data, size_t sz );

        T *data() noexcept
        {
            return m_array;
        }

    protected:
        A m_allocator;
        T *m_array = nullptr;
        size_t m_size = 0;
        size_t m_capacity = 0;
    };

}  // namespace fb

#include <FBCore/Containers/ArrayBase.inl>

#endif  // ArrayBase_h__
