/**
@file Euler.h
@brief %Euler class for %Ogre
@details License: Do whatever you want with it.
@version 2.3
@author Kojack
@author Transporter
@author Klaim

Extracted From: http://www.ogre3d.org/tikiwiki/tiki-ind ... e=Cookbook
*/
#ifndef HGUARD_OGRE_MATHS_EULER_H
#define HGUARD_OGRE_MATHS_EULER_H

//#include "FBCore/Math/Math.h"
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/Matrix3.h>

namespace fb
{
    /**
    @class Euler
    @brief Class for %Euler rotations

    <table><tr><td>Yaw is a rotation around the Y axis.</td><td>Pitch is a rotation around the X
    axis.</td><td>Roll is a rotation around the Z axis.</td></tr> <tr><td><img
    src="http://www.ogre3d.org/tikiwiki/tiki-download_file.php?fileId=2112" /></td><td><img
    src="http://www.ogre3d.org/tikiwiki/tiki-download_file.php?fileId=2113" /></td><td><img
    src="http://www.ogre3d.org/tikiwiki/tiki-download_file.php?fileId=2114" /></td></tr></table>
    */
    template <class T>
    class Euler
    {
    public:
        /// Default constructor.
        Euler();

        /**
        @brief Constructor which takes yaw, pitch and roll values.
        @param y Starting value for yaw
        @param p Starting value for pitch
        @param r Starting value for roll
        */
        Euler( const T &y, const T &p = T( 0.0 ), const T &r = T( 0.0 ) );

        Euler( const Vector3<T> &rads ) : mYaw( rads.Y() ), mPitch( rads.X() ), mRoll( rads.Z() )
        {
        }

        /**
        @brief Default constructor with presets.
        @param quaternion Calculate starting values from this quaternion
        */
        explicit Euler( const Quaternion<T> &quaternion );

        explicit Euler( const Matrix3<T> &matrix );

        /// Get the Yaw angle.
        T yaw() const;

        /// Get the Pitch angle.
        T pitch() const;

        /// Get the Roll angle.
        T roll() const;

        /**
        @brief Set the yaw.
        @param y New value for yaw
        */
        Euler &setYaw( T y );

        /**
        @brief Set the pitch.
        @param p New value for pitch
        */
        Euler &setPitch( T p );

        /**
        @brief Set the roll.
        @param r New value for roll
        */
        Euler &setRoll( T r );

        /**
        @brief Set all rotations at once.
        @param y New value for yaw
        @param p New value for pitch
        @param r New value for roll
        */
        Euler &orientation( const T &y, const T &p, const T &r );

        /**
        @brief Apply a relative yaw.
        @param y Angle to add on current yaw
        */
        Euler &yaw( const T &y );

        /**
        @brief Apply a relative pitch.
        @param p Angle to add on current pitch
        */
        Euler &pitch( const T &p );

        /**
        @brief Apply a relative roll.
        @param r Angle to add on current roll
        */
        Euler &roll( const T &r );

        /**
        @brief Apply all relative rotations at once.
        @param y Angle to add on current yaw
        @param p Angle to add on current pitch
        @param r Angle to add on current roll
        */
        Euler &rotate( const T &y, const T &p, const T &r );

        /// Get a vector pointing forwards.
        Vector3<T> forward() const;

        /// Get a vector pointing to the right.
        Vector3<T> right() const;

        /// Get a vector pointing up.
        Vector3<T> up() const;

        Vector3<T> toRadians() const
        {
            return Vector3<T>( mYaw, mPitch, mRoll );
        }

        Vector3<T> toDegrees() const
        {
            return Vector3<T>( mYaw, mPitch, mRoll ) * Math<T>::rad_to_deg();
        }

        /**
        @brief Calculate the quaternion of the euler object.
        @details The result is cached, it is only recalculated when the component euler angles are
        changed.
        */
        Quaternion<T> toQuaternion() const;

        /// Casting operator. This allows any ogre function that wants a Quaternion to accept a Euler
        /// instead.
        operator Quaternion<T>() const;

