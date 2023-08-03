#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Math.h>
#include <array>

#ifdef FB_USE_RANDOMC_LIB
#    include <random/randomc.h>
#endif

#include <float.h>
#include <stdlib.h>  //for rand
//#include <utility>

#if FB_USE_BOOST
#    include <boost/algorithm/clamp.hpp>
#    include <boost/math/constants/constants.hpp>
#    include <boost/math/special_functions/relative_difference.hpp>
#endif

#include <cmath>

namespace fb
{
    template <>
    s32 MathI::m_randomSeed = 0;
    template <>
    s32 Math<s64>::m_randomSeed = 0;
    template <>
    s32 MathF::m_randomSeed = 0;
    template <>
    s32 MathD::m_randomSeed = 0;

    template <>
    s32 Math<long double>::m_randomSeed = 0;

    // template<>
    // const s32 Math<s32>::EPSILON = (s32)FLT_EPSILON;
    // template<>
    // const s32 Math<s32>::ZERO_TOLERANCE = 0;
    // template<>
    // const s32 Math<s32>::MAX_F32 = 0;
    // template<>
    // const s32 Math<s32>::PI = (s32)((float)(4.0*atan(1.0)));
    // template<>
    // const s32 Math<s32>::TWO_PI = (s32)2.0f*Math<s32>::PI;
    // template<>
    // const s32 Math<s32>::HALF_PI = (s32)0.5f*Math<s32>::PI;
    // template<>
    // const s32 Math<s32>::INV_PI = (s32)(1.0f/Math<s32>::PI);
    // template<>
    // const s32 Math<s32>::INV_TWO_PI = (s32)(1.0f/Math<s32>::TWO_PI);
    // template<>
    // const s32 Math<s32>::DEG_TO_RAD = (s32)(Math<s32>::PI/180.0f);
    // template<>
    // const s32 Math<s32>::RAD_TO_DEG = (s32)(180.0f/Math<s32>::PI);
    // template<>
    // const s64 Math<s64>::EPSILON = FLT_EPSILON;
    // template<>
    // const s64 Math<s64>::ZERO_TOLERANCE = 1e-06f;
    //
    // template<>
    // const s64 Math<s64>::MAX_F32 = 0;
    // template<>
    // const s64 Math<s64>::PI = (float)(4.0*atan(1.0));
    // template<>
    // const s64 Math<s64>::TWO_PI = 2.0f*Math<s64>::PI;
    // template<>
    // const s64 Math<s64>::HALF_PI = 0.5f*Math<s64>::PI;
    // template<>
    // const s64 Math<s64>::INV_PI = 1.0f/Math<s64>::PI;
    // template<>
    // const s64 Math<s64>::INV_TWO_PI = 1.0f/Math<s64>::TWO_PI;
    // template<>
    // const s64 Math<s64>::DEG_TO_RAD = Math<s64>::PI/180.0f;
    // template<>
    // const s64 Math<s64>::RAD_TO_DEG = 180.0f/Math<s64>::PI;
    // template<>
    // const f32 Math<f32>::EPSILON = FLT_EPSILON;
    // template<>
    // const f32 Math<f32>::ZERO_TOLERANCE = 1e-06f;
    // template<>
    // const f32 Math<f32>::MAX_F32 = FLT_MAX;
    //
    // template<>
    // const f32 Math<f32>::PI = (f32)((float)(4.0*atan(1.0)));
    // template<>
    // const f32 Math<f32>::TWO_PI = (f32)2.0f*Math<f32>::PI;

