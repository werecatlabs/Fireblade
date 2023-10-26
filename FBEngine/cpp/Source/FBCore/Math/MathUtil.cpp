#include <FBCore/FBCorePCH.h>
#include <FBCore/Math/MathUtil.h>
#include <boost/geometry.hpp>
//#include <boost/geometry/strategies/strategy_transform.hpp>

namespace fb
{
    template <class T>
    bool MathUtil<T>::equals( T f1, T f2 )
    {
        return std::fabs( f1 - f2 ) <= std::numeric_limits<T>::epsilon();
    }

    template <class T>
    bool MathUtil<T>::equals( T f1, T f2, T tolerance )
    {
        return std::fabs( f1 - f2 ) <= tolerance;
    }

    template <class T>
    bool MathUtil<T>::equals( const Vector2<T> &a, const Vector2<T> &b )
    {
        return MathUtil::equals( a.X(), b.X() ) && MathUtil::equals( a.Y(), b.Y() );
    }

    template <class T>
    bool MathUtil<T>::equals( const Vector3<T> &a, const Vector3<T> &b )
    {
        return MathUtil::equals( a.X(), b.X() ) && MathUtil::equals( a.Y(), b.Y() ) &&
               MathUtil::equals( a.Z(), b.Z() );
    }

    template <class T>
    bool MathUtil<T>::equals( const Quaternion<T> &a, const Quaternion<T> &b )
    {
        return MathUtil::equals( a.W(), b.W() ) && MathUtil::equals( a.X(), b.X() ) &&
               MathUtil::equals( a.Y(), b.Y() ) && MathUtil::equals( a.Z(), b.Z() );
    }

    template <class T>
    bool MathUtil<T>::equals( const Vector3<T> &a, const Vector3<T> &b, T tolerance )
    {
        return MathUtil::equals( a.X(), b.X(), tolerance ) &&
               MathUtil::equals( a.Y(), b.Y(), tolerance ) &&
               MathUtil::equals( a.Z(), b.Z(), tolerance );
    }

    template <class T>
    bool MathUtil<T>::equals( const Quaternion<T> &a, const Quaternion<T> &b, T tolerance )
    {
        return MathUtil::equals( a.W(), b.W(), tolerance ) &&
               MathUtil::equals( a.X(), b.X(), tolerance ) &&
               MathUtil::equals( a.Y(), b.Y(), tolerance ) &&
               MathUtil::equals( a.Z(), b.Z(), tolerance );
    }

    template <>
    bool MathUtil<s32>::isFinite( const Vector3<s32> &val )
    {
        return std::isfinite( static_cast<f32>( val.X() ) ) &&
               std::isfinite( static_cast<f32>( val.Y() ) ) &&
               std::isfinite( static_cast<f32>( val.Z() ) );
    }

    template <>
    bool MathUtil<s32>::isFinite( const Quaternion<s32> &val )
    {
        return std::isfinite( static_cast<f32>( val.W() ) ) &&
               std::isfinite( static_cast<f32>( val.X() ) ) &&
               std::isfinite( static_cast<f32>( val.Y() ) ) &&
               std::isfinite( static_cast<f32>( val.Z() ) );
    }

    template <class T>
    bool MathUtil<T>::isFinite( const Vector3<T> &val )
    {
        return std::isfinite( val.X() ) && std::isfinite( val.Y() ) && std::isfinite( val.Z() );
    }

    template <class T>
    bool MathUtil<T>::isFinite( const Quaternion<T> &val )
    {
        return std::isfinite( val.W() ) && std::isfinite( val.X() ) && std::isfinite( val.Y() ) &&
               std::isfinite( val.Z() );
    }

