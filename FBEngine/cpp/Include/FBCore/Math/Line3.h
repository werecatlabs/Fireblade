// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __FB_LINE_3D_H_INCLUDED__
#define __FB_LINE_3D_H_INCLUDED__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    /** A 3D line between two points with intersection methods.
     */
    template <class T>
    class Line3
    {
    public:
        /** Default constructor. */
        Line3();

        /** Constructor. */
        Line3( T xa, T ya, T za, T xb, T yb, T zb );

        /** Constructor. */
        Line3( const Vector3<T> &start, const Vector3<T> &end );

        /** Copy constructor. */
        Line3( const Line3<T> &other );

        // Operators

        /** Addition operator. */
        Line3<T> operator+( const Vector3<T> &point ) const;

        /** Addition assignment operator. */
        Line3<T> &operator+=( const Vector3<T> &point );

        /** Subtraction operator. */
        Line3<T> operator-( const Vector3<T> &point ) const;

        /** Subtraction assignment operator. */
        Line3<T> &operator-=( const Vector3<T> &point );

        /** Equality operator. */
        bool operator==( const Line3<T> &other ) const;

        /** Inequality operator. */
        bool operator!=( const Line3<T> &other ) const;

        // Functions

        /** Set the line. */
        void setLine( const T &xa, const T &ya, const T &za, const T &xb, const T &yb, const T &zb );

        /** Set the line. */
        void setLine( const Vector3<T> &nstart, const Vector3<T> &nend );

        /** Set the line. */
        void setLine( const Line3<T> &line );

        /** Get the length of the line. */
        f64 getLength() const;

        /** Get the squared length of the line. */
        T getLengthSQ() const;

        /** Get the middle of the line. */
        Vector3<T> getMiddle() const;

        /** Get the vector of the line. */
        Vector3<T> getVector() const;

        /** Check if a point is between the start and end points of the line. */
        bool isPointBetweenStartAndEnd( const Vector3<T> &point ) const;

        /** Get the closest point on the line to a point. */
        Vector3<T> getClosestPoint( const Vector3<T> &point ) const;

        /** Check if the line intersects with a sphere. */
        bool getIntersectionWithSphere( Vector3<T> sorigin, T sradius, f64 &outdistance ) const;

        /** Get the start point of the line. */
        Vector3<T> getStart() const;

        /** Set the start point of the line. */
        void setStart( Vector3<T> val );

        /** Get the end point of the line. */
        Vector3<T> getEnd() const;

        /** Set the end point of the line. */
        void setEnd( Vector3<T> end );

        /** Get the direction of the line. */
        Vector3<T> getDirection() const;

    private:
        /// < Start point of the line.
        Vector3<T> m_start;

        /// < End point of the line.
        Vector3<T> m_end;
    };

    template <class T>
    Line3<T>::Line3() : m_start( 0, 0, 0 ), m_end( 1, 1, 1 )
    {
    }

    template <class T>
    Line3<T>::Line3( T xa, T ya, T za, T xb, T yb, T zb ) : m_start( xa, ya, za ), m_end( xb, yb, zb )
    {
    }

    template <class T>
    Line3<T>::Line3( const Vector3<T> &start, const Vector3<T> &end ) : m_start( start ), m_end( end )
    {
    }

    template <class T>
    Line3<T>::Line3( const Line3<T> &other ) : m_start( other.m_start ), m_end( other.m_end )
    {
    }

    template <class T>
    Line3<T> Line3<T>::operator+( const Vector3<T> &point ) const
    {
        return Line3<T>( m_start + point, m_end + point );
    }

    template <class T>
    Line3<T> &Line3<T>::operator+=( const Vector3<T> &point )
    {
        m_start += point;
        m_end += point;
        return *this;
    }

    template <class T>
    Line3<T> Line3<T>::operator-( const Vector3<T> &point ) const
    {
        return Line3<T>( m_start - point, m_end - point );
    }

    template <class T>
    Line3<T> &Line3<T>::operator-=( const Vector3<T> &point )
    {
        m_start -= point;
        m_end -= point;
        return *this;
    }

    template <class T>
    bool Line3<T>::operator==( const Line3<T> &other ) const
    {
        return ( m_start == other.m_start && m_end == other.m_end ) ||
               ( m_end == other.m_start && m_start == other.m_end );
    }

    template <class T>
    bool Line3<T>::operator!=( const Line3<T> &other ) const
    {
        return !( m_start == other.m_start && m_end == other.m_end ) ||
               ( m_end == other.m_start && m_start == other.m_end );
    }

    template <class T>
    void Line3<T>::setLine( const T &xa, const T &ya, const T &za, const T &xb, const T &yb,
                            const T &zb )
    {
        m_start.set( xa, ya, za );
        m_end.set( xb, yb, zb );
    }

    template <class T>
    void Line3<T>::setLine( const Vector3<T> &nstart, const Vector3<T> &nend )
    {
        m_start.set( nstart );
        m_end.set( nend );
    }

    template <class T>
    void Line3<T>::setLine( const Line3<T> &line )
    {
        m_start.set( line.Start );
        m_end.set( line.End );
    }

    template <class T>
    f64 Line3<T>::getLength() const
    {
        return m_start.getDistanceFrom( m_end );
    }

    template <class T>
    T Line3<T>::getLengthSQ() const
    {
        return m_start.getDistanceFromSQ( m_end );
    }

    template <class T>
    Vector3<T> Line3<T>::getMiddle() const
    {
        return ( m_start + m_end ) * static_cast<T>( 0.5 );
    }

    template <class T>
    Vector3<T> Line3<T>::getVector() const
    {
        return m_end - m_start;
    }

    template <class T>
    bool Line3<T>::isPointBetweenStartAndEnd( const Vector3<T> &point ) const
    {
        return point.isBetweenPoints( m_start, m_end );
    }

    template <class T>
    Vector3<T> Line3<T>::getClosestPoint( const Vector3<T> &point ) const
    {
        Vector3<T> c = point - m_start;
        Vector3<T> v = m_end - m_start;
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
    bool Line3<T>::getIntersectionWithSphere( Vector3<T> sorigin, T sradius, f64 &outdistance ) const
    {
        Vector3<T> q = sorigin - m_start;
        f64 c = q.length();
        f64 v = q.dotProduct( getVector().normalize() );
        f64 d = sradius * sradius - ( c * c - v * v );

        if( d < 0.0 )
            return false;

        outdistance = v - Math<T>::Sqrt( d );
        return true;
    }

    template <class T>
    Vector3<T> Line3<T>::getStart() const
    {
        return m_start;
    }

    template <class T>
    void Line3<T>::setStart( const Vector3<T> start )
    {
        m_start = start;
    }

    template <class T>
    Vector3<T> Line3<T>::getEnd() const
    {
        return m_end;
    }

    template <class T>
    void Line3<T>::setEnd( const Vector3<T> end )
    {
        m_end = end;
    }

    template <class T>
    Vector3<T> Line3<T>::getDirection() const
    {
        return ( m_end - m_start ).normaliseCopy();
    }

    //! Typedef for a f64 line.
    using Line3D = Line3<f64>;

    //! Typedef for a f32 line.
    using Line3F = Line3<f32>;

    //! Typedef for a s32 line.
    using Line3I = Line3<s32>;

}  // end namespace fb

#endif