    // template<>
    // const f32 Math<f32>::HALF_PI = 0.5f*Math<f32>::PI;
    // template<>
    // const f32 Math<f32>::INV_PI = 1.0f/Math<f32>::PI;
    // template<>
    // const f32 Math<f32>::INV_TWO_PI = 1.0f/Math<f32>::TWO_PI;
    // template<>
    // const f32 Math<f32>::DEG_TO_RAD = Math<f32>::PI/180.0f;
    // template<>
    // const f32 Math<f32>::RAD_TO_DEG = 180.0f/Math<f32>::PI;
    //
    // template<>
    // const f64 Math<f64>::EPSILON = FLT_EPSILON;
    //
    // template<>
    // const f64 Math<f64>::ZERO_TOLERANCE = 1e-06f;
    // template<>
    // const f64 Math<f64>::MAX_F32 = FLT_MAX;
    // template<>
    // const f64 Math<f64>::PI = (float)(4.0*atan(1.0));
    // template<>
    // const f64 Math<f64>::TWO_PI = 2.0f*Math<f64>::PI;
    // template<>
    // const f64 Math<f64>::HALF_PI = 0.5f*Math<f64>::PI;
    // template<>
    // const f64 Math<f64>::INV_PI = 1.0f/Math<f64>::PI;
    // template<>
    // const f64 Math<f64>::INV_TWO_PI = 1.0f/Math<f64>::TWO_PI;
    // template<>
    // const f64 Math<f64>::DEG_TO_RAD = Math<f64>::PI/180.0f;
    // template<>
    // const f64 Math<f64>::RAD_TO_DEG = 180.0f/Math<f64>::PI;

    // Returns a random number between min and max
    template <class T>
    T ranged_random( T min, T max, [[maybe_unused]] int seed = 0 )
    {
#ifndef FB_USE_RANDOMC_LIB
        return min + ( max - min ) * rand() / 0x7fff;
#else
        TRandomMersenne random( seed );

        // f32 randNum = (f32)rand();
        f32 randNum = (f32)random.IRandom( 0, RAND_MAX );
        f32 randRatio = randNum / (f32)RAND_MAX;
        f32 result = (f32)min + ( (f32)max - (f32)min ) * randRatio;
        return (T)result;
#endif
    }

    template <>
    bool Math<s32>::equals( const s32 af, const s32 bf, const s32 maxDiff )
    {
#if FB_USE_BOOST
        return boost::math::epsilon_difference( af, bf ) < maxDiff;
#else
        return af == bf;
#endif
    }

    template <>
    bool Math<s64>::equals( const s64 af, const s64 bf, const s64 maxDiff )
    {
#if FB_USE_BOOST
        return Abs( af - bf ) < maxDiff;
#else
        return af == bf;
#endif
    }

    template <>
    bool Math<long double>::equals( const long double af, const long double bf,
                                    const long double maxDiff )
    {
#if FB_USE_BOOST
        return boost::math::epsilon_difference( af, bf ) < maxDiff;
#else
        return af == bf;
#endif
    }

    template <>
    bool Math<f32>::equals( const f32 af, const f32 bf, const f32 maxDiff )
    {
        // from TAMING THE FLOATING POINT BEAST
#if 0
		// fast routine, not portable due to shifting signed int
		// works on Microsoft compilers.
		int ai = *reinterpret_cast<int*>(&af);
		int bi = *reinterpret_cast<int*>(&bf);
		int test = (ai^bi)>>31;
		//assert((0 == test) || (0xFFFFFFFF == test));
		int diff = (((0x80000000 - ai)&(test)) | (ai & (~test))) - bi;
		int v1 = maxDiff + diff;
		int v2 = maxDiff - diff;
		return (v1|v2) >= 0;
#else
        // solid, fast routine across all platforms
        // with constant time behavior
        const int ai = *reinterpret_cast<const int *>( &af );
        const int bi = *reinterpret_cast<const int *>( &bf );
        int test = ( static_cast<unsigned>( ai ^ bi ) >> 31 ) - 1;
        // assert((0 == test) || (0xFFFFFFFF == test));
        int diff = ( ( ( 0x80000000 - ai ) & ( ~test ) ) | ( ai & test ) ) - bi;
        int v1 = static_cast<s32>( maxDiff ) + diff;
        int v2 = static_cast<s32>( maxDiff ) - diff;
        return ( v1 | v2 ) >= 0;
#endif
    }

