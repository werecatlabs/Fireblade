#ifndef IPhysicsWorld2_h__
#define IPhysicsWorld2_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    namespace physics
    {

        class IPhysicsScene2 : public ISharedObject
        {
        public:
            /** */
            ~IPhysicsScene2() override = default;

            /** */
            virtual void updateRigidBodies() = 0;

            /** */
            virtual void updateParticles() = 0;

            /** */
            virtual void addRigidBody( SmartPtr<IRigidBody2> body ) = 0;

            /** */
            virtual void removeRigidBody( SmartPtr<IRigidBody2> body ) = 0;

            /** */
            virtual void addParticle( SmartPtr<IPhysicsParticle2> particle ) = 0;

            /** */
            virtual void removeParticle( SmartPtr<IPhysicsParticle2> particle ) = 0;

            /** */
            virtual void _setId( u32 id ) = 0;

            /** */
            virtual u32 getId() const = 0;

            /** */
            virtual void setSize( const Vector2<real_Num> &size ) = 0;

            /** */
            virtual Vector2<real_Num> getSize() const = 0;

            virtual Vector2<real_Num> getGravity() const = 0;
            virtual void setGravity( const Vector2<real_Num> &gravity ) = 0;

            FB_CLASS_REGISTER_DECL;
        };

    }  // namespace physics
}  // end namespace fb

#endif  // IPhysicsWorld2_h__
