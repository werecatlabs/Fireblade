#ifndef __AABBOX3D_H_
#define __AABBOX3D_H_

#include <FBCore/Math/Math.h>
#include <FBCore/Math/Plane3.h>
#include <FBCore/Math/Line3.h>
#include <FBCore/Math/Matrix4.h>

namespace fb
{
    /**
     * @brief Axis-aligned bounding box in 3D space.
     *
     * @tparam T the type used to represent coordinates (e.g., float or double).
     */
    template <class T>
    class AABB3
    {
    public:
        enum class Extent
        {
            Null,     ///< An empty AABB.
            Finite,   ///< A finite AABB.
            Infinite  ///< An AABB that encompasses all possible values.
        };

        AABB3();
        AABB3( const AABB3<T> &other );
        AABB3( const Vector3<T> &min, const Vector3<T> &max );
        AABB3( const Vector3<T> &init );
        AABB3( T minx, T miny, T minz, T maxx, T maxy, T maxz );

        bool operator==( const AABB3<T> &other ) const;
        bool operator!=( const AABB3<T> &other ) const;

        /**
         * @brief Adds a point to the bounding box, causing it to grow bigger, if the point is outside of the box.
         *
         * @param p The point to add to this box.
         */
        void merge( const Vector3<T> &p );

        /**
         * @brief Adds another bounding box to this one, causing it to grow bigger if the other box is outside of this one.
         *
         * @param b The other bounding box to add to this one.
         */
        void merge( const AABB3<T> &b );

        /**
         * @brief Resets the bounding box with new min and max values.
         *
         * @param x The minimum X value.
         * @param y The minimum Y value.
         * @param z The minimum Z value.
         */
        void reset( T x, T y, T z );

        /**
         * @brief Resets the bounding box with a new AABB.
         *
         * @param initValue The new AABB.
         */
        void reset( const AABB3<T> &initValue );

        /**
         * @brief Resets the bounding box with a new vector.
         *
         * @param initValue The new vector.
         */
        void reset( const Vector3<T> &initValue );

        /**
         * @brief Adds a point to the bounding box, causing it to grow bigger, if the point is outside of the box.
         *
         * @param x The X coordinate of the point.
         * @param y The Y coordinate of the point.
         * @param z The Z coordinate of the point.
         */
        void merge( T x, T y, T z );

        /**
         * @brief Determines if a point is inside this box.
         *
         * @param p The point to check.
         * @return True if the point is inside this box, false otherwise.
         */
        bool isPointInside( const Vector3<T> &p ) const;

        /**
         * @brief Determines if a point is inside this box, including the box borders.
         *
         * @param p The point to check.
         * @return True if the point is inside this box, false otherwise.
         */
        bool isPointTotalInside( const Vector3<T> &p ) const;

        /**
         * @brief Determines if this box intersects another box.
         *
         * @param other The other box to check.
         * @return True if there is an intersection, false otherwise.
         */
        bool intersects( const AABB3<T> &other ) const;

        /**
         * Checks if the box is fully inside the other box.
         *
         * @param other The other box to check.
         * @return True if this box is fully inside the other box, false otherwise.
         */
        bool isFullInside( const AABB3<T> &other ) const;

        /**
         * Tests if the box intersects with a line.
         *
         * @param line The line to test intersection with.
         * @return True if there is an intersection, false if not.
         */
        bool intersectsWithLine( const Line3<T> &line ) const;

        /**
         * Tests if the box intersects with a line.
         *
         * @param linemiddle The middle point of the line.
         * @param linevect The direction vector of the line.
         * @param halflength Half the length of the line.
         * @return True if there is an intersection, false if not.
         */
        bool intersectsWithLine( const Vector3<T> &linemiddle, const Vector3<T> &linevect,
                                 T halflength ) const;

        /**
         * Classifies a relation with a plane.
         *
         * @param plane The plane to classify relation to.
         * @return ISREL3D_FRONT if the box is in front of the plane,
         *         ISREL3D_BACK if the box is back of the plane, and
         *         ISREL3D_CLIPPED if it is on both sides of the plane.
         */
        s32 classifyPlaneRelation( const Plane3<T> &plane ) const;