    template <class T>
    Quaternion<T> MathUtil<T>::getRotationTo( const Vector3<T> &src, const Vector3<T> &dest,
                                              const Vector3<T> &fallbackAxis /*= Vector3<T>::ZERO*/ )
    {
        // Based on Stan Melax's article in Game Programming Gems
        Quaternion<T> q;
        // Copy, since cannot modify local
        Vector3<T> v0 = src;
        Vector3<T> v1 = dest;
        v0.normalise();
        v1.normalise();

        T d = v0.dotProduct( v1 );
        // If dot == 1, vectors are the same
        if( d >= 1.0f )
        {
            return Quaternion<T>::identity();
        }
        if( d < T( 1e-6 - 1.0 ) )
        {
            if( fallbackAxis != Vector3<T>::ZERO )
            {
                // rotate 180 degrees about the fallback axis
                q.fromAngleAxis( Math<T>::pi(), fallbackAxis );
            }
            else
            {
                // Generate an axis
                Vector3<T> axis = Vector3<T>::UNIT_X.crossProduct( src );
                if( axis.isZeroLength() )  // pick another if collinear
                    axis = Vector3<T>::UNIT_Y.crossProduct( src );

                axis.normalise();
                q.fromAngleAxis( Math<T>::pi(), axis );
            }
        }
        else
        {
            T s = Math<T>::Sqrt( ( 1 + d ) * 2 );
            T invs = 1 / s;

            Vector3<T> c = v0.crossProduct( v1 );

            q.X() = c.X() * invs;
            q.Y() = c.Y() * invs;
            q.Z() = c.Z() * invs;
            q.W() = s * T( 0.5 );
            q.normalise();
        }
        return q;
    }

    template <class T>
    Quaternion<T> MathUtil<T>::getOrientationFromDirection( const Vector3<T> &vec,
                                                            const Vector3<T> &localDirectionVector,
                                                            bool bYawFixed,
                                                            const Vector3<T> &yawFixedAxis )
    {
        FB_ASSERT( vec.isValid() );
        FB_ASSERT( localDirectionVector.isValid() );
        FB_ASSERT( yawFixedAxis.isValid() );

        // Do nothing if given a zero vector
        if( vec == Vector3<T>::zero() )
        {
            return Quaternion<T>::identity();
        }

        // The direction we want the local direction point to
        auto targetDir = vec.normaliseCopy();

        // Calculate target orientation relative to world space
        Quaternion<T> targetOrientation;

        if( bYawFixed )
        {
            // Calculate the quaternion for rotate local Z to target direction
            auto xVec = yawFixedAxis.crossProduct( targetDir );
            xVec.normalise();

            auto yVec = targetDir.crossProduct( xVec );
            yVec.normalise();

            auto unitZToTarget = Quaternion<T>( xVec, yVec, targetDir );

            if( localDirectionVector == -Vector3<T>::UNIT_Z )
            {
                // Special case for avoid calculate 180 degree turn
                targetOrientation = Quaternion<T>( -unitZToTarget.Y(), -unitZToTarget.Z(),
                                                   unitZToTarget.W(), unitZToTarget.X() );
            }
            else
            {
                // Calculate the quaternion for rotate local direction to target direction
                auto localToUnitZ =
                    Quaternion<T>::getRotationTo( localDirectionVector, Vector3<T>::UNIT_Z );
                targetOrientation = unitZToTarget * localToUnitZ;
            }
        }
        else
        {
            auto currentOrient = Quaternion<T>::identity();

            // Get current local direction relative to world space
            auto currentDir = currentOrient * localDirectionVector;

            if( ( currentDir + targetDir ).squaredLength() < 0.00005f )
            {
                // Oops, a 180 degree turn (infinite possible rotation axes)
                // Default to yaw i.e. use current UP
                targetOrientation = Quaternion<T>( -currentOrient.Y(), -currentOrient.Z(),
                                                   currentOrient.W(), currentOrient.X() );
            }
            else
            {
                // Derive shortest arc to new direction
                auto rotQuat = Quaternion<T>::getRotationTo( currentDir, targetDir );
                targetOrientation = rotQuat * currentOrient;
            }
        }

        targetOrientation.normalise();

        FB_ASSERT( targetOrientation.isSane() );
        return targetOrientation;
    }

