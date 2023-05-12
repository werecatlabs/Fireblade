#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/PolygonUtil.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{

    template <class T>
    String PolygonUtil<T>::printPolygonData( const Polygon2F &polygon )
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
    void PolygonUtil<T>::weldPoints( Set<Vector2<T>> &points, T tollerance )
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
    Polygon2<T> PolygonUtil<T>::createPolygon( const Set<Vector2<T>> &points )
    {
        Polygon2<T> polygon( static_cast<u32>( points.size() ) );

        for( auto point : points )
        {
            polygon.addPoint( point );
        }

        return polygon;
    }

    template <class T>
    Polygon2<T> PolygonUtil<T>::createPolygon( const Array<Vector2<T>> &points )
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
    Set<Vector2<T>> PolygonUtil<T>::orderPoints( const Set<Vector2<T>> &polygonPoints,
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
    Array<Vector2<T>> PolygonUtil<T>::orderPoints( const Array<Vector2<T>> &polygonPoints,
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
    void PolygonUtil<T>::getPolygonIntervals( const Polygon2F &polygon, const Vector2F &axis, f32 &min,
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
    void PolygonUtil<T>::getPolygonIntervals( const Polygon3F &polygon, const Vector3F &axis, f32 &min,
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
    f32 PolygonUtil<T>::intervalDistance( f32 minA, f32 maxA, f32 minB, f32 maxB )
    {
        if( minA < minB )
        {
            return minB - maxA;
        }

        return minA - maxB;
    }

    // explicit instantiation
    template class PolygonUtil<s32>;
    template class PolygonUtil<f32>;
    template class PolygonUtil<f64>;
}  // end namespace fb
