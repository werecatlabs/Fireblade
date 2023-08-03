#ifndef __FB_QUATERNION_H_
#define __FB_QUATERNION_H_

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Matrix3.h>
#include <FBCore/Core/StringTypes.h>
#include <cmath>
#include <complex>

namespace fb
{
    //! Templated class for a 3D Quaternion.
    /*!
     * A quaternion is a mathematical concept used to represent 3D rotations in space. It consists of
     * a vector part and a scalar part. Quaternions are useful for animating 3D objects in a way that is
     * free from the problem of gimbal lock, which can occur when using Euler angles to represent rotations.
     */
    template <typename T>
    class Quaternion
    {
    public:
        //! Default constructor.
        Quaternion();

        //! Constructor.
        /*!
         * Constructs a Quaternion from the given values.
         * \param W The scalar part of the Quaternion.
         * \param X The x-component of the vector part of the Quaternion.
         * \param Y The y-component of the vector part of the Quaternion.
         * \param Z The z-component of the vector part of the Quaternion.
         */
        Quaternion( T W, T X, T Y, T Z );

        //! Constructor which converts euler angles to a Quaternion.
        /*!
         * Constructs a Quaternion from the given Euler angles.
         * \param x The rotation around the x-axis.
         * \param y The rotation around the y-axis.
         * \param z The rotation around the z-axis.
         */
        Quaternion( T x, T y, T z );

        //! Constructor which converts a matrix to a Quaternion.
        /*!
         * Constructs a Quaternion from the given rotation matrix.
         * \param mat The rotation matrix to be converted.
         */
        Quaternion( const Matrix3<T> &mat );

        //! Constructor from a pointer.
        Quaternion( const T *ptr );

        //! Copy constructor.
        Quaternion( const Quaternion &other );

        //! Constructor which sets the Quaternion to represent the given axes.
        /*!
         * Constructs a Quaternion from the given axes.
         * \param xaxis The x-axis.
         * \param yaxis The y-axis.
         * \param zaxis The z-axis.
         */
        Quaternion( const Vector3<T> &xaxis, const Vector3<T> &yaxis, const Vector3<T> &zaxis );

        //! Assignment operator.
        Quaternion &operator=( const Quaternion &other );

        //! Equal operator.
        bool operator==( const Quaternion &other ) const;

        //! Not equal operator.
        bool operator!=( const Quaternion &other ) const;

        //! Add operator.
        Quaternion operator+( const Quaternion &other ) const;

        //! Subtract operator.
        Quaternion operator-( const Quaternion &other ) const;

        //! Negation operator.
        Quaternion operator-() const;

        /**
         * Multiplies this quaternion by another quaternion and returns the result.
         *
         * Quaternion multiplication is not generally commutative, so in most cases
         * p * q != q * p.
         *
         * The multiplication is performed by taking the Hamilton product of the
         * two quaternions.
         *
         * @param q The quaternion to multiply.
         * @return The result of the multiplication.
         */
        Quaternion operator*( const Quaternion &other ) const;

        //! Multiplication operator.
        Quaternion operator*( T s ) const;

        /**
         * Multiplies the given scalar to the given quaternion, returning a new quaternion.
         *
         * @tparam T The type of the quaternion and scalar.
         * @param scalar The scalar to multiply to the quaternion.
         * @param rkQ The quaternion to be multiplied.
         * @return The new quaternion obtained after multiplying the given scalar to the given quaternion.
         */
        friend Quaternion<T> operator*( T fScalar, const Quaternion<T> &rkQ )
        {
            return Quaternion( fScalar * rkQ.w, fScalar * rkQ.x, fScalar * rkQ.y, fScalar * rkQ.z );
        }

        //! Multiplication operator.
        Quaternion &operator*=( T s );

        /**
         * Multiplies this quaternion by a vector and returns the result.
         *
         * The multiplication is performed as follows:
         * Q * V * Q^-1
         * where Q is this quaternion, V is the input vector, and Q^-1 is the inverse
         * of this quaternion. This is equivalent to rotating the vector by the
         * rotation represented by this quaternion.
         *
         * This implementation uses the nVidia SDK method for quaternion-vector
         * multiplication.
         *
         * @param v The vector to multiply.
         * @return The result of the multiplication.
         */
        Vector3<T> operator*( const Vector3<T> &v ) const;

        //! Multiplication operator.
        Quaternion &operator*=( const Quaternion &other );

        //! Cast operator to array.
        operator const T *() const;

        //! Cast operator to array.
        operator T *();

        //! Subscript operator.
        T operator[]( int i ) const;

        //! Subscript operator.
        T &operator[]( int i );

        //! Returns the scalar part of the Quaternion.
        T W() const;

