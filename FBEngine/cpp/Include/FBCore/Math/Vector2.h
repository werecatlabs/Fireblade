#ifndef Vector2_h__
#define Vector2_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Memory/SmartPtr.h>

namespace fb
{

    /** A 2d vector class. */
    template <class T>
    class Vector2
    {
    public:
        /** Default constructor. */
        Vector2();

        /**
         * Constructs a Vector2 from its x and y components.
         *
         * @param x The x component of the Vector2.
         * @param y The y component of the Vector2.
         */
        Vector2( T x, T y );

        /**
         * Constructs a Vector2 from another Vector2.
         *
         * @param other The other Vector2 to construct from.
         */
        Vector2( const Vector2<T> &other );

        // operators
        Vector2<T> operator-() const;

        Vector2<T> &operator=( const Vector2<T> &other );

        Vector2<T> operator+( const Vector2<T> &other ) const;
        Vector2<T> &operator+=( const Vector2<T> &other );

        Vector2<T> operator-( const Vector2<T> &other ) const;
        Vector2<T> &operator-=( const Vector2<T> &other );

        Vector2<T> operator*( const Vector2<T> &other ) const;
        Vector2<T> &operator*=( const Vector2<T> &other );
        Vector2<T> operator*( T v ) const;
        Vector2<T> &operator*=( T v );

        Vector2<T> operator/( const Vector2<T> &other ) const;
        Vector2<T> &operator/=( const Vector2<T> &other );
        Vector2<T> operator/( T v ) const;
        Vector2<T> &operator/=( T v );

        bool operator<=( const Vector2<T> &other ) const;
        bool operator>=( const Vector2<T> &other ) const;

        bool operator<( const Vector2<T> &other ) const;
        bool operator>( const Vector2<T> &other ) const;

        bool operator==( const Vector2<T> &other ) const;
        bool operator!=( const Vector2<T> &other ) const;

        // member access
        explicit operator const T *() const;
        explicit operator T *();
        T operator[]( s32 i ) const;
        T &operator[]( s32 i );

        T X() const;
        T &X();

        T Y() const;
        T &Y();

        /**
         * Returns whether this vector is equal to another vector within a
         * small margin of error.
         *
         * @param other The other vector to compare to.
         * @return Whether the vectors are approximately equal.
         */
        bool equals( const Vector2<T> &other ) const;

        /**
         * Sets the x and y components of this vector.
         *
         * @param nx The new x component.
         * @param ny The new y component.
         */
        void set( T nx, T ny );

        /**
         * Sets this vector to be the same as another vector.
         *
         * @param p The vector to copy from.
         */
        void set( const Vector2<T> &p );

        /**
         * Returns the length of the vector.
         *
         * @return The length of the vector.
         */
        T length() const;

        /**
         * Returns the squared length of the vector.
         *
         * @return The squared length of the vector.
         */
        T squaredLength() const;

        /**
         * Returns the dot product of this vector with another vector.
         *
         * @param other The other vector.
         * @return The dot product of the two vectors.
         */
        T dotProduct( const Vector2<T> &other ) const;

        //! Returns distance from another point. Here, the vector is interpreted as
        //! point in 2 dimensional space.
        T getDistanceFrom( const Vector2<T> &other ) const;

        //! Returns squared distance from another point. Here, the vector is interpreted as
        //! point in 2 dimensional space.
        T getDistanceFromSQ( const Vector2<T> &other ) const;

        //! rotates the point around a center by an amount of degrees.
        void rotateBy( T degrees, const Vector2<T> &center );

        //! normalizes the vector.
        void normalise();

        //! normalizes the vector.
        T normaliseLength();

        //! normalizes the vector and return a reference to it self.
        Vector2<T> normaliseCopy() const;

        //! Sets the length of the vector to a new value
        void setLength( T newlength );

        // returns (y,-x)
        Vector2<T> perp() const;

        // returns (y,-x)/sqrt(x*x+y*y)
        Vector2<T> unitPerp() const;

        // returns DotPerp((x,y),(V.x,V.y)) = x*V.y - y*V.x
        T dotPerp( const Vector2<T> &other ) const;