        /**
         * Gets the center of the bounding box.
         *
         * @return The center of the bounding box.
         */
        Vector3<T> getCenter() const;

        /**
         * Gets the extent of the box.
         *
         * @return The extent of the box.
         */
        Vector3<T> getExtent() const;

        /**
         * Gets half the size of the box.
         *
         * @return Half the size of the box.
         */
        Vector3<T> getHalfSize( void ) const;

        /**
         * Stores all 8 edges of the box into an array.
         *
         * @param edges Pointer to the array of 8 edges.
         */
        void getEdges( Vector3<T> *edges ) const;

        /**
         * Checks if the box is empty, which means that there is no space within the min and the max edge.
         *
         * @return True if the box is empty, false otherwise.
         */
        bool isEmpty() const;

        /**
         * Repairs the box, if for example MinEdge and MaxEdge are swapped.
         */
        void repair();

        /**
         * Calculates a new interpolated bounding box.
         *
         * @param other The other box to interpolate between.
         * @param d Value between 0.0f and 1.0f.
         * @return A new interpolated bounding box.
         */
        AABB3<T> getInterpolated( const AABB3<T> &other, f32 d ) const;

        /**
         * Transforms the box according to the matrix supplied.
         * @remarks
         * By calling this method you get the axis-aligned box which
         * surrounds the transformed version of this box. Therefore, each
         * corner of the box is transformed by the matrix, and then the
         * extents are mapped back onto the axes to produce another
         * AABB. This is useful when you have a local AABB for an object
         * which is then transformed.
         * @param matrix: The transformation matrix to apply.
         */
        void transform( const Matrix4<T> &matrix );

        /**
         * Sets the extents of the bounding box by specifying minimum and maximum coordinates.
         * @param min: The new minimum coordinates of the box.
         * @param max: The new maximum coordinates of the box.
         */
        void setExtents( const Vector3<T> &min, const Vector3<T> &max );

        /**
         * Sets the bounding box to a null box.
         * A null box is one where the minimum and maximum bounds are equal and set to the maximum value of T.
         */
        void setNull();

        /**
         * Determines whether the bounding box is a null box.
         * A null box is one where the minimum and maximum bounds are equal and set to the maximum value of T.
         * @return Returns true if the bounding box is a null box, and false otherwise.
         */
        bool isNull( void ) const;

        /**
         * Determines whether the bounding box has finite dimensions.
         * @return Returns true if the bounding box has finite dimensions, and false otherwise.
         */
        bool isFinite( void ) const;

        /**
         * Sets the bounding box to an infinite box.
         * An infinite box is one where the minimum and maximum bounds are set to the maximum and minimum values of T, respectively.
         */
        void setInfinite();

        /**
         * Determines whether the bounding box is an infinite box.
         * An infinite box is one where the minimum and maximum bounds are set to the maximum and minimum values of T, respectively.
         * @return Returns true if the bounding box is an infinite box, and false otherwise.
         */
        bool isInfinite( void ) const;

        /**
         * Determines whether the bounding box is a valid box.
         * A valid box is one where the minimum and maximum bounds are not NaN or infinity.
         * @return Returns true if the bounding box is valid, and false otherwise.
         */
        bool isValid( void ) const;

        /**
         * Gets the minimum coordinates of the bounding box.
         * @return The minimum coordinates of the bounding box.
         */
        const Vector3<T> &getMinimum() const;

        /**
         * Sets the minimum coordinates of the bounding box.
         * @param val: The new minimum coordinates of the box.
         */
        void setMinimum( const Vector3<T> &minimum );

        /**
         * Gets the maximum coordinates of the bounding box.
         * @return The maximum coordinates of the bounding box.
         */
        const Vector3<T> &getMaximum() const;

        /**
         * Sets the maximum coordinates of the bounding box.
         * @param maximum: The new maximum coordinates of the box.
         */
        void setMaximum( const Vector3<T> &maximum );

