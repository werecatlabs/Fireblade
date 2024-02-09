#ifndef __FB_Vector3_H_
#define __FB_Vector3_H_

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Memory/SmartPtr.h>

namespace fb
{

    /** A 3d vector class. */
    template <class T>
    class Vector3
    {
    public:
        /** Default constructor. */
        Vector3();

        /**
         * Constructor.
         * @param x The x-coordinate.
         * @param y The y-coordinate.
         * @param z The z-coordinate.
         */
        Vector3( T x, T y, T z );

        /**
         * Copy constructor.
         * @param other The vector to copy.
         */
        Vector3( const Vector3<T> &other );

        /**
         * Constructor.
         * @param ptr Pointer to an array of 3 coordinates.
         */
        explicit Vector3( const T *ptr );

        // operators
        Vector3<T> operator-() const;

        Vector3<T> &operator=( const Vector3<T> &other );

        Vector3<T> operator+( const Vector3<T> &other ) const;
        Vector3<T> &operator+=( const Vector3<T> &other );

        Vector3<T> operator-( const Vector3<T> &other ) const;
        Vector3<T> &operator-=( const Vector3<T> &other );

        Vector3<T> operator*( const Vector3<T> &other ) const;
        Vector3<T> &operator*=( const Vector3<T> &other );
        Vector3<T> operator*( T v ) const;
        Vector3<T> &operator*=( T v );

        Vector3<T> operator/( const Vector3<T> &other ) const;
        Vector3<T> &operator/=( const Vector3<T> &other );
        Vector3<T> operator/( T v ) const;
        Vector3<T> &operator/=( T v );

        bool operator<=( const Vector3<T> &other ) const;
        bool operator>=( const Vector3<T> &other ) const;
        bool operator<( const Vector3<T> &other ) const;
        bool operator>( const Vector3<T> &other ) const;

        bool operator==( const Vector3<T> &other ) const;
        bool operator!=( const Vector3<T> &other ) const;

        bool operator<( T val ) const;
        bool operator>( T val ) const;

        explicit operator const T *() const;
        explicit operator T *();
        T operator[]( s32 i ) const;
        T &operator[]( s32 i );

        T X() const;
        T &X();

        T Y() const;
        T &Y();

        T Z() const;
        T &Z();

        /**
         * Returns if this vector equals the other one, taking floating point rounding errors into account.
         * @param other The vector to compare to.
         * @param tolerance The error tolerance.
         * @return True if the vectors are equal, false otherwise.
         */
        bool equals( const Vector3<T> &other, T tolerance = T( 0.0000001 ) ) const;

        /**
         * Sets the vector components.
         * @param nx The x-coordinate.
         * @param ny The y-coordinate.
         * @param nz The z-coordinate.
         */
        void set( T nx, T ny, T nz );

        /**
         * Sets the vector components to the values of another vector.
         * @param p The vector to copy.
         */
        void set( const Vector3<T> &p );

        /**
         * Returns the length of the vector.
         * @return The length of the vector.
         */
        T length() const;

        //! Returns squared length of the vector.
        /**
         * This is useful because it is much faster than getLength().
         *
         * \return The squared length of the vector.
         */
        T squaredLength() const;

        //! Returns the dot product with another vector.
        /**
         * Calculates and returns the dot product of this vector with another vector.
         *
         * \param other The other vector to calculate the dot product with.
         * \return The dot product of this vector with the other vector.
         */
        T dotProduct( const Vector3<T> &other ) const;

        //! Returns the absolute dot product with another vector.
        /**
         * Calculates and returns the absolute value of the dot product of this vector with another vector.
         *
         * \param vec The other vector to calculate the dot product with.
         * \return The absolute dot product of this vector with the other vector.
         */
        T dotProductABS( const Vector3<T> &vec ) const;

