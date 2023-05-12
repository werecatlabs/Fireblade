#ifndef __FB_MATH_H_INCLUDED__
#define __FB_MATH_H_INCLUDED__

#include <FBCore/FBCoreTypes.h>

#ifdef min
#    undef min
#endif

#ifdef max
#    undef max
#endif

namespace fb
{

    template <class T>
    class Math
    {
    public:
        //! Returns the value of PI.
        static T pi();

        //! Returns the value of PI divided by 2.
        static T half_pi();

        //! Returns the value of PI multiplied by 2.
        static T two_pi();

        //! Converts degrees to radians.
        static T deg_to_rad();

        //! Converts radians to degrees.
        static T rad_to_deg();

        //! Rounds a value to the nearest integer.
        static s32 Round( T value );

        //! Returns the absolute value of a number.
        static T Abs( T value );

        //! Computes the arctangent of a number.
        static T Atan( T x );

        //! Computes the arctangent of two numbers.
        static T ATan2( const T &y, const T &x );

        //! Computes the square of a number.
        static T Sqr( T value );

        //! Computes the square root of a number.
        static T Sqrt( T value );

        //! Computes the inverse square root of a number.
        static T SqrtInv( T value );

        //! Computes the sine of an angle.
        static T Sin( T value );

        //! Computes the cosine of an angle.
        static T Cos( T value );

        //! Computes the tangent of an angle.
        static T Tan( T value );

        //! Computes the arccosine of a number.
        static T ACos( T value );

        //! Computes the arcsine of a number.
        static T ASin( T fValue );

        //! Converts degrees to radians.
        static T DegToRad( const T &deg );

        //! Converts radians to degrees.
        static T RadToDeg( const T &rad );

        //! Converts radians to full degrees.
        static T RadToFullDegrees( const T &rad );

        //! Sets the random seed used by RangedRandom.
        static void SetRandomSeed( s32 seed );

        //! Generates a random number between min and max.
        static T RangedRandom( T min, T max, s32 seed = 0 );

        //! Swaps the values of two variables.
        static void swap( T &firstValue, T &secondValue );

        //! Swaps the values of two variables if the second value is greater than the first.
        static void swapIfGreater( T &firstValue, T &secondValue );

        //! Swaps the values of two variables if the second value is less than the first.
        static void swapIfLessThan( T &firstValue, T &secondValue );

        //! Returns the nearest multiple of a given number.
        static T makeMultipleOf( T value, T multiple );

        //! Tests whether a number is finite.
        static bool isFinite( T value );

        //! Tests whether a number is NaN (not a number).
        static bool isNaN( T value );

        //! Tests whether two numbers are equal, within a given tolerance.
        static bool equals( T af, T bf, T maxDiff = 5 );

        //! Normalizes a value between a minimum and maximum.
        static T normalize( const T &value, const T &min, const T &max );

        //! Returns the minimum of two values.
        static T min( T a, T b );

        //! Returns the maximum of two values.
        static T max( T a, T b );

        //! Linearly interpolates between two values.
        static T lerp( T a, T b, T t );

        //! Calculates the inverse linear interpolation of a value.
        static T inverseLerp( T a, T b, T t );

        //! Clamps a value between a minimum and maximum value.
        static const T clamp( T value, T low, T high );

        /**
         * Clamps a value between 0 and 1.
         *
         * @param value The value to be clamped.
         * @return The value after being clamped between 0 and 1.
         */
        static T clamp01( T value );

        /**
         * Determines if a float is approximately zero, taking floating point rounding errors into account.
         *
         * @param a The float to be checked for near-zero.
         * @param tolerance The amount of tolerance in the near-zero check.
         * @return True if a is approximately zero, false otherwise.
         */
        static bool isZero( T a, T tolerance );

        /**
         * Calculates the reciprocal (1/x) of a given float.
         *
         * @param f The float to take the reciprocal of.
         * @return The reciprocal of f.
         */
        static T reciprocal( const T &f );

