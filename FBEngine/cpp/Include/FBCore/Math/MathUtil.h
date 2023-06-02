#ifndef MathUtil_h__
#define MathUtil_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Math/Polygon2.h>
#include <FBCore/Math/Polygon3.h>
#include <FBCore/Math/Ray3.h>
#include <FBCore/Math/Sphere3.h>
#include <FBCore/Math/Cylinder3.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/Deque.h>

namespace fb
{
    
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

        /** Prints the polygon data to a string. */
        static String printPolygonData( const Polygon2F &polygon );

        /** Welds the points of a polygon together. */
        static void weldPoints( Set<Vector2<T>> &points, T tollerance );

        /** Creates a polygon from the points passed in. */
        static Polygon2<T> createPolygon( const Set<Vector2<T>> &points );

        /** Creates a polygon from the points passed in. */
        static Polygon2<T> createPolygon( const Array<Vector2<T>> &points );

        /** Orders points in the specified direction. */
        static Set<Vector2<T>> orderPoints( const Set<Vector2<T>> &polygonPoints,
                                            const Vector2<T> &centerPoint, u8 pointOrdering = 0 );

        /** Orders points in the specified direction. */
        static Array<Vector2<T>> orderPoints( const Array<Vector2<T>> &polygonPoints,
                                              const Vector2<T> &centerPoint, u8 pointOrdering = 0 );

        /** Calculates polygon intervals along an axis. */
        static void getPolygonIntervals( const Polygon2F &polygon, const Vector2F &axis, f32 &min,
                                         f32 &max );

        /** Calculates polygon intervals along an axis. */
        static void getPolygonIntervals( const Polygon3F &polygon, const Vector3F &axis, f32 &min,
                                         f32 &max );