        //! Returns distance from another point.
        /**
         * Calculates and returns the Euclidean distance between this vector (interpreted as a point in 3-dimensional space)
         * and another point (represented as a 3D vector).
         *
         * \param other The other point to calculate the distance from.
         * \return The distance between this point and the other point.
         */
        T getDistanceFrom( const Vector3<T> &other ) const;

        //! Returns squared distance from another point.
        /**
         * Calculates and returns the squared Euclidean distance between this vector (interpreted as a point in 3-dimensional space)
         * and another point (represented as a 3D vector). This is useful because it is much faster than getDistanceFrom().
         *
         * \param other The other point to calculate the squared distance from.
         * \return The squared distance between this point and the other point.
         */
        T getDistanceFromSQ( const Vector3<T> &other ) const;

        //! Calculates the cross product with another vector.
        /**
         * Calculates and returns the cross product of this vector with another vector.
         *
         * \param p The other vector to calculate the cross product with.
         * \return The cross product of this vector with the other vector.
         */
        Vector3<T> crossProduct( const Vector3<T> &p ) const;

        //! Returns if this vector interpreted as a point is on a line between two other points.
        /**
         * Determines if this vector (interpreted as a point in 3-dimensional space) lies on the line segment between
         * two other points (also represented as 3D vectors). It is assumed that the point is on the line.
         *
         * \param begin The starting point of the line segment.
         * \param end The ending point of the line segment.
         * \return True if this point lies on the line segment, false otherwise.
         */
        bool isBetweenPoints( const Vector3<T> &begin, const Vector3<T> &end ) const;

        /**
         * Normalises the vector.
         *
         * Returns a normalised copy of this vector, which has the same direction but a length of 1.
         *
         * \return A normalised copy of this vector.
         */
        Vector3<T> normaliseCopy() const;

        /** Normalises the vector. */
        Vector3<T> &normaliseFast();

        //! Returns the length of the vector.
        /** \return The length of the vector. */
        T normaliseLength();

        //! Normalizes the vector.
        void normalise();

        //! Sets the length of the vector to a new value.
        /** \param newlength: The new length of the vector. */
        void setLength( T newlength );

        //! Inverts the vector.
        void invert();

        //! Rotates the vector by a specified number of degrees around the Y axis and the specified center.
        /** \param degrees: Number of degrees to rotate around the Y axis.
        \param center: The center of the rotation. */
        void rotateXZBy( T degrees, const Vector3<T> &center );

        //! Rotates the vector by a specified number of degrees around the Z axis and the specified center.
        /** \param degrees: Number of degrees to rotate around the Z axis.
        \param center: The center of the rotation. */
        void rotateXYBy( T degrees, const Vector3<T> &center );

        //! Rotates the vector by a specified number of degrees around the X axis and the specified center.
        /** \param degrees: Number of degrees to rotate around the X axis.
        \param center: The center of the rotation. */
        void rotateYZBy( T degrees, const Vector3<T> &center );

        //! Returns an interpolated vector between this and another vector.
        /** \param other: The other vector to interpolate between.
        \param d: The interpolation value between 0.0f and 1.0f.
        \return The interpolated vector. */
        Vector3<T> getInterpolated( const Vector3<T> &other, T d ) const;

        //! Returns a quadratic interpolated vector between this and two other vectors.
        /** \param other0: The first vector to interpolate between.
        \param other1: The second vector to interpolate between.
        \param factor: The interpolation value between 0.0f and 1.0f.
        \return The quadratic interpolated vector. */
        Vector3<T> getInterpolated_quadratic( const Vector3<T> &other0, const Vector3<T> &other1,
                                              T factor ) const;

        //! Gets the Y and Z rotations of a vector.
        /** \return A vector representing the rotation in degrees of this vector.
        The Z component of the vector will always be 0. */
        Vector3<T> getHorizontalAngle();

        //! Fills an array of 4 values with the vector data (usually floats).
        /** Useful for setting in shader constants, for example.
        \param array: A pointer to an array of 4 values. */
        void getAs4Values( T *array ) const;