    template <>
    bool Math<f64>::equals( const f64 af, const f64 bf, [[maybe_unused]] const f64 maxDiff )
    {
#if FB_USE_BOOST
        return boost::math::epsilon_difference( af, bf ) < maxDiff;
#else
        return af == bf;
#endif
    }

    template <>
    s32 Math<s32>::RangedRandom( s32 min, s32 max, s32 seed )
    {
        if( seed )
        {
            return ranged_random<s32>( min, max, seed );
        }

        return ranged_random<s32>( min, max, m_randomSeed++ );
    }

    template <>
    s64 Math<s64>::RangedRandom( s64 min, s64 max, s32 seed )
    {
        if( seed )
        {
            return ranged_random<s64>( min, max, seed );
        }

        return ranged_random<s64>( min, max, m_randomSeed++ );
    }

    template <>
    long double Math<long double>::RangedRandom( long double min, long double max, s32 seed )
    {
        if( seed )
        {
            return ranged_random<long double>( min, max, seed );
        }

        return ranged_random<long double>( min, max, m_randomSeed++ );
    }

    template <>
    f32 Math<f32>::RangedRandom( f32 min, f32 max, s32 seed )
    {
        if( seed )
        {
            return ranged_random<f32>( min, max, seed );
        }

        return ranged_random<f32>( min, max, m_randomSeed++ );
    }

    template <>
    f64 Math<f64>::RangedRandom( f64 min, f64 max, s32 seed )
    {
        if( seed )
        {
            return ranged_random<f64>( min, max, seed );
        }

        return ranged_random<f64>( min, max, m_randomSeed++ );
    }

    template <>
    bool Math<s32>::isFinite( s32 value )
    {
        return value > std::numeric_limits<s32>::min() && value < std::numeric_limits<s32>::max();
    }

    template <>
    bool Math<s64>::isFinite( s64 value )
    {
        return value > std::numeric_limits<s64>::min() && value < std::numeric_limits<s64>::max();
    }

    template <>
    bool Math<long double>::isFinite( long double value )
    {
        return value > std::numeric_limits<long double>::min() &&
               value < std::numeric_limits<long double>::max();
    }

    template <>
    bool Math<f32>::isFinite( f32 value )
    {
        return std::isfinite( value );
    }

    template <>
    bool Math<f64>::isFinite( f64 value )
    {
        return std::isfinite( value );
    }

    template <>
    bool Math<s32>::isNaN( s32 value )
    {
        return value == value;
    }

    template <>
    bool Math<f32>::isNaN( f32 value )
    {
        return std::isnan( value );
    }

    template <>
    bool Math<s64>::isNaN( s64 value )
    {
        return value == value;
    }

    template <>
    bool Math<f64>::isNaN( f64 value )
    {
        return std::isnan( value );
    }

    template <>
    bool Math<long double>::isNaN( long double value )
    {
        return std::isnan( static_cast<f64>( value ) );
    }

    template <class T>
    void Math<T>::Limit( T &v, T val )
    {
        if( v > val )
            v = val;
    }

    template <class T>
    T Math<T>::trunc( T value )
    {
        return static_cast<T>( std::trunc( static_cast<float>( value ) ) );
    }

    template <class T>
    T Math<T>::epsilon()
    {
        return std::numeric_limits<T>::epsilon();
    }

    template <class T>
    T Math<T>::ASin( T fValue )
    {
        if( T( -1.0 ) < fValue )
        {
            if( fValue < T( 1.0 ) )
            {
                return T( asin( static_cast<f32>( fValue ) ) );
            }
            return Math<T>::half_pi();
        }

        return Math<T>::half_pi();
    }

    template <class T>
    T Math<T>::wrapRadians( T r )
    {
        return static_cast<T>( std::remainder( r, T( 2.0 ) * Math<T>::pi() ) );
    }

