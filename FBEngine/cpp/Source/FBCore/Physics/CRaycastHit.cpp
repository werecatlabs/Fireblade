#include <FBCore/FBCorePCH.h>
#include "FBCore/Physics/CRaycastHit.h"
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Interface/Physics/IRigidBody3.h>

namespace fb
{
    namespace physics
    {
        SmartPtr<IPhysicsShape3> CRaycastHit::getCollider() const
        {
            return m_collider;
        }

        void CRaycastHit::setCollider( SmartPtr<IPhysicsShape3> shape )
        {
            m_collider = shape;
        }

        SmartPtr<IRigidBody3> CRaycastHit::getRigidBody() const
        {
            return m_rigidbody;
        }

        void CRaycastHit::setRigidBody( SmartPtr<IRigidBody3> rigidBody )
        {
            m_rigidbody = rigidBody;
        }

        Vector2<real_Num> CRaycastHit::getBarycentricCoordinate() const
        {
            return m_barycentricCoordinate;
        }

        void CRaycastHit::setBarycentricCoordinate( const Vector2<real_Num> &barycentricCoordinate )
        {
            m_barycentricCoordinate = barycentricCoordinate;
        }

        Vector2<real_Num> CRaycastHit::getLightmapCoord() const
        {
            return m_lightmapCoord;
        }

        void CRaycastHit::setLightmapCoord( const Vector2<real_Num> &lightmapCoord )
        {
            m_lightmapCoord = lightmapCoord;
        }

        Vector3<real_Num> CRaycastHit::getNormal() const
        {
            return m_normal;
        }

        void CRaycastHit::setNormal( const Vector3<real_Num> &normal )
        {
            m_normal = normal;
        }

        Vector3<real_Num> CRaycastHit::getPoint() const
        {
            return m_point;
        }

        void CRaycastHit::setPoint( const Vector3<real_Num> &point )
        {
            m_point = point;
        }

        Vector2<real_Num> CRaycastHit::getTextureCoord() const
        {
            return m_textureCoord;
        }

        void CRaycastHit::setTextureCoord( const Vector2<real_Num> &textureCoord )
        {
            m_textureCoord = textureCoord;
        }

        Vector2<real_Num> CRaycastHit::getTextureCoord2() const
        {
            return m_textureCoord2;
        }

        void CRaycastHit::setTextureCoord2( const Vector2<real_Num> &textureCoord2 )
        {
            m_textureCoord2 = textureCoord2;
        }

        real_Num CRaycastHit::getDistance() const
        {
            return m_distance;
        }

        void CRaycastHit::setDistance( real_Num distance )
        {
            m_distance = distance;
        }

        s32 CRaycastHit::getTriangleIndex() const
        {
            return m_triangleIndex;
        }

        void CRaycastHit::setTriangleIndex( s32 triangleIndex )
        {
            m_triangleIndex = triangleIndex;
        }

        u32 CRaycastHit::getCollisionMask() const
        {
            return m_collisionMask;
        }

        void CRaycastHit::setCollisionMask( u32 collisionMask )
        {
            m_collisionMask = collisionMask;
        }

        bool CRaycastHit::getCheckStatic() const
        {
            return m_checkStatic;
        }

        void CRaycastHit::setCheckStatic( bool checkStatic )
        {
            m_checkStatic = checkStatic;
        }

        bool CRaycastHit::getCheckDynamic() const
        {
            return m_checkDynamic;
        }

        void CRaycastHit::setCheckDynamic( bool checkDynamic )
        {
            m_checkDynamic = checkDynamic;
        }
    }  // namespace physics
}  // end namespace fb
