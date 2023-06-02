#include <FBCore/FBCorePCH.h>
#include "FBCore/Math/RotationalSpline3.h"

namespace fb
{

    template <class T>
    RotationalSpline3<T>::RotationalSpline3() : mAutoCalc( true )
    {
    }

    template <class T>
    RotationalSpline3<T>::~RotationalSpline3()
    {
    }

    template <class T>
    void RotationalSpline3<T>::addPoint( const Quaternion<T> &p )
    {
        mPoints.push_back( p );
        if( mAutoCalc )
        {
            recalcTangents();
        }
    }

    template <class T>
    Quaternion<T> RotationalSpline3<T>::interpolate( f32 t, bool useShortestPath )
    {
        // Work out which segment this is in
        f32 fSeg = t * ( mPoints.size() - 1 );
        unsigned int segIdx = static_cast<unsigned>( fSeg );
        // Apportion t
        t = fSeg - segIdx;

        return interpolate( segIdx, t, useShortestPath );
    }

    template <class T>
    Quaternion<T> RotationalSpline3<T>::interpolate( u32 fromIndex, f32 t, bool useShortestPath )
    {
        if( ( fromIndex + 1 ) == mPoints.size() )
        {
            // Duff request, cannot blend to nothing
            // Just return source
            return mPoints[fromIndex];
        }

        // Fast special cases
        if( t == 0.0f )
        {
            return mPoints[fromIndex];
        }
        if( t == 1.0f )
        {
            return mPoints[fromIndex + 1];
        }

        // f32 interpolation
        // Use squad using tangents we've already set up
        Quaternion<T> &p = mPoints[fromIndex];
        Quaternion<T> &q = mPoints[fromIndex + 1];
        Quaternion<T> &a = mTangents[fromIndex];
        Quaternion<T> &b = mTangents[fromIndex + 1];

        // NB interpolate to nearest rotation
        return Quaternion<T>::squad( t, p, a, b, q, useShortestPath );
    }

    template <class T>
    void RotationalSpline3<T>::recalcTangents( void )
    {
        // ShoeMake (1987) approach
        // Just like Catmull-Rom really, just more gnarly
        // And no, I don't understand how to derive this!
        //
        // let p = point[i], pInv = p.Inverse
        // tangent[i] = p * exp( -0.25 * ( log(pInv * point[i+1]) + log(pInv * point[i-1]) ) )
        //
        // Assume endpoint tangents are parallel with line with neighbour

        unsigned int i, numPoints;
        bool isClosed;

        numPoints = static_cast<unsigned>( mPoints.size() );

        if( numPoints < 2 )
        {
            // Can't do anything yet
            return;
        }

        mTangents.resize( numPoints );

        if( mPoints[0] == mPoints[numPoints - 1] )
        {
            isClosed = true;
        }
        else
        {
            isClosed = false;
        }

        Quaternion<T> invp, part1, part2, preExp;
        for( i = 0; i < numPoints; ++i )
        {
            Quaternion<T> &p = mPoints[i];
            invp = p.inverse();

            if( i == 0 )
            {
                // special case start
                part1 = ( invp * mPoints[i + 1] ).log();
                if( isClosed )
                {
                    // Use numPoints-2 since numPoints-1 == end == start == this one
                    part2 = ( invp * mPoints[numPoints - 2] ).log();
                }
                else
                {
                    part2 = ( invp * p ).log();
                }
            }
            else if( i == numPoints - 1 )
            {
                // special case end
                if( isClosed )
                {
                    // Wrap to [1] (not [0], this is the same as end == this one)
                    part1 = ( invp * mPoints[1] ).log();
                }
                else
                {
                    part1 = ( invp * p ).log();
                }
                part2 = ( invp * mPoints[i - 1] ).log();
            }
            else
            {
                part1 = ( invp * mPoints[i + 1] ).log();
                part2 = ( invp * mPoints[i - 1] ).log();
            }

            preExp = T( -0.25 ) * ( part1 + part2 );
            mTangents[i] = p * preExp.exp();
        }
    }

    template <class T>
    const Quaternion<T> &RotationalSpline3<T>::getPoint( unsigned short index ) const
    {
        FB_ASSERT( index < mPoints.size() && "Point index is out of bounds!!" );

        return mPoints[index];
    }

    template <class T>
    unsigned short RotationalSpline3<T>::getNumPoints( void ) const
    {
        return static_cast<unsigned short>( mPoints.size() );
    }

    template <class T>
    void RotationalSpline3<T>::clear( void )
    {
        mPoints.clear();
        mTangents.clear();
    }

    template <class T>
    void RotationalSpline3<T>::updatePoint( unsigned short index, const Quaternion<T> &value )
    {
        FB_ASSERT( index < mPoints.size() && "Point index is out of bounds!!" );

        mPoints[index] = value;
        if( mAutoCalc )
        {
            recalcTangents();
        }
    }

    template <class T>
    void RotationalSpline3<T>::setAutoCalculate( bool autoCalc )
    {
        mAutoCalc = autoCalc;
    }

    template class RotationalSpline3<f32>;
    template class RotationalSpline3<f64>;
}  // namespace fb
