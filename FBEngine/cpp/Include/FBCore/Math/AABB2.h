#ifndef _AABBOX2D_H_
#define _AABBOX2D_H_

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Line2.h>

namespace fb
{
    /** Axis aligned bounding box in 2d space.
     */
    template <class T>
    class AABB2
    {
    public:
        /** Default constructor.
         */
        AABB2() = default;

        /** Constructor.
         * @param x: Position on the x-axis.
         * @param y: Position on the y-axis.
         * @param x2: Position on the x-axis of the other corner.
         * @param y2: Position on the y-axis of the other corner.
         */
        AABB2( T x, T y, T x2, T y2 );

        /** Constructor.
         * @param min: Position of one corner.
         * @param max: Position of the other corner.
         */
        AABB2( const Vector2<T> &min, const Vector2<T> &max );

        /** Constructor.
         * @param other: Another AABB2.
         */
        AABB2( const AABB2<T> &other );

        /** Constructor.
         * @param pos: Position of one corner.
         * @param size: Size of the AABB2.
         * @param useExtents: If true, 'size' represents extents of the box. Otherwise, it's the maximum position.
         */
        AABB2( const Vector2<T> &pos, const Vector2<T> &size, bool useExtents );

        /** Adds a Vector2 to the box and returns a new box that contains both boxes.
         * @param pos: The Vector2 to add to the box.
         * @return Returns a new box that contains both boxes.
         */
        AABB2<T> operator+( const Vector2<T> &pos ) const;

        /** Adds a Vector2 to the box and returns a reference to the box.
         * @param pos: The Vector2 to add to the box.
         * @return Returns a reference to the box.
         */
        const AABB2<T> &operator+=( const Vector2<T> &pos );

        /** Subtracts a Vector2 from the box and returns a new box that contains both boxes.
         * @param pos: The Vector2 to subtract from the box.
         * @return Returns a new box that contains both boxes.
         */
        AABB2<T> operator-( const Vector2<T> &pos ) const;

        /** Subtracts a Vector2 from the box and returns a reference to the box.
         * @param pos: The Vector2 to subtract from the box.
         * @return Returns a reference to the box.
         */
        const AABB2<T> &operator-=( const Vector2<T> &pos );

        /** Checks if two AABB2 are the same.
         * @param other: Another AABB2.
         * @return Returns true if both AABB2 are the same, false if not.
         */
        bool operator==( const AABB2<T> &other ) const;

        /** Checks if two AABB2 are different.
         * @param other: Another AABB2.
         * @return Returns true if both AABB2 are different, false if not.
         */
        bool operator!=( const AABB2<T> &other ) const;

        /** Copies an AABB2 to this one.
         * @param other: Another AABB2.
         * @return Returns a reference to this AABB2.
         */
        const AABB2<T> &operator=( const AABB2<T> &other );

        /** Compares the size of two AABB2.
         * @param other: Another AABB2.
         * @return Returns true if this AABB2 is smaller than 'other', false if not.
         */
        bool operator<( const AABB2<T> &other ) const;

        /**
         * @brief Calculates the area of the AABB2.
         *
         * @return T The area of the AABB2.
         */
        T getArea() const;

        /**
         * @brief Determines if a point is inside the AABB2.
         *
         * @param pos The point to test.
         * @return true If the point is inside the AABB2.
         * @return false If the point is outside the AABB2.
         */
        bool isInside( const Vector2<T> &pos ) const;

        /**
         * @brief Determines if this AABB2 intersects with another AABB2.
         *
         * @param other The AABB2 to test for intersection.
         * @return true If the AABB2s intersect.
         * @return false If the AABB2s do not intersect.
         */
        bool intersects( const AABB2<T> &other ) const;

        /**
         * @brief Determines if this AABB2 intersects with a line segment.
         *
         * @param line The line segment to test for intersection.
         * @return true If the AABB2 intersects with the line segment.
         * @return false If the AABB2 does not intersect with the line segment.
         */
        bool intersects( const Line2<T> &line ) const;

        /**
         * @brief Clips this AABB2 with another AABB2.
         *
         * @param other The AABB2 to clip against.
         */
        void clipAgainst( const AABB2<T> &other );

        /**
         * @brief Constrains this AABB2 to fit inside another AABB2.
         *
         * @param other The AABB2 to fit inside.
         * @return true If this AABB2 can fit inside the other AABB2.
         * @return false If this AABB2 cannot fit inside the other AABB2.
         */
        bool constrainTo( const AABB2<T> &other );

        /**
         * @brief Returns the width of the AABB2.
         *
         * @return T The width of the AABB2.
         */
        T getWidth() const;

        /**
         * @brief Returns the height of the AABB2.
         *
         * @return T The height of the AABB2.
         */
        T getHeight() const;

        /**
         * @brief Swaps the minimum and maximum values if necessary to ensure that the minimum is
         * in the lower left and the maximum is in the upper right.
         */
        void repair();

