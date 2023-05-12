#ifndef __FB_LINE2_H_
#define __FB_LINE2_H_

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    //! A 2D line class between two points with intersection methods.
    template <class T>
    class Line2
    {
    public:
        //! Default constructor.
        Line2();

        //! Constructor with coordinates of start and end points.
        Line2( T xa, T ya, T xb, T yb );

        //! Constructor with start and end vectors.
        Line2( const Vector2<T> &start, const Vector2<T> &end );

        //! Copy constructor.
        Line2( const Line2<T> &other );

        // Operators

        //! Add a point to this line.
        Line2<T> operator+( const Vector2<T> &point ) const;

        //! Add a point to this line.
        Line2<T> &operator+=( const Vector2<T> &point );

        //! Subtract a point from this line.
        Line2<T> operator-( const Vector2<T> &point ) const;

        //! Subtract a point from this line.
        Line2<T> &operator-=( const Vector2<T> &point );

        //! Check if two lines are equal.
        bool operator==( const Line2<T> &other ) const;

        //! Check if two lines are not equal.
        bool operator!=( const Line2<T> &other ) const;

        // Functions

        //! Set the start and end points of this line.
        void setLine( const T &xa, const T &ya, const T &xb, const T &yb );

        //! Set the start and end points of this line using vectors.
        void setLine( const Vector2<T> &start, const Vector2<T> &end );

        //! Set the start and end points of this line using another line.
        void setLine( const Line2<T> &line );

        //! Get the length of this line.
        T getLength() const;

        //! Get the squared length of this line.
        T getLengthSQ() const;

        //! Get the middle point of this line.
        Vector2<T> getMiddle() const;

        //! Get the vector of this line.
        Vector2<T> getVector() const;

        //! Check if this line intersects with another line.
        /*!
            \param l: Other line to test intersection with.
            \param out: If there is an intersection, the location of the intersection will
                be stored in this vector.
            \return Returns true if there is an intersection, false if not.
        */
        bool intersectWith( const Line2<T> &l, Vector2<T> &out ) const;

        //! Check if this line intersects with a sphere.
        bool intersectWith( const Vector2<T> &center, T radius ) const;

        //! Get the unit vector of this line.
        Vector2<T> getUnitVector() const;

        //! Get the direction of this line.
        Vector2<T> getDirection() const;

        //! Get the angle between this line and another line.
        T getAngleWith( const Line2<T> &l );

        //! Get the orientation of a point relative to this line.
        /*!
            \return Returns 0 if the point is on the line, <0 if to the left, or >0 if to the right.
        */
        T getPointOrientation( const Vector2<T> &point );

        //! Check if a point is on this line.
        bool isPointOnLine( const Vector2<T> &point );

        //! Check if a point is between the start and end points of this line.
        bool isPointBetweenStartAndEnd( const Vector2<T> &point ) const;

        //! Get the closest point on this line to a point.
        Vector2<T> getClosestPoint( const Vector2<T> &point ) const;

        //! Returns the starting point of the line.
        Vector2<T> getStart() const;

        //! Sets the starting point of the line to the given vector.
        void setStart( Vector2<T> &start );

        //! Returns the end point of the line.
        Vector2<T> getEnd() const;

        //! Sets the end point of the line to the given vector.
        void setEnd( const Vector2<T> &end );

    private:
        Vector2<T> m_start;  // Starting point of the line
        Vector2<T> m_end;    // End point of the line
    };

    template <class T>
    Line2<T>::Line2() : m_start( 0, 0 ), m_end( 1, 1 )
    {
    }

    template <class T>
    Line2<T>::Line2( T xa, T ya, T xb, T yb ) : m_start( xa, ya ), m_end( xb, yb )
    {
    }

    template <class T>
    Line2<T>::Line2( const Vector2<T> &start, const Vector2<T> &end ) : m_start( start ), m_end( end )
    {
    }

    template <class T>
    Line2<T>::Line2( const Line2<T> &other ) : m_start( other.m_start ), m_end( other.m_end )
    {
    }

    template <class T>
    Line2<T> Line2<T>::operator+( const Vector2<T> &point ) const
    {
        return Line2<T>( m_start + point, m_end + point );
    }

    template <class T>
    Line2<T> &Line2<T>::operator+=( const Vector2<T> &point )
    {
        m_start += point;
        m_end += point;
        return *this;
    }

    template <class T>
    Line2<T> Line2<T>::operator-( const Vector2<T> &point ) const
    {
        return Line2<T>( m_start - point, m_end - point );
    }

    template <class T>
    Line2<T> &Line2<T>::operator-=( const Vector2<T> &point )
    {
        m_start -= point;
        m_end -= point;
        return *this;
    }

    template <class T>
    bool Line2<T>::operator==( const Line2<T> &other ) const
    {
        return ( m_start == other.m_start && m_end == other.m_end ) ||
               ( m_end == other.m_start && m_start == other.m_end );
    }

    template <class T>
    bool Line2<T>::operator!=( const Line2<T> &other ) const
    {
        return !( m_start == other.m_start && m_end == other.m_end ) ||
               ( m_end == other.m_start && m_start == other.m_end );
    }

    template <class T>
    void Line2<T>::setLine( const T &xa, const T &ya, const T &xb, const T &yb )
    {
        m_start.set( xa, ya );
        m_end.set( xb, yb );
    }

    template <class T>
    void Line2<T>::setLine( const Vector2<T> &nstart, const Vector2<T> &nend )
    {
        m_start.set( nstart );
        m_end.set( nend );
    }

    template <class T>
    void Line2<T>::setLine( const Line2<T> &line )
    {
        m_start.set( line.m_start );
        m_end.set( line.m_end );
    }

    template <class T>
    T Line2<T>::getLength() const
    {
        return m_start.getDistanceFrom( m_end );
    }

    template <class T>
    T Line2<T>::getLengthSQ() const
    {
        return m_start.getDistanceFromSQ( m_end );
    }

    template <class T>
    Vector2<T> Line2<T>::getMiddle() const
    {
        return ( m_start + m_end ) * T( 0.5 );
    }

    template <class T>
    Vector2<T> Line2<T>::getVector() const
    {
        return Vector2<T>( m_start.X() - m_end.X(), m_start.Y() - m_end.Y() );
    }

    template <class T>
    Vector2<T> Line2<T>::getUnitVector() const
    {
        T len = static_cast<T>( 1.0 / getLength() );
        return Vector2<T>( ( m_end.X() - m_start.X() ) * len, ( m_end.Y() - m_start.Y() ) * len );
    }

    template <class T>
    Vector2<T> Line2<T>::getDirection() const
    {
        T len = static_cast<T>( 1.0 / getLength() );
        return Vector2<T>( ( m_end.X() - m_start.X() ) * len, ( m_end.Y() - m_start.Y() ) * len );
    }

    template <class T>
    T Line2<T>::getAngleWith( const Line2<T> &l )
    {
        Vector2<T> vect = getVector();
        Vector2<T> vect2 = l.getVector();
        return vect.getAngleWith( vect2 );
    }

    template <class T>
    T Line2<T>::getPointOrientation( const Vector2<T> &point )
    {
        return ( ( m_end.X() - m_start.X() ) * ( point.Y() - m_start.Y() ) -
                 ( point.X() - m_start.X() ) * ( m_end.Y() - m_start.Y() ) );
    }

    template <class T>
    bool Line2<T>::isPointOnLine( const Vector2<T> &point )
    {
        T d = getPointOrientation( point );
        return ( d == 0 && point.isBetweenPoints( m_start, m_end ) );
    }

    template <class T>
    bool Line2<T>::isPointBetweenStartAndEnd( const Vector2<T> &point ) const
    {
        return point.isBetweenPoints( m_start, m_end );
    }

    template <class T>
    Vector2<T> Line2<T>::getClosestPoint( const Vector2<T> &point ) const
    {
        Vector2<T> c = point - m_start;
        Vector2<T> v = m_end - m_start;
        T d = static_cast<T>( v.length() );
        v /= d;
        T t = v.dotProduct( c );

        if( t < static_cast<T>( 0.0 ) )
            return m_start;

        if( t > d )
            return m_end;

        v *= t;

        return m_start + v;
    }

    template <class T>
    Vector2<T> Line2<T>::getStart() const
    {
        return m_start;
    }

    template <class T>
    void Line2<T>::setStart( Vector2<T> &start )
    {
        m_start = start;
    }

    template <class T>
    Vector2<T> Line2<T>::getEnd() const
    {
        return m_end;
    }

    template <class T>
    void Line2<T>::setEnd( const Vector2<T> &end )
    {
        m_end = end;
    }

    /// A typedef for an integer line.
    using Line2I = Line2<s32>;

    /// A typedef for a float line.
    using Line2F = Line2<f32>;

    /// A typedef for a double line.
    using Line2D = Line2<f64>;

}  // end namespace fb

#endif
