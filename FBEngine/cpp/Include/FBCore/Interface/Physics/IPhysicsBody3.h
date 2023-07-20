#ifndef _IPhysicsBody3_H
#define _IPhysicsBody3_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Transform3.h>
#include <FBCore/Math/AABB3.h>
#include <FBCore/Interface/Physics/FBPhysicsTypes.h>

namespace fb
{
    namespace physics
    {

        /** A body that interacts in the physics world.
         */
        class IPhysicsBody3 : public ISharedObject
        {
        public:
            static const hash_type STATE_MESSAGE_ATTACH_SHAPE;
            static const hash_type STATE_MESSAGE_DETACH_SHAPE;
            static const hash_type STATE_MESSAGE_MASS;
            static const hash_type STATE_MESSAGE_INERTIA_TENSOR;

            /** Destructor. */
            ~IPhysicsBody3() override = default;

            /** Gets the scene this body belongs to. */
            virtual SmartPtr<IPhysicsScene3> getScene() const = 0;

            /** Sets the scene this body belongs to. */
            virtual void setScene( SmartPtr<IPhysicsScene3> scene ) = 0;

            /** Sets position the physics body. */
            virtual void setTransform( const Transform3<real_Num> &transform ) = 0;

            /** Gets the position of the body. */
            virtual Transform3<real_Num> getTransform() const = 0;

            /** Sets an actor flag. */
            virtual void setActorFlag( ActorFlag::Enum flag, bool value ) = 0;

            /** Gets the actor flags. */
            virtual ActorFlag::Enum getActorFlags() const = 0;

            /** Gets the body's mass. */
            virtual real_Num getMass() const = 0;

            /** Sets the body's mass. */
            virtual void setMass( real_Num mass ) = 0;

            /** Used to decide which objects to collide with. */
            virtual void setCollisionType( u32 type ) = 0;

            /** Gets the mask used to decide which objects to collide with.*/
            virtual u32 getCollisionType() const = 0;

            /** Used to decide which objects to collide with. */
            virtual void setCollisionMask( u32 mask ) = 0;

            /** Gets the mask used to decide which objects to collide with.*/
            virtual u32 getCollisionMask() const = 0;

            /** Set if the body is enabled. */
            virtual void setEnabled( bool enabled ) = 0;

            /** Gets if the body is enabled. */
            virtual bool isEnabled() const = 0;

            /** Gets the uer data. */
            virtual void *getUserDataById( u32 id ) const = 0;

            /** Sets the uer data. */
            virtual void setUserDataById( u32 id, void *userData ) = 0;

            /** Gets the uer data. */
            void *getUserData() const override = 0;

            /** Sets the uer data. */
            void setUserData( void *userData ) override = 0;

            /** Gets if the body is in kinematic mode. */
            virtual bool getKinematicMode() const = 0;

            /** Sets if the body is in kinematic mode. */
            virtual void setKinematicMode( bool kinematicMode ) = 0;

            /** Clones the physics body. */
            virtual SmartPtr<IPhysicsBody3> clone() = 0;

            /** Gets the data as a properties object.
            @return The data as a properties object.
            */
            virtual SmartPtr<Properties> getProperties() const = 0;

            /** Sets the data as a properties object.
            @param properties The properties object.
            */
            virtual void setProperties( SmartPtr<Properties> properties ) = 0;

            /** Gets the state object. */
            virtual SmartPtr<IStateContext> getStateObject() const = 0;

            /** Sets the state object. */
            virtual void setStateObject( SmartPtr<IStateContext> stateObject ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
