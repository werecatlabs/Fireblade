#include <FBCore/FBCorePCH.h>
#include <FBCore/Physics/RaycastHit.h>
#include <FBCore/Interface/Physics/IPhysicsShape3.h>
#include <FBCore/Interface/Physics/IRigidBody3.h>

namespace fb
{
    namespace physics
    {
        SmartPtr<IPhysicsShape3> RaycastHit::getCollider() const
        {
            return m_collider;
        }

        void RaycastHit::setCollider( SmartPtr<IPhysicsShape3> shape )
        {
            m_collider = shape;
        }

        SmartPtr<IRigidBody3> RaycastHit::getRigidBody() const
        {
            return m_rigidbody;
        }

        void RaycastHit::setRigidBody( SmartPtr<IRigidBody3> rigidBody )
        {
            m_rigidbody = rigidBody;
        }

        Vector2<real_Num> RaycastHit::getBarycentricCoordinate() const
        {
            return m_barycentricCoordinate;
        }

        void RaycastHit::setBarycentricCoordinate( const Vector2<real_Num> &barycentricCoordinate )
        {
            m_barycentricCoordinate = barycentricCoordinate;
        }

        Vector2<real_Num> RaycastHit::getLightmapCoord() const
        {
            return m_lightmapCoord;
        }

        void RaycastHit::setLightmapCoord( const Vector2<real_Num> &lightmapCoord )
        {
            m_lightmapCoord = lightmapCoord;
        }

        Vector3<real_Num> RaycastHit::getNormal() const
        {
            return m_normal;
        }

        void RaycastHit::setNormal( const Vector3<real_Num> &normal )
        {
            m_normal = normal;
        }

        Vector3<real_Num> RaycastHit::getPoint() const
        {
            return m_point;
        }

        void RaycastHit::setPoint( const Vector3<real_Num> &point )
        {
            m_point = point;
        }

        Vector2<real_Num> RaycastHit::getTextureCoord() const
        {
            return m_textureCoord;
        }

        void RaycastHit::setTextureCoord( const Vector2<real_Num> &textureCoord )
        {
            m_textureCoord = textureCoord;
        }

        Vector2<real_Num> RaycastHit::getTextureCoord2() const
        {
            return m_textureCoord2;
        }

        void RaycastHit::setTextureCoord2( const Vector2<real_Num> &textureCoord2 )
        {
            m_textureCoord2 = textureCoord2;
        }

        real_Num RaycastHit::getDistance() const
        {
            return m_distance;
        }

        void RaycastHit::setDistance( real_Num distance )
        {
            m_distance = distance;
        }

        s32 RaycastHit::getTriangleIndex() const
        {
            return m_triangleIndex;
        }

        void RaycastHit::setTriangleIndex( s32 triangleIndex )
        {
            m_triangleIndex = triangleIndex;
        }

        u32 RaycastHit::getCollisionMask() const
        {
            return m_collisionMask;
        }

        void RaycastHit::setCollisionMask( u32 collisionMask )
        {
            m_collisionMask = collisionMask;
        }

        bool RaycastHit::getCheckStatic() const
        {
            return m_checkStatic;
        }

        void RaycastHit::setCheckStatic( bool checkStatic )
        {
            m_checkStatic = checkStatic;
        }

        bool RaycastHit::getCheckDynamic() const
        {
            return m_checkDynamic;
        }

        void RaycastHit::setCheckDynamic( bool checkDynamic )
        {
            m_checkDynamic = checkDynamic;
        }
    }  // namespace physics
}  // end namespace fb
