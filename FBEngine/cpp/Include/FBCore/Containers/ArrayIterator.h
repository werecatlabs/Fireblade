#ifndef ArrayIterator_h__
#define ArrayIterator_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>

namespace fb
{

    template <typename ContainerType, typename ValueType>
    class ArrayIterator
    {
    public:
        using value_type = ValueType;
        using reference = ValueType &;
        using pointer = ValueType *;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        /*
        template <typename C, typename T>
        friend ArrayIterator<C, T> operator+( difference_type offset, const ArrayIterator<C, T> &v )
        {
            return v + offset;
        }

        template <typename C, typename T, typename U>
        friend bool operator==( const ArrayIterator<C, T> &i, const ArrayIterator<C, U> &j )
        {
            return i.my_index == j.my_index;
        }

        template <typename C, typename T, typename U>
        friend bool operator<( const ArrayIterator<C, T> &i, const ArrayIterator<C, U> &j )
        {
            return i.my_item < j.my_item;
        }

        template <typename C, typename T, typename U>
        friend difference_type operator-( const ArrayIterator<C, T> &i, const ArrayIterator<C, U> &j )
        {
            return i.my_item - j.my_item;
        }
        */

        template <typename C, typename U>
        friend class ArrayIterator;

        ArrayIterator();

        ArrayIterator( const ArrayIterator &other );

        template <typename CType, typename ConstValue>
        ArrayIterator( const ArrayIterator<CType, ConstValue> &other );

        ArrayIterator( const ContainerType &vector, size_t index, void *ptr = 0 );

        ~ArrayIterator();

        value_type &operator*();

        value_type &operator*() const;

        pointer operator->() const;

        reference operator[]( difference_type k ) const;

        ArrayIterator &operator=( const ArrayIterator & )
        {
            return *this;
        }

        ArrayIterator operator+( difference_type offset ) const
        {
            return ArrayIterator( *my_vector, my_index + offset );
        }

        ArrayIterator &operator+=( difference_type offset )
        {
            my_index += offset;
            my_item = NULL;
            return *this;
        }

        ArrayIterator operator-( difference_type offset ) const
        {
            return ArrayIterator( *my_vector, my_index - offset );
        }

        ArrayIterator &operator-=( difference_type offset )
        {
            my_index -= offset;
            my_item = NULL;
            return *this;
        }

        bool operator==( const ArrayIterator &other ) const
        {
            return my_index == other.my_index;
        }

        bool operator!=( const ArrayIterator &other ) const
        {
            return my_index != other.my_index;
        }

        template <typename B, typename U>
        bool operator==( const ArrayIterator<B, U> &other ) const
        {
            return my_index == other.my_index;
        }

        bool operator<( const ArrayIterator &other ) const
        {
            return my_index < other.my_index;
        }

        bool operator>( const ArrayIterator &other ) const
        {
            return my_index > other.my_index;
        }

        bool operator<=( const ArrayIterator &other ) const
        {
            return my_index <= other.my_index;
        }

        bool operator>=( const ArrayIterator &other ) const
        {
            return my_index >= other.my_index;
        }

        ArrayIterator &operator++()
        {
            auto &array = *my_vector;
            auto index = my_index++;
            if( index < array.size() )
            {
                my_item = (value_type *)&array[index];
            }
            else
            {
                my_item = nullptr;
                //*this = array.end();
            }

            return *this;
        }

        ArrayIterator operator++( int )
        {
            auto result = *this;
            operator++();
            return result;
        }

        ArrayIterator &operator--()
        {
            auto &array = *my_vector;
            auto index = my_index--;
            if( index < array.size() )
            {
                my_item = (value_type *)&array[index];
            }
            else
            {
                my_item = nullptr;
                *this = array.end();
            }

            return *this;
        }

        ArrayIterator operator--( int )
        {
            return ArrayIterator();
        }

        friend ArrayIterator operator+( difference_type offset, const ArrayIterator & );

        difference_type operator-( ArrayIterator it ) const
        {
            return 0;
        }

    protected:
        //! concurrent_vector over which we are iterating.
        ContainerType *my_vector = nullptr;

        //! Index into the vector
        size_t my_index = 0;

        //! Caches my_vector-&gt;internal_subscript(my_index)
        /** NULL if cached value is not available */
        mutable ValueType *my_item = nullptr;
    };

}  // namespace fb

#include <FBCore/Containers/ArrayIterator.inl>

#endif  // ArrayIterator_h__