    private:
        Vector3<T> m_minimum;  ///< The minimum coordinates of the bounding box.
        Vector3<T> m_maximum;  ///< The maximum coordinates of the bounding box.
        Extent m_extent;       ///< The extent of the bounding box. Can be Null, Finite, or Infinite.
    };

    template <class T>
    AABB3<T>::AABB3() : m_minimum( -1, -1, -1 ), m_maximum( 1, 1, 1 )
    {
    }

    template <class T>
    AABB3<T>::AABB3( const AABB3<T> &other ) : m_minimum( other.m_minimum ), m_maximum( other.m_maximum )
    {
    }

    template <class T>
    AABB3<T>::AABB3( const Vector3<T> &min, const Vector3<T> &max ) : m_minimum( min ), m_maximum( max )
    {
    }

    template <class T>
    AABB3<T>::AABB3( const Vector3<T> &init ) : m_minimum( init ), m_maximum( init )
    {
    }

    template <class T>
    AABB3<T>::AABB3( T minx, T miny, T minz, T maxx, T maxy, T maxz ) :
        m_minimum( minx, miny, minz ),
        m_maximum( maxx, maxy, maxz )
    {
    }

    template <class T>
    bool AABB3<T>::operator==( const AABB3<T> &other ) const
    {
        return ( m_minimum == other.m_minimum && other.m_maximum == m_maximum );
    }

    template <class T>
    bool AABB3<T>::operator!=( const AABB3<T> &other ) const
    {
        return !( m_minimum == other.m_minimum && other.m_maximum == m_maximum );
    }

    template <class T>
    void AABB3<T>::merge( const Vector3<T> &p )
    {
        merge( p.X(), p.Y(), p.Z() );
    }

    template <class T>
    void AABB3<T>::merge( const AABB3<T> &b )
    {
        merge( b.m_minimum );
        merge( b.m_maximum );
    }

    template <class T>
    void AABB3<T>::reset( T x, T y, T z )
    {
        m_maximum.set( x, y, z );
        m_minimum = m_maximum;
    }

    template <class T>
    void AABB3<T>::reset( const AABB3<T> &initValue )
    {
        *this = initValue;
    }

    template <class T>
    void AABB3<T>::reset( const Vector3<T> &initValue )
    {
        m_maximum = initValue;
        m_minimum = initValue;
    }

    template <class T>
    void AABB3<T>::merge( T x, T y, T z )
    {
        if( x > m_maximum.X() )
            m_maximum.X() = x;
        if( y > m_maximum.Y() )
            m_maximum.Y() = y;
        if( z > m_maximum.Z() )
            m_maximum.Z() = z;

        if( x < m_minimum.X() )
            m_minimum.X() = x;
        if( y < m_minimum.Y() )
            m_minimum.Y() = y;
        if( z < m_minimum.Z() )
            m_minimum.Z() = z;
    }

    template <class T>
    bool AABB3<T>::isPointInside( const Vector3<T> &p ) const
    {
        return ( p.X() >= m_minimum.X() && p.X() <= m_maximum.X() && p.Y() >= m_minimum.Y() &&
                 p.Y() <= m_maximum.Y() && p.Z() >= m_minimum.Z() && p.Z() <= m_maximum.Z() );
    };

    template <class T>
    bool AABB3<T>::isPointTotalInside( const Vector3<T> &p ) const
    {
        return ( p.X() > m_minimum.X() && p.X() < m_maximum.X() && p.Y() > m_minimum.Y() &&
                 p.Y() < m_maximum.Y() && p.Z() > m_minimum.Z() && p.Z() < m_maximum.Z() );
    };

    template <class T>
    bool AABB3<T>::intersects( const AABB3<T> &other ) const
    {
        return ( m_minimum.Z() <= other.m_maximum.Z() && m_maximum.Z() >= other.m_minimum.Z() ) &&
               ( m_minimum.X() <= other.m_maximum.X() && m_maximum.X() >= other.m_minimum.X() ) &&
               ( m_minimum.Y() <= other.m_maximum.Y() && m_maximum.Y() >= other.m_minimum.Y() );
    }

