#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Memory/Data.h>

namespace fb
{
    template <>
    const Vector3<s32> Vector3<s32>::ZERO( 0, 0, 0 );
    template <>
    const Vector3<s32> Vector3<s32>::UNIT_X( 1, 0, 0 );
    template <>
    const Vector3<s32> Vector3<s32>::UNIT_Y( 0, 1, 0 );
    template <>
    const Vector3<s32> Vector3<s32>::UNIT_Z( 0, 0, 1 );
    template <>
    const Vector3<s32> Vector3<s32>::UNIT( 1, 1, 1 );

    template <>
    const Vector3<f32> Vector3<f32>::ZERO( 0.0f, 0.0f, 0.0f );
    template <>
    const Vector3<f32> Vector3<f32>::UNIT_X( 1.0f, 0.0f, 0.0f );
    template <>
    const Vector3<f32> Vector3<f32>::UNIT_Y( 0.0f, 1.0f, 0.0f );
    template <>
    const Vector3<f32> Vector3<f32>::UNIT_Z( 0.0f, 0.0f, 1.0f );
    template <>
    const Vector3<f32> Vector3<f32>::UNIT( 1.0f, 1.0f, 1.0f );

    template <>
    const Vector3<f64> Vector3<f64>::ZERO( 0.0, 0.0, 0.0 );
    template <>
    const Vector3<f64> Vector3<f64>::UNIT_X( 1.0, 0.0, 0.0 );
    template <>
    const Vector3<f64> Vector3<f64>::UNIT_Y( 0.0, 1.0, 0.0 );
    template <>
    const Vector3<f64> Vector3<f64>::UNIT_Z( 0.0, 0.0, 1.0 );
    template <>
    const Vector3<f64> Vector3<f64>::UNIT( 1.0, 1.0, 1.0 );

    template <class T>
    auto Vector3<T>::fromCoords( T radius, T latitude, T longitude ) -> Vector3<T>
    {
        auto xPos = radius * Math<T>::Cos( latitude ) * Math<T>::Cos( longitude );
        auto zPos = radius * Math<T>::Cos( latitude ) * Math<T>::Sin( longitude );
        auto yPos = radius * Math<T>::Sin( latitude );

        return Vector3( xPos, zPos, yPos );
    }

    template <class T>
    auto Vector3<T>::isValid() const -> bool
    {
        return isFinite();
    }

    template <class T>
    auto Vector3<T>::isFinite() const -> bool
    {
        return Math<T>::isFinite( x ) && Math<T>::isFinite( y ) && Math<T>::isFinite( z );
    }

    template <class T>
    auto Vector3<T>::operator==( const Vector3<T> &other ) const -> bool
    {
        const auto values = ptr();
        return Math<T>::equals( values[0], other[0] ) && Math<T>::equals( values[1], other[1] ) &&
               Math<T>::equals( values[2], other[2] );
    }

    template <class T>
    auto Vector3<T>::operator!=( const Vector3<T> &other ) const -> bool
    {
        const auto values = ptr();
        return !Math<T>::equals( values[0], other[0] ) || !Math<T>::equals( values[1], other[1] ) ||
               !Math<T>::equals( values[2], other[2] );
    }

    template <class T>
    void Vector3<T>::rotateYZBy( T degrees, const Vector3<T> &center )
    {
        degrees *= Math<T>::deg_to_rad();
        T cs = Math<T>::Cos( degrees );
        T sn = Math<T>::Sin( degrees );
        Z() -= center.Z();
        Y() -= center.Y();
        set( X(), Y() * cs - Z() * sn, Y() * sn + Z() * cs );
        Z() += center.Z();
        Y() += center.Y();
    }

    template <class T>
    void Vector3<T>::rotateXZBy( T degrees, const Vector3<T> &center )
    {
        degrees *= Math<T>::deg_to_rad();
        T cs = Math<T>::Cos( degrees );
        T sn = Math<T>::Sin( degrees );
        X() -= center.X();
        Z() -= center.Z();
        set( X() * cs - Z() * sn, Y(), X() * sn + Z() * cs );
        X() += center.X();
        Z() += center.Z();
    }

    template <class T>
    void Vector3<T>::rotateXYBy( T degrees, const Vector3<T> &center )
    {
        degrees *= Math<T>::deg_to_rad();
        T cs = Math<T>::Cos( degrees );
        T sn = Math<T>::Sin( degrees );
        X() -= center.X();
        Y() -= center.Y();
        set( X() * cs - Y() * sn, X() * sn + Y() * cs, Z() );
        X() += center.X();
        Y() += center.Y();
    }

    template <typename T>
    void Vector3<T>::generateComplementBasis( Vector3<T> &u, Vector3<T> &v, const Vector3<T> &w )
    {
        if( Math<T>::Abs( w[0] ) >= Math<T>::Abs( w[1] ) )
        {
            // W.x or W.z is the largest magnitude component, swap them
            auto invLength = Math<T>::SqrtInv( w[0] * w[0] + w[2] * w[2] );
            u[0] = -w[2] * invLength;
            u[1] = static_cast<T>( 0 );
            u[2] = +w[0] * invLength;
            v[0] = w[1] * u[2];
            v[1] = w[2] * u[0] - w[0] * u[2];
            v[2] = -w[1] * u[0];
        }
        else
        {
            // W.y or W.z is the largest magnitude component, swap them
            auto invLength = Math<T>::SqrtInv( w[1] * w[1] + w[2] * w[2] );
            u[0] = static_cast<T>( 0 );
            u[1] = +w[2] * invLength;
            u[2] = -w[1] * invLength;
            v[0] = w[1] * u[2] - w[2] * u[1];
            v[1] = -w[0] * u[2];
            v[2] = w[0] * u[1];
        }
    }

    // explicit instantiation
    template class Vector3<s32>;
    template class Vector3<f32>;
    template class Vector3<f64>;
}  // end namespace fb