        //! Returns the scalar part of the Quaternion.
        T &W();

        //! Returns the x-component of the vector part of the Quaternion.
        T X() const;

        //! Returns the x-component of the vector part of the Quaternion.
        T &X();

        //! Returns the y-component of the vector part of the Quaternion.
        T Y() const;
        /**
         * @brief Returns a reference to the y component of the quaternion.
         * @return A reference to the y component of the quaternion.
         */
        T &Y();

        /**
         * @brief Returns the z component of the quaternion.
         * @return The z component of the quaternion.
         */
        T Z() const;

        /**
         * @brief Returns a reference to the z component of the quaternion.
         * @return A reference to the z component of the quaternion.
         */
        T &Z();

        /**
         * @brief Calculates the dot product of this quaternion with another quaternion.
         * @param other The other quaternion.
         * @return The dot product of the two quaternions.
         */
        T dotProduct( const Quaternion &other ) const;

        /**
         * Normalizes this quaternion and returns a reference to it.
         *
         * If the quaternion is already normalized, this function does nothing.
         *
         * @return A reference to this quaternion, after normalization.
         */
        void normalise();

        /**
         * Normalizes a copy of this quaternion and returns it.
         */
        Quaternion normaliseCopy() const;

        /**
         * @brief Sets the components of the quaternion.
         * @param x The x component.
         * @param y The y component.
         * @param z The z component.
         * @param w The w component.
         */
        void set( T x, T y, T z, T w );

        /**
         * Calculates the inverse of this quaternion.
         *
         * The inverse of a quaternion is defined as the quaternion
         * that when multiplied by this quaternion produces the identity
         * quaternion. If the norm of the quaternion is zero, an identity
         * quaternion is returned to indicate that the input quaternion
         * was invalid and the inverse could not be calculated.
         *
         * @return The inverse of this quaternion.
         */
        Quaternion inverse() const;

        /**
         * @brief Calculates the exponential of this quaternion.
         * @return The exponential of this quaternion.
         */
        Quaternion exp() const;

        /**
         * @brief Calculates the natural logarithm of this quaternion.
         * @return The natural logarithm of this quaternion.
         */
        Quaternion log() const;

        /**
         * @brief Interpolates between two quaternions based on a given time parameter.
         * @param t The time parameter, in the range [0, 1].
         * @param q1 The starting quaternion.
         * @param q2 The ending quaternion.
         * @param shortestPath Whether to choose the shortest path between the quaternions.
         * @return The interpolated quaternion.
         */
        static Quaternion slerp( T t, const Quaternion &q1, const Quaternion &q2,
                                 bool shortestPath = false );

        /**
         * @brief Performs spherical quadratic interpolation between four quaternions based on a given time parameter.
         * @param t The time parameter, in the range [0, 1].
         * @param rkP The starting quaternion.
         * @param rkA The first control quaternion.
         * @param rkB The second control quaternion.
         * @param rkQ The ending quaternion.
         * @param shortestPath Whether to choose the shortest path between the quaternions.
         * @return The interpolated quaternion.
         */
        static Quaternion squad( T fT, const Quaternion &rkP, const Quaternion &rkA,
                                 const Quaternion &rkB, const Quaternion &rkQ,
                                 bool shortestPath = false );

        /**
         * @brief Sets this quaternion to represent a rotation around a given axis by a given angle.
         * @param angle The angle of rotation, in radians.
         * @param axis The axis of rotation.
         */
        void fromAngleAxis( T angle, const Vector3<T> &axis );

        /**
         * @brief Sets this quaternion to represent a rotation around a given axis by given euler angles.
         * @param degrees The euler angles of rotation, in degrees.
         */
        void fromDegrees( const Vector3<T> &degrees );

        /**
         * @brief Sets this quaternion to represent a rotation around a given axis by given euler angles.
         * @param radians The euler angles of rotation, in radians.
         */
        void fromRadians( const Vector3<T> &radians );

        /**
         * @brief Sets the Quaternion to identity.
         */
        void makeIdentity();

        /**
         * @brief Sets the Quaternion based on a rotation matrix.
         *
         * @param kRot The Matrix3<T> rotation matrix.
         */
        void fromRotationMatrix( const Matrix3<T> &kRot );

        /**
         * @brief Converts the Quaternion to a rotation matrix.
         *
         * @param kRot A reference to a Matrix3<T> where the rotation matrix will be stored.
         */
        void toRotationMatrix( Matrix3<T> &kRot ) const;

        /**
         * @brief Sets the Quaternion based on three orthogonal axes.
         *
         * @param xAxis The x-axis.
         * @param yAxis The y-axis.
         * @param zAxis The z-axis.
         */
        void fromAxes( const Vector3<T> &xAxis, const Vector3<T> &yAxis, const Vector3<T> &zAxis );

