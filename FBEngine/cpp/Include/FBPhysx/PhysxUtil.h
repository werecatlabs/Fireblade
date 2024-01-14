#ifndef PhysxUtil_h__
#define PhysxUtil_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Math/Transform3.h>
#include "PxQuat.h"
#include "PxMat44.h"
#include "PxMat33.h"
#include "PxVec3.h"
#include "PxBounds3.h"

namespace fb
{
    namespace physics
    {
        class PhysxUtil
        {
        public:
            static bool equals( const physx::PxVec3 &v1, const physx::PxVec3 &v2, real_Num epsilon = 0.0001f );

            static bool equals( const physx::PxQuat &q1, const physx::PxQuat &q2, real_Num epsilon = 0.0001f );

            static bool equals( const physx::PxTransform &t1, const physx::PxTransform &t2,
                         real_Num epsilon = 0.0001f );

            static Vector3<real_Num> toFB( const physx::PxVec3 &vec3 );
            static Quaternion<real_Num> toFB( const physx::PxQuat &q );
            static AABB3<real_Num> toFB( const physx::PxBounds3 &b );
            static Transform3<real_Num> toFB( const physx::PxTransform &t );

            static physx::PxVec3 toPx( const Vector3<real_Num> &vec3 );
            static physx::PxQuat toPx( const Quaternion<real_Num> &q );
            static physx::PxBounds3 toPx( const AABB3<real_Num> &b );
            static physx::PxTransform toPx( const Transform3<real_Num> &t );
            static physx::PxMat44 toPx( const Vector3<real_Num> &pos, const Quaternion<real_Num> &rot );
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // PhysxUtil_h__
