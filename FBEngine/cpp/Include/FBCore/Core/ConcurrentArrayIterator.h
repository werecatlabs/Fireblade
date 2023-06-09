#ifndef ConcurrentArrayIterator_h__
#define ConcurrentArrayIterator_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>

namespace fb
{

    template <typename Container, typename Value>
    class ConcurrentArrayIterator
    {
    public:
        typedef ptrdiff_t difference_type;
        typedef Value value_type;
        typedef Value *pointer;
        typedef Value &reference;
        typedef std::random_access_iterator_tag iterator_category;

        //! concurrent_vector over which we are iterating.
        Container *my_vector;

        //! Index into the vector
        size_t my_index;

        //! Caches my_vector-&gt;internal_subscript(my_index)
        /** NULL if cached value is not available */
        mutable Value *my_item;

        template <typename C, typename T>
        friend ConcurrentArrayIterator<C, T> operator+( difference_type offset,
                                                        const ConcurrentArrayIterator<C, T> &v );

        template <typename C, typename T, typename U>
        friend bool operator==( const ConcurrentArrayIterator<C, T> &i,
                                const ConcurrentArrayIterator<C, U> &j );

        template <typename C, typename T, typename U>
        friend bool operator<( const ConcurrentArrayIterator<C, T> &i,
                               const ConcurrentArrayIterator<C, U> &j );

        template <typename C, typename T, typename U>
        friend difference_type operator-( const ConcurrentArrayIterator<C, T> &i,
                                          const ConcurrentArrayIterator<C, U> &j );

        template <typename C, typename U>
        friend class ConcurrentArrayIterator;

        ConcurrentArrayIterator();

        ConcurrentArrayIterator( const ConcurrentArrayIterator &other );

        ConcurrentArrayIterator( const Container &vector, size_t index, void *ptr = 0 );

        ~ConcurrentArrayIterator();

        ConcurrentArrayIterator &operator=( const ConcurrentArrayIterator & )
        {
            return *this;
        }

        ConcurrentArrayIterator operator+( difference_type offset ) const
        {
            return ConcurrentArrayIterator( *my_vector, my_index + offset );
        }
        ConcurrentArrayIterator &operator+=( difference_type offset )
        {
            my_index += offset;
            my_item = NULL;
            return *this;
        }
        ConcurrentArrayIterator operator-( difference_type offset ) const
        {
            return ConcurrentArrayIterator( *my_vector, my_index - offset );
        }
        ConcurrentArrayIterator &operator-=( difference_type offset )
        {
            my_index -= offset;
            my_item = NULL;
            return *this;
        }

        bool operator==( const ConcurrentArrayIterator &other ) const
        {
            return my_index == other.my_index;
        }

        bool operator!=( const ConcurrentArrayIterator &other ) const
        {
            return my_index != other.my_index;
        }

        bool operator<( const ConcurrentArrayIterator &other ) const
        {
            return my_index < other.my_index;
        }

        bool operator>( const ConcurrentArrayIterator &other ) const
        {
            return my_index > other.my_index;
        }

        bool operator<=( const ConcurrentArrayIterator &other ) const
        {
            return my_index <= other.my_index;
        }

        bool operator>=( const ConcurrentArrayIterator &other ) const
        {
            return my_index >= other.my_index;
        }

        ConcurrentArrayIterator &operator++()
        {
            auto &array = *my_vector;
            auto index = my_index++;
            if( index < array.size() )
            {
                my_item = (Value *)&array[index];
            }
            else
            {
                my_item = nullptr;
                //*this = array.end();
            }

            return *this;
        }

        ConcurrentArrayIterator operator++( int )
        {
            auto result = *this;
            operator++();
            return *result;
        }

        ConcurrentArrayIterator &operator--()
        {
            auto &array = *my_vector;
            auto index = my_index--;
            if( index < array.size() )
            {
                my_item = (Value *)&array[index];
            }
            else
            {
                my_item = nullptr;
                *this = array.end();
            }

            return *this;
        }

        ConcurrentArrayIterator operator--( int )
        {
            auto result = *this;
            operator--();
            return *result;
        }

        friend ConcurrentArrayIterator operator+( difference_type offset,
                                                  const ConcurrentArrayIterator & )
        {
            return ConcurrentArrayIterator();
        }

        difference_type operator-( ConcurrentArrayIterator it ) const
        {
            return my_index - it.my_index;
        }

        reference operator*() const
        {
            if( !my_item )
            {
                if( my_index < my_vector->size() )
                {
                    my_item = (Value *)&( *my_vector )[my_index];
                }
            }

            return *my_item;
        }

        pointer operator->() const
        {
            if( !my_item )
            {
                if( my_index < my_vector->size() )
                {
                    my_item = (Value *)&( *my_vector )[my_index];
                }
            }

            return my_item;
        }

        reference operator[]( difference_type k ) const
        {
            return my_vector[my_index + k];
        }
    };

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

#endif  // ConcurrentArrayIterator_h__