    template <class T>
    bool AABB3<T>::isFullInside( const AABB3<T> &other ) const
    {
        return ( m_minimum.Z() >= other.m_minimum.Z() && m_maximum.Z() <= other.m_maximum.Z() ) &&
               ( m_minimum.X() >= other.m_minimum.X() && m_maximum.X() <= other.m_maximum.X() ) &&
               ( m_minimum.Y() >= other.m_minimum.Y() && m_maximum.Y() <= other.m_maximum.Y() );
    }

    template <class T>
    bool AABB3<T>::intersectsWithLine( const Line3<T> &line ) const
    {
        auto lineVector = line.getVector();
        auto lineLength = lineVector.normaliseLength();
        return intersectsWithLine( line.getMiddle(), lineVector, lineLength * T( 0.5 ) );
    }

    template <class T>
    bool AABB3<T>::intersectsWithLine( const Vector3<T> &linemiddle, const Vector3<T> &linevect,
                                       T halflength ) const
    {
        const Vector3<T> e = getExtent() * static_cast<T>( 0.5 );
        const Vector3<T> t = getCenter() - linemiddle;
        T r;

        if( ( Math<T>::Abs( t.X() ) > e.X() + halflength * Math<T>::Abs( linevect.X() ) ) ||
            ( Math<T>::Abs( t.Y() ) > e.Y() + halflength * Math<T>::Abs( linevect.Y() ) ) ||
            ( Math<T>::Abs( t.Z() ) > e.Z() + halflength * Math<T>::Abs( linevect.Z() ) ) )
            return false;

        r = e.Y() * static_cast<T>( Math<T>::Abs( linevect.Z() ) ) +
            e.Z() * static_cast<T>( Math<T>::Abs( linevect.Y() ) );
        if( Math<T>::Abs( t.Y() * linevect.Z() - t.Z() * linevect.Y() ) > r )
            return false;

        r = e.X() * static_cast<T>( Math<T>::Abs( linevect.Z() ) ) +
            e.Z() * static_cast<T>( Math<T>::Abs( linevect.X() ) );
        if( Math<T>::Abs( t.Z() * linevect.X() - t.X() * linevect.Z() ) > r )
            return false;

        r = e.X() * static_cast<T>( Math<T>::Abs( linevect.Y() ) ) +
            e.Y() * static_cast<T>( Math<T>::Abs( linevect.X() ) );
        if( Math<T>::Abs( t.X() * linevect.Y() - t.Y() * linevect.X() ) > r )
            return false;

        return true;
    }

    template <class T>
    s32 AABB3<T>::classifyPlaneRelation( const Plane3<T> &plane ) const
    {
        auto nearPoint = Vector3<T>( m_maximum );
        auto farPoint = Vector3<T>( m_minimum );

        auto planeNormal = plane.getNormal();
        auto planeDistance = plane.getDistance();

        if( planeNormal.X() > T( 0.0 ) )
        {
            nearPoint.X() = m_minimum.X();
            farPoint.X() = m_maximum.X();
        }

        if( planeNormal.Y() > T( 0.0 ) )
        {
            nearPoint.Y() = m_minimum.Y();
            farPoint.Y() = m_maximum.Y();
        }

        if( planeNormal.Z() > T( 0.0 ) )
        {
            nearPoint.Z() = m_minimum.Z();
            farPoint.Z() = m_maximum.Z();
        }

        if( planeNormal.dotProduct( nearPoint ) + planeDistance > T( 0.0 ) )
        {
            return static_cast<s32>( Plane3<T>::EIntersectionRelation3D::ISREL3D_FRONT );
        }

        if( planeNormal.dotProduct( farPoint ) + planeDistance > T( 0.0 ) )
        {
            return static_cast<s32>( Plane3<T>::EIntersectionRelation3D::ISREL3D_CLIPPED );
        }

        return static_cast<s32>( Plane3<T>::EIntersectionRelation3D::ISREL3D_BACK );
    }