        //! Returns true if this vector has a length of 0.
        /** \return True if the length of this vector is 0, false otherwise. */
        bool isZeroLength() const;

        //! Causes negative members to become positive.
        void makeAbs();

        /** Sets this vector's components to the minimum of its own and the
            ones of the passed in vector.
            @remarks
                'Minimum' in this case means the combination of the lowest
                value of x, y and z from both vectors. Lowest is taken just
                numerically, not magnitude, so -1 < 0.
        */
        void makeFloor( const Vector3 &cmp );

        /** Sets this vector's components to the maximum of its own and the
            ones of the passed in vector.
            @remarks
                'Maximum' in this case means the combination of the highest
                value of x, y and z from both vectors. Highest is taken just
                numerically, not magnitude, so 1 > -3.
        */
        void makeCeil( const Vector3 &cmp );

        //! Returns a vector which is perpendicular to this vector and the other vector.
        Vector3 perpendicular( void ) const;

        //! Returns the midpoint between this and the passed vector.
        Vector3 midPoint( const Vector3<T> &vec ) const;

        //! Pointer accessor for direct copying
        T *ptr();

        //! Pointer accessor for direct copying
        const T *ptr() const;

        //! Returns a zero vector.
        static Vector3 zero();

        //! Returns a unit vector.
        static Vector3 unit();

        //! Returns a unit vector pointing along the positive X axis.
        static Vector3 unitX();

        //! Returns a unit vector pointing along the positive Y axis.
        static Vector3 unitY();

        //! Returns a unit vector pointing along the positive Z axis.
        static Vector3 unitZ();

        //! Returns a positive vector along the x axis.
        static Vector3 positiveX();

        //! Returns a negative vector along the x axis.
        static Vector3 negativeX();

        //! Returns a positive vector along the y axis.
        static Vector3 positiveY();

        //! Returns a negative vector along the y axis.
        static Vector3 negativeY();

        //! Returns a positive vector along the z axis.
        static Vector3 positiveZ();

        //! Returns a negative vector along the z axis.
        static Vector3 negativeZ();

        //! Returns a unit vector pointing up (0,1,0).
        static Vector3 up();

        //! Returns a unit vector pointing down (0,-1,0).
        static Vector3 down();

        //! Returns a unit vector pointing right (1,0,0).
        static Vector3 right();

        //! Returns a unit vector pointing left (-1,0,0).
        static Vector3 left();

        //! Returns a unit vector pointing backward (0,0,1).
        static Vector3 back();

        //! Returns a unit vector pointing forward (0,0,-1).
        static Vector3 forward();

        /** Creates a Vector3 from spherical coordinates.
         * @param radius The distance from the origin to the point.
         * @param latitude The angle between the point and the xz-plane, in radians.
         * @param longitude The angle between the point and the positive z-axis, in radians.
         */
        static Vector3 fromCoords( T radius, T latitude, T longitude );

        /** Checks whether the vector's elements are valid numbers (not NaN or infinity).
         * @return True if the vector is valid, false otherwise.
         */
        bool isValid() const;

        /** Checks whether the vector's elements are finite numbers (not NaN or infinity).
         * @return True if the vector is finite, false otherwise.
         */
        bool isFinite() const;

        /** Generates a complement basis for a given vector w.
         * @param u On output, the first basis vector.
         * @param v On output, the second basis vector.
         * @param w The original vector to generate the basis from.
         */
        static void generateComplementBasis( Vector3<T> &u, Vector3<T> &v, const Vector3<T> &w );

        /** The zero vector. */
        FBCore_API static const Vector3 ZERO;

        /** The x-axis unit vector. */
        FBCore_API static const Vector3 UNIT_X;

        /** The y-axis unit vector. */
        FBCore_API static const Vector3 UNIT_Y;

