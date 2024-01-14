#ifndef __FB_Vector4_H_
#define __FB_Vector4_H_

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Memory/SmartPtr.h>

namespace fb
{

    /** A 4d vector class. */
    template <class T>
    class Vector4
    {
    public:
        /** Default constructor. */
        Vector4();

        Vector4( T x, T y, T z, T w );
        Vector4( const Vector4<T> &other );
        explicit Vector4( const T *ptr );

        // operators
        Vector4<T> operator-() const;
        Vector4<T> &operator=( const Vector4<T> &other );

        Vector4<T> operator+( const Vector4<T> &other ) const;
        Vector4<T> &operator+=( const Vector4<T> &other );

        Vector4<T> operator-( const Vector4<T> &other ) const;
        Vector4<T> &operator-=( const Vector4<T> &other );

        Vector4<T> operator*( const Vector4<T> &other ) const;
        Vector4<T> &operator*=( const Vector4<T> &other );
        Vector4<T> operator*( T v ) const;
        Vector4<T> &operator*=( T v );

        Vector4<T> operator/( const Vector4<T> &other ) const;
        Vector4<T> &operator/=( const Vector4<T> &other );
        Vector4<T> operator/( T v ) const;
        Vector4<T> &operator/=( T v );

        bool operator<=( const Vector4<T> &other ) const;
        bool operator>=( const Vector4<T> &other ) const;
        bool operator<( const Vector4<T> &other ) const;
        bool operator>( const Vector4<T> &other ) const;

        bool operator==( const Vector4<T> &other ) const;
        bool operator!=( const Vector4<T> &other ) const;

        // member access
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

        T W() const;
        T &W();

        bool equals( const Vector4<T> &other, f32 tolerance = Math<T>::epsilon() ) const;

        static Vector4<T> zero();

        /** Data. */
        T *ptr();

        /** Const data. */
        const T *ptr() const;

        /** Checks if the vector's elements have a valid value. */
        bool isValid() const;

        /** Checks if the vector's elements are finite. */
        bool isFinite() const;

        FBCore_API static const Vector4 ZERO;
        FBCore_API static const Vector4 UNIT_X;
        FBCore_API static const Vector4 UNIT_Y;
        FBCore_API static const Vector4 UNIT_Z;
        FBCore_API static const Vector4 UNIT;

        /** The x-component of the vector. */
        T x;

        /** The y-component of the vector. */
        T y;

        /** The z-component of the vector. */
        T z;

        /** The w-component of the vector. */
        T w;

    private:
        s32 compare( const Vector4 &other ) const;
    };

    template <class T>
    Vector4<T>::Vector4() : x( 0 ), y( 0 ), z( 0 ), w( 0 )
    {
    }

    template <class T>
    Vector4<T>::Vector4( T x, T y, T z, T w ) : x( x ), y( y ), z( z ), w( w )
    {
    }

