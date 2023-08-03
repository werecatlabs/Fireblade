#ifndef __FBCoreUtil_H_
#define __FBCoreUtil_H_

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/Set.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <deque>
#include <algorithm>
#include <xmmintrin.h>

#if FB_USE_TBB
#    include <tbb/concurrent_vector.h>
#endif

namespace fb
{

    /**
     * Provides utility functions for dealing with core types.
     */
    class FBCore_API Util
    {
    public:
        /**
         * Creates a set from an array.
         *
         * @tparam T The type of elements in the array.
         * @param data The array from which to create the set.
         * @return The created set.
         */
        template <class T>
        static Set<T> createSet( const Array<T> &data );

        /**
         * Erases an element from an array.
         *
         * @tparam T The type of elements in the array.
         * @param array The array from which to erase the element.
         * @param element The element to erase.
         * @return `true` if the element was successfully erased, `false` otherwise.
         */
        template <class T>
        static bool erase( Array<T> &array, const T &element );

#if FB_USE_TBB
        /**
         * Erases an element from a concurrent vector.
         *
         * @tparam T The type of elements in the concurrent vector.
         * @param array The concurrent vector from which to erase the element.
         * @param element The element to erase.
         * @return `true` if the element was successfully erased, `false` otherwise.
         */
        template <class T>
        static bool erase( tbb::concurrent_vector<T> &array, const T &element );

        /**
         * Erases an element of a specific type from a concurrent vector.
         *
         * @tparam T The type of elements in the concurrent vector.
         * @tparam ElemType The type of element to erase.
         * @param array The concurrent vector from which to erase the element.
         * @param element The element to erase.
         * @return `true` if the element was successfully erased, `false` otherwise.
         */
        template <class T, class ElemType>
        static bool eraseElementOfType( tbb::concurrent_vector<T> &array, const ElemType &element );
#endif

        /**
         * Performs aligned memory copy using SSE2 instructions.
         *
         * @param dest The destination memory address.
         * @param src The source memory address.
         * @param size The number of bytes to copy.
         */
        static void X_aligned_memcpy_sse2( void *dest, const void *src, size_t size );

        /**
         * Sleeps for a specified number of seconds with higher accuracy.
         *
         * @param seconds The number of seconds to sleep.
         */
        static void accurateSleep( double seconds );

        /**
         * Gets the resolution of the desktop screen.
         *
         * @param horizontal Reference to store the horizontal resolution.
         * @param vertical Reference to store the vertical resolution.
         */
        static void getDesktopResolution( int &horizontal, int &vertical );

        /**
         * Aligns an offset to the next multiple of an alignment value.
         *
         * @tparam T The type of the offset and alignment.
         * @param offset The offset value to align.
         * @param alignment The alignment value.
         * @return The aligned offset value.
         */
        template <typename T>
        static T alignToNextMultiple( T offset, T alignment );

        /**
         * Converts a floating-point value to a compact representation.
         *
         * @param input The input value to convert.
         * @param range The range value to use for conversion (optional, default is 1000).
         * @return The compact representation of the input value.
         */
        static short compactFloat( double input, int range = 1000 );

        /**
         * Converts a compact representation of a floating-point value to its original form.
         *
         * @param input The compact representation of the value.
         * @param range The range value used for compacting the value (optional, default is 1000).
         * @return The expanded floating-point value.
         */
        static double expandToFloat( short input, int range = 1000 );

        /**
         * Converts the pixel format between BGRA and RGBA.
         *
         * @param input The input pixel data.
         * @param pixel_width The width of the pixel data.
         * @param pixel_height The height of the pixel data.
         * @param output The output pixel data.
         */
        static void convertBetweenBGRAandRGBA( unsigned char *input, int pixel_width, int pixel_height,
                                               unsigned char *output );

        /**
         * The range used for compacting and expanding floating-point values.
         * Change to 20000 to `SHRT_MAX` if you don't mind whole numbers being turned into fractional ones.
         */
        static const int compact_range;

        /**
         * Finds the lowest value in a range defined by iterators.
         *
         * @tparam IT_TYPE The type of the iterators.
         * @tparam T The type of the value to find.
         * @param begin The beginning iterator of the range.
         * @param end The ending iterator of the range.
         * @param val Reference to store the lowest value found.
         */
        template <class IT_TYPE, class T>
        static void lowest( IT_TYPE begin, IT_TYPE end, T &val );