    template <class T>
    Vector3<T> MathUtil<T>::reflect( const Vector3<T> &vec, const Vector3<T> &normal )
    {
        return vec - T( 2.0 ) * normal * normal.dotProduct( vec );
    }

    template <class T>
    Vector3<T> MathUtil<T>::toSpherical( const Vector3<T> &cartesian )
    {
        using CartesianType =
            boost::geometry::model::point<long double, 3, boost::geometry::cs::cartesian>;
        CartesianType cartesianType( cartesian.X(), cartesian.Y(), cartesian.Z() );

        using SphericalType =
            boost::geometry::model::point<long double, 3,
                                          boost::geometry::cs::spherical<boost::geometry::radian>>;
        SphericalType spherical;

        boost::geometry::strategy::transform::from_cartesian_3_to_spherical_polar_3<CartesianType,
                                                                                    SphericalType>
            strategy;
        transform( cartesianType, spherical, strategy );

        return Vector3<T>( static_cast<T>( spherical.get<2>() ), static_cast<T>( spherical.get<0>() ),
                           static_cast<T>( spherical.get<1>() ) );
    }

    template <class T>
    Vector3<T> MathUtil<T>::toCartesian( const Vector3<T> &spherical )
    {
        using SphericalType =
            boost::geometry::model::point<long double, 3,
                                          boost::geometry::cs::spherical<boost::geometry::radian>>;

        SphericalType sphericalType( spherical.Y(), spherical.Z(), spherical.Z() );

        using CartesianType =
            boost::geometry::model::point<long double, 3, boost::geometry::cs::cartesian>;
        CartesianType cartesian( 0, 0, 0 );

        boost::geometry::strategy::transform::from_spherical_polar_3_to_cartesian_3<SphericalType,
                                                                                    CartesianType>
            strat;
        transform( sphericalType, cartesian, strat );

        return Vector3<T>( static_cast<T>( cartesian.get<0>() ), static_cast<T>( cartesian.get<1>() ),
                           static_cast<T>( cartesian.get<2>() ) );
    }

    template <class T>
    Vector3<T> MathUtil<T>::toPosition( const Vector3<T> &rotation, Vector3<T> &up, Vector3<T> &pitchVec,
                                        Vector3<T> &yawVec )
    {
        Quaternion<T> yaw;
        Quaternion<T> pitch;

        yaw.fromAngleAxis( rotation.Y(), Vector3<T>::UNIT_Y );
        pitch.fromAngleAxis( rotation.Z(), Vector3<T>::UNIT_X );

        yawVec = ( yaw * -Vector3<T>::UNIT_Z );
        pitchVec = ( pitch * -Vector3<T>::UNIT_Z );

        up = ( yaw * pitch ) * Vector3<T>::UNIT_Y;
        return ( ( yaw * pitch ) * -Vector3<T>::UNIT_Z ) * rotation.X();
    }

    template <class T>
    Vector3<T> MathUtil<T>::toRotation( const Vector3<T> &cartesian, const Vector3<T> &pitchVec,
                                        const Vector3<T> &yawVec )
    {
        Vector3<T> spherical;
        spherical.X() = cartesian.length();
        spherical.Y() = Math<T>::wrapRadians( Math<T>::ATan2( -yawVec.X(), -yawVec.Z() ) );
        spherical.Z() = Math<T>::wrapRadians( Math<T>::ATan2( pitchVec.Y(), -pitchVec.Z() ) );
        return spherical;
    }

    template <class T>
    T MathUtil<T>::average( const Array<T> &v )
    {
        auto sum = T( 0.0 );
        for( auto &value : v )
        {
            sum += value;
        }

        return sum / static_cast<T>( v.size() );
    }

    template <class T>
    T MathUtil<T>::average( const std::deque<T> &v )
    {
        auto sum = T( 0.0 );
        for( auto &value : v )
        {
            sum += value;
        }

        return sum / static_cast<T>( v.size() );
    }

