#ifndef FBPhysxPlaneShape_h__
#define FBPhysxPlaneShape_h__

#include <FBCore/Interface/Physics/IPlaneShape3.h>
#include <FBPhysx/FBPhysxShape.h>

namespace fb
{
    namespace physics
    {

        /** Implementation of a plane shape. */
        class PhysxPlaneShape : public PhysxShape<IPlaneShape3>
        {
        public:
            /** Constructor */
            PhysxPlaneShape() = default;

            /** Destructor */
            ~PhysxPlaneShape() override = default;

            /** @copydoc PhysxShape<IPlaneShape3>::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc PhysxShape<IPlaneShape3>::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc PhysxShape<IPlaneShape3>::getExtents */
            Vector3<real_Num> getExtents() const;

            /** @copydoc PhysxShape<IPlaneShape3>::setExtents */
            void setExtents( const Vector3<real_Num> &extents );

            /** @copydoc PhysxShape<IPlaneShape3>::setAABB */
            void setAABB( const AABB3<real_Num> &box );

            /** @copydoc PhysxShape<IPlaneShape3>::getAABB */
            AABB3<real_Num> getAABB() const;

            /** @copydoc PhysxShape<IPlaneShape3>::getDistance */
            real_Num getDistance() const override;

            /** @copydoc PhysxShape<IPlaneShape3>::setDistance */
            void setDistance( real_Num distance ) override;

            /** @copydoc PhysxShape<IPlaneShape3>::getNormal */
            Vector3<real_Num> getNormal() const override;

            /** @copydoc PhysxShape<IPlaneShape3>::setNormal */
            void setNormal( const Vector3<real_Num> &normal ) override;

            /** @copydoc PhysxShape<IPlaneShape3>::getPlane */
            Plane3<real_Num> getPlane() const override;

            /** @copydoc PhysxShape<IPlaneShape3>::createShape */
            void createShape();

            /** @copydoc PhysxShape<IPlaneShape3>::destroyShape */
            void destroyShape();

        protected:
            /** The extents of the plane. */
            Vector3<real_Num> m_extents;

            /** The PhysX material. */
            RawPtr<physx::PxMaterial> m_material;

            /** The plane. */
            Plane3<real_Num> m_plane;
        };

    }  // end namespace physics
}  // end namespace fb

#endif  // FBPhysxBoxShape_h__