        /**
         * Finds the highest value in a range defined by iterators.
         *
         * @tparam IT_TYPE The type of the iterators.
         * @tparam T The type of the value to find.
         * @param begin The beginning iterator of the range.
         * @param end The ending iterator of the range.
         * @param val Reference to store the highest value found.
         */
        template <class IT_TYPE, class T>
        static void highest( IT_TYPE begin, IT_TYPE end, T &val );

        /**
         * Computes the average value of an array.
         *
         * @tparam T The type of elements in the array.
         * @param v The array to compute the average from.
         * @return The average value of the array.
         */
        template <class T>
        static T average( const Array<T> &v );

        /**
         * Computes the average value of a deque.
         *
         * @tparam T The type of elements in the deque.
         * @param v The deque to compute the average from.
         * @return The average value of the deque.
         */
        template <class T>
        static T average( const std::deque<T> &v );

        /**
         * Computes the cross product of two __m128 vectors.
         *
         * @param a The first vector.
         * @param b The second vector.
         * @return The cross product of the two vectors.
         */
        static __m128 crossProduct( __m128 a, __m128 b );

        /**
         * Removes an element from an Array container.
         *
         * @tparam T The type of elements in the Array container.
         * @param array The Array container from which to remove the element.
         * @param element The element to remove.
         * @return `true` if the element was successfully removed, `false` otherwise.
         */
        template <class T>
        static bool eraseElement( Array<T> &array, const T &element );

        /**
         * Compares two values of type s32.
         *
         * @param a Pointer to the first value to compare.
         * @param b Pointer to the second value to compare.
         * @return A negative value if `a` is less than `b`, zero if they are equal, or a positive value if `a` is greater than `b`.
         */
        static s32 compare( const void *a, const void *b );

        /**
         * Checks if there are any duplicate values in the specified range defined by iterators.
         *
         * @tparam Iterator The type of the iterators.
         * @param first The beginning iterator of the range.
         * @param end The ending iterator of the range.
         * @return `true` if duplicates are found, `false` otherwise.
         */
        template <typename Iterator>
        static bool hasDuplicates( Iterator first, Iterator end );

        /**
         * Checks if there are any duplicate values in the specified container.
         *
         * @tparam Container The type of the container.
         * @param v The container to check for duplicates.
         * @return `true` if duplicates are found, `false` otherwise.
         */
        template <typename Container>
        static bool hasDuplicates( const Container &v );

        /**
         * Sorts an Array container of SmartPtr objects by their names.
         *
         * @tparam T The type of the objects stored in the Array container.
         * @param vector The Array container to sort by name.
         */
        template <class T>
        static void sortByName( Array<SmartPtr<T>> &vector );

        static int CalculateNearest2Pow( int input );

        template <class T>
        static bool areAllValuesUnique( const std::vector<T> &vec )
        {
            std::set<int> uniqueElements;

            for( int num : vec )
            {
                // If the element is already in the set, it's not unique
                if( uniqueElements.count( num ) > 0 )
                {
                    return false;
                }
                uniqueElements.insert( num );
            }

            // If the size of the set is equal to the size of the vector, all values are unique
            return uniqueElements.size() == vec.size();
        }
    };

    template <class T>
    Set<T> Util::createSet( const Array<T> &data )
    {
        Set<T> set;
        for( u32 i = 0; i < data.size(); ++i )
        {
            set.insert( data[i] );
        }

        return set;
    }

    template <class T>
    bool Util::erase( Array<T> &array, const T &element )
    {
        if( array.empty() )
        {
            return false;
        }

        auto hasElement = false;

        typename Array<T>::iterator it = std::find( array.begin(), array.end(), element );
        if( it != array.end() )
        {
            hasElement = true;
        }

        if( hasElement )
        {
            Array<T> newArray;
            newArray.reserve( array.size() );

            for( size_t i = 0; i < array.size(); ++i )
            {
                if( element != array[i] )
                {
                    newArray.push_back( array[i] );
                }
            }

            array = newArray;
        }

        return hasElement;
    }

#if FB_USE_TBB