    template <class T>
    T MathUtil<T>::latlong_distance( const Vector2<T> &lat_long_1, const Vector2<T> &lat_long_2 )
    {
        auto lat1 = lat_long_1[0];
        auto lon1 = lat_long_1[1];
        auto lat2 = lat_long_2[0];
        auto lon2 = lat_long_2[1];

        auto dlon = lon2 - lon1;
        auto dlat = lat2 - lat1;

        auto a =
            Math<T>::Sin( dlat / T( 2.0 ) ) * T( 2.0 ) +
            Math<T>::Cos( lat1 ) * Math<T>::Cos( lat2 ) * Math<T>::Sin( dlon / T( 2.0 ) ) * T( 2.0 );
        auto c = T( 2.0 ) * Math<T>::ATan2( Math<T>::Sqrt( a ), Math<T>::Sqrt( T( 1.0 ) - a ) );

        // approximate radius of earth in km
        const auto R = T( 6373.0 );
        auto distance = R * c;
        return distance * T( 1000.0 );
    }

    template <class T>
    void MathUtil<T>::swap( Vector3<T> &a, Vector3<T> &b )
    {
        auto temp = a;
        a = b;
        b = temp;
    }

    template <class T>
    Vector3<T> MathUtil<T>::round( const Vector3<T> &v, s32 decimals )
    {
        auto val = Math<T>::Pow( T( 10 ), static_cast<T>( decimals ) );
        return Vector3<T>( Math<T>::round( v.X() * val ) / val, Math<T>::round( v.Y() * val ) / val,
                           Math<T>::round( v.Z() * val ) / val );
    }

    template <class T>
    void MathUtil<T>::repair( Vector3<T> &Min, Vector3<T> &Max )
    {
        T t;

        if( Min.X() > Max.X() )
        {
            t = Min.X();
            Min.X() = Max.X();
            Max.X() = t;
        }

        if( Min.Y() > Max.Y() )
        {
            t = Min.Y();
            Min.Y() = Max.Y();
            Max.Y() = t;
        }

        if( Min.Z() > Max.Z() )
        {
            t = Min.Z();
            Min.Z() = Max.Z();
            Max.Z() = t;
        }
    }

    template <class T>
    template <class IT_TYPE>
    void MathUtil<T>::lowest( IT_TYPE begin, IT_TYPE end, T &val )
    {
        val = T( 1e10 );
        IT_TYPE i = begin;
        for( ; i != end; ++i )
        {
            val = std::min( val, *i );
        }
    }

    template <class T>
    template <class IT_TYPE>
    void MathUtil<T>::highest( IT_TYPE begin, IT_TYPE end, T &val )
    {
        val = T( -1e10 );
        IT_TYPE i = begin;
        for( ; i != end; ++i )
        {
            val = std::max( val, *i );
        }
    }

    template <class T>
    Transform3<T> MathUtil<T>::transformFromPlaneEquation( const Plane3<T> plane )
    {
        return Transform3<T>();
    }

