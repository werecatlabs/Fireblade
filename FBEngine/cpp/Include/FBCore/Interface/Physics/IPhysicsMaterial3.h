#ifndef __IPhysicsMaterial3__H
#define __IPhysicsMaterial3__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{
    namespace physics
    {

        /** Interface for a 3d physics material. */
        class IPhysicsMaterial3 : public ISharedObject
        {
        public:
            ~IPhysicsMaterial3() override = default;

            virtual void setFriction( f32 friction, s32 direction ) = 0;

            virtual void setRestitution( f32 restitution ) = 0;

            virtual Vector3<real_Num> getContactPosition() const = 0;
            virtual Vector3<real_Num> getContactNormal() const = 0;

            virtual SmartPtr<IRigidBody3> getPhysicsBodyA() const = 0;
            virtual SmartPtr<IRigidBody3> getPhysicsBodyB() const = 0;

            /** Gets the data as a properties object.
            @return The data as a properties object.
            */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /** Sets the data as a properties object.
            @param properties The properties object.
            */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
