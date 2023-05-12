#ifndef MathUtil_h__
#define MathUtil_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Math/Ray3.h>
#include <FBCore/Math/Sphere3.h>
#include <FBCore/Math/Cylinder3.h>
#include <FBCore/Base/Array.h>
#include <deque>

namespace fb
{
    //---------------------------------------------
    template <class T>
    class MathUtil
    {
    public:
        static bool equals( T f1, T f2 );
        static bool equals( T f1, T f2, T tolerance );

        static bool equals( const Vector2<T> &a, const Vector2<T> &b );
        static bool equals( const Vector3<T> &a, const Vector3<T> &b );
        static bool equals( const Quaternion<T> &a, const Quaternion<T> &b );

        static bool equals( const Vector3<T> &a, const Vector3<T> &b, T tolerance );
        static bool equals( const Quaternion<T> &a, const Quaternion<T> &b, T tolerance );

        static bool isFinite( const Vector3<T> &val );
        static bool isFinite( const Quaternion<T> &val );

        /** Gets the shortest arc quaternion to rotate this vector to the destination
           vector.
       @remarks
           If you call this with a dest vector that is close to the inverse
           of this vector, we will rotate 180 degrees around the 'fallbackAxis'
           (if specified, or a generated axis if not) since in this case
           ANY axis of rotation is valid.
       */
        static Quaternion<T> getRotationTo( const Vector3<T> &src, const Vector3<T> &dest,
                                            const Vector3<T> &fallbackAxis = Vector3<T>::ZERO );

        static Quaternion<T> getOrientationFromDirection( const Vector3<T> &vec,
                                                          const Vector3<T> &localDirectionVector,
                                                          bool bYawFixed,
                                                          const Vector3<T> &yawFixedAxis );

        static Vector3<T> reflect( const Vector3<T> &vec, const Vector3<T> &normal );

        static Vector3<T> toSpherical( const Vector3<T> &cartesian );
        static Vector3<T> toCartesian( const Vector3<T> &spherical );

        static Vector3<T> toPosition( const Vector3<T> &rotation, Vector3<T> &up, Vector3<T> &pitchVec,
                                      Vector3<T> &yawVec );
        static Vector3<T> toRotation( const Vector3<T> &cartesian, const Vector3<T> &pitchVec,
                                      const Vector3<T> &yawVec );

        static T average( const Array<T> &v );
        static T average( const std::deque<T> &v );

        static T latlong_distance( const Vector2<T> &lat_long_1, const Vector2<T> &lat_long_2 );

        static void swap( Vector3<T> &a, Vector3<T> &b );
        static Vector3<T> round( const Vector3<T> &v, s32 decimals = 10 );

        static void repair( Vector3<T> &Min, Vector3<T> &Max );

        template <class IT_TYPE>
        static void lowest( IT_TYPE begin, IT_TYPE end, T &val );

        template <class IT_TYPE>
        static void highest( IT_TYPE begin, IT_TYPE end, T &val );

        static Transform3<T> transformFromPlaneEquation( Plane3<T> plane );

        static std::pair<bool, T> intersects( const Ray3<T> &ray, const AABB3<T> &box );

        static std::pair<bool, T> intersects( const Ray3<T> &ray, const Sphere3<T> &sphere,
                                              bool discardInside = true );

        static std::pair<bool, T> intersects( const Ray3<T> &ray, const Cylinder3<T> &box );
    };

    using MathUtilF = MathUtil<f32>;
    using MathUtilD = MathUtil<f64>;
}  // end namespace fb

#endif  // MathUtil_h__