    template <class T>
    Vector3<T> AABB3<T>::getCenter() const
    {
        return ( m_minimum + m_maximum ) / T( 2.0 );
    }

    template <class T>
    Vector3<T> AABB3<T>::getExtent() const
    {
        return m_maximum - m_minimum;
    }

    template <class T>
    void AABB3<T>::getEdges( Vector3<T> *edges ) const
    {
        const Vector3<T> middle = getCenter();
        const Vector3<T> diag = middle - m_maximum;

        /*
        s are stored in this way:
        Hey, am I an ascii artist, or what? :) niko.
        /4--------/0
        /  |      / |
        /   |     /  |
        6---------2  |
        |   5- - -| -1
        |  /      |  /
        |/        | /
        7---------3/
        */

        edges[0].set( middle.X() + diag.X(), middle.Y() + diag.Y(), middle.Z() + diag.Z() );
        edges[1].set( middle.X() + diag.X(), middle.Y() - diag.Y(), middle.Z() + diag.Z() );
        edges[2].set( middle.X() + diag.X(), middle.Y() + diag.Y(), middle.Z() - diag.Z() );
        edges[3].set( middle.X() + diag.X(), middle.Y() - diag.Y(), middle.Z() - diag.Z() );
        edges[4].set( middle.X() - diag.X(), middle.Y() + diag.Y(), middle.Z() + diag.Z() );
        edges[5].set( middle.X() - diag.X(), middle.Y() - diag.Y(), middle.Z() + diag.Z() );
        edges[6].set( middle.X() - diag.X(), middle.Y() + diag.Y(), middle.Z() - diag.Z() );
        edges[7].set( middle.X() - diag.X(), middle.Y() - diag.Y(), middle.Z() - diag.Z() );
    }

    template <class T>
    bool AABB3<T>::isEmpty() const
    {
        return m_minimum == m_maximum;
    }

    template <class T>
    void AABB3<T>::repair()
    {
        T t;

        if( m_minimum.X() > m_maximum.X() )
        {
            t = m_minimum.X();
            m_minimum.X() = m_maximum.X();
            m_maximum.X() = t;
        }

        if( m_minimum.Y() > m_maximum.Y() )
        {
            t = m_minimum.Y();
            m_minimum.Y() = m_maximum.Y();
            m_maximum.Y() = t;
        }

        if( m_minimum.Z() > m_maximum.Z() )
        {
            t = m_minimum.Z();
            m_minimum.Z() = m_maximum.Z();
            m_maximum.Z() = t;
        }
    }

    template <class T>
    AABB3<T> AABB3<T>::getInterpolated( const AABB3<T> &other, f32 d ) const
    {
        auto inv = T( 1.0 ) - d;

        auto minInv = Vector3<T>( static_cast<T>( static_cast<f32>( other.m_minimum.X() ) * inv ),
                                  static_cast<T>( static_cast<f32>( other.m_minimum.Y() ) * inv ),
                                  static_cast<T>( static_cast<f32>( other.m_minimum.Z() ) * inv ) );
        auto maxInv = Vector3<T>( static_cast<T>( static_cast<f32>( other.m_maximum.X() ) * inv ),
                                  static_cast<T>( static_cast<f32>( other.m_maximum.Y() ) * inv ),
                                  static_cast<T>( static_cast<f32>( other.m_maximum.Z() ) * inv ) );

        auto minD = Vector3<T>( static_cast<T>( static_cast<f32>( m_minimum.X() ) * d ),
                                static_cast<T>( static_cast<f32>( m_minimum.Y() ) * d ),
                                static_cast<T>( static_cast<f32>( m_minimum.Z() ) * d ) );
        auto maxD = Vector3<T>( static_cast<T>( static_cast<f32>( m_maximum.X() ) * d ),
                                static_cast<T>( static_cast<f32>( m_maximum.Y() ) * d ),
                                static_cast<T>( static_cast<f32>( m_maximum.Z() ) * d ) );

        return AABB3<T>( minInv + minD, maxInv + maxD );
    }

