#ifndef __IPhysicsRigidBody2__H
#define __IPhysicsRigidBody2__H

#include <FBCore/Interface/Physics/IPhysicsBody2D.h>

namespace fb
{
    namespace physics
    {

        /** An interface for a 2d rigid body.
         */
        class IRigidBody2 : public IPhysicsBody2D
        {
        public:
            static const u32 RBF_CONSTRAINBOUNDS = ( 1 << 0 );
            static const u32 RBF_CAPSCREENPOSITION = ( 1 << 1 );
            static const u32 RBF_ENABLE_COLLISION = ( 1 << 2 );
            static const u32 RBF_DAMPLINEARVELOCITY = ( 1 << 3 );
            static const u32 RBF_ENABLEPHYSICS = ( 1 << 4 );
            static const u32 RBF_CONSTRAIN_X = ( 1 << 5 );
            static const u32 RBF_CONSTRAIN_Y = ( 1 << 6 );
            static const u32 RBF_ALLOWROTATION = ( 1 << 7 );
            // static const u32 RBF_SAME_TYPE_COLLISION		= (1 << 8);
            static const u32 RBF_ENABLE_PARTICLE_COLLISION = ( 1 << 9 );
            static const u32 RBF_USE_TARGET_POSITION_X = ( 1 << 10 );
            static const u32 RBF_USE_TARGET_POSITION_Y = ( 1 << 11 );
            static const u32 RBF_ENABLE = ( 1 << 12 );

            /** */
            ~IRigidBody2() override = default;

            /** */
            virtual void setCollisionShape( const SmartPtr<IPhysicsShape2> &shape ) = 0;

            /** */
            virtual const SmartPtr<IPhysicsShape2> &getCollisionShape() const = 0;

            FB_CLASS_REGISTER_DECL;
        };
    }  // end namespace physics
}  // end namespace fb

#endif
