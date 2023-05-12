#ifndef _FBCoreUtil_H_
#define _FBCoreUtil_H_

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/Set.h>
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
    //-------------------------------------------------
    /** Provides some functions for dealing with core types.
     */
    class FBCore_API CoreUtil
    {
    public:
        /** */
        template <class T>
        static Set<T> createSet( const Array<T> &data );

        template <class T>
        static bool erase( Array<T> &array, const T &element );

#if FB_USE_TBB
        template <class T>
        static bool erase( tbb::concurrent_vector<T> &array, const T &element );

        template <class T, class ElemType>
        static bool eraseElementOfType( tbb::concurrent_vector<T> &array, const ElemType &element );
#endif

        static void X_aligned_memcpy_sse2( void *dest, const void *src, size_t size );
        static void accurateSleep( double seconds );

        static void getDesktopResolution( int &horizontal, int &vertical );

        /// Aligns the input 'offset' to the next multiple of 'alignment'.
        /// Alignment can be any value except 0. Some examples:
        ///
        /// alignToNextMultiple( 0, 4 ) = 0;
        /// alignToNextMultiple( 1, 4 ) = 4;
        /// alignToNextMultiple( 2, 4 ) = 4;
        /// alignToNextMultiple( 3, 4 ) = 4;
        /// alignToNextMultiple( 4, 4 ) = 4;
        /// alignToNextMultiple( 5, 4 ) = 8;
        ///
        /// alignToNextMultiple( 0, 3 ) = 0;
        /// alignToNextMultiple( 1, 3 ) = 3;
        template <typename T>
        static T alignToNextMultiple( T offset, T alignment );

        static short compactFloat( double input, int range = 1000 );
        static double expandToFloat( short input, int range = 1000 );

        static void convertBetweenBGRAandRGBA( unsigned char *input, int pixel_width, int pixel_height,
                                               unsigned char *output );

        // change to 20000 to SHORT_MAX if you don't mind whole numbers
        // being turned into fractional ones
        static const int compact_range;

        template <class IT_TYPE, class T>
        static void lowest( IT_TYPE begin, IT_TYPE end, T &val );

        template <class IT_TYPE, class T>
        static void highest( IT_TYPE begin, IT_TYPE end, T &val );

        template <class T>
        static T average( const Array<T> &v );

        template <class T>
        static T average( const std::deque<T> &v );

        static __m128 crossProduct( __m128 a, __m128 b );

        /** Remove an element from and std vector. Returns true if successful. */
        template <class T>
        static bool eraseElement( Array<T> &Array, const T &element );

        static s32 compare( const void *a, const void *b )
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
        static bool hasDuplicates( Iterator first, Iterator end )
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
        static bool hasDuplicates( const Container &v )
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
        static void sortByName( Array<SmartPtr<T>> &vector )
        {
            std::sort( vector.begin(), vector.end(), []( SmartPtr<T> a, SmartPtr<T> b ) -> bool {
                return a->getName() < b->getName();
            } );
        }
    };
}  // end namespace fb

#include <FBCore/Base/CoreUtil.inl>

#endif
