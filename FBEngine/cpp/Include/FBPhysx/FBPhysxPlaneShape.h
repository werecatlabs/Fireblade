#ifndef FBPhysxPlaneShape_h__
#define FBPhysxPlaneShape_h__

#include <FBCore/Interface/Physics/IPlaneShape3.h>
#include <FBPhysx/FBPhysxShape.h>

namespace fb
{
    namespace physics
    {
        class PhysxPlaneShape : public PhysxShape<IPlaneShape3>
        {
        public:
            PhysxPlaneShape() = default;
            ~PhysxPlaneShape() override = default;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            Vector3<real_Num> getExtents() const;
            void setExtents( const Vector3<real_Num> &extents );

            void setAABB( const AABB3<real_Num> &box );
            AABB3<real_Num> getAABB() const;

            real_Num getDistance() const override;
            void setDistance( real_Num distance ) override;

            Vector3<real_Num> getNormal() const override;
            void setNormal( const Vector3<real_Num> &normal ) override;

            Plane3<real_Num> getPlane() const override;

            void createShape();
            void destroyShape();

        protected:
            Vector3<real_Num> m_extents;
            RawPtr<physx::PxMaterial> m_material;
            Plane3<real_Num> m_plane;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxBoxShape_h__