        /** The z-axis unit vector. */
        FBCore_API static const Vector3 UNIT_Z;

        /** The unit vector. */
        FBCore_API static const Vector3 UNIT;

        /** The x-component of the vector. */
        T x;

        /** The y-component of the vector. */
        T y;

        /** The z-component of the vector. */
        T z;

    private:
        /** Compares this vector with another vector.
         * @param other The vector to compare this vector with.
         * @return 0 if the vectors are equal, a negative value if this vector is less than other,
         *         or a positive value if this vector is greater than other.
         */
        s32 compare( const Vector3 &other ) const;
    };

    template <class T>
    Vector3<T>::Vector3() : x( 0 ), y( 0 ), z( 0 )
    {
    }

    template <class T>
    Vector3<T>::Vector3( T x, T y, T z ) : x( x ), y( y ), z( z )
    {
    }

    template <class T>
    Vector3<T>::Vector3( const Vector3<T> &other )
    {
        *this = other;
    }

    template <class T>
    Vector3<T>::Vector3( const T *ptr )
    {
        auto p = this->ptr();
        p[0] = ptr[0];
        p[1] = ptr[1];
        p[2] = ptr[2];
    }

    //
    // operators
    //

    template <class T>
    Vector3<T> Vector3<T>::operator-() const
    {
        return Vector3<T>( -x, -y, -z );
    }

    template <class T>
    Vector3<T> &Vector3<T>::operator=( const Vector3<T> &other )
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    template <class T>
    Vector3<T> Vector3<T>::operator+( const Vector3<T> &other ) const
    {
        return Vector3<T>( x + other.x, y + other.y, z + other.z );
    }

    template <class T>
    Vector3<T> &Vector3<T>::operator+=( const Vector3<T> &other )
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template <class T>
    Vector3<T> Vector3<T>::operator-( const Vector3<T> &other ) const
    {
        return Vector3<T>( x - other.x, y - other.y, z - other.z );
    }

    template <class T>
    Vector3<T> &Vector3<T>::operator-=( const Vector3<T> &other )
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    template <class T>
    Vector3<T> Vector3<T>::operator*( const Vector3<T> &other ) const
    {
        return Vector3<T>( x * other.x, y * other.y, z * other.z );
    }

    template <class T>
    Vector3<T> &Vector3<T>::operator*=( const Vector3<T> &other )
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    template <class T>
    Vector3<T> Vector3<T>::operator*( const T v ) const
    {
        return Vector3<T>( x * v, y * v, z * v );
    }

    template <class T>
    Vector3<T> &Vector3<T>::operator*=( const T v )
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    template <class T>
    Vector3<T> Vector3<T>::operator/( const Vector3<T> &other ) const
    {
        return Vector3<T>( x / other.x, y / other.y, z / other.z );
    }

    template <class T>
    Vector3<T> &Vector3<T>::operator/=( const Vector3<T> &other )
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    template <class T>
    Vector3<T> Vector3<T>::operator/( const T v ) const
    {
        auto invValue = T( 1.0 ) / v;
        return Vector3<T>( x * invValue, y * invValue, z * invValue );
    }

    template <class T>
    Vector3<T> &Vector3<T>::operator/=( const T v )
    {
        auto invValue = T( 1.0 ) / v;
        x *= invValue;
        y *= invValue;
        z *= invValue;
        return *this;
    }

    template <class T>
    bool Vector3<T>::operator<=( const Vector3<T> &other ) const
    {
        return compare( other ) <= 0;
    }

    template <class T>
    bool Vector3<T>::operator>=( const Vector3<T> &other ) const
    {
        return compare( other ) >= 0;
    }

    template <class T>
    bool Vector3<T>::operator<( const Vector3<T> &other ) const
    {
        return compare( other ) < 0;
    }

    template <class T>
    bool Vector3<T>::operator>( const Vector3<T> &other ) const
    {
        return compare( other ) > 0;
    }

