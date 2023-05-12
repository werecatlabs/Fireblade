#ifndef polygonUtil_H
#define polygonUtil_H

#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Polygon2.h>
#include <FBCore/Math/Polygon3.h>
#include <FBCore/Base/StringTypes.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/Set.h>

namespace fb
{
    /// Define the types of point ordering
    enum PolygonPointOrdering
    {
        /// clockwise ordering
        PPO_CLOCKWISE,

        /// counter clockwise (anti-clockwise) ordering
        PPO_COUNTER_CLOCKWISE
    };

    //---------------------------------------------
    /** Provides useful functions for dealing with polygons.
     */
    template <class T>
    class FBCore_API PolygonUtil
    {
    public:
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

    using PolygonUtilI = PolygonUtil<s32>;
    using PolygonUtilF = PolygonUtil<f32>;
}  // end namespace fb

#endif
