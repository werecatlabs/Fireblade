#ifndef Plane3_h__
#define Plane3_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    //! Template plane class with some intersection testing methods.
    template <class T>
    class Plane3
    {
    public:
        //! Enumeration for intersection relations of 3d objects
        enum class EIntersectionRelation3D
        {
            ISREL3D_FRONT,
            ISREL3D_BACK,
            ISREL3D_PLANAR,
            ISREL3D_SPANNING,
            ISREL3D_CLIPPED
        };

        /** The "positive side" of the plane is the half space to which the
            plane normal points. The "negative side" is the other half
            space. The flag "no side" indicates the plane itself.
        */
        enum class Side
        {
            NO_SIDE,
            POSITIVE_SIDE,
            NEGATIVE_SIDE,
            BOTH_SIDE
        };

        // Constructors
        Plane3();
        Plane3( const Vector3<T> &MPoint, const Vector3<T> &Normal );
        Plane3( T px, T py, T pz, T nx, T ny, T nz );
        Plane3( const Plane3<T> &other );
        Plane3( const Vector3<T> &point1, const Vector3<T> &point2, const Vector3<T> &point3 );

        // operators
        bool operator==( const Plane3<T> &other ) const;
        bool operator!=( const Plane3<T> &other ) const;

        // functions
        void setPlane( const Vector3<T> &point, const Vector3<T> &nvector );
        void setPlane( const Vector3<T> &nvect, T d );
        void setPlane( const Vector3<T> &point1, const Vector3<T> &point2, const Vector3<T> &point3 );

        //! Returns an intersection with a 3d line.
        //! \param lineVect: Vector of the line to intersect with.
        //! \param linePoint: Point of the line to intersect with.
        //! \param outIntersection: Place to store the intersection point, if there is one.
        //! \return Returns true if there was an intersection, false if there was not.
        bool getIntersectionWithLine( const Vector3<T> &linePoint, const Vector3<T> &lineVect,
                                      Vector3<T> &outIntersection ) const;

        //! Returns where on a line between two points an intersection with this plane happened.
        //! Only useful if known that there is an intersection.
        //! \param linePoint1: Point1 of the line to intersect with.
        //! \param linePoint2: Point2 of the line to intersect with.
        //! \return Returns where on a line between two points an intersection with this plane happened.
        //! For example, 0.5 is returned if the intersection happened exectly in the middle of the two
        //! points.
        f32 getKnownIntersectionWithLine( const Vector3<T> &linePoint1,
                                          const Vector3<T> &linePoint2 ) const;

        //! Returns an intersection with a 3d line, limited between two 3d points.
        //! \param linePoint1: Point 1 of the line.
        //! \param linePoint2: Point 2 of the line.
        //! \param outIntersection: Place to store the intersection point, if there is one.
        //! \return Returns true if there was an intersection, false if there was not.
        bool getIntersectionWithLimitedLine( const Vector3<T> &linePoint1, const Vector3<T> &linePoint2,
                                             Vector3<T> &outIntersection ) const;

        //! Classifies the relation of a point to this plane.
        //! \param point: Point to classify its relation.
        //! \return Returns ISREL3D_FRONT if the point is in front of the plane,
        //! ISREL3D_BACK if the point is behind of the plane, and
        //! ISREL3D_PLANAR if the point is within the plane.
        s32 classifyPointRelation( const Vector3<T> &point ) const;

        //! Recalculates the distance from origin by applying
        //! a new member point to the plane.
        void recalculateD( const Vector3<T> &MPoint );

        //! Returns a member point of the plane.
        Vector3<T> getMemberPoint() const;
        //! Tests if there is a intersection between this plane and another
        //! \return Returns true if there is a intersection.
        bool existsInterSection( const Plane3<T> &other ) const;

        //! Intersects this plane with another.
        //! \return Returns true if there is a intersection, false if not.
        bool getIntersectionWithPlane( const Plane3<T> &other, Vector3<T> &outLinePoint,
                                       Vector3<T> &outLineVect ) const;

        //! Returns the intersection point with two other planes if there is one.
        bool getIntersectionWithPlanes( const Plane3<T> &o1, const Plane3<T> &o2,
                                        Vector3<T> &outPoint ) const;

        //! Returns if the plane is front of backfacing. Note that this only
        //! works if the normal is Normalized.
        //! \param lookDirection: Look direction.
        //! \return Returns true if the plane is front facing, which mean it would
        //! be visible, and false if it is backfacing.
        bool isFrontFacing( const Vector3<T> &lookDirection ) const;

        //! Returns the distance to a point.  Note that this only
        //! works if the normal is Normalized.
        T getDistance( const Vector3<T> &point ) const;

        s32 getSide( const Vector3<T> &centre, const Vector3<T> &halfSize ) const;

        // member variables
        Vector3<T> getNormal() const;
        void setNormal( const Vector3<T> &normal );

        T getDistance() const;
        void setDistance( T distance );

    private:
        Vector3<T> m_normal;  // normal vector
        T m_distance;         // distance from origin
    };

    template <class T>
    Plane3<T>::Plane3() : m_normal( 0, 1, 0 )
    {
        recalculateD( Vector3<T>( 0, 0, 0 ) );
    }

    template <class T>
    Plane3<T>::Plane3( const Vector3<T> &point, const Vector3<T> &normal ) : m_normal( normal )
    {
        recalculateD( point );
    }

    template <class T>
    Plane3<T>::Plane3( T px, T py, T pz, T nx, T ny, T nz ) : m_normal( nx, ny, nz )
    {
        recalculateD( Vector3<T>( px, py, pz ) );
    }

    template <class T>
    Plane3<T>::Plane3( const Plane3<T> &other ) :
        m_normal( other.m_normal ),
        m_distance( other.m_distance )
    {
    }

    template <class T>
    Plane3<T>::Plane3( const Vector3<T> &point1, const Vector3<T> &point2, const Vector3<T> &point3 )
    {
        setPlane( point1, point2, point3 );
    }

    template <class T>
    bool Plane3<T>::operator==( const Plane3<T> &other ) const
    {
        return ( m_distance == other.m_distance && m_normal == other.m_normal );
    }

    template <class T>
    bool Plane3<T>::operator!=( const Plane3<T> &other ) const
    {
        return !( m_distance == other.m_distance && m_normal == other.m_normal );
    }

    template <class T>
    void Plane3<T>::setPlane( const Vector3<T> &point, const Vector3<T> &nvector )
    {
        m_normal = nvector;
        m_normal.normalize();
        recalculateD( point );
    }

    template <class T>
    void Plane3<T>::setPlane( const Vector3<T> &nvect, T d )
    {
        m_normal = nvect;
        m_distance = d;
    }

    template <class T>
    void Plane3<T>::setPlane( const Vector3<T> &point1, const Vector3<T> &point2,
                              const Vector3<T> &point3 )
    {
        // creates the plane from 3 memberpoints
        m_normal = ( point2 - point1 ).crossProduct( point3 - point1 );
        m_normal.normalise();

        recalculateD( point1 );
    }

    template <class T>
    bool Plane3<T>::getIntersectionWithLine( const Vector3<T> &linePoint, const Vector3<T> &lineVect,
                                             Vector3<T> &outIntersection ) const
    {
        T t2 = m_normal.dotProduct( lineVect );

        if( t2 == 0 )
            return false;

        T t = -( m_normal.dotProduct( linePoint ) + m_distance ) / t2;
        outIntersection = linePoint + ( lineVect * t );
        return true;
    }

    template <class T>
    f32 Plane3<T>::getKnownIntersectionWithLine( const Vector3<T> &linePoint1,
                                                 const Vector3<T> &linePoint2 ) const
    {
        Vector3<T> vect = linePoint2 - linePoint1;
        T t2 = static_cast<f32>( m_normal.dotProduct( vect ) );
        return static_cast<f32>( -( ( m_normal.dotProduct( linePoint1 ) + m_distance ) / t2 ) );
    }

    template <class T>
    bool Plane3<T>::getIntersectionWithLimitedLine( const Vector3<T> &linePoint1,
                                                    const Vector3<T> &linePoint2,
                                                    Vector3<T> &outIntersection ) const
    {
        return ( getIntersectionWithLine( linePoint1, linePoint2 - linePoint1, outIntersection ) &&
                 outIntersection.isBetweenPoints( linePoint1, linePoint2 ) );
    }

    template <class T>
    s32 Plane3<T>::classifyPointRelation( const Vector3<T> &point ) const
    {
        const T d = m_normal.dotProduct( point ) + m_distance;

        if( d < -Math<T>::epsilon() )
        {
            return EIntersectionRelation3D::ISREL3D_FRONT;
        }

        if( d > Math<T>::epsilon() )
        {
            return EIntersectionRelation3D::ISREL3D_BACK;
        }

        return EIntersectionRelation3D::ISREL3D_PLANAR;
    }

    template <class T>
    void Plane3<T>::recalculateD( const Vector3<T> &MPoint )
    {
        m_distance = -MPoint.dotProduct( m_normal );
    }

    template <class T>
    Vector3<T> Plane3<T>::getMemberPoint() const
    {
        return m_normal * -m_distance;
    }

    template <class T>
    bool Plane3<T>::existsInterSection( const Plane3<T> &other ) const
    {
        Vector3<T> cross = other.m_normal.crossProduct( m_normal );
        return cross.length() > MathF::epsilon();
    }

    template <class T>
    bool Plane3<T>::getIntersectionWithPlane( const Plane3<T> &other, Vector3<T> &outLinePoint,
                                              Vector3<T> &outLineVect ) const
    {
        T fn00 = m_normal.length();
        T fn01 = m_normal.dotProduct( other.m_normal );
        T fn11 = other.m_normal.length();
        T det = fn00 * fn11 - fn01 * fn01;

        if( fabs( det ) < Math<T>::epsilon() )
            return false;

        det = 1.0 / det;
        T fc0 = ( fn11 * -m_distance + fn01 * other.m_distance ) * det;
        T fc1 = ( fn00 * -other.m_distance + fn01 * m_distance ) * det;

        outLineVect = m_normal.crossProduct( other.m_normal );
        outLinePoint = m_normal * static_cast<T>( fc0 ) + other.m_normal * static_cast<T>( fc1 );
        return true;
    }

    template <class T>
    bool Plane3<T>::getIntersectionWithPlanes( const Plane3<T> &o1, const Plane3<T> &o2,
                                               Vector3<T> &outPoint ) const
    {
        Vector3<T> linePoint, lineVect;
        if( getIntersectionWithPlane( o1, linePoint, lineVect ) )
            return o2.getIntersectionWithLine( linePoint, lineVect, outPoint );

        return false;
    }

    template <class T>
    bool Plane3<T>::isFrontFacing( const Vector3<T> &lookDirection ) const
    {
        const f32 d = m_normal.dotProduct( lookDirection );
        return Math<T>::equals( d, T( 0.0 ) );
    }

    template <class T>
    T Plane3<T>::getDistance( const Vector3<T> &point ) const
    {
        return point.dotProduct( m_normal ) + m_distance;
    }

    template <class T>
    s32 Plane3<T>::getSide( const Vector3<T> &centre, const Vector3<T> &halfSize ) const
    {
        // Calculate the distance between box centre and the plane
        T dist = getDistance( centre );

        // Calculate the maximise allows absolute distance for
        // the distance between box centre and plane
        T maxAbsDist = Math<T>::Abs( m_normal.X() * halfSize.X() ) +
                       Math<T>::Abs( m_normal.Y() * halfSize.Y() ) +
                       Math<T>::Abs( m_normal.Z() * halfSize.Z() );

        if( dist < -maxAbsDist )
        {
            return Side::NEGATIVE_SIDE;
        }

        if( dist > maxAbsDist )
        {
            return Side::POSITIVE_SIDE;
        }

        return Side::BOTH_SIDE;
    }

    template <class T>
    Vector3<T> Plane3<T>::getNormal() const
    {
        return m_normal;
    }

    template <class T>
    void Plane3<T>::setNormal( const Vector3<T> &normal )
    {
        m_normal = normal;
    }

    template <class T>
    T Plane3<T>::getDistance() const
    {
        return m_distance;
    }

    template <class T>
    void Plane3<T>::setDistance( T distance )
    {
        m_distance = distance;
    }

    //! Typedef for an integer 3d plane.
    using Plane3I = Plane3<s32>;

    //! Typedef for a f32 3d plane.
    using Plane3F = Plane3<f32>;

    //! Typedef for a f32 3d plane.
    using Plane3D = Plane3<f64>;

}  // end namespace fb

#endif  // Plane3_h__