    template <class T>
    bool Vector3<T>::operator<( const T val ) const
    {
        return squaredLength() < val;
    }

    template <class T>
    bool Vector3<T>::operator>( const T val ) const
    {
        return squaredLength() > val;
    }

    template <class T>
    Vector3<T>::operator const T *() const
    {
        const auto p = ptr();
        return p;
    }

    template <class T>
    Vector3<T>::operator T *()
    {
        auto p = ptr();
        return p;
    }

    template <class T>
    T Vector3<T>::operator[]( s32 i ) const
    {
        const auto p = ptr();
        return p[i];
    }

    template <class T>
    T &Vector3<T>::operator[]( s32 i )
    {
        auto p = ptr();
        return p[i];
    }

    template <class T>
    T Vector3<T>::X() const
    {
        return x;
    }

    template <class T>
    T &Vector3<T>::X()
    {
        return x;
    }

    template <class T>
    T Vector3<T>::Y() const
    {
        return y;
    }

    template <class T>
    T &Vector3<T>::Y()
    {
        return y;
    }

    template <class T>
    T Vector3<T>::Z() const
    {
        return z;
    }

    template <class T>
    T &Vector3<T>::Z()
    {
        return z;
    }

    template <class T>
    bool Vector3<T>::equals( const Vector3<T> &other, T tolerance ) const
    {
        FB_UNUSED( tolerance );

        return Math<T>::equals( x, other.x ) && Math<T>::equals( y, other.y ) &&
               Math<T>::equals( z, other.z );
    }

    template <class T>
    void Vector3<T>::set( const T x, const T y, const T z )
    {
        auto p = ptr();
        p[0] = x;
        p[1] = y;
        p[2] = z;
    }

    template <class T>
    void Vector3<T>::set( const Vector3<T> &other )
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    template <class T>
    T Vector3<T>::length() const
    {
        return Math<T>::Sqrt( squaredLength() );
    }

    template <class T>
    T Vector3<T>::squaredLength() const
    {
        return x * x + y * y + z * z;
    }

    template <class T>
    T Vector3<T>::dotProduct( const Vector3<T> &other ) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    template <class T>
    T fb::Vector3<T>::dotProductABS( const Vector3<T> &vec ) const
    {
        return Math<T>::Abs( x * vec.x ) + Math<T>::Abs( y * vec.y ) + Math<T>::Abs( z * vec.z );
    }

    template <class T>
    T Vector3<T>::getDistanceFrom( const Vector3<T> &other ) const
    {
        return Vector3<T>( x - other.x, y - other.y, z - other.z ).length();
    }

    template <class T>
    T Vector3<T>::getDistanceFromSQ( const Vector3<T> &other ) const
    {
        return Vector3<T>( x - other.x, y - other.y, z - other.z ).squaredLength();
    }

    template <class T>
    Vector3<T> Vector3<T>::crossProduct( const Vector3<T> &p ) const
    {
        return Vector3<T>( y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x );
    }

    template <class T>
    bool Vector3<T>::isBetweenPoints( const Vector3<T> &begin, const Vector3<T> &end ) const
    {
        T f = ( end - begin ).squaredLength();
        return getDistanceFromSQ( begin ) < f && getDistanceFromSQ( end ) < f;
    }

    template <class T>
    Vector3<T> Vector3<T>::normaliseCopy() const
    {
        auto ret = *this;
        ret.normalise();
        return ret;
    }

    template <class T>
    Vector3<T> &Vector3<T>::normaliseFast()
    {
        T lengthSQ = x * x + y * y + z * z;
        if( lengthSQ < Math<T>::epsilon() )
        {
            return *this;
        }

        T l = Math<T>::SqrtInv( lengthSQ );
        x *= l;
        y *= l;
        z *= l;
        return *this;
    }