    template <class T>
    T Math<T>::wrapDegrees( T degrees, T range )
    {
        return static_cast<T>( std::remainder( degrees, T( 2.0 ) * range ) );
    }

    template <class T>
    T Math<T>::wrapGradians( T r )
    {
        float i;
        r = static_cast<T>( modff( static_cast<float>( r ), &i ) );

        if( r > T( 0.5 ) )
        {
            r -= T( 1.0 );
        }

        if( r <= T( -0.5 ) )
        {
            r += T( 1.0 );
        }

        return r;
    }

    template <class T>
    T Math<T>::wrap( T val, T lowerBound, T upperBound )
    {
        if( val < lowerBound || val > upperBound )
        {
            return static_cast<T>( std::fmod( val, upperBound ) );
        }

        return val;
    }

    template <>
    s32 Math<s32>::ACos( s32 fValue )
    {
        if( static_cast<s32>( -1.0 ) < fValue )
        {
            if( fValue < static_cast<s32>( 1.0 ) )
                return static_cast<s32>( acos( static_cast<f32>( fValue ) ) );
            return static_cast<s32>( 0.0 );
        }
        return pi();
    }

    template <class T>
    T Math<T>::ACos( T fValue )
    {
        if( T( -1.0 ) < fValue )
        {
            if( fValue < T( 1.0 ) )
                return static_cast<T>( ::acos( fValue ) );
            return T( 0.0 );
        }
        return pi();
    }

    template <class T>
    T Math<T>::Ln( T value )
    {
        return static_cast<T>( std::log( value ) );
    }

    template <class T>
    s32 Math<T>::Round( T value )
    {
        return static_cast<s32>( ::floor( value + 0.5f ) );
    }

    template <class T>
    T Math<T>::Sin( T value )
    {
        return static_cast<T>( sin( static_cast<f32>( value ) ) );
    }

    template <class T>
    T Math<T>::Cos( T value )
    {
        return static_cast<T>( cos( static_cast<f32>( value ) ) );
    }

    template <class T>
    T Math<T>::Tan( T value )
    {
        return static_cast<T>( tan( static_cast<f32>( value ) ) );
    }

    template <>
    s32 Math<s32>::Atan( s32 x )
    {
        return static_cast<s32>( std::atan( static_cast<f32>( x ) ) );
    }

    template <class T>
    T Math<T>::Atan( T x )
    {
        return static_cast<T>( std::atan( x ) );
    }

    template <>
    s32 Math<s32>::ATan2( const s32 &y, const s32 &x )
    {
        return static_cast<s32>( std::atan2( static_cast<f32>( y ), static_cast<f32>( x ) ) );
    }

    template <class T>
    T Math<T>::ATan2( const T &y, const T &x )
    {
        return static_cast<T>( std::atan2( y, x ) );
    }

    template <class T>
    T Math<T>::Sqr( T value )
    {
        return value * value;
    }

    template <class T>
    T Math<T>::Sqrt( T value )
    {
        return std::sqrt( value );
    }

    template <>
    s32 Math<s32>::Sqrt( s32 value )
    {
        return static_cast<s32>( std::sqrt( static_cast<f32>( value ) ) );
    }

    template <>
    s64 Math<s64>::Sqrt( s64 value )
    {
        return static_cast<s64>( std::sqrt( static_cast<f32>( value ) ) );
    }

    template <class T>
    T Math<T>::SqrtInv( T value )
    {
        return T( 1.0 ) / Math<T>::Sqrt( value );
    }

    template <>
    s32 Math<s32>::SqrtInv( s32 value )
    {
        return static_cast<s32>( static_cast<float>( 1.0 ) / sqrt( static_cast<float>( value ) ) );
    }

    template <>
    s64 Math<s64>::SqrtInv( s64 value )
    {
        const auto sqrtValue = std::sqrt( static_cast<f32>( value ) );
        const auto fValue = static_cast<f32>( 1.0 ) / sqrtValue;
        return static_cast<s64>( fValue );
    }