        /**
        @brief Calculate the current euler angles of a given quaternion object.
        @param quaternion Quaternion which is used to calculate current euler angles.
        */
        Euler &fromQuaternion( const Quaternion<T> &quaternion );

        /**
        @brief Calculate the current euler angles of a given matrix object.
        @param matrix Matrix3 which is used to calculate current euler angles.
        */
        Euler &fromMatrix3( const Matrix3<T> &matrix );

        /**
        @brief Set the yaw and pitch to face in the given direction.
        @details The direction doesn't need to be normalised. Roll is always unaffected.
        @param setYaw If false, the yaw isn't changed.
        @param setPitch If false, the pitch isn't changed.
        */
        Euler &direction( const Vector3<T> &v, bool setYaw = true, bool setPitch = true );

        /**
        @brief Normalise the selected rotations to be within the +/-180 degree range.
        @details The normalise uses a wrap around, so for example a yaw of 360 degrees becomes 0 degrees,
        and -190 degrees becomes 170.
        @param normYaw If false, the yaw isn't normalized.
        @param normPitch If false, the pitch isn't normalized.
        @param normRoll If false, the roll isn't normalized.
        */
        Euler &normalise( bool normYaw = true, bool normPitch = true, bool normRoll = true );

        /**
        @brief Return the relative euler angles required to rotate from the current forward direction to
        the specified dir vector.
        @details The result euler can then be added to the current euler to immediately face dir.
        The rotation won't flip upside down then roll instead of a 180 degree yaw.
        @param setYaw If false, the angle is set to 0. If true, the angle is calculated.
        @param setPitch If false, the angle is set to 0. If true, the angle is calculated.
        @param shortest If false, the full value of each angle is used. If true, the angles are
        normalised and the shortest rotation is found to face the correct direction. For example, when
        false a yaw of 1000 degrees and a dir of (0,0,-1) will return a -1000 degree yaw. When true, the
        same yaw and dir would give 80 degrees (1080 degrees faces the same way as (0,0,-1).
        */
        Euler rotationTo( const Vector3<T> &dir, bool setYaw = true, bool setPitch = true,
                          bool shortest = true ) const;

        /// Clamp the yaw angle to a range of +/-limit.
        Euler &limitYaw( const T &limit );

        /// Clamp the pitch angle to a range of +/-limit.
        Euler &limitPitch( const T &limit );

        /// Clamp the roll angle to a range of +/-limit.
        Euler &limitRoll( const T &limit );

        /// Stream operator, for printing the euler component angles to a stream
        friend std::ostream &operator<<( std::ostream &o, const Euler &e );

        /// Add two euler objects.
        Euler operator+( const Euler &rhs ) const;

        /**
        @brief Subtract two euler objects.
        @details This finds the difference as relative angles.
        */
        Euler operator-( const Euler &rhs ) const;

        /// Interpolate the euler angles by rhs.
        Euler operator*( T rhs ) const;

        /// Interpolate the euler angle by lhs.
        friend Euler operator*( T lhs, const Euler &rhs );

        /**
        @brief Multiply two eulers.
        @details This has the same effect as multiplying quaternions.
        @returns The result is a quaternion.
        */
        Quaternion<T> operator*( Euler rhs ) const;

        /// Apply the euler rotation to the vector rhs.
        Vector3<T> operator*( const Vector3<T> &rhs ) const;

        /// Copy assignment operator (Euler)
        Euler &operator=( const Euler &src );

        /// Copy assignment operator (Quaternion)
        Euler &operator=( const Quaternion<T> &quaternion );

        /// Copy assignment operator (Matrix3)
        Euler &operator=( const Matrix3<T> &matrix );

        friend bool operator==( const Euler &left, const Euler &right );

        friend bool operator!=( const Euler &left, const Euler &right );

        friend bool sameOrientation( const Euler &left, const Euler &right );

    protected:
        void wrapAngle( T &angle );

        void limitAngle( T &angle, const T &limit );

        //!< Rotation around the Y axis.
        T mYaw;

        //!< Rotation around the X axis.
        T mPitch;

        //!< Rotation around the Z axis.
        T mRoll;

        //!< Cached quaternion equivalent of this euler object.
        mutable Quaternion<T> mCachedQuaternion;