    template <class T>
    T Vector3<T>::normaliseLength()
    {
        T lengthSQ = squaredLength();
        if( lengthSQ < Math<T>::epsilon() )
        {
            return T( 0.0 );
        }

        T l = Math<T>::SqrtInv( lengthSQ );
        x *= l;
        y *= l;
        z *= l;
        return T( 1.0 ) / l;
    }

    template <class T>
    void Vector3<T>::normalise()
    {
        T lengthSQ = squaredLength();
        if( lengthSQ < Math<T>::epsilon() )
        {
            return;
        }

        T l = Math<T>::Sqrt( lengthSQ );
        x /= l;
        y /= l;
        z /= l;
    }

    template <class T>
    void Vector3<T>::setLength( T newlength )
    {
        normalise();
        *this *= newlength;
    }

    template <class T>
    void Vector3<T>::invert()
    {
        x *= T( -1.0 );
        y *= T( -1.0 );
        z *= T( -1.0 );
    }

    template <class T>
    Vector3<T> Vector3<T>::getInterpolated( const Vector3<T> &other, const T d ) const
    {
        const T inv = static_cast<T>( 1.0 ) - d;
        return Vector3<T>( other.X() * inv + X() * d, other.Y() * inv + Y() * d,
                           other.Z() * inv + Z() * d );
    }

    template <class T>
    Vector3<T> Vector3<T>::getInterpolated_quadratic( const Vector3<T> &v2, const Vector3<T> &v3,
                                                      const T d ) const
    {
        // this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
        const T inv = static_cast<T>( 1.0 ) - d;
        const T mul0 = inv * inv;
        const T mul1 = static_cast<T>( 2.0 ) * d * inv;
        const T mul2 = d * d;

        return Vector3<T>( X() * mul0 + v2.X() * mul1 + v3.X() * mul2,
                           Y() * mul0 + v2.Y() * mul1 + v3.Y() * mul2,
                           Z() * mul0 + v2.Z() * mul1 + v3.Z() * mul2 );
    }

    template <class T>
    Vector3<T> Vector3<T>::getHorizontalAngle()
    {
        Vector3<T> angle;

        angle.Y() = Math<T>::ATan2( X(), Z() );
        angle.Y() *= Math<T>::rad_to_deg();

        if( angle.Y() < T( 0.0 ) )
        {
            angle.Y() += T( 360.0 );
        }

        if( angle.Y() >= T( 360.0 ) )
        {
            angle.Y() -= T( 360.0 );
        }

        auto z1 = Math<T>::Sqrt( X() * X() + Z() * Z() );

        angle.X() = Math<T>::ATan2( z1, Y() );
        angle.X() *= Math<T>::rad_to_deg();
        angle.X() -= T( 90.0 );

        if( angle.X() < T( 0.0 ) )
        {
            angle.X() += T( 360.0 );
        }

        if( angle.X() >= T( 360.0 ) )
        {
            angle.X() -= T( 360.0 );
        }

        return angle;
    }

    template <class T>
    void Vector3<T>::getAs4Values( T *array ) const
    {
        array[0] = x;
        array[1] = y;
        array[2] = z;
        array[3] = T( 0.0 );
    }

    template <class T>
    bool Vector3<T>::isZeroLength() const
    {
        auto sqlen = ( x * x ) + ( y * y ) + ( z * z );

        return sqlen < Math<T>::epsilon();
    }

    template <class T>
    s32 Vector3<T>::compare( const Vector3 &other ) const
    {
        return Memory::Memcmp( ptr(), other.ptr(), 3 * sizeof( T ) );
    }

    template <class T>
    T *Vector3<T>::ptr()
    {
        return &x;
    }

    template <class T>
    const T *Vector3<T>::ptr() const
    {
        return &x;
    }

    template <class T>
    void Vector3<T>::makeAbs()
    {
        x = Math<T>::Abs( x );
        y = Math<T>::Abs( y );
        z = Math<T>::Abs( z );
    }

