#ifndef __LinearSpline2_h__
#define __LinearSpline2_h__

#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/UtilityTypes.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Matrix4.h>

namespace fb
{
    //------------------------------------------------
    template <class T>
    class LinearSpline2 : public CSharedObject<ISharedObject>
    {
    public:
        LinearSpline2()
        {
            // Set up matrix
            // Hermite polynomial
            m_coeffs[0][0] = 2;
            m_coeffs[0][1] = -2;
            m_coeffs[0][2] = 1;
            m_coeffs[0][3] = 1;
            m_coeffs[1][0] = -3;
            m_coeffs[1][1] = 3;
            m_coeffs[1][2] = -2;
            m_coeffs[1][3] = -1;
            m_coeffs[2][0] = 0;
            m_coeffs[2][1] = 0;
            m_coeffs[2][2] = 1;
            m_coeffs[2][3] = 0;
            m_coeffs[3][0] = 1;
            m_coeffs[3][1] = 0;
            m_coeffs[3][2] = 0;
            m_coeffs[3][3] = 0;
        }

        ~LinearSpline2() override = default;

        void setPoints( const Array<Pair<T, Vector2<T>>> &points )
        {
            for( auto &p : points )
            {
                m_times.push_back( p.first );

                auto v = p.second;
                m_points.push_back( Vector3<T>( v.x, v.y, 0.0 ) );
            }
        }

        void setPoints( const Array<T> &keys, const Array<Vector2<T>> &points )
        {
            m_times = keys;

            for( auto &p : points )
            {
                m_points.push_back( Vector3<T>( p.x, p.y, 0.0 ) );
            }
        }

        void setPoints( const Array<Vector2<T>> &points )
        {
            for( auto &p : points )
            {
                m_points.push_back( Vector3<T>( p.x, p.y, 0.0 ) );
            }
        }

        Vector2<T> interpolate( T t )
        {
            // Work out which segment this is in
            auto fSeg = t * ( m_points.size() - 1 );
            auto segIdx = (u32)fSeg;

            // Apportion t
            auto p = fSeg - segIdx;

            return interpolate( segIdx, p );
        }

        Vector2<T> interpolate( u32 fromIndex, T t ) const
        {
            // Bounds check
            FB_ASSERT( fromIndex < m_points.size() && "fromIndex out of bounds" );

            if( ( fromIndex + 1 ) == m_points.size() )
            {
                // Duff request, cannot blend to nothing
                // Just return source
                auto p = m_points[fromIndex];
                return Vector2<T>( p.x, p.y );
            }

            // Fast special cases
            if( t == T( 0.0 ) )
            {
                auto p = m_points[fromIndex];
                return Vector2<T>( p.x, p.y );
            }
            else if( t == T( 1.0 ) )
            {
                auto p = m_points[fromIndex + 1];
                return Vector2<T>( p.x, p.y );
            }

            // Real interpolation
            // Form a vector of powers of t
            T t2, t3;
            t2 = t * t;
            t3 = t2 * t;

            Vector4<T> powers( t3, t2, t, 1 );

            // Algorithm is ret = powers * mCoeffs * Matrix4(point1, point2, tangent1, tangent2)
            const auto point1 = m_points[fromIndex];
            const auto point2 = m_points[fromIndex + 1];
            const auto tan1 = m_tangents[fromIndex];
            const auto tan2 = m_tangents[fromIndex + 1];
            auto pt = Matrix4<T>::identity();

            pt[0][0] = point1.X();
            pt[0][1] = point1.Y();
            pt[0][2] = point1.Z();
            pt[0][3] = T( 1.0 );
            pt[1][0] = point2.X();
            pt[1][1] = point2.Y();
            pt[1][2] = point2.Z();
            pt[1][3] = T( 1.0 );
            pt[2][0] = tan1.X();
            pt[2][1] = tan1.Y();
            pt[2][2] = tan1.Z();
            pt[2][3] = T( 1.0 );
            pt[3][0] = tan2.X();
            pt[3][1] = tan2.Y();
            pt[3][2] = tan2.Z();
            pt[3][3] = T( 1.0 );

            Vector4<T> ret = powers * m_coeffs * pt;
            return Vector2<T>( ret.X(), ret.Y() );
        }

        u32 getNumPoints() const
        {
            return (u32)m_points.size();
        }

        /** Clears all the points in the spline. */
        void clear()
        {
            m_points.clear();
        }

        /** Tells the spline whether it should automatically calculate tangents on demand
            as points are added.
        @remarks
            The spline calculates tangents at each point automatically based on the input points.
            Normally it does this every time a point changes. However, if you have a lot of points
            to add in one go, you probably don't want to incur this overhead and would prefer to
            defer the calculation until you are finished setting all the points. You can do this
            by calling this method with a parameter of 'false'. Just remember to manually call
            the recalcTangents method when you are done.
        @param autoCalc If true, tangents are calculated for you whenever a point changes. If false,
            you must call reclacTangents to recalculate them when it best suits.
        */
        void setAutoCalculate( bool autoCalc )
        {
            m_autoCalc = autoCalc;
        }

        /** Recalculates the tangents associated with this spline.
        @remarks
            If you tell the spline not to update on demand by calling setAutoCalculate(false)
            then you must call this after completing your updates to the spline points.
        */
        void recalcTangents()
        {
            // Catmull-Rom approach
            //
            // tangent[i] = 0.5 * (point[i+1] - point[i-1])
            //
            // Assume endpoint tangents are parallel with line with neighbour

            size_t i, numPoints;
            bool isClosed;

            numPoints = m_points.size();
            if( numPoints < 2 )
            {
                // Can't do anything yet
                return;
            }

            // Closed or open?
            if( m_points[0] == m_points[numPoints - 1] )
            {
                isClosed = true;
            }
            else
            {
                isClosed = false;
            }

            m_tangents.resize( numPoints );

            for( i = 0; i < numPoints; ++i )
            {
                if( i == 0 )
                {
                    // Special case start
                    if( isClosed )
                    {
                        // Use numPoints-2 since numPoints-1 is the last point and == [0]
                        m_tangents[i] = T( 0.5 ) * ( m_points[1] - m_points[numPoints - 2] );
                    }
                    else
                    {
                        m_tangents[i] = T( 0.5 ) * ( m_points[1] - m_points[0] );
                    }
                }
                else if( i == numPoints - 1 )
                {
                    // Special case end
                    if( isClosed )
                    {
                        // Use same tangent as already calculated for [0]
                        m_tangents[i] = m_tangents[0];
                    }
                    else
                    {
                        m_tangents[i] = T( 0.5 ) * ( m_points[i] - m_points[i - 1] );
                    }
                }
                else
                {
                    m_tangents[i] = T( 0.5 ) * ( m_points[i + 1] - m_points[i - 1] );
                }
            }
        }

    protected:
        bool m_autoCalc = true;

        Array<T> m_times;
        Array<Vector3<T>> m_points;
        Array<Vector3<T>> m_tangents;

        /// Matrix of coefficients
        Matrix4<T> m_coeffs = Matrix4<T>::identity();
    };
}  // end namespace fb

#include "LinearSpline2.inl"

#endif  // __LinearSpline2_h__