    template <class T>
    T Math<T>::Abs( T value )
    {
        return ( value < T( 0 ) ) ? -value : value;
    }

    template <class T>
    T Math<T>::normalize( const T &value, const T &min, const T &max )
    {
        return ( ( value - min ) / ( max - min ) );
    }

    template <class T>
    T Math<T>::min( const T a, const T b )
    {
        return a < b ? a : b;
    }

    template <class T>
    T Math<T>::max( const T a, const T b )
    {
        return a < b ? b : a;
    }

    template <class T>
    bool Math<T>::isZero( const T a, const T tolerance )
    {
        return Math<T>::Abs( a ) < tolerance;
    }

    template <class T>
    T Math<T>::DegToRad( const T &deg )
    {
        return static_cast<T>( deg * deg_to_rad() );
    }

    template <class T>
    T Math<T>::RadToDeg( const T &rad )
    {
        return static_cast<T>( rad * rad_to_deg() );
    }

    template <class T>
    T Math<T>::RadToFullDegrees( const T &rad )
    {
        T angleDegrees = static_cast<T>( rad * rad_to_deg() );
        if( angleDegrees < T( 0.0 ) )
        {
            angleDegrees = angleDegrees + T( 360.0 );
        }

        return angleDegrees;
    }

    template <class T>
    void Math<T>::SetRandomSeed( s32 seed )
    {
        m_randomSeed = seed;
    }

    template <class T>
    void Math<T>::swap( T &firstValue, T &secondValue )
    {
        auto tmp = firstValue;
        firstValue = secondValue;
        secondValue = tmp;
    }

    template <class T>
    void Math<T>::swapIfGreater( T &firstValue, T &secondValue )
    {
        if( firstValue > secondValue )
        {
            swap( firstValue, secondValue );
        }
    }

    template <class T>
    void Math<T>::swapIfLessThan( T &firstValue, T &secondValue )
    {
        if( firstValue < secondValue )
        {
            swap( firstValue, secondValue );
        }
    }

    template <class T>
    T Math<T>::makeMultipleOf( T numToRound, T multiple )
    {
        if( multiple == T( 0.0 ) )
        {
            return numToRound;
        }

        auto remainder = Mod( Abs( numToRound ), multiple );
        if( remainder == T( 0.0 ) )
        {
            return numToRound;
        }

        if( numToRound < T( 0.0 ) )
        {
            return -( Abs( numToRound ) - remainder );
        }

        return numToRound + multiple - remainder;
    }

    template <class T>
    T Math<T>::nextPow2( T x )
    {
        T y;
        for( y = 1; y < x; y *= 2 )
            ;
        return y;
    }

    template <class T>
    T Math<T>::Exp( T val )
    {
        return static_cast<T>( ::exp( val ) );
    }

    template <typename T>
    s32 Math<T>::Sign( T val )
    {
        return ( T( 0 ) < val ) - ( val < T( 0 ) );
    }

    template <class T>
    T Math<T>::half_pi()
    {
        return pi() / T( 2.0 );
    }

    template <class T>
    T Math<T>::two_pi()
    {
        return pi() * T( 2.0 );
    }

    template <class T>
    T Math<T>::pi()
    {
        return T( 3.14159265358979323846 );
    }

    template <class T>
    T Math<T>::Floor( T val )
    {
        return static_cast<T>( std::floor( val ) );
    }

    template <class T>
    s32 Math<T>::FloorToInt( T val )
    {
        return static_cast<s32>( std::floor( val ) );
    }

    template <>
    s32 Math<s32>::Mod( s32 x, s32 y )
    {
        return static_cast<s32>( std::fmod( static_cast<f32>( x ), static_cast<f32>( y ) ) );
    }

    template <class T>
    T Math<T>::Mod( T x, T y )
    {
        return static_cast<T>( std::fmod( x, y ) );
    }

    template <class T>
    T Math<T>::Pow( T val, T powVal )
    {
        return static_cast<T>( ::pow( val, powVal ) );
    }

