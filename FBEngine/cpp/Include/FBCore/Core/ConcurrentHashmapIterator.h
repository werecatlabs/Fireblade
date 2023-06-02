#ifndef ConcurrentHashmapIterator_h__
#define ConcurrentHashmapIterator_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>

namespace fb
{

    template <typename Container, typename Value>
    class ConcurrentHashmapIterator : std::iterator<std::forward_iterator_tag, Value>
    {
    public:
        typedef ptrdiff_t difference_type;
        typedef Value value_type;
        typedef Value *pointer;
        typedef Value &reference;
        typedef std::random_access_iterator_tag iterator_category;

        typename Container::key_type first;
        Value second;

        //! concurrent_vector over which we are iterating.
        Container *my_vector;

        //! Index into the vector
        size_t my_index;

        //! Caches my_vector-&gt;internal_subscript(my_index)
        /** NULL if cached value is not available */
        mutable Value *my_item;

        std::pair<typename Container::key_type, Value> m_value;

        template <typename C, typename T>
        friend ConcurrentHashmapIterator<C, T> operator+( difference_type offset,
                                                          const ConcurrentHashmapIterator<C, T> &v );

        template <typename C, typename T, typename U>
        friend bool operator==( const ConcurrentHashmapIterator<C, T> &i,
                                const ConcurrentHashmapIterator<C, U> &j );

        template <typename C, typename T, typename U>
        friend bool operator<( const ConcurrentHashmapIterator<C, T> &i,
                               const ConcurrentHashmapIterator<C, U> &j );

        template <typename C, typename T, typename U>
        friend difference_type operator-( const ConcurrentHashmapIterator<C, T> &i,
                                          const ConcurrentHashmapIterator<C, U> &j );

        template <typename C, typename U>
        friend class ConcurrentHashmapIterator;

        ConcurrentHashmapIterator();

        ConcurrentHashmapIterator( const ConcurrentHashmapIterator &other );

        template <class B, class BValue>
        ConcurrentHashmapIterator( const ConcurrentHashmapIterator<B, BValue> &other )
        {
        }

        ConcurrentHashmapIterator( const Container &vector, size_t index, void *ptr = 0 );

        ~ConcurrentHashmapIterator();

        std::pair<typename Container::key_type, Value> *operator->();
        const std::pair<typename Container::key_type, Value> *operator->() const;

        ConcurrentHashmapIterator &operator=( const ConcurrentHashmapIterator & )
        {
            return *this;
        }

        ConcurrentHashmapIterator operator+( difference_type offset ) const
        {
            return ConcurrentHashmapIterator( *my_vector, my_index + offset );
        }
        ConcurrentHashmapIterator &operator+=( difference_type offset )
        {
            my_index += offset;
            my_item = NULL;
            return *this;
        }
        ConcurrentHashmapIterator operator-( difference_type offset ) const
        {
            return ConcurrentHashmapIterator( *my_vector, my_index - offset );
        }
        ConcurrentHashmapIterator &operator-=( difference_type offset )
        {
            my_index -= offset;
            my_item = NULL;
            return *this;
        }

        bool operator==( const ConcurrentHashmapIterator &other ) const
        {
            return my_index == other.my_index;
        }

        bool operator!=( const ConcurrentHashmapIterator &other ) const
        {
            return my_index != other.my_index;
        }

        bool operator<( const ConcurrentHashmapIterator &other ) const
        {
            return my_index < other.my_index;
        }

        bool operator>( const ConcurrentHashmapIterator &other ) const
        {
            return my_index > other.my_index;
        }

        bool operator<=( const ConcurrentHashmapIterator &other ) const
        {
            return my_index <= other.my_index;
        }

        bool operator>=( const ConcurrentHashmapIterator &other ) const
        {
            return my_index >= other.my_index;
        }

        ConcurrentHashmapIterator &operator++()
        {
            /*
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
            */

            return *this;
        }

        ConcurrentHashmapIterator operator++( int )
        {
            auto result = *this;
            operator++();
            return *result;
        }

        ConcurrentHashmapIterator &operator--()
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

        ConcurrentHashmapIterator operator--( int )
        {
            auto result = *this;
            operator--();
            return *result;
        }

        friend ConcurrentHashmapIterator operator+( difference_type offset,
                                                    const ConcurrentHashmapIterator & )
        {
            return ConcurrentHashmapIterator();
        }

        difference_type operator-( ConcurrentHashmapIterator it ) const
        {
            return my_index - it.my_index;
        }

        std::pair<typename Container::key_type, Value> &operator*()
        {
            return m_value;
        }

        const std::pair<typename Container::key_type, Value> &operator*() const
        {
            return m_value;
        }

        /*
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
        }*/

        std::pair<typename Container::key_type, Value> operator[]( difference_type k ) const
        {
            return std::pair<typename Container::key_type, Value>();
        }
    };

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

#endif  // ConcurrentHashmapIterator_h__
