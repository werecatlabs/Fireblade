#ifndef __FB_TRIANGLE_3D_H_INCLUDED__
#define __FB_TRIANGLE_3D_H_INCLUDED__

#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Line3.h>
#include <FBCore/Math/Plane3.h>
#include <FBCore/Math/AABB3.h>

namespace fb
{
    //! 3d triangle template class for doing collision detection and other things.
    template <class T>
    class Triangle3
    {
    public:
        //! Constructor for an all 0 triangle
        Triangle3();

        //! Constructor for triangle with given three vertices
        Triangle3( Vector3<T> v1, Vector3<T> v2, Vector3<T> v3 );

        //! Determines if the triangle is totally inside a bounding box.
        //! \param box: Box to check.
        //! \return Returns true if the triangle is withing the box,
        //! and false otherwise.
        bool isTotalInsideBox( const AABB3<T> &box ) const;

        bool operator==( const Triangle3<T> &other ) const;
        bool operator!=( const Triangle3<T> &other ) const;

        //! Returns the closest point on a triangle to a point on the same plane.
        //! \param p: Point which must be on the same plane as the triangle.
        Vector3<T> closestPointOnTriangle( const Vector3<T> &p ) const;

        //! Returns if a point is inside the triangle
        //! \param p: Point to test. Assumes that this point is already on the plane
        //! of the triangle.
        //! \return Returns true if the point is inside the triangle, otherwise false.
        bool isPointInside( const Vector3<T> &p ) const;

        //! Returns if a point is inside the triangle. This method is an implementation
        //! of the example used in a paper by Kasper Fauerby original written
        //! by Keidy from Mr-Gamemaker.
        //! \param p: Point to test. Assumes that this point is already on the plane
        //! of the triangle.
        //! \return Returns true if the point is inside the triangle, otherwise false.
        bool isPointInsideFast( const Vector3<T> &p ) const;

        /** */
        bool isOnSameSide( const Vector3<T> &p1, const Vector3<T> &p2, const Vector3<T> &a,
                           const Vector3<T> &b ) const;

        //! Returns an intersection with a 3d line.
        //! \param line: Line to intersect with.
        //! \param outIntersection: Place to store the intersection point, if there is one.
        //! \return Returns true if there was an intersection, false if there was not.
        bool getIntersectionWithLimitedLine( const Line3<T> &line, Vector3<T> &outIntersection ) const;

        //! Returns an intersection with a 3d line.
        //! Please note that also points are returned as intersection, which
        //! are on the line, but not between the start and end point of the line.
        //! If you want the returned point be between start and end, please
        //! use getIntersectionWithLimitedLine().
        //! \param lineVect: Vector of the line to intersect with.
        //! \param linePoint: Point of the line to intersect with.
        //! \param outIntersection: Place to store the intersection point, if there is one.
        //! \return Returns true if there was an intersection, false if there was not.
        bool getIntersectionWithLine( const Vector3<T> &linePoint, const Vector3<T> &lineVect,
                                      Vector3<T> &outIntersection ) const;

        //! Calculates the intersection between a 3d line and
        //! the plane the triangle is on.
        //! \param lineVect: Vector of the line to intersect with.
        //! \param linePoint: Point of the line to intersect with.
        //! \param outIntersection: Place to store the intersection point, if there is one.
        //! \return Returns true if there was an intersection, false if there was not.
        bool getIntersectionOfPlaneWithLine( const Vector3<T> &linePoint, const Vector3<T> &lineVect,
                                             Vector3<T> &outIntersection ) const;

        //! Returns the normal of the triangle.
        //! Please note: The normal is not normalized.
        Vector3<T> getNormal() const;

        //! Returns if the triangle is front of backfacing.
        //! \param lookDirection: Look direction.
        //! \return Returns true if the plane is front facing, which mean it would
        //! be visible, and false if it is backfacing.
        bool isFrontFacing( const Vector3<T> &lookDirection ) const;

        //! Returns the plane of this triangle.
        Plane3<T> getPlane() const;

        //! Returns the area of the triangle
        T getArea() const;

        //! sets the triangle's points
        void set( const Vector3<T> &a, const Vector3<T> &b, const Vector3<T> &c );

    protected:
        //! the three points of the triangle
        Vector3<T> pointA;
        Vector3<T> pointB;
        Vector3<T> pointC;
    };

    template <class T>
    Triangle3<T>::Triangle3()
    {
    }

    template <class T>
    Triangle3<T>::Triangle3( Vector3<T> v1, Vector3<T> v2, Vector3<T> v3 ) :
        pointA( v1 ),
        pointB( v2 ),
        pointC( v3 )
    {
    }

    template <class T>
    bool Triangle3<T>::isTotalInsideBox( const AABB3<T> &box ) const
    {
        return ( box.isPointInside( pointA ) && box.isPointInside( pointB ) &&
                 box.isPointInside( pointC ) );
    }

    template <class T>
    bool Triangle3<T>::operator==( const Triangle3<T> &other ) const
    {
        return other.pointA == pointA && other.pointB == pointB && other.pointC == pointC;
    }

