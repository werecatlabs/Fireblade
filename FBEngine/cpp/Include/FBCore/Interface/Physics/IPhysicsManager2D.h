#ifndef __IPhysicsManager2d__H
#define __IPhysicsManager2d__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>

namespace fb
{
    namespace physics
    {
        
        /** A physics manager class to manager 2d physics. */
        class IPhysicsManager2D : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IPhysicsManager2D() override = default;

            /** Used to update the rigid bodies in the physics manager.
            @remarks
                This is useful to update the rigid bodies on a separate thread.
            */
            virtual void updateRigidBodies() = 0;

            /** Used to update the particles in the physics manager.
            @remarks
                This is useful to update the particles on a separate thread.
            */
            virtual void updateParticles() = 0;

            /** Adds physics world.
             */
            virtual SmartPtr<IPhysicsScene2> addWorld( u32 id ) = 0;

            /** Removes a physics world.
             */
            virtual void removeWorld( SmartPtr<IPhysicsScene2> world ) = 0;

            /** Finds a physics world. Returns null if no physics world with the id passed is found.
             */
            virtual SmartPtr<IPhysicsScene2> findWorld( u32 id ) const = 0;

            /** Gets the physics worlds.
             */
            virtual Array<SmartPtr<IPhysicsScene2>> getWorlds() const = 0;

            /** Creates a collision from the type provided.
            @remarks
                Creates a collision shape from the type specified with default properties.
            @param
                type The of collision to be created.
            */
            virtual SmartPtr<IPhysicsShape2> createCollisionShapeByType( hash32 type ) = 0;

            /** Creates a collision shape from the template type provided.
            @return An instance of a collision shape.
            */
            template <class T>
            SmartPtr<T> createCollisionShape()
            {
                auto typeInfo = T::typeInfo();
                auto typeHash = typeInfo->getHash();
                auto shape = createCollisionShapeByType( typeHash );
                return fb::static_pointer_cast<T>( shape );
            }

            /** Creates a rigid body. */
            virtual SmartPtr<IRigidBody2> createRigidBody() = 0;

            /** Creates a rigid body. */
            virtual SmartPtr<IRigidBody2> createRigidBody( SmartPtr<IPhysicsShape2> collisionShape ) = 0;

            /** Creates a particle. */
            virtual SmartPtr<IPhysicsParticle2> createParticle(
                u8 particleType, SmartPtr<IPhysicsShape2> collisionShape ) = 0;

            /** Create creates a soft body. */
            virtual SmartPtr<IPhysicsSoftBody2> createSoftBody() = 0;

            /** Clears the physics manager. */
            virtual void clear() = 0;

            /** Gets a particle by id. */
            virtual SmartPtr<IPhysicsParticle2> getParticle( u32 id ) const = 0;

            /** An on change flags event. */
            virtual void OnChangeFlags( IPhysicsBody2D *body ) = 0;

            //
            // Query functions
            //

            /** Used to find out if two bodies are colliding. */
            virtual bool isColliding( IPhysicsBody2D *bodyA, IPhysicsBody2D *bodyB ) const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