    template <class T>
    std::pair<bool, T> MathUtil<T>::intersects( const Ray3<T> &ray, const AABB3<T> &box )
    {
        if( box.isNull() )
        {
            return std::pair<bool, T>( false, T( 0.0 ) );
        }

        if( box.isInfinite() )
        {
            return std::pair<bool, T>( true, T( 0.0 ) );
        }

        auto lowt = T( 0.0 );
        auto t = T( 0.0 );
        auto hit = false;

        Vector3<T> hitpoint;
        auto min = box.getMinimum();
        auto max = box.getMaximum();
        auto rayorig = ray.getOrigin();
        auto raydir = ray.getDirection();

        // Check origin inside first
        if( rayorig > min && rayorig < max )
        {
            return std::pair<bool, T>( true, T( 0.0 ) );
        }

        // Check each face in turn, only check closest 3
        // Min x
        if( rayorig.X() <= min.X() && raydir.X() > T( 0.0 ) )
        {
            t = ( min.X() - rayorig.X() ) / raydir.X();

            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if( hitpoint.Y() >= min.Y() && hitpoint.Y() <= max.Y() && hitpoint.Z() >= min.Z() &&
                hitpoint.Z() <= max.Z() && ( !hit || t < lowt ) )
            {
                hit = true;
                lowt = t;
            }
        }
        // Max x
        if( rayorig.X() >= max.X() && raydir.X() < T( 0.0 ) )
        {
            t = ( max.X() - rayorig.X() ) / raydir.X();

            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if( hitpoint.Y() >= min.Y() && hitpoint.Y() <= max.Y() && hitpoint.Z() >= min.Z() &&
                hitpoint.Z() <= max.Z() && ( !hit || t < lowt ) )
            {
                hit = true;
                lowt = t;
            }
        }

        // Min y
        if( rayorig.Y() <= min.Y() && raydir.Y() > T( 0.0 ) )
        {
            t = ( min.Y() - rayorig.Y() ) / raydir.Y();

            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if( hitpoint.X() >= min.X() && hitpoint.X() <= max.X() && hitpoint.Z() >= min.Z() &&
                hitpoint.Z() <= max.Z() && ( !hit || t < lowt ) )
            {
                hit = true;
                lowt = t;
            }
        }

        // Max y
        if( rayorig.Y() >= max.Y() && raydir.Y() < T( 0.0 ) )
        {
            t = ( max.Y() - rayorig.Y() ) / raydir.Y();

            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if( hitpoint.X() >= min.X() && hitpoint.X() <= max.X() && hitpoint.Z() >= min.Z() &&
                hitpoint.Z() <= max.Z() && ( !hit || t < lowt ) )
            {
                hit = true;
                lowt = t;
            }
        }

        // Min z
        if( rayorig.Z() <= min.Z() && raydir.Z() > T( 0.0 ) )
        {
            t = ( min.Z() - rayorig.Z() ) / raydir.Z();

            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if( hitpoint.X() >= min.X() && hitpoint.X() <= max.X() && hitpoint.Y() >= min.Y() &&
                hitpoint.Y() <= max.Y() && ( !hit || t < lowt ) )
            {
                hit = true;
                lowt = t;
            }
        }

        // Max z
        if( rayorig.Z() >= max.Z() && raydir.Z() < T( 0.0 ) )
        {
            t = ( max.Z() - rayorig.Z() ) / raydir.Z();

            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if( hitpoint.X() >= min.X() && hitpoint.X() <= max.X() && hitpoint.Y() >= min.Y() &&
                hitpoint.Y() <= max.Y() && ( !hit || t < lowt ) )
            {
                hit = true;
                lowt = t;
            }
        }

        return std::pair<bool, T>( hit, lowt );
    }

    template <class T>
    std::pair<bool, T> MathUtil<T>::intersects( const Ray3<T> &ray, const Sphere3<T> &sphere,
                                                bool discardInside /*= true*/ )
    {
        // Adjust ray origin relative to sphere center
        auto rayorig = ray.getOrigin() - sphere.getCenter();
        auto rayDirection = ray.getDirection();
        auto radius = sphere.getRadius();

        // Check origin inside first
        if( rayorig.squaredLength() <= radius * radius && discardInside )
        {
            return std::pair<bool, T>( true, T( 0.0 ) );
        }

        // Mmm, quadratics
        // Build coeffs which can be used with std quadratic solver
        // ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
        auto a = rayDirection.dotProduct( rayDirection );
        auto b = T( 2.0 ) * rayorig.dotProduct( rayDirection );
        auto c = rayorig.dotProduct( rayorig ) - radius * radius;

        // Calc determinant
        auto d = ( b * b ) - ( T( 4.0 ) * a * c );
        if( d < 0 )
        {
            // No intersection
            return std::pair<bool, T>( false, T( 0.0 ) );
        }
        // BTW, if d=0 there is one intersection, if d > 0 there are 2
        // But we only want the closest one, so that's ok, just use the
        // '-' version of the solver
        auto t = ( -b - Math<T>::Sqrt( d ) ) / ( T( 2.0 ) * a );
        if( t < T( 0.0 ) )
        {
            t = ( -b + Math<T>::Sqrt( d ) ) / ( T( 2.0 ) * a );
        }

        return std::pair<bool, T>( true, t );
    }

