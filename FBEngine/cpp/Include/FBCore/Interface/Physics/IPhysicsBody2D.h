#ifndef _IPhysicsBody2_H
#define _IPhysicsBody2_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Transform2.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/AABB2.h>

namespace fb
{
    namespace physics
    {

        /**
         * @brief A body that interacts in the physics world.
         */
        class IPhysicsBody2D : public ISharedObject
        {
        public:
            /**
             * @brief Virtual destructor.
             */
            ~IPhysicsBody2D() override = default;

            /**
             * @brief Sets position the physics body.
             * @param position The new position of the body.
             */
            virtual void setPosition( const Vector2<real_Num> &position ) = 0;

            /**
             * @brief Gets the position of the body.
             * @return The position of the body.
             */
            virtual Vector2<real_Num> getPosition() const = 0;

            /**
             * @brief Sets the target position of the body.
             * @param position The target position of the body.
             */
            virtual void setTargetPosition( const Vector2<real_Num> &position ) = 0;

            /**
             * @brief Gets the target position of the body.
             * @return The target position of the body.
             */
            virtual Vector2<real_Num> getTargetPosition() const = 0;

            /**
             * @brief Sets the orientation of the body.
             * @param orientation The new orientation of the body.
             */
            virtual void setOrientation( real_Num orientation ) = 0;

            /**
             * @brief Gets the orientation of the body.
             * @return The orientation of the body.
             */
            virtual real_Num getOrientation() const = 0;

            /**
             * @brief Gets the angular velocity of the body.
             * @return The angular velocity of the body.
             */
            virtual real_Num getAngularVelocity() const = 0;

            /**
             * @brief Adds a force to the body.
             * @param force The force to add to the body.
             */
            virtual void addForce( const Vector2<real_Num> &force ) = 0;

            /**
             * @brief Sets the force on the body.
             * @param force The new force on the body.
             */
            virtual void setForce( const Vector2<real_Num> &force ) = 0;

            /**
             * @brief Gets the force on the body.
             * @return The force on the body.
             */
            virtual Vector2<real_Num> getForce() const = 0;

            /**
             * @brief Adds a torque to the body.
             * @param torque The torque to add to the body.
             */
            virtual void addTorque( real_Num torque ) = 0;

            /**
             * @brief Sets the torque on the body.
             * @param torque The new torque on the body.
             */
            virtual void setTorque( real_Num torque ) = 0;

            /**
             * @brief Gets the torque on the body.
             * @return The torque on the body.
             */
            virtual real_Num getTorque() const = 0;

            /**
             * @brief Adds velocity to the rigid body.
             * @param velocity The velocity to add to the body.
             * @param relPos The relative position to apply the velocity to.
             */
            virtual void addVelocity( const Vector2<real_Num> &velocity,
                                      const Vector2<real_Num> &relPos = Vector2<real_Num>::ZERO ) = 0;

            /**
             * @brief Sets the velocity of the body.
             * @param velocity The new velocity of the body.
             */
            virtual void setVelocity( const Vector2<real_Num> &velocity ) = 0;

            /** Gets the velocity of the body. */
            virtual Vector2<real_Num> getVelocity() const = 0;

            /** Sets velocity the physics body. */
            virtual void setMaxVelocity( const Vector2<real_Num> &velocity ) = 0;

            /** Gets the velocity of the body. */
            virtual Vector2<real_Num> getMaxVelocity() const = 0;

            /** Sets the damp rate of the linear velocity*/
            virtual void setLinearDampValue( real_Num linearDampValue ) = 0;

            /** Gets the linear damp value. */
            virtual real_Num getLinearDampValue() const = 0;

            /** Sets the angular damp value. */
            virtual void setAngularDampValue( real_Num angularDampValue ) = 0;

            /** Gets the angular damp value. */
            virtual real_Num getAngularDampValue() const = 0;

            /** Sets an air resistance value. */
            virtual void setAirResistance( real_Num airResistance ) = 0;

            /** Gets the air resistance value. */
            virtual real_Num getAirResistance() const = 0;

            /** Sets the restitution value. */
            virtual void setRestitution( real_Num restitution ) = 0;

            /** Gets the restitution value. */
            virtual real_Num getRestitution() const = 0;

            /** Sets the mass of the rigid body. */
            virtual void setMass( real_Num mass ) = 0;

            /** Gets the mass of the rigid body. */
            virtual real_Num getMass() const = 0;

            /** Gets the inverse mass of the rigid body. */
            virtual real_Num getMassInv() const = 0;

            /** Sets a flag of this physics body. */
            virtual void setFlag( u32 flag, bool value ) = 0;

            /** Gets a flag of this physics body. */
            virtual bool getFlag( u32 flag ) const = 0;

            /** Gets the type of physics body. Useful type casting. */
            virtual u32 getBodyType() const = 0;

            /** Sets the type of entity.
            @remarks
            Useful for knowing what type object owns this object.
            @param
            type The type of object this physics object represents.
            */
            virtual void setObjectType( hash_type type ) = 0;

            /** Gets the type of entity. */
            virtual hash_type getObjectType() const = 0;

            /** Sets the id of the world this physics body belongs to. */
            virtual void setWorldId( hash_type worldId ) = 0;

            /** Gets the id of the world this physics body belongs. */
            virtual hash_type getWorldId() const = 0;

            /** Setting this to false stops the body from taking part in the simulation.
             */
            virtual void setEnabled( bool enabled ) = 0;

            /** Gets a boolean indicating whether the body is enabled. */
            virtual bool isEnabled() const = 0;

            /** Get the local bounding box of the body. */
            virtual AABB2<real_Num> getLocalAABB() const = 0;

            /** Get the world bounding box of the body. */
            virtual AABB2<real_Num> getWorldAABB() const = 0;

            /** Sets the material id of the body. */
            virtual void setMaterialId( hash_type materialId ) = 0;

            /** Gets the material id of the body. */
            virtual hash_type getMaterialId() const = 0;

            /** Used to decide which objects to collide with. */
            virtual void setCollisionType( u32 mask ) = 0;

            /** Gets the mask used to decide which objects to collide with.*/
            virtual u32 getCollisionType() const = 0;

            /** Used to decide which objects to collide with. */
            virtual void setCollisionMask( u32 mask ) = 0;

            /** Gets the mask used to decide which objects to collide with.*/
            virtual u32 getCollisionMask() const = 0;

            /** Sleep. */
            virtual void setSleep( bool sleep ) = 0;

            /** Sleep. */
            virtual bool isSleeping() const = 0;

            /** */
            virtual void setContraintAABB( const AABB2<real_Num> &contraintRect ) = 0;

            /** */
            virtual AABB2<real_Num> getContraintAABB() const = 0;

            virtual bool getKinematicMode() const = 0;
            virtual void setKinematicMode( bool val ) = 0;

            virtual Vector2<real_Num> getGravity() const = 0;
            virtual void setGravity( const Vector2<real_Num> &gravity ) = 0;

            virtual bool getEnableGravity() const = 0;
            virtual void setEnableGravity( bool enableGravity ) = 0;

            virtual void addEffect( SmartPtr<IPhysicsEffect2> effect ) = 0;
            virtual void removeEffect( SmartPtr<IPhysicsEffect2> effect ) = 0;

            virtual Array<SmartPtr<IPhysicsConstraint2>> getConstraints() const = 0;
            virtual void removeConstraints() = 0;
            virtual void removeConstraint( SmartPtr<IPhysicsConstraint2> constraint ) = 0;
            virtual void addConstraint( SmartPtr<IPhysicsConstraint2> constraint ) = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