        /**
         * @brief Converts the Quaternion to three orthogonal axes.
         *
         * @param xAxis A reference to the x-axis.
         * @param yAxis A reference to the y-axis.
         * @param zAxis A reference to the z-axis.
         */
        void toAxes( Vector3<T> &xAxis, Vector3<T> &yAxis, Vector3<T> &zAxis ) const;

        /**
         * @brief Constructs a Quaternion based on an angle and axis.
         *
         * @param rfAngle The angle in radians.
         * @param rkAxis The axis of rotation.
         *
         * @return The resulting Quaternion.
         */
        static Quaternion<T> angleAxis( const T &rfAngle, const Vector3<T> &rkAxis );

        /**
         * @brief Constructs a Quaternion based on Euler angles.
         *
         * @param x The x-angle in radians.
         * @param y The y-angle in radians.
         * @param z The z-angle in radians.
         *
         * @return The resulting Quaternion.
         */
        static Quaternion<T> euler( T x, T y, T z );

        /**
         * @brief Constructs a Quaternion based on Euler angles in degrees.
         *
         * @param x The x-angle in degrees.
         * @param y The y-angle in degrees.
         * @param z The z-angle in degrees.
         *
         * @return The resulting Quaternion.
         */
        static Quaternion<T> eulerDegrees( T x, T y, T z );

        /**
         * @brief Returns the identity Quaternion.
         *
         * @return The identity Quaternion.
         */
        static const Quaternion identity();

        /**
         * @brief Returns a pointer to the Quaternion's data.
         *
         * @return A pointer to the Quaternion's data.
         */
        T *ptr();

        /**
         * @brief Returns a constant pointer to the Quaternion's data.
         *
         * @return A constant pointer to the Quaternion's data.
         */
        const T *ptr() const;

        /**
         * @brief Sets the Quaternion based on Euler angles.
         *
         * @param x The x-angle in radians.
         * @param y The y-angle in radians.
         * @param z The z-angle in radians.
         */
        void set( T x, T y, T z );

        /**
         * @brief Returns the yaw angle in radians.
         *
         * @param reprojectAxis Whether or not to reproject the Quaternion onto the sphere.
         *
         * @return The yaw angle in radians.
         */
        T getYaw( bool reprojectAxis = true ) const;

        /**
         * @brief Calculates the norm of this quaternion.
         *
         * @return The norm of the quaternion.
         */
        T norm() const;

        /**
         * @brief Calculates the squared magnitude of this quaternion.
         *
         * @return The squared magnitude of the quaternion.
         */
        T magnitudeSquared() const;

        /**
         * @brief Calculates the magnitude of this quaternion.
         *
         * @return The magnitude of the quaternion.
         */
        T magnitude() const;

        /**
         * @brief Rotates the given 3D vector by this quaternion.
         *
         * @param v The vector to be rotated.
         * @return The rotated vector.
         */
        Vector3<T> rotate( const Vector3<T> &v ) const;

        /**
         * @brief Inversely rotates the given 3D vector by this quaternion.
         *
         * @param v The vector to be inversely rotated.
         * @return The inversely rotated vector.
         */
        Vector3<T> rotateInv( const Vector3<T> &v ) const;

        /**
         * @brief Computes the rotation of x-axis.
         *
         * @return The rotated x-axis vector.
         */
        Vector3<T> getBasisVector0() const;

        /**
         * @brief Computes the rotation of y-axis.
         *
         * @return The rotated y-axis vector.
         */
        Vector3<T> getBasisVector1() const;

        /**
         * @brief Computes the rotation of z-axis.
         *
         * @return The rotated z-axis vector.
         */
        Vector3<T> getBasisVector2() const;

        /**
         * @brief Returns the conjugate of this quaternion.
         *
         * @return The conjugate quaternion.
         */
        Quaternion<T> getConjugate() const;

        /**
         * @brief Checks if the quaternion elements have a valid value.
         *
         * @return True if the quaternion elements are valid.
         */
        bool isValid() const;

        /**
         * @brief Checks if the quaternion elements are finite.
         *
         * @return True if the quaternion elements are finite.
         */
        bool isFinite() const;

        /**
         * @brief Checks if the quaternion elements are unit length.
         *
         * @return True if the quaternion elements are unit length.
         */
        bool isUnit() const;

        /**
         * @brief Checks if the quaternion elements have a valid value.
         *
         * @return True if the quaternion elements are sane.
         */
        bool isSane() const;

