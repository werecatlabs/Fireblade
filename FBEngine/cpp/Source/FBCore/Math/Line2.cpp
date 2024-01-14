#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/Line2.h>
#include <FBCore/Math/AABB2.h>

namespace fb
{

    template <class T>
    auto Line2<T>::intersectWith( const Line2<T> &l, Vector2<T> &out ) const -> bool
    {
#if 0
		bool found=false;

		f32 a1,a2,b1,b2;

		// calculate slopes, deal with infinity
		if (End.X()-Start.X() == T(0))
			b1 = (f32)1e+10;
		else
			b1 = (End.Y()-Start.Y()) / (End.X()-Start.X());

		if (l.End.X()-l.Start.X() == T(0))
			b2 = (f32)1e+10;
		else
			b2 = (l.End.Y()-l.Start.Y()) / (l.End.X()-l.Start.X());

		// calculate position
		a1 = Start.Y()   - b1 *  Start.X();
		a2 = l.Start.Y() - b2 * l.Start.X();
		out.X() = - (a1-a2)/(b1-b2);
		out.Y() = a1 + b1*out.X();

		// did the lines cross?
		if ((Start.X()-out.X()) *(out.X()-End.X())	 >= -MathF::epsilon() &&
			(l.Start.X()-out.X())*(out.X()-l.End.X())>= -MathF::epsilon() &&
			(Start.Y()-out.Y())  *(out.Y()-End.Y())  >= -MathF::epsilon() &&
			(l.Start.Y()-out.Y())*(out.Y()-l.End.Y())>= -MathF::epsilon() )
		{
			found = true;
		}

		return found;

#else

        // Uses the method given at:
        // http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
        const T commonDenominator = ( l.m_end.Y() - l.m_start.Y() ) * ( m_end.X() - m_start.X() ) -
                                    ( l.m_end.X() - l.m_start.X() ) * ( m_end.Y() - m_start.Y() );

        const T numeratorA = ( l.m_end.X() - l.m_start.X() ) * ( m_start.Y() - l.m_start.Y() ) -
                             ( l.m_end.Y() - l.m_start.Y() ) * ( m_start.X() - l.m_start.X() );

        const T numeratorB = ( m_end.X() - m_start.X() ) * ( m_start.Y() - l.m_start.Y() ) -
                             ( m_end.Y() - m_start.Y() ) * ( m_start.X() - l.m_start.X() );

        if( Math<T>::equals( commonDenominator, T( 0.0 ) ) )
        {
            // The lines are either coincident or parallel
            if( Math<T>::equals( numeratorA, T( 0.0 ) ) && Math<T>::equals( numeratorB, T( 0.0 ) ) )
            {
                // Try and find a common Endpoint
                if( l.m_start == m_start || l.m_end == m_start )
                {
                    out = m_start;
                }
                else if( l.m_end == m_end || l.m_start == m_end )
                {
                    out = m_end;
                }
                else
                {
                    // one line is contained in the other, so for lack of a better
                    // answer, pick the average of both lines
                    out = ( ( m_start + m_end + l.m_start + l.m_end ) * T( 0.25 ) );

                    // do a rectagle test
                    AABB2<T> rectA( m_start, m_end );
                    AABB2<T> rectB( l.m_start, l.m_end );

                    rectA.repair();
                    rectB.repair();

                    if( !rectA.intersects( rectB ) )
                    {
                        return false;
                    }
                }

                return true;  // coincident
            }

            return false;  // parallel
        }

        // Get the point of intersection on this line, checking that
        // it is within the line segment.
        const T uA = numeratorA / commonDenominator;
        if( uA < -Math<T>::epsilon() || uA > ( T( 1.0 ) + Math<T>::epsilon() ) )
        {
            return false;  // Outside the line segment
        }

        const T uB = numeratorB / commonDenominator;
        if( uB < -Math<T>::epsilon() || uB > ( T( 1.0 ) + Math<T>::epsilon() ) )
        {
            return false;  // Outside the line segment
        }

        // Calculate the intersection point.
        out.X() = m_start.X() + uA * ( m_end.X() - m_start.X() );
        out.Y() = m_start.Y() + uA * ( m_end.Y() - m_start.Y() );

        return true;
#endif
    }

    template <class T>
    auto Line2<T>::intersectWith( const Vector2<T> &center, T radius ) const -> bool
    {
        Vector2<T> closestPoint = getClosestPoint( center );
        T distance = ( center - closestPoint ).length();
        if( Math<T>::Abs( distance ) <= radius )
        {
            return true;
        }

        return false;
    }

    // explicit instantiation
    template class Line2<f32>;
    template class Line2<f64>;
    template class Line2<s32>;
}  // End namespace fb
