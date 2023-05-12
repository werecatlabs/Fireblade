#ifndef CollisionTests_h__
#define CollisionTests_h__

#include <FBCore/Math/Line2.h>

namespace fb
{
    template <class T>
    class CollisionTests
    {
    public:
        static bool test( const Line2<T> &line, const AABB2<T> &box );
    };

    template <class T>
    bool CollisionTests<T>::test( const Line2<T> &line, const AABB2<T> &box )
    {
        auto a_rectangleMinX = box.m_minimum.X();
        auto a_rectangleMinY = box.m_minimum.Y();
        auto a_rectangleMaxX = box.m_maximum.X();
        auto a_rectangleMaxY = box.m_maximum.Y();
        auto a_p1x = line.m_start.X();
        auto a_p1y = line.m_start.Y();
        auto a_p2x = line.m_end.X();
        auto a_p2y = line.m_end.Y();

        // Find min and max X for the segment
        auto minX = a_p1x;
        auto maxX = a_p2x;

        if( a_p1x > a_p2x )
        {
            minX = a_p2x;
            maxX = a_p1x;
        }

        // Find the intersection of the segment's and rectangle's x-projections

        if( maxX > a_rectangleMaxX )
        {
            maxX = a_rectangleMaxX;
        }

        if( minX < a_rectangleMinX )
        {
            minX = a_rectangleMinX;
        }

        if( minX > maxX )  // If their projections do not intersect return false
        {
            return false;
        }

        // Find corresponding min and max Y for min and max X we found before
        auto minY = a_p1y;
        auto maxY = a_p2y;

        auto dx = a_p2x - a_p1x;

        if( Math<T>::Abs( dx ) > T( 0.0000001 ) )
        {
            auto a = ( a_p2y - a_p1y ) / dx;
            auto b = a_p1y - a * a_p1x;
            minY = a * minX + b;
            maxY = a * maxX + b;
        }

        if( minY > maxY )
        {
            T tmp = maxY;
            maxY = minY;
            minY = tmp;
        }

        // Find the intersection of the segment's and rectangle's y-projections
        if( maxY > a_rectangleMaxY )
        {
            maxY = a_rectangleMaxY;
        }

        if( minY < a_rectangleMinY )
        {
            minY = a_rectangleMinY;
        }

        if( minY > maxY )  // If Y-projections do not intersect return false
        {
            return false;
        }

        return true;
    }

    using CollisionTestsF = CollisionTests<f32>;
    using CollisionTestsD = CollisionTests<f64>;
}  // namespace fb

#endif  // CollisionTests_h__