        //!< Is the cached quaternion out of date?
        mutable bool mChanged = false;
    };

    template <class T>
    Euler<T>::Euler() : mYaw( T( 0.0 ) ), mPitch( T( 0.0 ) ), mRoll( T( 0.0 ) ), mChanged( true )
    {
    }

    template <class T>
    Euler<T>::Euler( const T &y, const T &p, const T &r ) :
        mYaw( y ),
        mPitch( p ),
        mRoll( r ),
        mChanged( true )
    {
    }

    template <class T>
    Euler<T>::Euler( const Quaternion<T> &quaternion )
    {
        fromQuaternion( quaternion );
    }

    template <class T>
    Euler<T>::Euler( const Matrix3<T> &matrix )
    {
        fromMatrix3( matrix );
    }

    template <class T>
    T Euler<T>::yaw() const
    {
        return mYaw;
    }

    template <class T>
    T Euler<T>::pitch() const
    {
        return mPitch;
    }

    template <class T>
    T Euler<T>::roll() const
    {
        return mRoll;
    }

    template <class T>
    Euler<T> &Euler<T>::setYaw( T y )
    {
        mYaw = y;
        mChanged = true;
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::setPitch( T p )
    {
        mPitch = p;
        mChanged = true;
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::setRoll( T r )
    {
        mRoll = r;
        mChanged = true;
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::orientation( const T &y, const T &p, const T &r )
    {
        mYaw = y;
        mPitch = p;
        mRoll = r;
        mChanged = true;
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::yaw( const T &y )
    {
        mYaw += y;
        mChanged = true;
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::pitch( const T &p )
    {
        mPitch += p;
        mChanged = true;
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::roll( const T &r )
    {
        mRoll += r;
        mChanged = true;
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::rotate( const T &y, const T &p, const T &r )
    {
        mYaw += y;
        mPitch += p;
        mRoll += r;
        mChanged = true;
        return *this;
    }

    template <class T>
    Vector3<T> Euler<T>::forward() const
    {
        return toQuaternion() * Vector3<T>::negativeZ();
    }

    template <class T>
    Vector3<T> Euler<T>::right() const
    {
        return toQuaternion() * Vector3<T>::positiveX();
    }

    template <class T>
    Vector3<T> Euler<T>::up() const
    {
        return toQuaternion() * Vector3<T>::positiveY();
    }

    template <class T>
    Quaternion<T> Euler<T>::toQuaternion() const
    {
        if( mChanged )
        {
            mCachedQuaternion = Quaternion<T>::angleAxis( mYaw, Vector3<T>::positiveY() ) *
                                Quaternion<T>::angleAxis( mPitch, Vector3<T>::positiveX() ) *
                                Quaternion<T>::angleAxis( mRoll, Vector3<T>::positiveZ() );
            mChanged = false;
        }

        return mCachedQuaternion;
    }

    template <class T>
    Euler<T>::operator Quaternion<T>() const
    {
        return toQuaternion();
    }

    template <class T>
    Euler<T> &Euler<T>::fromQuaternion( const Quaternion<T> &quaternion )
    {
        Matrix3<T> rotmat;
        quaternion.toRotationMatrix( rotmat );
        fromMatrix3( rotmat );
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::fromMatrix3( const Matrix3<T> &matrix )
    {
        matrix.toEulerAnglesYXZ( mYaw, mPitch, mRoll );
        mChanged = true;
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::direction( const Vector3<T> &v, bool setYaw, bool setPitch )
    {
        Vector3<T> d( v.normalisedCopy() );
        if( setPitch )
            mPitch = Math<T>::ASin( d.y );
        if( setYaw )
            mYaw = Math<T>::ATan2( -d.X(), -d.Z() );
        mChanged = setYaw || setPitch;
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::normalise( bool normYaw, bool normPitch, bool normRoll )
    {
        if( normYaw )
            wrapAngle( mYaw );

        if( normPitch )
            wrapAngle( mPitch );

        if( normRoll )
            wrapAngle( mRoll );

        return *this;
    }

    template <class T>
    Euler<T> Euler<T>::rotationTo( const Vector3<T> &dir, bool setYaw, bool setPitch,
                                   bool shortest ) const
    {
        Euler t1;
        Euler t2;
        t1.direction( dir, setYaw, setPitch );
        t2 = t1 - *this;
        if( shortest && setYaw )
        {
            t2.normalise();
        }
        return t2;
    }

    template <class T>
    Euler<T> &Euler<T>::limitYaw( const T &limit )
    {
        limitAngle( mYaw, limit );
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::limitPitch( const T &limit )
    {
        limitAngle( mPitch, limit );
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::limitRoll( const T &limit )
    {
        limitAngle( mRoll, limit );
        return *this;
    }

    template <class T>
    Euler<T> Euler<T>::operator+( const Euler &rhs ) const
    {
        return Euler( mYaw + rhs.mYaw, mPitch + rhs.mPitch, mRoll + rhs.mRoll );
    }

    template <class T>
    Euler<T> Euler<T>::operator-( const Euler &rhs ) const
    {
        return Euler( mYaw - rhs.mYaw, mPitch - rhs.mPitch, mRoll - rhs.mRoll );
    }

    template <class T>
    Euler<T> Euler<T>::operator*( T rhs ) const
    {
        return Euler( mYaw * rhs, mPitch * rhs, mRoll * rhs );
    }

    template <class T>
    Euler<T> operator*( T lhs, const Euler<T> &rhs )
    {
        return Euler( lhs * rhs.mYaw, lhs * rhs.mPitch, lhs * rhs.mRoll );
    }

    template <class T>
    Quaternion<T> Euler<T>::operator*( Euler<T> rhs ) const
    {
        Euler e1( *this ), e2( rhs );
        return e1.toQuaternion() * e2.toQuaternion();
    }

    template <class T>
    Vector3<T> Euler<T>::operator*( const Vector3<T> &rhs ) const
    {
        return toQuaternion() * rhs;
    }

    template <class T>
    Euler<T> &Euler<T>::operator=( const Euler<T> &src )
    {
        orientation( src.yaw(), src.pitch(), src.roll() );
        return *this;
    }

    template <class T>
    Euler<T> &Euler<T>::operator=( const Quaternion<T> &quaternion )
    {
        fromQuaternion( quaternion );
        return *this;
    }

    template <class T>
    void Euler<T>::limitAngle( T &angle, const T &limit )
    {
        if( angle > limit )
        {
            angle = limit;
            mChanged = true;
        }
        else if( angle < -limit )
        {
            angle = -limit;
            mChanged = true;
        }
    }

    template <class T>
    void Euler<T>::wrapAngle( T &angle )
    {
        auto rangle = angle.valueRadians();
        if( rangle < -Math<T>::pi() )
        {
            rangle = fmod( rangle, -Math<T>::TWO_PI );
            if( rangle < -Math<T>::pi() )
            {
                rangle += Math<T>::TWO_PI;
            }
            angle = rangle;
            mChanged = true;
        }
        else if( rangle > Math<T>::pi() )
        {
            rangle = fmod( rangle, Math<T>::TWO_PI );
            if( rangle > Math<T>::pi() )
            {
                rangle -= Math<T>::TWO_PI;
            }
            angle = rangle;
            mChanged = true;
        }
    }

    template <class T>
    bool sameOrientation( const Euler<T> &left, const Euler<T> &right )
    {
        // I'm comparing resulting vectors to avoid having to compare angles that are the same but in different values.
        // Only the resulting oriented vectors really have any meaning in the end.
        return left.forward().positionEquals( right.forward() ) &&
               left.up().positionEquals( right.up() );
    }

    template <class T>
    bool operator!=( const Euler<T> &left, const Euler<T> &right )
    {
        return !( left == right );
    }

    template <class T>
    bool operator==( const Euler<T> &left, const Euler<T> &right )
    {
        return left.mYaw == right.mYaw && left.mPitch == right.mPitch && left.mRoll == right.mRoll;
    }

    template <class T>
    Euler<T> &Euler<T>::operator=( const Matrix3<T> &matrix )
    {
        fromMatrix3( matrix );
        return *this;
    }

}  // end namespace fb

#endif