        //! Calculates the angle of this vector in grad in the trigonometric sense.
        //! This method has been suggested by Pr3t3nd3r.
        //! \return Returns a value between 0 and 360.
        T getAngleTrig() const;

        //! Calculates the angle of this vector in grad in the counter trigonometric sense.
        //! \return Returns a value between 0 and 360.
        T getAngle() const;

        //! Calculates the angle between this vector and another one in grad.
        //! \return Returns a value between 0 and 90.
        T getAngleWith( const Vector2<T> &b ) const;

        //! Returns if this vector interpreted as a point is on a line between two other points.
        /** It is assumed that the point is on the line. */
        //! \param begin: Beginning vector to compare between.
        //! \param end: Ending vector to compare between.
        //! \return True if this vector is between begin and end.  False if not.
        bool isBetweenPoints( const Vector2<T> &begin, const Vector2<T> &end ) const;

        //! returns interpolated vector
        //! \param other: other vector to interpolate between
        //! \param d: value between 0.0f and 1.0f.
        Vector2<T> getInterpolated( const Vector2<T> &other, T d ) const;

        //! Returns interpolated vector. ( quadratic )
        /** \param other0: other vector to interpolate between
        \param other1: other vector to interpolate between
        \param factor: value between 0.0f and 1.0f. */
        Vector2<T> getInterpolated_quadratic( const Vector2<T> &v2, const Vector2<T> &v3, T d ) const;

        //! sets this vector to the interpolated vector between a and b.
        void interpolate( const Vector2<T> &a, const Vector2<T> &b, T t );

        /** Data. */
        T *ptr();

        /** Const data. */
        const T *ptr() const;

        /** Returns a zero vector. */
        static Vector2 zero();

        /** Returns a unit vector. */
        static Vector2 unit();

        /** Checks if the vector's elements have a valid value. */
        bool isValid() const;

        /** Checks if the vector's elements are finite. */
        bool isFinite() const;

        FBCore_API static const Vector2 ZERO;
        FBCore_API static const Vector2 UNIT_X;
        FBCore_API static const Vector2 UNIT_Y;
        FBCore_API static const Vector2 UNIT;

        /** The x-component of the vector. */
        T x;

        /** The y-component of the vector. */
        T y;

    private:
        s32 compare( const Vector2 &other ) const;
    };

    template <class T>
    Vector2<T>::Vector2() : x( 0 ), y( 0 )
    {
    }

    template <class T>
    Vector2<T>::Vector2( T x, T y ) : x( x ), y( y )
    {
        FB_ASSERT( Math<T>::isFinite( x ) );
        FB_ASSERT( Math<T>::isFinite( y ) );
    }

    template <class T>
    Vector2<T>::Vector2( const Vector2<T> &other )
    {
        FB_ASSERT( other.isValid() );

        x = other.x;
        y = other.y;
    }

    template <class T>
    Vector2<T> Vector2<T>::operator-() const
    {
        FB_ASSERT( isValid() );
        return Vector2<T>( -x, -y );
    }