        /** Calculates interval distance. */
        static f32 intervalDistance( f32 minA, f32 maxA, f32 minB, f32 maxB );
    };

    template <class T>
    String MathUtil<T>::printPolygonData( const Polygon2F &polygon )
    {
        String polygonDataStr;
        auto numPoints = polygon.getNumPoints();

        polygonDataStr += String( "NumPoints: " );
        polygonDataStr += StringUtil::toString( numPoints );
        polygonDataStr += String( " \n" );

        for( u32 i = 0; i < numPoints; ++i )
        {
            const auto &point = polygon.getPoint( i );

            polygonDataStr += StringUtil::toString( point );
            polygonDataStr += String( " \n" );
        }

        return polygonDataStr;
    }

    template <class T>
    void MathUtil<T>::weldPoints( Set<Vector2<T>> &points, T tollerance )
    {
        for( auto pointA : points )
        {
            // check if another vertex is within range
            for( auto pointB : points )
            {
                if( pointA != pointB )
                {
                    // check if another vertex is within range
                    if( ( pointB - pointA ).length() < tollerance )
                    {
                        // points.erase(polyPointIdxB);
                        // polyPointIdx = 0;
                        break;
                    }
                }
            }
        }
    }

    template <class T>
    Polygon2<T> MathUtil<T>::createPolygon( const Set<Vector2<T>> &points )
    {
        Polygon2<T> polygon( static_cast<u32>( points.size() ) );

        for( auto point : points )
        {
            polygon.addPoint( point );
        }

        return polygon;
    }

    template <class T>
    Polygon2<T> MathUtil<T>::createPolygon( const Array<Vector2<T>> &points )
    {
        Polygon2<T> polygon( static_cast<u32>( points.size() ) );

        for( size_t polyPointIdx = 0; polyPointIdx < points.size(); ++polyPointIdx )
        {
            const auto &point = points[polyPointIdx];
            polygon.addPoint( point );
        }

        return polygon;
    }

    template <class T>
    Set<Vector2<T>> MathUtil<T>::orderPoints( const Set<Vector2<T>> &polygonPoints,
                                                 const Vector2<T> &centerPoint, u8 pointOrdering )
    {
        FB_UNUSED( pointOrdering );

        Set<Vector2<T>> orderedPoints;

        auto curAngle = T( 0.0 );
        auto minAngle = std::numeric_limits<T>::max();

        bool finished = false;
        while( !finished )
        {
            finished = true;

            Vector2<T> newPoint;

            auto newAngle = T( 0.0 );
            auto curDiff = std::numeric_limits<T>::max();

            for( auto point : polygonPoints )
            {
                auto vectorFromCenter = ( point - centerPoint ).normaliseCopy();
                auto pointAngle = Math<T>::ATan2( static_cast<T>( vectorFromCenter.X() ),
                                                  static_cast<T>( vectorFromCenter.Y() ) );
                pointAngle = Math<T>::RadToFullDegrees( pointAngle );

                auto diff = pointAngle - curAngle;

                if( diff > Math<T>::epsilon() && diff < curDiff )
                {
                    curDiff = diff;
                    minAngle = pointAngle;
                    newAngle = pointAngle;
                    newPoint = point;
                    finished = false;
                }
            }

            if( !finished )
            {
                curAngle = newAngle;
                orderedPoints.insert( newPoint );
            }
        }

        return orderedPoints;
    }

    template <class T>
    Array<Vector2<T>> MathUtil<T>::orderPoints( const Array<Vector2<T>> &polygonPoints,
                                                   const Vector2<T> &centerPoint, u8 pointOrdering )
    {
        FB_UNUSED( pointOrdering );

        Array<Vector2<T>> orderedPoints;
        orderedPoints.reserve( polygonPoints.size() );

        auto curAngle = T( 0.0 );
        auto minAngle = std::numeric_limits<T>::max();

        bool finished = false;
        while( !finished )
        {
            finished = true;

            Vector2<T> newPoint;

            auto newAngle = T( 0.0 );
            auto curDiff = std::numeric_limits<T>::max();

            for( u32 i = 0; i < polygonPoints.size(); ++i )
            {
                const auto &point = polygonPoints[i];
                auto vectorFromCenter = ( point - centerPoint ).normaliseCopy();
                auto pointAngle = Math<T>::ATan2( vectorFromCenter.X(), vectorFromCenter.Y() );
                pointAngle = Math<T>::RadToFullDegrees( pointAngle );

                auto diff = pointAngle - curAngle;

                if( diff > Math<T>::epsilon() && diff < curDiff )
                {
                    curDiff = diff;
                    minAngle = pointAngle;
                    newAngle = pointAngle;
                    newPoint = point;
                    finished = false;
                }
            }

            if( !finished )
            {
                curAngle = newAngle;
                orderedPoints.push_back( newPoint );
            }
        }

        return orderedPoints;
    }

    template <class T>
    void MathUtil<T>::getPolygonIntervals( const Polygon2F &polygon, const Vector2F &axis, f32 &min,
                                              f32 &max )
    {
        min = 1e10;
        max = -1e10;

        auto numPoints = polygon.getNumPoints();
        for( u32 polyIdx = 0; polyIdx < numPoints; ++polyIdx )
        {
            auto curPoint = polygon.getPoint( polyIdx );
            auto distance = axis.dotProduct( curPoint );

            if( min > distance )
            {
                min = distance;
            }

            if( max < distance )
            {
                max = distance;
            }
        }
    }

    template <class T>
    void MathUtil<T>::getPolygonIntervals( const Polygon3F &polygon, const Vector3F &axis, f32 &min,
                                              f32 &max )
    {
        min = 1e10;
        max = -1e10;

        auto numPoints = polygon.getNumPoints();
        for( u32 polyIdx = 0; polyIdx < numPoints; ++polyIdx )
        {
            auto curPoint = polygon.getPoint( polyIdx );
            auto distance = axis.dotProduct( curPoint );

            if( min > distance )
            {
                min = distance;
            }

            if( max < distance )
            {
                max = distance;
            }
        }
    }

    template <class T>
    f32 MathUtil<T>::intervalDistance( f32 minA, f32 maxA, f32 minB, f32 maxB )
    {
        if( minA < minB )
        {
            return minB - maxA;
        }

        return minA - maxB;
    }

    using MathUtilF = MathUtil<f32>;
    using MathUtilD = MathUtil<f64>;

}  // end namespace fb

#endif  // MathUtil_h__
