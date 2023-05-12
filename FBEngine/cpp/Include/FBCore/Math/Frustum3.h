#ifndef Frustum3_h__
#define Frustum3_h__

#include <FBCore/Math/Plane3.h>
#include <FBCore/Math/Matrix4.h>

namespace fb
{
    class Frustum3Types
    {
    public:
        /** Worldspace clipping planes.
         */
        enum FrustumPlane
        {
            FRUSTUM_PLANE_NEAR = 0,
            FRUSTUM_PLANE_FAR = 1,
            FRUSTUM_PLANE_LEFT = 2,
            FRUSTUM_PLANE_RIGHT = 3,
            FRUSTUM_PLANE_TOP = 4,
            FRUSTUM_PLANE_BOTTOM = 5
        };
    };

    /**
     * A class that represents a frustum, which is a truncated pyramid with the apex at the camera's
     * position and the base equal to the near plane of the camera's view volume.
     *
     * @tparam T The type of the coordinates.
     */
    template <class T>
    class Frustum3
    {
    public:
        /**
         * Default constructor that initializes an empty frustum.
         */
        Frustum3() = default;

        /**
         * Default destructor.
         */
        ~Frustum3() = default;

        /**
         * Set the view-projection matrix of the frustum.
         *
         * @param viewProjection The view-projection matrix.
         */
        void setViewProjection( const Matrix4<T> &viewProjection );

        /**
         * Check if a box intersects with the frustum.
         *
         * @param centre The centre of the box.
         * @param extents The extents of the box.
         * @return True if the box intersects with the frustum, false otherwise.
         */
        bool intersects( const Vector3<T> &centre, const Vector3<T> &extents ) const;

    protected:
        /**
         * The planes that define the frustum.
         */
        Plane3<T> m_planes[6];
    };

    template <class T>
    void Frustum3<T>::setViewProjection( const Matrix4<T> &viewProjection )
    {
        m_planes[Frustum3Types::FRUSTUM_PLANE_LEFT].getNormal().X() =
            viewProjection[3][0] + viewProjection[0][0];
        m_planes[Frustum3Types::FRUSTUM_PLANE_LEFT].getNormal().Y() =
            viewProjection[3][1] + viewProjection[0][1];
        m_planes[Frustum3Types::FRUSTUM_PLANE_LEFT].getNormal().Z() =
            viewProjection[3][2] + viewProjection[0][2];
        m_planes[Frustum3Types::FRUSTUM_PLANE_LEFT].setDistance( viewProjection[3][3] +
                                                                 viewProjection[0][3] );

        m_planes[Frustum3Types::FRUSTUM_PLANE_RIGHT].getNormal().X() =
            viewProjection[3][0] - viewProjection[0][0];
        m_planes[Frustum3Types::FRUSTUM_PLANE_RIGHT].getNormal().Y() =
            viewProjection[3][1] - viewProjection[0][1];
        m_planes[Frustum3Types::FRUSTUM_PLANE_RIGHT].getNormal().Z() =
            viewProjection[3][2] - viewProjection[0][2];
        m_planes[Frustum3Types::FRUSTUM_PLANE_RIGHT].setDistance( viewProjection[3][3] -
                                                                  viewProjection[0][3] );

        m_planes[Frustum3Types::FRUSTUM_PLANE_TOP].getNormal().X() =
            viewProjection[3][0] - viewProjection[1][0];
        m_planes[Frustum3Types::FRUSTUM_PLANE_TOP].getNormal().Y() =
            viewProjection[3][1] - viewProjection[1][1];
        m_planes[Frustum3Types::FRUSTUM_PLANE_TOP].getNormal().Z() =
            viewProjection[3][2] - viewProjection[1][2];
        m_planes[Frustum3Types::FRUSTUM_PLANE_TOP].setDistance( viewProjection[3][3] -
                                                                viewProjection[1][3] );

        m_planes[Frustum3Types::FRUSTUM_PLANE_BOTTOM].getNormal().X() =
            viewProjection[3][0] + viewProjection[1][0];
        m_planes[Frustum3Types::FRUSTUM_PLANE_BOTTOM].getNormal().Y() =
            viewProjection[3][1] + viewProjection[1][1];
        m_planes[Frustum3Types::FRUSTUM_PLANE_BOTTOM].getNormal().Z() =
            viewProjection[3][2] + viewProjection[1][2];
        m_planes[Frustum3Types::FRUSTUM_PLANE_BOTTOM].setDistance( viewProjection[3][3] +
                                                                   viewProjection[1][3] );

        m_planes[Frustum3Types::FRUSTUM_PLANE_NEAR].getNormal().X() =
            viewProjection[3][0] + viewProjection[2][0];
        m_planes[Frustum3Types::FRUSTUM_PLANE_NEAR].getNormal().Y() =
            viewProjection[3][1] + viewProjection[2][1];
        m_planes[Frustum3Types::FRUSTUM_PLANE_NEAR].getNormal().Z() =
            viewProjection[3][2] + viewProjection[2][2];
        m_planes[Frustum3Types::FRUSTUM_PLANE_NEAR].setDistance( viewProjection[3][3] +
                                                                 viewProjection[2][3] );

        m_planes[Frustum3Types::FRUSTUM_PLANE_FAR].getNormal().X() =
            viewProjection[3][0] - viewProjection[2][0];
        m_planes[Frustum3Types::FRUSTUM_PLANE_FAR].getNormal().Y() =
            viewProjection[3][1] - viewProjection[2][1];
        m_planes[Frustum3Types::FRUSTUM_PLANE_FAR].getNormal().Z() =
            viewProjection[3][2] - viewProjection[2][2];
        m_planes[Frustum3Types::FRUSTUM_PLANE_FAR].setDistance( viewProjection[3][3] -
                                                                viewProjection[2][3] );

        // Renormalise any normals which were not unit length
        for( int i = 0; i < 6; i++ )
        {
            T length = m_planes[i].getNormal().normaliseLength();
            m_planes[i].setDistance( m_planes[i].getDistance() / length );
        }
    }

    template <class T>
    bool Frustum3<T>::intersects( const Vector3<T> &centre, const Vector3<T> &extents ) const
    {
        // For each plane, see if all points are on the negative side
        // If so, object is not visible
        for( s32 plane = 0; plane < 6; ++plane )
        {
            Plane3Types::Side side = m_planes[plane].getSide( centre, extents );
            if( side == Plane3Types::NEGATIVE_SIDE )
            {
                return false;
            }
        }

        return true;
    }

    using Frustum3F = Frustum3<f32>;
    using Frustum3D = Frustum3<f64>;

}  // end namespace fb

#endif  // Frustum3_h__