    template <class T>
    Vector4<T>::Vector4( const Vector4<T> &other )
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }

    template <class T>
    Vector4<T>::Vector4( const T *ptr )
    {
        x = ptr[0];
        y = ptr[1];
        z = ptr[2];
        w = ptr[3];
    }

    template <class T>
    Vector4<T> Vector4<T>::operator-() const
    {
        return Vector4<T>( -X(), -Y(), -Z(), -W() );
    }

    template <class T>
    Vector4<T> &Vector4<T>::operator=( const Vector4<T> &other )
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    template <class T>
    Vector4<T> Vector4<T>::operator+( const Vector4<T> &other ) const
    {
        return Vector4<T>( x + other.x, y + other.y, z + other.z, w + other.w );
    }

    template <class T>
    Vector4<T> &Vector4<T>::operator+=( const Vector4<T> &other )
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    template <class T>
    Vector4<T> Vector4<T>::operator-( const Vector4<T> &other ) const
    {
        return Vector4<T>( x - other.x, y - other.y, z - other.z, w - other.w );
    }

    template <class T>
    Vector4<T> &Vector4<T>::operator-=( const Vector4<T> &other )
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    template <class T>
    Vector4<T> Vector4<T>::operator*( const Vector4<T> &other ) const
    {
        return Vector4<T>( x * other.x, y * other.y, z * other.z, w * other.w );
    }

    template <class T>
    Vector4<T> &Vector4<T>::operator*=( const Vector4<T> &other )
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }

    template <class T>
    Vector4<T> Vector4<T>::operator*( const T v ) const
    {
        return Vector4<T>( x * v, y * v, z * v, w * v );
    }

    template <class T>
    Vector4<T> &Vector4<T>::operator*=( const T v )
    {
        x *= v;
        y *= v;
        z *= v;
        w *= v;
        return *this;
    }

    template <class T>
    Vector4<T> Vector4<T>::operator/( const Vector4<T> &other ) const
    {
        return Vector4<T>( x / other.x, y / other.y, z / other.z, w / other.w );
    }

    template <class T>
    Vector4<T> &Vector4<T>::operator/=( const Vector4<T> &other )
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
    }

    template <class T>
    Vector4<T> Vector4<T>::operator/( const T v ) const
    {
        auto invValue = T( 1.0 ) / v;
        return Vector4<T>( x * invValue, y * invValue, z * invValue, w * invValue );
    }

    template <class T>
    Vector4<T> &Vector4<T>::operator/=( const T v )
    {
        auto invValue = T( 1.0 ) / v;
        x *= invValue;
        y *= invValue;
        z *= invValue;
        w *= invValue;
        return *this;
    }

    template <class T>
    bool Vector4<T>::operator<=( const Vector4<T> &other ) const
    {
        return compare( other ) <= 0;
    }

    template <class T>
    bool Vector4<T>::operator>=( const Vector4<T> &other ) const
    {
        return compare( other ) >= 0;
    }

    template <class T>
    bool Vector4<T>::operator<( const Vector4<T> &other ) const
    {
        return compare( other ) < 0;
    }

    template <class T>
    bool Vector4<T>::operator>( const Vector4<T> &other ) const
    {
        return compare( other ) > 0;
    }

    template <class T>
    bool Vector4<T>::operator==( const Vector4<T> &other ) const
    {
        return Math<T>::equals( X(), other.X() ) && Math<T>::equals( Y(), other.Y() ) &&
               Math<T>::equals( Z(), other.Z() ) && Math<T>::equals( W(), other.W() );
    }

    template <class T>
    bool Vector4<T>::operator!=( const Vector4<T> &other ) const
    {
        return !Math<T>::equals( X(), other.X() ) || !Math<T>::equals( Y(), other.Y() ) ||
               !Math<T>::equals( Z(), other.Z() ) || !Math<T>::equals( W(), other.W() );
    }

    template <class T>
    Vector4<T>::operator const T *() const
    {
        const auto p = ptr();
        return p;
    }

    template <class T>
    Vector4<T>::operator T *()
    {
        auto p = ptr();
        return p;
    }

    template <class T>
    T Vector4<T>::operator[]( s32 i ) const
    {
        const auto values = ptr();
        return values[i];
    }

    template <class T>
    T &Vector4<T>::operator[]( s32 i )
    {
        auto values = ptr();
        return values[i];
    }

    template <class T>
    T Vector4<T>::X() const
    {
        return x;
    }

    template <class T>
    T &Vector4<T>::X()
    {
        return x;
    }

    template <class T>
    T Vector4<T>::Y() const
    {
        return y;
    }

    template <class T>
    T &Vector4<T>::Y()
    {
        return y;
    }

    template <class T>
    T Vector4<T>::Z() const
    {
        return z;
    }

    template <class T>
    T &Vector4<T>::Z()
    {
        return z;
    }

    template <class T>
    T Vector4<T>::W() const
    {
        return w;
    }

    template <class T>
    T &Vector4<T>::W()
    {
        return w;
    }

    template <class T>
    bool Vector4<T>::equals( const Vector4<T> &other, [[maybe_unused]] const f32 tolerance ) const
    {
        return Math<T>::equals( x, other.x ) && Math<T>::equals( y, other.y ) &&
               Math<T>::equals( z, other.z ) && Math<T>::equals( w, other.w );
    }

    template <class T>
    s32 Vector4<T>::compare( const Vector4 &other ) const
    {
        return Memory::Memcmp( ptr(), other.ptr(), 4 * sizeof( T ) );
    }

    template <class T>
    T *Vector4<T>::ptr()
    {
        return &x;
    }

    template <class T>
    const T *Vector4<T>::ptr() const
    {
        return &x;
    }

    template <class T>
    Vector4<T> Vector4<T>::zero()
    {
        return Vector4<T>( T( 0.0 ), T( 0.0 ), T( 0.0 ), T( 0.0 ) );
    }

    template <class T>
    bool Vector4<T>::isValid() const
    {
        return isFinite();
    }

    template <class T>
    bool Vector4<T>::isFinite() const
    {
        return Math<T>::isFinite( x ) && Math<T>::isFinite( y ) && Math<T>::isFinite( z ) &&
               Math<T>::isFinite( w );
    }

    /**
     * @brief Scalar multiplication operator for Vector4 class.
     *
     * This overload provides the ability to multiply a Vector4 object by a scalar value,
     * by invoking the Vector4::operator* method.
     *
     * @tparam S The scalar type.
     * @tparam T The element type of the Vector4.
     * @param scalar The scalar value to multiply.
     * @param vector The Vector4 to be multiplied.
     * @return The resulting Vector4.
     */
    template <class S, class T>
    Vector4<T> operator*( const S scalar, const Vector4<T> &vector )
    {
        return vector * scalar;
    }

    //! Typedef for a 4d vector.
    using Vector4I = Vector4<s32>;

    //! Typedef for a 4d vector.
    using Vector4F = Vector4<f32>;

    //! Typedef for a 4d vector.
    using Vector4D = Vector4<f64>;

}  // end namespace fb

#endif
