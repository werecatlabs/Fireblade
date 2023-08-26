#ifndef CRaycastHit_h__
#define CRaycastHit_h__

#include <FBApplication/FBApplicationPrerequisites.h>

#include <FBCore/Interface/Physics/IRaycastHit.h>

namespace fb
{
    namespace physics
    {
        class CRaycastHit : public IRaycastHit
        {
        public:
            CRaycastHit() = default;
            ~CRaycastHit() override = default;

            SmartPtr<IPhysicsShape3> getCollider() const override;
            void setCollider( SmartPtr<IPhysicsShape3> shape ) override;

            SmartPtr<IRigidBody3> getRigidBody() const override;
            void setRigidBody( SmartPtr<IRigidBody3> rigidBody ) override;

            Vector2<real_Num> getBarycentricCoordinate() const override;
            void setBarycentricCoordinate( const Vector2<real_Num> &barycentricCoordinate ) override;

            Vector2<real_Num> getLightmapCoord() const override;
            void setLightmapCoord( const Vector2<real_Num> &lightmapCoord ) override;

            Vector3<real_Num> getNormal() const override;
            void setNormal( const Vector3<real_Num> &normal ) override;

            Vector3<real_Num> getPoint() const override;
            void setPoint( const Vector3<real_Num> &point ) override;

            Vector2<real_Num> getTextureCoord() const override;
            void setTextureCoord( const Vector2<real_Num> &textureCoord ) override;

            Vector2<real_Num> getTextureCoord2() const override;
            void setTextureCoord2( const Vector2<real_Num> &textureCoord2 ) override;

            real_Num getDistance() const override;
            void setDistance( real_Num distance ) override;

            s32 getTriangleIndex() const override;
            void setTriangleIndex( s32 triangleIndex ) override;

            u32 getCollisionMask() const override;
            void setCollisionMask( u32 collisionMask ) override;

            bool getCheckStatic() const override;
            void setCheckStatic( bool checkStatic ) override;

            bool getCheckDynamic() const override;
            void setCheckDynamic( bool checkDynamic ) override;

        protected:
            SmartPtr<IPhysicsShape3> m_collider;
            SmartPtr<IRigidBody3> m_rigidbody;

            Vector2<real_Num> m_barycentricCoordinate;
            Vector2<real_Num> m_lightmapCoord;
            Vector3<real_Num> m_normal;
            Vector3<real_Num> m_point;
            Vector2<real_Num> m_textureCoord;
            Vector2<real_Num> m_textureCoord2;

            real_Num m_distance = static_cast<real_Num>( 0.0 );
            s32 m_triangleIndex = 0;
            u32 m_collisionMask = 0;

            bool m_checkStatic = true;
            bool m_checkDynamic = true;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // CRaycastHit_h__