        /**
         * Gets the shortest arc quaternion to rotate this vector to the destination vector.
         *
         * If you call this with a dest vector that is close to the inverse of this vector, we will rotate
         * 180 degrees around the 'fallbackAxis' (if specified, or a generated axis if not) since in this
         * case ANY axis of rotation is valid.
         *
         * @param src The source vector.
         * @param dest The destination vector.
         * @param fallbackAxis The fallback axis to use if the vectors are close to the inverse of each other.
         *                     Default is Vector3<T>::zero().
         * @return The shortest arc quaternion to rotate this vector to the destination vector.
         */
        static Quaternion<T> getRotationTo( const Vector3<T> &src, const Vector3<T> &dest,
                                            const Vector3<T> &fallbackAxis = Vector3<T>::zero() );
        T w;  //!< The w component of the quaternion.
        T x;  //!< The x component of the quaternion.
        T y;  //!< The y component of the quaternion.
        T z;  //!< The z component of the quaternion.
    };

    template <typename T>
    Quaternion<T>::Quaternion() : x( T( 0.0 ) ), y( T( 0.0 ) ), z( T( 0.0 ) ), w( T( 1.0 ) )
    {
    }

    template <typename T>
    Quaternion<T>::Quaternion( T w, T x, T y, T z ) : w( w ), x( x ), y( y ), z( z )
    {
    }

    template <typename T>
    Quaternion<T>::Quaternion( T x, T y, T z )
    {
        set( x, y, z );
    }

    template <typename T>
    Quaternion<T>::Quaternion( const Quaternion &other ) :
        w( other.w ),
        x( other.x ),
        y( other.y ),
        z( other.z )
    {
    }

    template <typename T>
    Quaternion<T>::Quaternion( const T *ptr )
    {
        x = ptr[0];
        y = ptr[1];
        z = ptr[1];
        w = ptr[3];
    }

    template <typename T>
    Quaternion<T>::Quaternion( const Matrix3<T> &mat )
    {
        fromRotationMatrix( mat );
    }

    template <typename T>
    Quaternion<T>::Quaternion( const Vector3<T> &xaxis, const Vector3<T> &yaxis,
                               const Vector3<T> &zaxis )
    {
        fromAxes( xaxis, yaxis, zaxis );
    }

    template <typename T>
    bool Quaternion<T>::operator==( const Quaternion &other ) const
    {
        return Math<T>::equals( X(), other.X() ) && Math<T>::equals( Y(), other.Y() ) &&
               Math<T>::equals( Z(), other.Z() ) && Math<T>::equals( W(), other.W() );
    }

    template <typename T>
    bool Quaternion<T>::operator!=( const Quaternion &other ) const
    {
        return !( Math<T>::equals( X(), other.X() ) && Math<T>::equals( Y(), other.Y() ) &&
                  Math<T>::equals( Z(), other.Z() ) && Math<T>::equals( W(), other.W() ) );
    }