    template <class T>
    bool Util::erase( tbb::concurrent_vector<T> &array, const T &element )
    {
        if( array.empty() )
        {
            return false;
        }

        bool hasElement = false;
        typename tbb::concurrent_vector<T>::iterator it =
            std::find( array.begin(), array.end(), element );
        if( it != array.end() )
        {
            hasElement = true;
        }

        if( hasElement )
        {
            tbb::concurrent_vector<T> newArray;
            for( unsigned int i = 0; i < array.size(); ++i )
            {
                if( element != array[i] )
                    newArray.push_back( array[i] );
            }

            array = newArray;
        }

        return hasElement;
    }

    template <class T, class ElemType>
    bool Util::eraseElementOfType( tbb::concurrent_vector<T> &array, const ElemType &element )
    {
        if( array.empty() )
        {
            return false;
        }

        auto hasElement = false;

        typename tbb::concurrent_vector<T>::iterator it =
            std::find( array.begin(), array.end(), element );
        if( it != array.end() )
        {
            hasElement = true;
        }

        if( hasElement )
        {
            tbb::concurrent_vector<T> newArray;
            newArray.reserve( array.size() );

            for( size_t i = 0; i < array.size(); ++i )
            {
                if( element != array[i] )
                {
                    newArray.push_back( array[i] );
                }
            }

            array = newArray;
        }

        return hasElement;
    }
#endif

    template <typename T>
    T Util::alignToNextMultiple( T offset, T alignment )
    {
        return ( ( offset + alignment - 1u ) / alignment ) * alignment;
    }

    template <class IT_TYPE, class T>
    void Util::lowest( IT_TYPE begin, IT_TYPE end, T &val )
    {
        val = T( 1e10 );
        IT_TYPE i = begin;
        for( ; i != end; ++i )
        {
            val = std::min( val, *i );
        }
    }

    template <class IT_TYPE, class T>
    void Util::highest( IT_TYPE begin, IT_TYPE end, T &val )
    {
        val = T( -1e10 );
        IT_TYPE i = begin;
        for( ; i != end; ++i )
        {
            val = std::max( val, *i );
        }
    }

    template <class T>
    T Util::average( const Array<T> &v )
    {
        T sum = T( 0.0 );
        typename Array<T>::const_iterator it = v.begin();
        typename Array<T>::const_iterator endIt = v.end();
        for( ; it != endIt; ++it )
        {
            sum += *it;
        }

        return sum / v.size();
    }

    template <class T>
    T Util::average( const std::deque<T> &v )
    {
        T sum = T( 0.0 );
        typename std::deque<T>::const_iterator it = v.begin();
        typename std::deque<T>::const_iterator endIt = v.end();
        for( ; it != endIt; ++it )
        {
            sum += *it;
        }

        return sum / static_cast<T>( v.size() );
    }

    template <class T>
    bool Util::eraseElement( Array<T> &array, const T &element )
    {
        auto iter = array.begin();
        for( ; iter != array.end(); ++iter )
        {
            if( element == ( *iter ) )
            {
                array.erase( iter );
                return true;
            }
        }

        return false;
    }

    inline s32 Util::compare( const void *a, const void *b )
    {
        auto int_a = *( (size_t *)a );
        auto int_b = *( (size_t *)b );

        if( int_a == int_b )
            return 0;
        if( int_a < int_b )
            return -1;

        return 1;
    }

    template <typename Iterator>
    bool Util::hasDuplicates( Iterator first, Iterator end )
    {
        for( auto i = first; i != end; ++i )
        {
            for( auto j = first; i != j; ++j )
            {
                if( *i == *j )
                {
                    return true;
                }
            }
        }
        return false;
    }

    template <typename Container>
    bool Util::hasDuplicates( const Container &v )
    {
        for( const auto &i : v )
        {
            for( const auto &j : v )
            {
                if( &i == &j )
                {
                    break;
                }

                if( i == j )
                {
                    return true;
                }
            }
        }
        return false;
    }

    template <class T>
    void Util::sortByName( Array<SmartPtr<T>> &vector )
    {
        std::sort( vector.begin(), vector.end(),
                   []( SmartPtr<T> a, SmartPtr<T> b ) -> bool { return a->getName() < b->getName(); } );
    }

    template <class T>
    void fill_n( T *d, size_t size, T val )
    {
        for( size_t i = 0; i < size; ++i )
        {
            d[i] = val;
        }
    }

}  // namespace fb

#endif