    template <class T>
    bool Triangle3<T>::operator!=( const Triangle3<T> &other ) const
    {
        return other.pointA != pointA || other.pointB != pointB || other.pointC != pointC;
    }

    template <class T>
    Vector3<T> Triangle3<T>::closestPointOnTriangle( const Vector3<T> &p ) const
    {
        Vector3<T> rab = Line3<T>( pointA, pointB ).getClosestPoint( p );
        Vector3<T> rbc = Line3<T>( pointB, pointC ).getClosestPoint( p );
        Vector3<T> rca = Line3<T>( pointC, pointA ).getClosestPoint( p );

        T d1 = rab.getDistanceFrom( p );
        T d2 = rbc.getDistanceFrom( p );
        T d3 = rca.getDistanceFrom( p );

        if( d1 < d2 )
            return d1 < d3 ? rab : rca;

        return d2 < d3 ? rbc : rca;
    }

    template <class T>
    bool Triangle3<T>::isPointInside( const Vector3<T> &p ) const
    {
        return ( isOnSameSide( p, pointA, pointB, pointC ) &&
                 isOnSameSide( p, pointB, pointA, pointC ) &&
                 isOnSameSide( p, pointC, pointA, pointB ) );
    }

    template <class T>
    bool Triangle3<T>::isPointInsideFast( const Vector3<T> &p ) const
    {
        Vector3<T> f = pointB - pointA;
        Vector3<T> g = pointC - pointA;

        f32 a = f.dotProduct( f );
        f32 b = f.dotProduct( g );
        f32 c = g.dotProduct( g );

        f32 ac_bb = ( a * c ) - ( b * b );
        Vector3<T> vp = p - pointA;

        f32 d = vp.dotProduct( f );
        f32 e = vp.dotProduct( g );
        f32 x = ( d * c ) - ( e * b );
        f32 y = ( e * a ) - ( d * b );
        f32 z = x + y - ac_bb;

        return ( ( ( (u32 &)z ) & ~( ( (u32 &)x ) | ( (u32 &)y ) ) ) & 0x80000000 ) != 0;
    }

    template <class T>
    bool Triangle3<T>::isOnSameSide( const Vector3<T> &p1, const Vector3<T> &p2, const Vector3<T> &a,
                                     const Vector3<T> &b ) const
    {
        auto bminusa = b - a;
        auto cp1 = bminusa.crossProduct( p1 - a );
        auto cp2 = bminusa.crossProduct( p2 - a );

        return cp1.dotProduct( cp2 ) >= T( 0.0 );
    }

    template <class T>
    bool Triangle3<T>::getIntersectionWithLimitedLine( const Line3<T> &line,
                                                       Vector3<T> &outIntersection ) const
    {
        return getIntersectionWithLine( line.getStart(), line.getVector(), outIntersection ) &&
               outIntersection.isBetweenPoints( line.getStart(), line.getEnd() );
    }

    template <class T>
    bool Triangle3<T>::getIntersectionWithLine( const Vector3<T> &linePoint, const Vector3<T> &lineVect,
                                                Vector3<T> &outIntersection ) const
    {
        if( getIntersectionOfPlaneWithLine( linePoint, lineVect, outIntersection ) )
        {
            return isPointInside( outIntersection );
        }

        return false;
    }

    template <class T>
    bool Triangle3<T>::getIntersectionOfPlaneWithLine( const Vector3<T> &linePoint,
                                                       const Vector3<T> &lineVect,
                                                       Vector3<T> &outIntersection ) const
    {
        auto n = getNormal();
        auto normal = n.normaliseCopy();

        auto t2 = normal.dotProduct( lineVect );

        if( Math<T>::isZero( t2 ) )
        {
            return false;
        }

        auto d = pointA.dotProduct( normal );
        auto t = -( normal.dotProduct( linePoint ) - d ) / t2;
        outIntersection = linePoint + ( lineVect * t );

        return true;
    }

    template <class T>
    Vector3<T> Triangle3<T>::getNormal() const
    {
        auto a = pointB - pointA;
        auto b = pointC - pointA;
        return a.crossProduct( b );
    }

    template <class T>
    bool Triangle3<T>::isFrontFacing( const Vector3<T> &lookDirection ) const
    {
        auto n = getNormal();
        n.normalise();
        return n.dotProduct( lookDirection ) <= T( 0.0 );
    }

    template <class T>
    Plane3<T> Triangle3<T>::getPlane() const
    {
        return Plane3<T>( pointA, pointB, pointC );
    }

    template <class T>
    T Triangle3<T>::getArea() const
    {
        auto a = pointB - pointA;
        auto b = pointC - pointA;
        auto r = a.crossProduct( b );
        return r.length() * T( 0.5 );
    }

    template <class T>
    void Triangle3<T>::set( const Vector3<T> &a, const Vector3<T> &b, const Vector3<T> &c )
    {
        pointA = a;
        pointB = b;
        pointC = c;
    }

    /// Typedef for an integer 3d triangle.
    using Triangle3I = Triangle3<s32>;

    /// Typedef for a f32 3d triangle.
    using Triangle3F = Triangle3<f32>;

    /// Typedef for a f64 3d triangle.
    using Triangle3D = Triangle3<f64>;

}  // end namespace fb

#endif