    template <class T>
    std::pair<bool, T> MathUtil<T>::intersects( const Ray3<T> &ray, const Cylinder3<T> &cylinder )
    {
        T t[2];
        auto cylinderAxis = cylinder.getAxis();
        auto cylinderDirection = cylinderAxis.getDirection();
        auto cylinderHeight = cylinder.getHeight();
        auto cylinderRadius = cylinder.getRadius();

        Vector3<T> U, V, W = cylinderDirection;
        Vector3<T>::generateComplementBasis( U, V, W );
        auto halfHeight = static_cast<T>( 0.5 ) * cylinderHeight;
        auto rSqr = cylinderRadius * cylinderRadius;

        // convert incoming line origin to cylinder coordinates
        auto origin = ray.getOrigin();
        auto dir = ray.getDirection();

        auto diff = origin - cylinderAxis.getStart();
        auto P = Vector3<T>( U.dotProduct( diff ), V.dotProduct( diff ), W.dotProduct( diff ) );

        // Get the z-value, in cylinder coordinates, of the incoming line's
        // unit-length direction.
        auto dz = W.dotProduct( dir );

        if( Math<T>::Abs( dz ) >= T( 1.0 ) - Math<T>::epsilon() )
        {
            // The line is parallel to the cylinder axis.  Determine if the line
            // intersects the cylinder end disks.
            auto radialSqrDist = rSqr - P.X() * P.X() - P.Y() * P.Y();
            if( radialSqrDist < static_cast<T>( 0 ) )
            {
                // Line outside the cylinder, no intersection.
                return std::pair<bool, T>( false, T( 0.0 ) );
            }

            // Line intersects the cylinder end disks.
            if( dz > static_cast<T>( 0 ) )
            {
                t[0] = -P.Z() - halfHeight;
                t[1] = -P.Z() + halfHeight;
            }
            else
            {
                t[0] = P.Z() - halfHeight;
                t[1] = P.Z() + halfHeight;
            }

            return std::pair<bool, T>( true, t[0] );
        }

        // convert incoming line unit-length direction to cylinder coordinates
        Vector3<T> D( U.dotProduct( dir ), V.dotProduct( dir ), dz );

        T a0, a1, a2, discr, root, inv, tValue;

        if( Math<T>::Abs( D.Z() ) <= Math<T>::epsilon() )
        {
            // The line is perpendicular to the cylinder axis.
            if( Math<T>::Abs( P.Z() ) > halfHeight )
            {
                // Line is outside the planes of the cylinder end disks.
                return std::pair<bool, T>( false, T( 0.0 ) );
            }

            // Test intersection of line P+t*D with infinite cylinder
            // x^2+y^2 = r^2.  This reduces to computing the roots of a
            // quadratic equation.  If P = (px,py,pz) and D = (dx,dy,dz),
            // then the quadratic equation is
            //   (dx^2+dy^2)*t^2 + 2*(px*dx+py*dy)*t + (px^2+py^2-r^2) = 0
            a0 = P.X() * P.X() + P.Y() * P.Y() - rSqr;
            a1 = P.X() * D.X() + P.Y() * D.Y();
            a2 = D.X() * D.X() + D.Y() * D.Y();
            discr = a1 * a1 - a0 * a2;
            if( discr < static_cast<T>( 0 ) )
            {
                // Line does not intersect cylinder.
                return std::pair<bool, T>( false, T( 0.0 ) );
            }
            if( discr > Math<T>::epsilon() )
            {
                // Line intersects cylinder in two places.
                root = Math<T>::Sqrt( discr );
                inv = static_cast<T>( 1 ) / a2;
                t[0] = ( -a1 - root ) * inv;
                t[1] = ( -a1 + root ) * inv;
                return std::pair<bool, T>( true, t[0] );
            }
            // Line is tangent to the cylinder.
            t[0] = -a1 / a2;
            return std::pair<bool, T>( true, t[0] );
        }

        // Test plane intersections first.
        int quantity = 0;
        inv = static_cast<T>( 1.0 ) / D.Z();

        auto t0 = ( -halfHeight - P.Z() ) * inv;
        auto xTmp = P.X() + t0 * D.X();
        auto yTmp = P.Y() + t0 * D.Y();
        if( xTmp * xTmp + yTmp * yTmp <= rSqr )
        {
            // Planar intersection inside the top cylinder end disk.
            t[quantity++] = t0;
        }

        auto t1 = ( +halfHeight - P.Z() ) * inv;
        xTmp = P.X() + t1 * D.X();
        yTmp = P.Y() + t1 * D.Y();
        if( xTmp * xTmp + yTmp * yTmp <= rSqr )
        {
            // Planar intersection inside the bottom cylinder end disk.
            t[quantity++] = t1;
        }

        if( quantity == 2 )
        {
            // Line intersects both top and bottom cylinder end disks.
            if( t[0] > t[1] )
            {
                auto save = t[0];
                t[0] = t[1];
                t[1] = save;
            }

            return std::pair<bool, T>( true, t[0] );
        }

        // If quantity == 1, then the line must intersect cylinder wall in a
        // single point somewhere between the end disks.  This case is detected
        // in the following code that tests for intersection between line and
        // cylinder wall.
        a0 = P.X() * P.X() + P.Y() * P.Y() - rSqr;
        a1 = P.X() * D.X() + P.Y() * D.Y();
        a2 = D.X() * D.X() + D.Y() * D.Y();
        discr = a1 * a1 - a0 * a2;
        if( discr < static_cast<T>( 0 ) )
        {
            // Line does not intersect cylinder wall.
            FB_ASSERT( quantity == 0 );
            return std::pair<bool, T>( false, T( 0.0 ) );
        }
        if( discr > Math<T>::epsilon() )
        {
            root = Math<T>::Sqrt( discr );
            inv = static_cast<T>( 1 ) / a2;
            tValue = ( -a1 - root ) * inv;
            if( t0 <= t1 )
            {
                if( t0 <= tValue && tValue <= t1 )
                {
                    t[quantity++] = tValue;
                }
            }
            else
            {
                if( t1 <= tValue && tValue <= t0 )
                {
                    t[quantity++] = tValue;
                }
            }

            if( quantity == 2 )
            {
                // Line intersects one of the cylinder end disks and once on the
                // cylinder wall.
                if( t[0] > t[1] )
                {
                    auto save = t[0];
                    t[0] = t[1];
                    t[1] = save;
                }

                return std::pair<bool, T>( true, t[0] );
            }

            tValue = ( -a1 + root ) * inv;
            if( t0 <= t1 )
            {
                if( t0 <= tValue && tValue <= t1 )
                {
                    t[quantity++] = tValue;
                }
            }
            else
            {
                if( t1 <= tValue && tValue <= t0 )
                {
                    t[quantity++] = tValue;
                }
            }
        }
        else
        {
            tValue = -a1 / a2;
            if( t0 <= t1 )
            {
                if( t0 <= tValue && tValue <= t1 )
                {
                    t[quantity++] = tValue;
                }
            }
            else
            {
                if( t1 <= tValue && tValue <= t0 )
                {
                    t[quantity++] = tValue;
                }
            }
        }

        if( quantity == 2 )
        {
            if( t[0] > t[1] )
            {
                auto save = t[0];
                t[0] = t[1];
                t[1] = save;
            }
        }

        return std::pair<bool, T>( quantity != 0, t[0] );
    }

    // explicit instantiation
    template class MathUtil<s32>;

    template class MathUtil<f32>;

    template class MathUtil<f64>;
}  // end namespace fb