    template <class T>
    f32 Math<T>::roundToFloat( T value )
    {
        return static_cast<f32>( floor( static_cast<double>( value + T( 0.5 ) ) ) );
    }

    template <class T>
    s32 Math<T>::roundToInt( T value )
    {
        return static_cast<int>( floor( static_cast<double>( value + T( 0.5 ) ) ) );
    }

    template <class T>
    T Math<T>::round( T value )
    {
        return static_cast<T>( floor( static_cast<double>( value + T( 0.5 ) ) ) );
    }

    template <class T>
    T Math<T>::round( T value, u32 prec )
    {
        auto pow_10 = Math<T>::Pow( T( 10.0 ), static_cast<T>( prec ) );
        return Math<T>::round( value * pow_10 ) / pow_10;
    }

    static std::array<int, 257> perm = {
        151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103,
        30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,
        197, 62,  94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174,
        20,  125, 136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231,
        83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143,
        54,  65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196,
        135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124,
        123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,
        182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101,
        155, 167, 43,  172, 9,   129, 22,  39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185,
        112, 104, 218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,
        51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184, 84,  204, 176,
        115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,  222, 114, 67,  29,  24,  72,  243,
        141, 128, 195, 78,  66,  215, 61,  156, 180, 151
    };

    template <class T>
    T Math<T>::PerlinNoise( T x, T y )
    {
        auto X = Math<T>::FloorToInt( x ) & 0xff;
        auto Y = Math<T>::FloorToInt( y ) & 0xff;
        x -= Math<T>::Floor( x );
        y -= Math<T>::Floor( y );
        auto u = Fade( x );
        auto v = Fade( y );
        auto A = ( perm[X] + Y ) & 0xff;
        auto B = ( perm[X + 1] + Y ) & 0xff;
        return lerp( v, lerp( u, Grad( perm[A], x, y ), Grad( perm[B], x - 1, y ) ),
                     lerp( u, Grad( perm[A + 1], x, y - 1 ), Grad( perm[B + 1], x - 1, y - 1 ) ) );
    }

    template <class T>
    T Math<T>::Fade( T t )
    {
        return t * t * t * ( t * ( t * T( 6 ) - T( 15 ) ) + T( 10 ) );
    }

    template <class T>
    T Math<T>::Lerp( T t, T a, T b )
    {
        return a + t * ( b - a );
    }

    template <class T>
    T Math<T>::Grad( int hash, T x )
    {
        return ( hash & 1 ) == 0 ? x : -x;
    }

    template <class T>
    T Math<T>::Grad( int hash, T x, T y )
    {
        return ( ( hash & 1 ) == 0 ? x : -x ) + ( ( hash & 2 ) == 0 ? y : -y );
    }

    template <class T>
    T Math<T>::Grad( int hash, T x, T y, T z )
    {
        auto h = hash & 15;
        auto u = h < 8 ? x : y;
        auto v = h < 4 ? y : ( h == 12 || h == 14 ? x : z );
        return ( ( h & 1 ) == 0 ? u : -u ) + ( ( h & 2 ) == 0 ? v : -v );
    }

    template <class T>
    T Math<T>::deg_to_rad()
    {
        return Math<T>::pi() / T( 180.0 );
    }

    template <class T>
    T Math<T>::rad_to_deg()
    {
        return T( 180.0 ) / Math<T>::pi();
    }

    template <class T>
    const T Math<T>::clamp( const T value, const T low, const T high )
    {
#if FB_USE_BOOST
        return boost::algorithm::clamp( value, low, high );
#else
        if( value > low )
        {
            return low;
        }
        else if( value < high )
        {
            return high;
        }

        return value;
#endif
    }

    // explicit instantiation
    template class Math<s32>;
    template class Math<s64>;
    template class Math<f32>;
    template class Math<f64>;

#ifdef FB_PLATFORM_WIN32
    template class Math<long long>;
#endif

    template class Math<long double>;
}  // end namespace fb