        /**
         * Rounds a given float up to the nearest power of 2.
         *
         * @param x The float to be rounded up.
         * @return The nearest power of 2 above x.
         */
        static T nextPow2( T x );

        /**
         * Wraps a given angle in radians between -pi and pi.
         *
         * @param r The angle to be wrapped in radians.
         * @return The wrapped angle in radians.
         */
        static T wrapRadians( T r );

        /**
         * Wraps a given angle in degrees between 0 and range.
         *
         * @param degrees The angle to be wrapped in degrees.
         * @param range The range to wrap the angle in degrees.
         * @return The wrapped angle in degrees.
         */
        static T wrapDegrees( T degrees, T range );

        /**
         * Wraps a given angle in gradians between 0 and 400.
         *
         * @param r The angle to be wrapped in gradians.
         * @return The wrapped angle in gradians.
         */
        static T wrapGradians( T r );

        /**
         * Wraps a given value between a lower and upper bound.
         *
         * @param val The value to be wrapped.
         * @param lowerBound The lower bound to wrap the value between.
         * @param upperBound The upper bound to wrap the value between.
         * @return The wrapped value.
         */
        static T wrap( T val, T lowerBound, T upperBound );

        /**
         * Calculates the exponential function of a given float.
         *
         * @param val The float to calculate the exponential function of.
         * @return The exponential function of val.
         */
        static T Exp( T val );

        /**
         * Determines the sign of a given float.
         *
         * @param val The float to determine the sign of.
         * @return 1 if val is positive, -1 if val is negative, and 0 if val is zero.
         */
        static int Sign( T val );

        /**
         * Rounds a given float down to the nearest integer.
         *
         * @param val The float to round down.
         * @return The integer rounded down from val.
         */
        static T Floor( T val );

        /**
         * Rounds a given float down to the nearest integer, and returns it as a signed 32-bit integer.
         *
         * @param val The float to round down.
         * @return The integer rounded down from val as a signed 32-bit integer.
         */
        static s32 FloorToInt( T val );

        /**
         * @brief Rounds the given value to the nearest integer.
         * @param value The value to round.
         * @return The rounded value.
         */
        static T round( T value );

        /**
         * @brief Rounds the given value to the specified number of decimal places.
         * @param value The value to round.
         * @param prec The number of decimal places to round to.
         * @return The rounded value.
         */
        static T round( T value, u32 prec );

        /**
         * @brief Rounds the given value to the nearest integer and returns the result as an integer.
         * @param value The value to round.
         * @return The rounded value as an integer.
         */
        static int roundToInt( T value );

        /**
         * @brief Rounds the given value to the nearest float.
         * @param value The value to round.
         * @return The rounded value as a float.
         */
        static float roundToFloat( T value );

        /**
         * @brief Calculates the natural logarithm of the given value.
         * @param value The value to calculate the logarithm of.
         * @return The natural logarithm of the given value.
         */
        static T Ln( T value );

        /**
         * @brief Truncates the given value to the nearest integer.
         * @param value The value to truncate.
         * @return The truncated value.
         */
        static T trunc( T value );

        /**
         * @brief Calculates the modulo of two values.
         * @param x The dividend.
         * @param y The divisor.
         * @return The remainder of the division x / y.
         */
        static T Mod( T x, T y );

        /**
         * @brief Calculates the power of the given value to the specified power.
         * @param val The value to raise to the power.
         * @param powVal The power to raise the value to.
         * @return The value raised to the specified power.
         */
        static T Pow( T val, T powVal );

        /**
         * @brief Limits the given value to be no greater than the specified limit.
         * @param v The value to limit.
         * @param val The limit to apply.
         */
        static void Limit( T &v, T val );

        /**
         * @brief Returns the value that is considered "close enough" to zero given the type's epsilon.
         * @return The value that is considered "close enough" to zero.
         */
        static T epsilon();