    template <class T>
    Vector2<T> &Vector2<T>::operator=( const Vector2<T> &other )
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        x = other.x;
        y = other.y;
        return *this;
    }

    template <class T>
    Vector2<T> Vector2<T>::operator+( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return Vector2<T>( x + other.x, y + other.y );
    }

    template <class T>
    Vector2<T> &Vector2<T>::operator+=( const Vector2<T> &other )
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        x += other.x;
        y += other.y;
        return *this;
    }

    template <class T>
    Vector2<T> Vector2<T>::operator-( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return Vector2<T>( x - other.x, y - other.y );
    }

    template <class T>
    Vector2<T> &Vector2<T>::operator-=( const Vector2<T> &other )
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        x -= other.x;
        y -= other.y;
        return *this;
    }

    template <class T>
    Vector2<T> Vector2<T>::operator*( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return Vector2<T>( x * other.x, y * other.y );
    }

    template <class T>
    Vector2<T> &Vector2<T>::operator*=( const Vector2<T> &other )
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        x *= other.x;
        y *= other.y;
        return *this;
    }

    template <class T>
    Vector2<T> Vector2<T>::operator*( const T v ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( Math<T>::isFinite( v ) );

        return Vector2<T>( x * v, y * v );
    }

    template <class T>
    Vector2<T> &Vector2<T>::operator*=( const T v )
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( Math<T>::isFinite( v ) );

        x *= v;
        y *= v;
        return *this;
    }

    template <class T>
    Vector2<T> Vector2<T>::operator/( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return Vector2<T>( x / other.x, y / other.y );
    }

    template <class T>
    Vector2<T> &Vector2<T>::operator/=( const Vector2<T> &other )
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        x /= other.x;
        y /= other.y;
        return *this;
    }

    template <class T>
    Vector2<T> Vector2<T>::operator/( const T v ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( Math<T>::isFinite( v ) );

        return Vector2<T>( x / v, y / v );
    }

    template <class T>
    Vector2<T> &Vector2<T>::operator/=( const T v )
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( Math<T>::isFinite( v ) );

        x /= v;
        y /= v;
        return *this;
    }

    template <class T>
    bool Vector2<T>::operator<=( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return compare( other ) <= 0;
    }

    template <class T>
    bool Vector2<T>::operator>=( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return compare( other ) >= 0;
    }

    template <class T>
    bool Vector2<T>::operator<( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return compare( other ) < 0;
    }

    template <class T>
    bool Vector2<T>::operator>( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return compare( other ) > 0;
    }

    template <class T>
    bool Vector2<T>::operator==( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return Math<T>::equals( x, other.x ) && Math<T>::equals( y, other.y );
    }

    template <class T>
    bool Vector2<T>::operator!=( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return !Math<T>::equals( x, other.x ) || !Math<T>::equals( y, other.y );
    }

    template <class T>
    Vector2<T>::operator const T *() const
    {
        return &x;
    }

    template <class T>
    Vector2<T>::operator T *()
    {
        return &x;
    }

    template <class T>
    T &Vector2<T>::operator[]( s32 i )
    {
        FB_ASSERT( i < 2 );
        auto values = ptr();
        return values[i];
    }

    template <class T>
    T Vector2<T>::operator[]( s32 i ) const
    {
        FB_ASSERT( i < 2 );
        auto values = ptr();
        return values[i];
    }

    template <class T>
    T Vector2<T>::X() const
    {
        return x;
    }

    template <class T>
    T &Vector2<T>::X()
    {
        return x;
    }

    template <class T>
    T Vector2<T>::Y() const
    {
        return y;
    }

    template <class T>
    T &Vector2<T>::Y()
    {
        return y;
    }

    template <class T>
    bool Vector2<T>::equals( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return Math<T>::equals( x, other.x ) && Math<T>::equals( y, other.y );
    }

    template <class T>
    void Vector2<T>::set( T nx, T ny )
    {
        FB_ASSERT( Math<T>::isFinite( nx ) );
        FB_ASSERT( Math<T>::isFinite( ny ) );
        FB_ASSERT( isValid() );

        x = nx;
        y = ny;
    }

    template <class T>
    void Vector2<T>::set( const Vector2<T> &p )
    {
        FB_ASSERT( p.isValid() );
        x = p.x;
        y = p.y;
    }

    template <class T>
    T Vector2<T>::length() const
    {
        FB_ASSERT( isValid() );
        return Math<T>::Sqrt( x * x + y * y );
    }

    template <class T>
    T Vector2<T>::squaredLength() const
    {
        FB_ASSERT( isValid() );
        return x * x + y * y;
    }

    template <class T>
    T Vector2<T>::dotProduct( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        return x * other.x + y * other.y;
    }

    template <class T>
    T Vector2<T>::getDistanceFrom( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        auto d = Vector2<T>( x - other.x, y - other.y );
        return d.length();
    }

    template <class T>
    T Vector2<T>::getDistanceFromSQ( const Vector2<T> &other ) const
    {
        FB_ASSERT( isValid() );
        FB_ASSERT( other.isValid() );

        auto d = Vector2<T>( x - other.x, y - other.y );
        return d.squaredLength();
    }

    template <class T>
    void Vector2<T>::rotateBy( T degrees, const Vector2<T> &center )
    {
        degrees *= Math<T>::deg_to_rad();
        auto cs = Math<T>::Cos( degrees );
        auto sn = Math<T>::Sin( degrees );

        X() -= center.X();
        Y() -= center.Y();

        set( X() * cs - Y() * sn, X() * sn + Y() * cs );

        X() += center.X();
        Y() += center.Y();
    }

    template <class T>
    void Vector2<T>::normalise()
    {
        auto lengthSQ = squaredLength();
        if( lengthSQ < Math<T>::epsilon() )
        {
            return;
        }

        auto l = Math<T>::SqrtInv( lengthSQ );
        x *= l;
        y *= l;
    }

    template <class T>
    T Vector2<T>::normaliseLength()
    {
        auto lengthSQ = squaredLength();
        if( lengthSQ < Math<T>::epsilon() )
        {
            return T( 0.0 );
        }

        auto l = Math<T>::SqrtInv( lengthSQ );
        x *= l;
        y *= l;

        return T( 1.0 ) / l;
    }

    template <class T>
    Vector2<T> Vector2<T>::normaliseCopy() const
    {
        auto lengthSQ = squaredLength();
        if( lengthSQ < Math<T>::epsilon() )
        {
            return *this;
        }

        auto l = Math<T>::SqrtInv( lengthSQ );
        return Vector2<T>( x * l, y * l );
    }

    template <class T>
    void Vector2<T>::setLength( T newlength )
    {
        normalise();
        *this *= newlength;
    }

    template <class T>
    Vector2<T> Vector2<T>::perp() const
    {
        return Vector2<T>( y, -x );
    }

    template <class T>
    Vector2<T> Vector2<T>::unitPerp() const
    {
        Vector2<T> perp( y, -x );
        perp.normalise();
        return perp;
    }

    template <class T>
    T Vector2<T>::dotPerp( const Vector2<T> &other ) const
    {
        return x * other.y - y * other.x;
    }

    template <class T>
    T Vector2<T>::getAngleTrig() const
    {
        if( Math<T>::equals( X(), T( 0.0 ) ) )
        {
            return Y() < T( 0.0 ) ? T( 270.0 ) : T( 90.0 );
        }

        if( Math<T>::equals( Y(), T( 0.0 ) ) )
        {
            return X() < T( 0.0 ) ? T( 180.0 ) : T( 0.0 );
        }

        if( Y() > T( 0.0 ) )
        {
            if( X() > T( 0.0 ) )
            {
                return Math<T>::Atan( Y() / X() ) * Math<T>::rad_to_deg();
            }

            return T( 180.0 ) - Math<T>::Atan( Y() / -X() ) * Math<T>::rad_to_deg();
        }

        if( X() > T( 0.0 ) )
        {
            return T( 360.0 ) - Math<T>::Atan( -Y() / X() ) * Math<T>::rad_to_deg();
        }

        return T( 180.0 ) + Math<T>::Atan( -Y() / -X() ) * Math<T>::rad_to_deg();
    }

    template <class T>
    T Vector2<T>::getAngle() const
    {
        if( Math<T>::equals( Y(), T( 0.0 ) ) )
        {
            return X() < T( 0.0 ) ? T( 180.0 ) : T( 0.0 );
        }

        if( Math<T>::equals( X(), T( 0.0 ) ) )
        {
            return Y() < T( 0.0 ) ? T( 90.0 ) : T( 270.0 );
        }

        auto tmp = Y() / length();
        tmp = Math<T>::Atan( Math<T>::Sqrt( T( 1.0 ) - tmp * tmp ) / tmp ) * Math<T>::rad_to_deg();

        if( X() > T( 0.0 ) && Y() > T( 0.0 ) )
        {
            return tmp + T( 270.0 );
        }

        if( X() > T( 0.0 ) && Y() < T( 0.0 ) )
        {
            return tmp + T( 90.0 );
        }

        if( X() < 0.0 && Y() < 0.0 )
        {
            return T( 90.0 ) - tmp;
        }

        if( X() < T( 0.0 ) && Y() > T( 0.0 ) )
        {
            return T( 270.0 ) - tmp;
        }

        return tmp;
    }

    template <class T>
    T Vector2<T>::getAngleWith( const Vector2<T> &b ) const
    {
        auto tmp = X() * b.X() + Y() * b.Y();

        if( Math<T>::equals( tmp, T( 0.0 ) ) )
        {
            return T( 90.0 );
        }

        auto thisValue = ( X() * X() + Y() * Y() );
        auto otherValue = ( b.X() * b.X() + b.Y() * b.Y() );
        auto sqrtValue = Math<T>::Sqrt( thisValue * otherValue );

        tmp = tmp / sqrtValue;

        if( tmp < T( 0.0 ) )
        {
            tmp = -tmp;
        }

        return Math<T>::Atan( Math<T>::Sqrt( T( 1.0 ) - tmp * tmp ) / tmp ) * Math<T>::rad_to_deg();
    }

    template <class T>
    bool Vector2<T>::isBetweenPoints( const Vector2<T> &begin, const Vector2<T> &end ) const
    {
        auto f = ( end - begin ).squaredLength();
        return getDistanceFromSQ( begin ) < f && getDistanceFromSQ( end ) < f;
    }

    template <class T>
    Vector2<T> Vector2<T>::getInterpolated( const Vector2<T> &other, T d ) const
    {
        auto inv = T( 1.0 ) - d;
        return Vector2<T>( other.X() * inv + X() * d, other.Y() * inv + Y() * d );
    }

    template <class T>
    Vector2<T> Vector2<T>::getInterpolated_quadratic( const Vector2<T> &v2, const Vector2<T> &v3,
                                                      const T d ) const
    {
        // this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
        const auto inv = T( 1.0 ) - d;
        const auto mul0 = inv * inv;
        const auto mul1 = T( 2.0 ) * d * inv;
        const auto mul2 = d * d;

        return Vector2<T>( X() * mul0 + v2.X() * mul1 + v3.X() * mul2,
                           Y() * mul0 + v2.Y() * mul1 + v3.Y() * mul2 );
    }

    template <class T>
    void Vector2<T>::interpolate( const Vector2<T> &a, const Vector2<T> &b, const T t )
    {
        X() = b.X() + ( ( a.X() - b.X() ) * t );
        Y() = b.Y() + ( ( a.Y() - b.Y() ) * t );
    }

    template <class T>
    s32 Vector2<T>::compare( const Vector2 &other ) const
    {
        return Memory::Memcmp( ptr(), other.ptr(), 2 * sizeof( T ) );
    }

    template <class T>
    T *Vector2<T>::ptr()
    {
        return &x;
    }

    template <class T>
    const T *Vector2<T>::ptr() const
    {
        return &x;
    }

    template <class T>
    Vector2<T> Vector2<T>::zero()
    {
        return Vector2<T>( T( 0.0 ), T( 0.0 ) );
    }

    template <class T>
    Vector2<T> Vector2<T>::unit()
    {
        return Vector2<T>( T( 1.0 ), T( 1.0 ) );
    }

    template <class T>
    bool Vector2<T>::isValid() const
    {
        return isFinite();
    }

    template <class T>
    bool Vector2<T>::isFinite() const
    {
        return Math<T>::isFinite( x ) && Math<T>::isFinite( y );
    }

    /**
     * Multiply a scalar by a 2-dimensional vector.
     * @param scalar The scalar value to multiply with the vector.
     * @param vector The 2-dimensional vector to multiply with the scalar.
     * @return The resulting 2-dimensional vector after the multiplication.
     */
    template <class S, class T>
    Vector2<T> operator*( const S scalar, const Vector2<T> &vector )
    {
        return vector * scalar;
    }

    /// Typedef for integer 2d vector.
    using Vector2I = Vector2<s32>;

    /// Typedef for f32 2d vector.
    using Vector2F = Vector2<f32>;

    /// Typedef for f64 2d vector.
    using Vector2D = Vector2<f64>;

}  // end namespace fb

#endif  // Vector2_h__
