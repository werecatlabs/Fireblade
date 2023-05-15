#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/PhysxUtil.h>

namespace fb
{
    namespace physics
    {
        AABB3<real_Num> PhysxUtil::toFB( const physx::PxBounds3 &b )
        {
            return AABB3<real_Num>( toFB( b.minimum ), toFB( b.maximum ) );
        }

        Quaternion<real_Num> PhysxUtil::toFB( const physx::PxQuat &q )
        {
            return Quaternion<real_Num>( q.w, q.x, q.y, q.z );
        }

        Vector3<real_Num> PhysxUtil::toFB( const physx::PxVec3 &vec3 )
        {
            return Vector3<real_Num>( vec3.x, vec3.y, vec3.z );
        }

        Transform3<real_Num> PhysxUtil::toFB( const physx::PxTransform &t )
        {
            return Transform3<real_Num>( Vector3<real_Num>( t.p.x, t.p.y, t.p.z ),
                                         Quaternion<real_Num>( t.q.w, t.q.x, t.q.y, t.q.z ) );
        }

        physx::PxMat44 PhysxUtil::toPx( const Vector3<real_Num> &pos, const Quaternion<real_Num> &rot )
        {
            return physx::PxMat44( physx::PxMat33( toPx( rot ) ), toPx( pos ) );
        }

        physx::PxBounds3 PhysxUtil::toPx( const AABB3<real_Num> &b )
        {
            physx::PxBounds3 Pxb;
            Pxb.minimum = toPx( b.getMinimum() );
            Pxb.maximum = toPx( b.getMaximum() );
            return Pxb;
        }

        physx::PxQuat PhysxUtil::toPx( const Quaternion<real_Num> &q )
        {
            physx::PxQuat pxq;
            pxq.w = q.W();
            pxq.x = q.X();
            pxq.y = q.Y();
            pxq.z = q.Z();
            return pxq;
        }

        physx::PxVec3 PhysxUtil::toPx( const Vector3<real_Num> &vec3 )
        {
            return physx::PxVec3( vec3.X(), vec3.Y(), vec3.Z() );
        }

        physx::PxTransform PhysxUtil::toPx( const Transform3<real_Num> &t )
        {
            auto p = toPx( t.getPosition() );
            auto q = toPx( t.getOrientation() );
            return physx::PxTransform( p, q );
        }
    } // end namespace physics
}     // end namespace fb