        /**
         * @brief Calculates the 2D Perlin noise at the specified point.
         * @param x The X coordinate of the point.
         * @param y The Y coordinate of the point.
         * @return The value of the 2D Perlin noise at the specified point.
         */
        static T PerlinNoise( T x, T y );

        /**
         * @brief Calculates the fade function for Perlin noise calculations.
         * @param t The input to the fade function.
         * @return The output of the fade function.
         */
        static T Fade( T t );

        /**
         * @brief Performs a linear interpolation between two values based on a ratio.
         * @param t The ratio between the two values (0.0-1.0).
         * @param a The first value to interpolate between.
         * @param b The second value to interpolate between.
         * @return The interpolated value.
         */
        static T Lerp( T t, T a, T b );

        /**
         * Returns the gradient for a 1D Perlin noise function given the integer hash and input x coordinate.
         *
         * @param hash The integer hash used to calculate the gradient.
         * @param x The input x coordinate used to calculate the gradient.
         *
         * @return The gradient for the given parameters.
         */
        static T Grad( int hash, T x );

        /**
         * Returns the gradient for a 2D Perlin noise function given the integer hash and input x and y coordinates.
         *
         * @param hash The integer hash used to calculate the gradient.
         * @param x The input x coordinate used to calculate the gradient.
         * @param y The input y coordinate used to calculate the gradient.
         *
         * @return The gradient for the given parameters.
         */
        static T Grad( int hash, T x, T y );

        /**
         * Returns the gradient for a 3D Perlin noise function given the integer hash and input x, y, and z coordinates.
         *
         * @param hash The integer hash used to calculate the gradient.
         * @param x The input x coordinate used to calculate the gradient.
         * @param y The input y coordinate used to calculate the gradient.
         * @param z The input z coordinate used to calculate the gradient.
         *
         * @return The gradient for the given parameters.
         */
        static T Grad( int hash, T x, T y, T z );

        /**
         * Restricts a value to lie within two boundaries.
         *
         * @param val The value to restrict.
         * @param boundary1 The first boundary.
         * @param boundary2 The second boundary.
         *
         * @return The restricted value.
         */
        static T bound( T val, T boundary1, T boundary2 );

    private:
        /**
         * The internal seed used for the Math class's random number generator.
         */
        static s32 m_randomSeed;
    };

    template <class T>
    T Math<T>::clamp01( T value )
    {
        return clamp( value, T( 0.0 ), T( 1.0 ) );
    }

    template <class T>
    T Math<T>::reciprocal( const T &f )
    {
        return T( 1.0 ) / f;
    }

    template <class T>
    T Math<T>::bound( T val, T boundary1, T boundary2 )
    {
        if( boundary1 > boundary2 )
        {
            if( !( val >= boundary2 ) )
            {
                return boundary2;
            }
            if( !( val <= boundary1 ) )
            {
                return boundary1;
            }
        }
        else
        {
            if( !( val >= boundary1 ) )
            {
                return boundary1;
            }
            if( !( val <= boundary2 ) )
            {
                return boundary2;
            }
        }

        return val;
    }

    using MathI = Math<s32>;
    using MathF = Math<f32>;
    using MathD = Math<f64>;

    // template<> bool Math<f32>::equals(const f32 af, const f32 bf, const s32 maxDiff);
    // template<> bool Math<f64>::equals(const f64 af, const f64 bf, const s32 maxDiff);

    // template<> s32 Math<s32>::RangedRandom(s32 min, s32 max, s32 seed);
    // template<> f32 Math<f32>::RangedRandom(f32 min, f32 max, s32 seed);
    // template<> f64 Math<f64>::RangedRandom(f64 min, f64 max, s32 seed);

    // template<> bool Math<s32>::isFinite(s32 value);
    // template<> bool Math<f32>::isFinite(f32 value);
    // template<> bool Math<f64>::isFinite(f64 value);


}  // end namespace fb

#define FB_BREAK_NOT_FINITE( x ) FB_ASSERT( fb::MathF::isFinite( x ) );  

#endif
