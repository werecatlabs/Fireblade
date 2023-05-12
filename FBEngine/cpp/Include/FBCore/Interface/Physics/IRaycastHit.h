#ifndef IRaycastHit_h__
#define IRaycastHit_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    namespace physics
    {
        /** Interface for a raycast class. */
        class IRaycastHit : public ISharedObject
        {
        public:
            /** Destructor. */
            ~IRaycastHit() override = default;

            virtual SmartPtr<IPhysicsShape3> getCollider() const = 0;
            virtual void setCollider( SmartPtr<IPhysicsShape3> shape ) = 0;

            virtual SmartPtr<IRigidBody3> getRigidBody() const = 0;
            virtual void setRigidBody( SmartPtr<IRigidBody3> rigidBody ) = 0;

            virtual Vector2<real_Num> getBarycentricCoordinate() const = 0;
            virtual void setBarycentricCoordinate( const Vector2<real_Num> &barycentricCoordinate ) = 0;

            virtual Vector2<real_Num> getLightmapCoord() const = 0;
            virtual void setLightmapCoord( const Vector2<real_Num> &lightmapCoord ) = 0;

            virtual Vector3<real_Num> getNormal() const = 0;
            virtual void setNormal( const Vector3<real_Num> &normal ) = 0;

            virtual Vector3<real_Num> getPoint() const = 0;
            virtual void setPoint( const Vector3<real_Num> &point ) = 0;

            virtual Vector2<real_Num> getTextureCoord() const = 0;
            virtual void setTextureCoord( const Vector2<real_Num> &textureCoord ) = 0;

            virtual Vector2<real_Num> getTextureCoord2() const = 0;
            virtual void setTextureCoord2( const Vector2<real_Num> &textureCoord2 ) = 0;

            virtual real_Num getDistance() const = 0;
            virtual void setDistance( real_Num distance ) = 0;

            virtual s32 getTriangleIndex() const = 0;
            virtual void setTriangleIndex( s32 triangleIndex ) = 0;

            virtual u32 getCollisionMask() const = 0;
            virtual void setCollisionMask( u32 collisionMask ) = 0;

            virtual bool getCheckStatic() const = 0;
            virtual void setCheckStatic( bool checkStatic ) = 0;

            virtual bool getCheckDynamic() const = 0;
            virtual void setCheckDynamic( bool checkDynamic ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif  // IRaycastHit_h__
