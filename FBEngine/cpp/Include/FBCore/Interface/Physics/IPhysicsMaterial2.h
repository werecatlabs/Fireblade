#ifndef __IPhysicsMaterial2__H
#define __IPhysicsMaterial2__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace physics
    {

        /** Interface for a 2d physics material. */
        class IPhysicsMaterial2 : public ISharedObject
        {
        public:
            ~IPhysicsMaterial2() override = default;

            virtual void setFriction( real_Num friction, s32 direction ) = 0;
            virtual real_Num getFriction( s32 direction ) const = 0;

            virtual void setRestitution( real_Num restitution ) = 0;
            virtual real_Num getRestitution() const = 0;

            virtual void setContactPosition( const Vector2<real_Num> &position ) = 0;
            virtual Vector2<real_Num> getContactPosition() const = 0;

            virtual void setContactNormal( const Vector2<real_Num> &normal ) = 0;
            virtual Vector2<real_Num> getContactNormal() const = 0;

            virtual void setPhysicsBodyA( SmartPtr<IPhysicsBody2D> body ) = 0;
            virtual SmartPtr<IPhysicsBody2D> getPhysicsBodyA() const = 0;

            virtual void setPhysicsBodyB( SmartPtr<IPhysicsBody2D> body ) = 0;
            virtual SmartPtr<IPhysicsBody2D> getPhysicsBodyB() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