        /**
         * @brief Determines if the AABB2 is valid to draw. An AABB2 is not valid if the minimum value
         * is to the right or above the maximum value, or if the area of the AABB2 is zero.
         *
         * @return true If the AABB2 is valid to draw.
         * @return false If the AABB2 is not valid to draw.
         */
        bool isValid() const;

        /**
         * @brief Returns the center point of the AABB2.
         *
         * @return Vector2<T> The center point of the AABB2.
         */
        Vector2<T> getCenter() const;

        /**
         * @brief Returns the dimensions of the AABB2.
         *
         * @return Vector2<T> The dimensions of the AABB2.
         */
        Vector2<T> getSize() const;

        /**
         * @brief Adds a point to the AABB2, causing it to grow bigger if the point is outside of the box.
         *
         * @param p The point to add.
         */
        void addInternalPoint( const Vector2<T> &p );

        /**
         * @brief Returns the minimum point of the AABB2.
         *
         * @return Vector2<T> The minimum point of the AABB2.
         */
        Vector2<T> getMin() const;

        /**
         * @brief Sets the minimum point of the AABB2.
         *
         * @param minimum The new minimum point.
         */
        void setMin( const Vector2<T> &minimum );

        /**
         * @brief Returns the maximum point of the AABB2.
         *
         * @return Vector2<T> The maximum point of the AABB2.
         */
        Vector2<T> getMax() const;

        /**
         * @brief Sets the maximum point of the AABB2.
         *
         * @param maximum The new maximum point.
         */
        void setMax( const Vector2<T> &maximum );

    private:
        Vector2<T> m_minimum;
        Vector2<T> m_maximum;
    };

    template <class T>
    AABB2<T>::AABB2( T x, T y, T x2, T y2 ) : m_minimum( x, y ), m_maximum( x2, y2 )
    {
    }

    template <class T>
    AABB2<T>::AABB2( const Vector2<T> &min, const Vector2<T> &max ) : m_minimum( min ), m_maximum( max )
    {
    }

    template <class T>
    AABB2<T>::AABB2( const AABB2<T> &other ) : m_minimum( other.m_minimum ), m_maximum( other.m_maximum )
    {
    }

    template <class T>
    AABB2<T>::AABB2( const Vector2<T> &pos, const Vector2<T> &size, bool useExtents )
    {
        if( !useExtents )
        {
            m_minimum = pos;
            m_maximum = Vector2<T>( pos.X() + size.X(), pos.Y() + size.Y() );
        }
        else
        {
            m_minimum = pos - size;
            m_maximum = pos + size;
        }
    }

    template <class T>
    AABB2<T> AABB2<T>::operator+( const Vector2<T> &pos ) const
    {
        AABB2<T> ret( *this );
        ret.m_minimum += pos;
        ret.m_maximum += pos;
        return ret;
    }

    template <class T>
    const AABB2<T> &AABB2<T>::operator+=( const Vector2<T> &pos )
    {
        m_minimum += pos;
        m_maximum += pos;
        return *this;
    }

    template <class T>
    AABB2<T> AABB2<T>::operator-( const Vector2<T> &pos ) const
    {
        AABB2<T> ret( *this );
        ret.m_minimum -= pos;
        ret.m_maximum -= pos;
        return ret;
    }

    template <class T>
    const AABB2<T> &AABB2<T>::operator-=( const Vector2<T> &pos )
    {
        m_minimum -= pos;
        m_maximum -= pos;
        return *this;
    }

    template <class T>
    bool AABB2<T>::operator==( const AABB2<T> &other ) const
    {
        return ( m_minimum == other.m_minimum && m_maximum == other.m_maximum );
    }

    template <class T>
    bool AABB2<T>::operator!=( const AABB2<T> &other ) const
    {
        return ( m_minimum != other.m_minimum || m_maximum != other.m_maximum );
    }

    template <class T>
    const AABB2<T> &AABB2<T>::operator=( const AABB2<T> &other )
    {
        m_minimum = other.m_minimum;
        m_maximum = other.m_maximum;
        return *this;
    }

    template <class T>
    bool AABB2<T>::operator<( const AABB2<T> &other ) const
    {
        return getArea() < other.getArea();
    }

    template <class T>
    T AABB2<T>::getArea() const
    {
        return getWidth() * getHeight();
    }

    template <class T>
    bool AABB2<T>::isInside( const Vector2<T> &pos ) const
    {
        return ( m_minimum.X() <= pos.X() && m_minimum.Y() <= pos.Y() && m_maximum.X() >= pos.X() &&
                 m_maximum.Y() >= pos.Y() );
    }

    template <class T>
    bool AABB2<T>::intersects( const AABB2<T> &other ) const
    {
        return ( m_maximum.Y() > other.m_minimum.Y() && m_minimum.Y() < other.m_maximum.Y() &&
                 m_maximum.X() > other.m_minimum.X() && m_minimum.X() < other.m_maximum.X() );
    }