    template <class T>
    Vector3<T> AABB3<T>::getHalfSize( void ) const
    {
        return ( m_maximum - m_minimum ) * T( 0.5 );
    }

    template <class T>
    void AABB3<T>::transform( const Matrix4<T> &matrix )
    {
        Vector3<T> oldMin, oldMax, currentCorner;

        // Getting the old values so that we can use the existing merge method.
        oldMin = m_minimum;
        oldMax = m_maximum;

        // reset
        m_minimum = Vector3<T>( 1e10, 1e10, 1e10 );
        m_maximum = Vector3<T>( -1e10, -1e10, -1e10 );

        // We sequentially compute the corners in the following order :
        // 0, 6, 5, 1, 2, 4 ,7 , 3
        // This sequence allows us to only change one member at a time to get at all corners.

        // For each one, we transform it using the matrix
        // Which gives the resulting point and merge the resulting point.

        // First corner
        // min min min
        currentCorner = oldMin;
        merge( matrix * currentCorner );

        // min,min,max
        currentCorner.Z() = oldMax.Z();
        merge( matrix * currentCorner );

        // min max max
        currentCorner.Y() = oldMax.Y();
        merge( matrix * currentCorner );

        // min max min
        currentCorner.Z() = oldMin.Z();
        merge( matrix * currentCorner );

        // max max min
        currentCorner.X() = oldMax.X();
        merge( matrix * currentCorner );

        // max max max
        currentCorner.Z() = oldMax.Z();
        merge( matrix * currentCorner );

        // max min max
        currentCorner.Y() = oldMin.Y();
        merge( matrix * currentCorner );

        // max min min
        currentCorner.Z() = oldMin.Z();
        merge( matrix * currentCorner );
    }

    template <class T>
    void AABB3<T>::setExtents( const Vector3<T> &min, const Vector3<T> &max )
    {
        FB_ASSERT( ( min.X() <= max.X() && min.Y() <= max.Y() && min.Z() <= max.Z() ) );

        m_extent = Extent::Finite;
        m_minimum = min;
        m_maximum = max;
    }

    template <class T>
    void AABB3<T>::setNull()
    {
        m_extent = Extent::Null;
    }

    template <class T>
    bool AABB3<T>::isNull( void ) const
    {
        return ( m_extent == Extent::Null );
    }

    template <class T>
    bool AABB3<T>::isFinite( void ) const
    {
        return ( m_extent == Extent::Finite );
    }

    template <class T>
    void AABB3<T>::setInfinite()
    {
        m_extent = Extent::Infinite;
    }

    template <class T>
    bool AABB3<T>::isInfinite( void ) const
    {
        return ( m_extent == Extent::Infinite );
    }

    template <class T>
    bool AABB3<T>::isValid( void ) const
    {
        return isFinite() && m_minimum.isValid() && m_maximum.isValid();
    }

    template <class T>
    const Vector3<T> &AABB3<T>::getMinimum() const
    {
        FB_ASSERT( m_minimum.isValid() );
        return m_minimum;
    }

    template <class T>
    void AABB3<T>::setMinimum( const Vector3<T> &minimum )
    {
        m_minimum = minimum;
    }

    template <class T>
    const Vector3<T> &AABB3<T>::getMaximum() const
    {
        FB_ASSERT( m_maximum.isValid() );
        return m_maximum;
    }

    template <class T>
    void AABB3<T>::setMaximum( const Vector3<T> &maximum )
    {
        m_maximum = maximum;
    }

    /// A typedef for an integer 3d axis aligned bounding box.
    using AABB3I = AABB3<s32>;

    /// A typedef for an float 3d axis aligned bounding box.
    using AABB3F = AABB3<f32>;

    /// A typedef for an double 3d axis aligned bounding box.
    using AABB3D = AABB3<f64>;

}  // end namespace fb

#endif
