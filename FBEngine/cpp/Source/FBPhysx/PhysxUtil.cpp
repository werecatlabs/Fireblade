#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/PhysxUtil.h>

namespace fb::physics
{
    auto PhysxUtil::toFB( const physx::PxBounds3 &b ) -> AABB3<real_Num>
    {
        return AABB3<real_Num>( toFB( b.minimum ), toFB( b.maximum ) );
    }

    auto PhysxUtil::toFB( const physx::PxQuat &q ) -> Quaternion<real_Num>
    {
        return { q.w, q.x, q.y, q.z };
    }

    auto PhysxUtil::toFB( const physx::PxVec3 &vec3 ) -> Vector3<real_Num>
    {
        return { vec3.x, vec3.y, vec3.z };
    }

    auto PhysxUtil::toFB( const physx::PxTransform &t ) -> Transform3<real_Num>
    {
        return Transform3<real_Num>( Vector3<real_Num>( t.p.x, t.p.y, t.p.z ),
                                     Quaternion<real_Num>( t.q.w, t.q.x, t.q.y, t.q.z ) );
    }

    auto PhysxUtil::toPx( const Vector3<real_Num> &pos, const Quaternion<real_Num> &rot )
        -> physx::PxMat44
    {
        return physx::PxMat44( physx::PxMat33( toPx( rot ) ), toPx( pos ) );
    }

    auto PhysxUtil::toPx( const AABB3<real_Num> &b ) -> physx::PxBounds3
    {
        physx::PxBounds3 Pxb;
        Pxb.minimum = toPx( b.getMinimum() );
        Pxb.maximum = toPx( b.getMaximum() );
        return Pxb;
    }

    auto PhysxUtil::toPx( const Quaternion<real_Num> &q ) -> physx::PxQuat
    {
        physx::PxQuat pxq;
        pxq.w = q.W();
        pxq.x = q.X();
        pxq.y = q.Y();
        pxq.z = q.Z();
        return pxq;
    }

    auto PhysxUtil::toPx( const Vector3<real_Num> &vec3 ) -> physx::PxVec3
    {
        return { vec3.X(), vec3.Y(), vec3.Z() };
    }

    auto PhysxUtil::toPx( const Transform3<real_Num> &t ) -> physx::PxTransform
    {
        auto p = toPx( t.getPosition() );
        auto q = toPx( t.getOrientation() );
        return physx::PxTransform( p, q );
    }

    bool PhysxUtil::equals( const physx::PxTransform &t1, const physx::PxTransform &t2,
                            real_Num epsilon /*= 0.0001f */ )
    {
        return equals( t1.p, t2.p, epsilon ) && equals( t1.q, t2.q, epsilon );
    }

    bool PhysxUtil::equals( const physx::PxQuat &q1, const physx::PxQuat &q2,
                            real_Num epsilon /*= 0.0001f */ )
    {
        return ( fabs( q1.x - q2.x ) < epsilon ) && ( fabs( q1.y - q2.y ) < epsilon ) &&
               ( fabs( q1.z - q2.z ) < epsilon ) && ( fabs( q1.w - q2.w ) < epsilon );
    }

    bool PhysxUtil::equals( const physx::PxVec3 &v1, const physx::PxVec3 &v2,
                            real_Num epsilon /*= 0.0001f */ )
    {
        return ( fabs( v1.x - v2.x ) < epsilon ) && ( fabs( v1.y - v2.y ) < epsilon ) &&
               ( fabs( v1.z - v2.z ) < epsilon );
    }

}  // namespace fb::physics