    template <typename T>
    Quaternion<T> &Quaternion<T>::operator=( const Quaternion &other )
    {
        w = other.w;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::operator*( const Quaternion &q ) const
    {
        // Perform the Hamilton product of the two quaternions
        Quaternion<T> product;
        product.w = w * q.w - x * q.x - y * q.y - z * q.z;
        product.x = w * q.x + x * q.w + y * q.z - z * q.y;
        product.y = w * q.y + y * q.w + z * q.x - x * q.z;
        product.z = w * q.z + z * q.w + x * q.y - y * q.x;

        return product;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::operator*( T s ) const
    {
        return Quaternion<T>( s * w, s * x, s * y, s * z );
    }

    template <typename T>
    Quaternion<T> &Quaternion<T>::operator*=( T s )
    {
        X() *= s;
        Y() *= s;
        Z() *= s;
        W() *= s;
        return *this;
    }

    template <typename T>
    Quaternion<T> &Quaternion<T>::operator*=( const Quaternion &other )
    {
        *this = other * ( *this );
        return *this;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::operator+( const Quaternion &other ) const
    {
        return Quaternion<T>( w + other.w, x + other.x, y + other.y, z + other.z );
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::operator-( const Quaternion &other ) const
    {
        return Quaternion<T>( w - other.w, x - other.x, y - other.y, z - other.z );
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::operator-() const
    {
        return Quaternion( -w, -x, -y, -z );
    }

    template <typename T>
    void Quaternion<T>::set( T x, T y, T z, T w )
    {
        X() = x;
        Y() = y;
        Z() = z;
        W() = w;
    }

    template <typename T>
    void Quaternion<T>::normalise()
    {
        auto n = x * x + y * y + z * z + w * w;

        if( Math<T>::equals( n, T( 1.0 ) ) )
        {
            // The quaternion is already normalized
            return;
        }

        // Calculate the inverse square root of n to normalize the quaternion
        n = Math<T>::SqrtInv( n );
        x *= n;
        y *= n;
        z *= n;
        w *= n;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::normaliseCopy() const
    {
        auto ret = *this;
        ret.normalise();
        return ret;
    }

    template <typename T>
    T Quaternion<T>::dotProduct( const Quaternion &q2 ) const
    {
        return ( X() * q2.X() ) + ( Y() * q2.Y() ) + ( Z() * q2.Z() ) + ( W() * q2.W() );
    }

    template <typename T>
    void Quaternion<T>::fromAngleAxis( T angle, const Vector3<T> &axis )
    {
        // assert:  axis[] is unit length
        //
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        auto halfAngle = T( 0.5 ) * angle;
        auto sn = Math<T>::Sin( halfAngle );
        w = Math<T>::Cos( halfAngle );
        x = sn * axis[0];
        y = sn * axis[1];
        z = sn * axis[2];
    }

    template <typename T>
    void Quaternion<T>::makeIdentity()
    {
        W() = T( 1.0 );
        X() = T( 0.0 );
        Y() = T( 0.0 );
        Z() = T( 0.0 );
    }

    template <typename T>
    Quaternion<T>::operator const T *() const
    {
        return &w;
    }

    template <typename T>
    Quaternion<T>::operator T *()
    {
        return &w;
    }

    template <typename T>
    T Quaternion<T>::operator[]( int i ) const
    {
        FB_ASSERT( i >= 0 );
        FB_ASSERT( i <= 3 );
        return ( &w )[i];
    }

    template <typename T>
    T &Quaternion<T>::operator[]( int i )
    {
        FB_ASSERT( i >= 0 );
        FB_ASSERT( i <= 3 );
        return ( &w )[i];
    }

    template <typename T>
    T Quaternion<T>::X() const
    {
        return x;
    }

    template <typename T>
    T &Quaternion<T>::X()
    {
        return x;
    }

    template <typename T>
    T Quaternion<T>::Y() const
    {
        return y;
    }

    template <typename T>
    T &Quaternion<T>::Y()
    {
        return y;
    }

    template <typename T>
    T Quaternion<T>::Z() const
    {
        return z;
    }

    template <typename T>
    T &Quaternion<T>::Z()
    {
        return z;
    }

    template <typename T>
    T Quaternion<T>::W() const
    {
        return w;
    }

    template <typename T>
    T &Quaternion<T>::W()
    {
        return w;
    }

    template <typename T>
    void Quaternion<T>::fromDegrees( const Vector3<T> &degrees )
    {
        auto radians = degrees * Math<T>::deg_to_rad();
        set( radians.X(), radians.Y(), radians.Z() );
    }

    template <typename T>
    void Quaternion<T>::fromRadians( const Vector3<T> &radians )
    {
        set( radians.X(), radians.Y(), radians.Z() );
    }

    template <typename T>
    void Quaternion<T>::set( T x, T y, T z )
    {
        T angle;

        angle = x * T( 0.5 );
        T sr = Math<T>::Sin( angle );
        T cr = Math<T>::Cos( angle );

        angle = y * T( 0.5 );
        T sp = Math<T>::Sin( angle );
        T cp = Math<T>::Cos( angle );

        angle = z * T( 0.5 );
        T sy = Math<T>::Sin( angle );
        T cy = Math<T>::Cos( angle );

        T cpcy = cp * cy;
        T spcy = sp * cy;
        T cpsy = cp * sy;
        T spsy = sp * sy;

        X() = ( sr * cpcy - cr * spsy );
        Y() = ( cr * spcy + sr * cpsy );
        Z() = ( cr * cpsy - sr * spcy );
        W() = ( cr * cpcy + sr * spsy );

        normalise();
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::inverse() const
    {
        // Calculate the norm of the quaternion
        auto fNorm = norm();

        // If the norm is greater than zero, calculate and return the inverse
        if( fNorm > T( 0.0 ) )
        {
            auto fInvNorm = T( 1.0 ) / fNorm;
            return Quaternion( w * fInvNorm, -x * fInvNorm, -y * fInvNorm, -z * fInvNorm );
        }

        // If the norm is negative, return an invalid result to flag an error
        return Quaternion<T>();
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::exp() const
    {
        // If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
        // exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
        // use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

        auto fAngle( Math<T>::Sqrt( X() * X() + Y() * Y() + Z() * Z() ) );
        auto fSin = Math<T>::Sin( fAngle );

        Quaternion kResult;
        kResult.W() = Math<T>::Cos( fAngle );

        if( Math<T>::Abs( fSin ) >= Math<T>::epsilon() )
        {
            auto fCoeff = fSin / fAngle;
            kResult.X() = fCoeff * X();
            kResult.Y() = fCoeff * Y();
            kResult.Z() = fCoeff * Z();
        }
        else
        {
            kResult.X() = X();
            kResult.Y() = Y();
            kResult.Z() = Z();
        }

        return kResult;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::log() const
    {
        // If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
        // log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
        // sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

        auto kResult = Quaternion<T>::identity();
        kResult.W() = T( 0.0 );

        if( Math<T>::Abs( W() ) < T( 1.0 ) )
        {
            auto fAngle( Math<T>::ACos( W() ) );
            auto fSin = Math<T>::Sin( fAngle );
            if( Math<T>::Abs( fSin ) >= Math<T>::epsilon() )
            {
                auto fCoeff = fAngle / fSin;
                kResult.X() = fCoeff * X();
                kResult.Y() = fCoeff * Y();
                kResult.Z() = fCoeff * Z();
                return kResult;
            }
        }

        kResult.X() = X();
        kResult.Y() = Y();
        kResult.Z() = Z();

        return kResult;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::slerp( T t, const Quaternion<T> &q1, const Quaternion<T> &q2,
                                        bool shortestPath )
    {
        auto cosTheta = q1.dotProduct( q2 );

        // Adjust the sign of q2 to take the shortest path
        auto q2Adjusted = q2;
        if( shortestPath && cosTheta < T( 0 ) )
        {
            q2Adjusted = -q2;
            cosTheta = -cosTheta;
        }

        auto theta = std::acos( cosTheta );
        auto sinTheta = std::sin( theta );

        if( sinTheta < std::numeric_limits<T>::epsilon() )
        {
            // Quaternions are very close, use linear interpolation
            return Math<T>::lerp( q1, q2Adjusted, t );
        }

        auto coeff1 = std::sin( ( 1 - t ) * theta ) / sinTheta;
        auto coeff2 = std::sin( t * theta ) / sinTheta;

        return coeff1 * q1 + coeff2 * q2Adjusted;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::squad( T fT, const Quaternion<T> &rkP, const Quaternion<T> &rkA,
                                        const Quaternion<T> &rkB, const Quaternion<T> &rkQ,
                                        bool shortestPath )
    {
        auto fSlerpT = T( 2.0 ) * fT * ( T( 1.0 ) - fT );
        auto kSlerpP = slerp( fT, rkP, rkQ, shortestPath );
        auto kSlerpQ = slerp( fT, rkA, rkB );
        return slerp( fSlerpT, kSlerpP, kSlerpQ );
    }

    template <typename T>
    Vector3<T> Quaternion<T>::operator*( const Vector3<T> &v ) const
    {
        // Convert the quaternion to a 3D vector
        Vector3<T> qvec( x, y, z );

        // Calculate intermediate vectors
        Vector3<T> uv = qvec.crossProduct( v );
        Vector3<T> uuv = qvec.crossProduct( uv );

        // Scale the vectors and add them together to get the result
        uv *= T( 2.0 ) * w;
        uuv *= T( 2.0 );

        return v + uv + uuv;
    }

    template <typename T>
    void Quaternion<T>::fromRotationMatrix( const Matrix3<T> &kRot )
    {
        // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
        // article "Quaternion Calculus and Fast Animation".

        T fTrace = kRot[0][0] + kRot[1][1] + kRot[2][2];
        T fRoot;

        if( fTrace > 0.0 )
        {
            // |w| > 1/2, may as well choose w > 1/2
            fRoot = Math<T>::Sqrt( fTrace + T( 1.0 ) );  // 2w
            W() = T( 0.5 ) * fRoot;
            fRoot = T( 0.5 ) / fRoot;  // 1/(4w)
            X() = ( kRot[2][1] - kRot[1][2] ) * fRoot;
            Y() = ( kRot[0][2] - kRot[2][0] ) * fRoot;
            Z() = ( kRot[1][0] - kRot[0][1] ) * fRoot;
        }
        else
        {
            // |w| <= 1/2
            static u32 s_iNext[3] = { 1, 2, 0 };
            u32 i = 0;
            if( kRot[1][1] > kRot[0][0] )
                i = 1;
            if( kRot[2][2] > kRot[i][i] )
                i = 2;
            u32 j = s_iNext[i];
            u32 k = s_iNext[j];

            fRoot = Math<T>::Sqrt( kRot[i][i] - kRot[j][j] - kRot[k][k] + T( 1.0 ) );
            T *apkQuat[3] = { &x, &y, &z };
            *apkQuat[i] = T( 0.5 ) * fRoot;
            fRoot = T( 0.5 ) / fRoot;
            W() = ( kRot[k][j] - kRot[j][k] ) * fRoot;
            *apkQuat[j] = ( kRot[j][i] + kRot[i][j] ) * fRoot;
            *apkQuat[k] = ( kRot[k][i] + kRot[i][k] ) * fRoot;
        }
    }

    template <typename T>
    void Quaternion<T>::toRotationMatrix( Matrix3<T> &kRot ) const
    {
        auto fTx = x + x;
        auto fTy = y + y;
        auto fTz = z + z;
        auto fTwx = fTx * w;
        auto fTwy = fTy * w;
        auto fTwz = fTz * w;
        auto fTxx = fTx * x;
        auto fTxy = fTy * x;
        auto fTxz = fTz * x;
        auto fTyy = fTy * y;
        auto fTyz = fTz * y;
        auto fTzz = fTz * z;

        kRot[0][0] = T( 1.0 ) - ( fTyy + fTzz );
        kRot[0][1] = fTxy - fTwz;
        kRot[0][2] = fTxz + fTwy;
        kRot[1][0] = fTxy + fTwz;
        kRot[1][1] = T( 1.0 ) - ( fTxx + fTzz );
        kRot[1][2] = fTyz - fTwx;
        kRot[2][0] = fTxz - fTwy;
        kRot[2][1] = fTyz + fTwx;
        kRot[2][2] = T( 1.0 ) - ( fTxx + fTyy );
    }

    template <typename T>
    void Quaternion<T>::fromAxes( const Vector3<T> &xaxis, const Vector3<T> &yaxis,
                                  const Vector3<T> &zaxis )
    {
        Matrix3<T> kRot;

        kRot[0][0] = xaxis[0];
        kRot[1][0] = xaxis[1];
        kRot[2][0] = xaxis[2];

        kRot[0][1] = yaxis[0];
        kRot[1][1] = yaxis[1];
        kRot[2][1] = yaxis[2];

        kRot[0][2] = zaxis[0];
        kRot[1][2] = zaxis[1];
        kRot[2][2] = zaxis[2];

        fromRotationMatrix( kRot );
    }

    template <typename T>
    void Quaternion<T>::toAxes( Vector3<T> &xaxis, Vector3<T> &yaxis, Vector3<T> &zaxis ) const
    {
        Matrix3<T> kRot;

        toRotationMatrix( kRot );

        xaxis[0] = kRot[0][0];
        xaxis[1] = kRot[1][0];
        xaxis[2] = kRot[2][0];

        yaxis[0] = kRot[0][1];
        yaxis[1] = kRot[1][1];
        yaxis[2] = kRot[2][1];

        zaxis[0] = kRot[0][2];
        zaxis[1] = kRot[1][2];
        zaxis[2] = kRot[2][2];
    }

    template <typename T>
    T Quaternion<T>::norm() const
    {
        return w * w + x * x + y * y + z * z;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::angleAxis( const T &rfAngle, const Vector3<T> &rkAxis )
    {
        Quaternion r;
        r.fromAngleAxis( rfAngle, rkAxis );
        return r;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::euler( T x, T y, T z )
    {
        Matrix3<T> mat;
        mat.fromEulerAnglesXYZ( x, y, z );

        return Quaternion<T>( mat );
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::eulerDegrees( T x, T y, T z )
    {
        Matrix3<T> mat;
        mat.fromEulerAnglesXYZ( Math<T>::DegToRad( x ), Math<T>::DegToRad( y ), Math<T>::DegToRad( z ) );

        return Quaternion<T>( mat );
    }

    template <typename T>
    const Quaternion<T> Quaternion<T>::identity()
    {
        static const Quaternion qIdentity( T( 1.0 ), T( 0.0 ), T( 0.0 ), T( 0.0 ) );
        return qIdentity;
    }

    template <typename T>
    T *Quaternion<T>::ptr()
    {
        return &x;
    }

    template <typename T>
    const T *Quaternion<T>::ptr() const
    {
        return &x;
    }

    template <typename T>
    T Quaternion<T>::magnitudeSquared() const
    {
        return X() * X() + Y() * Y() + Z() * Z() + W() * W();
    }

    template <typename T>
    T Quaternion<T>::magnitude() const
    {
        return Math<T>::Sqrt( magnitudeSquared() );
    }

    template <typename T>
    Vector3<T> Quaternion<T>::rotate( const Vector3<T> &v ) const
    {
        const auto vx = T( 2.0 ) * v.X();
        const auto vy = T( 2.0 ) * v.Y();
        const auto vz = T( 2.0 ) * v.Z();
        const auto w2 = W() * W() - T( 0.5 );
        const auto dot2 = ( X() * vx + Y() * vy + Z() * vz );

        return Vector3<T>( ( vx * w2 + ( Y() * vz - Z() * vy ) * W() + X() * dot2 ),
                           ( vy * w2 + ( Z() * vx - X() * vz ) * W() + Y() * dot2 ),
                           ( vz * w2 + ( X() * vy - Y() * vx ) * W() + Z() * dot2 ) );
    }

    template <typename T>
    Vector3<T> Quaternion<T>::rotateInv( const Vector3<T> &v ) const
    {
        const auto vx = T( 2.0 ) * v.X();
        const auto vy = T( 2.0 ) * v.Y();
        const auto vz = T( 2.0 ) * v.Z();
        const auto w2 = W() * W() - T( 0.5 );
        const auto dot2 = ( X() * vx + Y() * vy + Z() * vz );

        return Vector3<T>( ( vx * w2 - ( Y() * vz - Z() * vy ) * W() + X() * dot2 ),
                           ( vy * w2 - ( Z() * vx - X() * vz ) * W() + Y() * dot2 ),
                           ( vz * w2 - ( X() * vy - Y() * vx ) * W() + Z() * dot2 ) );
    }

    template <typename T>
    Vector3<T> Quaternion<T>::getBasisVector0() const
    {
        const auto x2 = X() * T( 2.0 );
        const auto w2 = W() * T( 2.0 );

        return Vector3<T>( ( W() * w2 ) - T( 1.0 ) + X() * x2, ( Z() * w2 ) + Y() * x2,
                           ( -Y() * w2 ) + Z() * x2 );
    }

    template <typename T>
    Vector3<T> Quaternion<T>::getBasisVector1() const
    {
        const auto y2 = Y() * T( 2.0 );
        const auto w2 = W() * T( 2.0 );

        return Vector3<T>( ( -Z() * w2 ) + X() * y2, ( W() * w2 ) - T( 1.0 ) + Y() * y2,
                           ( X() * w2 ) + Z() * y2 );
    }

    template <typename T>
    Vector3<T> Quaternion<T>::getBasisVector2() const
    {
        const auto z2 = Z() * T( 2.0 );
        const auto w2 = W() * T( 2.0 );

        return Vector3<T>( ( Y() * w2 ) + X() * z2, ( -X() * w2 ) + Y() * z2,
                           ( W() * w2 ) - T( 1.0 ) + Z() * z2 );
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::getConjugate() const
    {
        return Quaternion<T>( W(), -X(), -Y(), -Z() );
    }

    template <typename T>
    bool Quaternion<T>::isValid() const
    {
        return isFinite();
    }

    template <typename T>
    bool Quaternion<T>::isFinite() const
    {
        return Math<T>::isFinite( w ) && Math<T>::isFinite( x ) && Math<T>::isFinite( y ) &&
               Math<T>::isFinite( z );
    }

    template <typename T>
    bool Quaternion<T>::isUnit() const
    {
        const T unitTolerance = T( 1e-4 );
        return isFinite() && Math<T>::Abs( magnitude() - T( 1.0 ) ) < unitTolerance;
    }

    template <typename T>
    bool Quaternion<T>::isSane() const
    {
        constexpr auto defaultTolerance = std::numeric_limits<T>::epsilon();
        constexpr auto defaultMagnitude = T( 1.0 );

        auto tolerance = defaultTolerance * T( 1000.0 );  // Adjust as needed

        return isFinite() && Math<T>::Abs( magnitude() - defaultMagnitude ) < tolerance;
    }

    template <typename T>
    Quaternion<T> Quaternion<T>::getRotationTo( const Vector3<T> &src, const Vector3<T> &dest,
                                                const Vector3<T> &fallbackAxis )
    {
        // From Sam Hocevar's article "Quaternion from two vectors:
        // the final version"
        auto a = Math<T>::Sqrt( src.squaredLength() * dest.squaredLength() );
        auto b = a + src.dotProduct( dest );
        auto axis = Vector3<T>::zero();

        if( b < static_cast<T>( 1e-06 ) * a )
        {
            b = static_cast<T>( 0.0 );
            axis = fallbackAxis != Vector3<T>::zero() ? fallbackAxis
                   : Math<T>::Abs( src.X() ) > Math<T>::Abs( src.Z() )
                       ? Vector3<T>( -src.Y(), src.X(), static_cast<T>( 0.0 ) )
                       : Vector3<T>( static_cast<T>( 0.0 ), -src.Z(), src.Y() );
        }
        else
        {
            axis = src.crossProduct( dest );
        }

        Quaternion<T> q( b, axis.X(), axis.Y(), axis.Z() );
        q.normalise();
        return q;
    }

    /// A 3D quaternion with single-precision floating-point components.
    using QuaternionF = Quaternion<f32>;

    /// A 3D quaternion with double-precision floating-point components.
    using QuaternionD = Quaternion<f64>;

}  // end namespace fb

#endif
