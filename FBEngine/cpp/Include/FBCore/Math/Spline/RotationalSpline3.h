#ifndef _FB_RotationalSpline3_H
#define _FB_RotationalSpline3_H

#include "FBCore/Math/Quaternion.h"
#include <FBCore/Memory/CSharedObject.h>
#include <vector>

namespace fb
{
    /** This class interpolates orientations (rotations) along a spline using
    derivatives of quaternions.
    */
    template <class T>
    class RotationalSpline3 : public CSharedObject<ISharedObject>
    {
    public:
        RotationalSpline3();
        ~RotationalSpline3() override;

        /** Adds a control point to the end of the spline. */
        void addPoint( const Quaternion<T> &p );

        /** Gets the detail of one of the control points of the spline. */
        const Quaternion<T> &getPoint( unsigned short index ) const;

        /** Gets the number of control points in the spline. */
        unsigned short getNumPoints( void ) const;

        /** Clears all the points in the spline. */
        void clear( void );

        /** Updates a single point in the spline.
        @remarks
        This point must already exist in the spline.
        */
        void updatePoint( unsigned short index, const Quaternion<T> &value );

        /** Returns an interpolated point based on a parametric value over the whole series.
        @remarks
        Given a t value between 0 and 1 representing the parametric distance along the
        whole length of the spline, this method returns an interpolated point.
        @param t Parametric value.
        @param useShortestPath Defines if rotation should take the shortest possible path
        */
        Quaternion<T> interpolate( f32 t, bool useShortestPath = true );

        /** Interpolates a single segment of the spline given a parametric value.
        @param fromIndex The point index to treat as t=0. fromIndex + 1 is deemed to be t=1
        @param t Parametric value
        @param useShortestPath Defines if rotation should take the shortest possible path
        */
        Quaternion<T> interpolate( unsigned int fromIndex, f32 t, bool useShortestPath = true );

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
        void setAutoCalculate( bool autoCalc );

        /** Recalculates the tangents associated with this spline.
        @remarks
        If you tell the spline not to update on demand by calling setAutoCalculate(false)
        then you must call this after completing your updates to the spline points.
        */
        void recalcTangents( void );

        bool getUseShortestRoute() const
        {
            return m_useShortestRoute;
        }

        void setUseShortestRoute( bool val )
        {
            m_useShortestRoute = val;
        }

    protected:
        ///
        bool mAutoCalc;

        ///
        Array<Quaternion<T>> mPoints;

        ///
        Array<Quaternion<T>> mTangents;

        bool m_useShortestRoute;
    };

    using RotationalSpline3F = RotationalSpline3<f32>;
    using RotationalSpline3D = RotationalSpline3<f64>;
}  // namespace fb

#endif