    template <class T>
    void Vector3<T>::makeFloor( const Vector3 &cmp )
    {
        x = std::min( x, cmp.x );
        y = std::min( y, cmp.y );
        z = std::min( z, cmp.z );
    }

    template <class T>
    void Vector3<T>::makeCeil( const Vector3<T> &cmp )
    {
        x = std::max( x, cmp.x );
        y = std::max( y, cmp.y );
        z = std::max( z, cmp.z );
    }

    template <class T>
    Vector3<T> fb::Vector3<T>::perpendicular( void ) const
    {
        static const auto fSquareZero = (T)( 1e-06 * 1e-06 );

        auto perp = this->crossProduct( Vector3::UNIT_X );

        // Check length
        if( perp.squaredLength() < fSquareZero )
        {
            /* This vector is the Y axis multiplied by a scalar, so we have
               to use another axis.
            */
            perp = this->crossProduct( Vector3::UNIT_Y );
        }

        perp.normalise();

        return perp;
    }

    template <class T>
    Vector3<T> fb::Vector3<T>::midPoint( const Vector3<T> &vec ) const
    {
        return Vector3<T>( ( x + vec.x ) * T( 0.5 ), ( y + vec.y ) * T( 0.5 ),
                           ( z + vec.z ) * T( 0.5 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::zero()
    {
        return Vector3<T>( T( 0.0 ), T( 0.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::unit()
    {
        return Vector3<T>( T( 1.0 ), T( 1.0 ), T( 1.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::unitX()
    {
        return Vector3<T>( T( 1.0 ), T( 0.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::unitY()
    {
        return Vector3<T>( T( 0.0 ), T( 1.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::unitZ()
    {
        return Vector3<T>( T( 0.0 ), T( 0.0 ), T( 1.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::positiveX()
    {
        return Vector3<T>( T( 1.0 ), T( 0.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::negativeX()
    {
        return Vector3<T>( T( -1.0 ), T( 0.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::positiveY()
    {
        return Vector3<T>( T( 0.0 ), T( 1.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::negativeY()
    {
        return Vector3<T>( T( 0.0 ), T( -1.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::positiveZ()
    {
        return Vector3<T>( T( 0.0 ), T( 0.0 ), T( 1.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::negativeZ()
    {
        return Vector3<T>( T( 0.0 ), T( 0.0 ), T( -1.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::up()
    {
        return Vector3<T>( T( 0.0 ), T( 1.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::down()
    {
        return Vector3<T>( T( 0.0 ), T( -1.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::right()
    {
        return Vector3<T>( T( 1.0 ), T( 0.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::left()
    {
        return Vector3<T>( T( -1.0 ), T( 0.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::back()
    {
        return Vector3<T>( T( 0.0 ), T( 0.0 ), T( 1.0 ) );
    }

    template <class T>
    Vector3<T> Vector3<T>::forward()
    {
        return Vector3<T>( T( 0.0 ), T( 0.0 ), T( -1.0 ) );
    }

    /**
     * @brief Scalar multiplication of a vector with a scalar value.
     * @tparam S The type of the scalar value.
     * @tparam T The type of the vector elements.
     * @param scalar The scalar value to multiply with.
     * @param vector The vector to multiply.
     * @return The result of scalar multiplication.
     */
    template <class S, class T>
    Vector3<T> operator*( S scalar, const Vector3<T> &vector )
    {
        return vector * scalar;
    }

    /**
     * @brief A typedef for a 3-dimensional vector with integer elements.
     */
    using Vector3I = Vector3<s32>;

    /**
     * @brief A typedef for a 3-dimensional vector with single-precision floating-point elements.
     */
    using Vector3F = Vector3<f32>;

    /**
     * @brief A typedef for a 3-dimensional vector with double-precision floating-point elements.
     */
    using Vector3D = Vector3<f64>;

}  // end namespace fb

#endif