    template <class T>
    bool AABB2<T>::intersects( const Line2<T> &line ) const
    {
        auto diff = line.getStart() - getCenter();
        auto perp = line.getDirection().perp();

        auto LHS = Math<T>::Abs( perp.dotProduct( diff ) );

        auto extent = getSize();
        auto RHS = extent[0] + extent[1];

        return LHS <= RHS;
    }

    template <class T>
    void AABB2<T>::clipAgainst( const AABB2<T> &other )
    {
        if( other.getMax().X() < m_maximum.X() )
        {
            m_maximum.X() = other.getMax().X();
        }

        if( other.getMax().Y() < m_maximum.Y() )
        {
            m_maximum.Y() = other.getMax().Y();
        }

        if( other.getMin().X() > m_minimum.X() )
        {
            m_minimum.X() = other.getMin().X();
        }

        if( other.getMin().Y() > m_minimum.Y() )
        {
            m_minimum.Y() = other.getMin().Y();
        }

        // correct possible invalid rect resulting from clipping
        if( m_minimum.Y() > m_maximum.Y() )
        {
            m_minimum.Y() = m_maximum.Y();
        }

        if( m_minimum.X() > m_maximum.X() )
        {
            m_minimum.X() = m_maximum.X();
        }
    }

    template <class T>
    bool AABB2<T>::constrainTo( const AABB2<T> &other )
    {
        if( other.getWidth() < getWidth() || other.getHeight() < getHeight() )
        {
            return false;
        }

        auto diff = other.m_maximum.X() - m_maximum.X();
        if( diff < 0 )
        {
            m_maximum.X() += diff;
            m_minimum.X() += diff;
        }

        diff = other.m_maximum.Y() - m_maximum.Y();
        if( diff < 0 )
        {
            m_maximum.Y() += diff;
            m_minimum.Y() += diff;
        }

        diff = m_minimum.X() - other.m_minimum.X();
        if( diff < 0 )
        {
            m_minimum.X() -= diff;
            m_maximum.X() -= diff;
        }

        diff = m_minimum.Y() - other.m_minimum.Y();
        if( diff < 0 )
        {
            m_minimum.Y() -= diff;
            m_maximum.Y() -= diff;
        }

        return true;
    }

    template <class T>
    T AABB2<T>::getWidth() const
    {
        return m_maximum.X() - m_minimum.X();
    }

    template <class T>
    T AABB2<T>::getHeight() const
    {
        return m_maximum.Y() - m_minimum.Y();
    }

    template <class T>
    void AABB2<T>::repair()
    {
        if( m_maximum.X() < m_minimum.X() )
        {
            auto t = m_maximum.X();
            m_maximum.X() = m_minimum.X();
            m_minimum.X() = t;
        }

        if( m_maximum.Y() < m_minimum.Y() )
        {
            auto t = m_maximum.Y();
            m_maximum.Y() = m_minimum.Y();
            m_minimum.Y() = t;
        }
    }

    template <class T>
    bool AABB2<T>::isValid() const
    {
        // thx to jox for a coraabbox2dion to this method
        auto xd = m_maximum.X() - m_minimum.X();
        auto yd = m_maximum.Y() - m_minimum.Y();

        return !( xd <= 0 || yd <= T( 0.0 ) || ( xd == 0 && yd == T( 0.0 ) ) );
    }

    template <class T>
    Vector2<T> AABB2<T>::getCenter() const
    {
        return Vector2<T>( ( m_minimum.X() + m_maximum.X() ) / T( 2.0 ),
                           ( m_minimum.Y() + m_maximum.Y() ) / T( 2.0 ) );
    }

    template <class T>
    Vector2<T> AABB2<T>::getSize() const
    {
        return Vector2<T>( getWidth(), getHeight() );
    }

    template <class T>
    void AABB2<T>::addInternalPoint( const Vector2<T> &p )
    {
        if( p.X() > m_maximum.X() )
        {
            m_maximum.X() = p.X();
        }

        if( p.Y() > m_maximum.Y() )
        {
            m_maximum.Y() = p.Y();
        }

        if( p.X() < m_minimum.X() )
        {
            m_minimum.X() = p.X();
        }

        if( p.Y() < m_minimum.Y() )
        {
            m_minimum.Y() = p.Y();
        }
    }

    template <class T>
    Vector2<T> AABB2<T>::getMin() const
    {
        return m_minimum;
    }

    template <class T>
    void AABB2<T>::setMin( const Vector2<T> &minimum )
    {
        m_minimum = minimum;
    }

    template <class T>
    Vector2<T> AABB2<T>::getMax() const
    {
        return m_maximum;
    }

    template <class T>
    void AABB2<T>::setMax( const Vector2<T> &maximum )
    {
        m_maximum = maximum;
    }

    using AABB2I = AABB2<s32>;
    using AABB2F = AABB2<f32>;
    using AABB2D = AABB2<f64>;

}  // end namespace fb

#endif
