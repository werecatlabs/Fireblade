#ifndef FBPhysxCharacterController_h__
#define FBPhysxCharacterController_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Interface/Physics/ICharacterController3.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <foundation/PxVec3.h>

namespace fb
{
    namespace physics
    {
        
        class PhysxCharacterController : public ICharacterController3
        {
        public:
            PhysxCharacterController();
            ~PhysxCharacterController() override;

            virtual void initialise( SmartPtr<scene::IDirector> objectTemplate );

            virtual void initialise( SmartPtr<scene::IDirector> objectTemplate,
                                     SmartPtr<Properties> instanceProperties );

            void update() override;

            void setPosition( const Vector3F &position ) override;

            Vector3F getPosition() const override;

            void setOrientation( const QuaternionF &orientation ) override;

            QuaternionF getOrientation() const override;

            f32 getMoveSpeed() const override;

            void setMoveSpeed( f32 moveSpeed ) override;

            bool getJump() const override;

            void setJump( bool jump ) override;

            bool isGrounded() const override;

            void _getObject( void **ppObject ) override;

            void setWalkVector( const Vector3F &vector ) override;

            void stop() override;

            real_Num getMass() const override;

            void setMass( real_Num mass ) override;

            void setCollisionType( u32 type ) override;

            u32 getCollisionType() const override;

            void setCollisionMask( u32 mask ) override;

            u32 getCollisionMask() const override;

            void setEnabled( bool enabled ) override;

            bool isEnabled() const override;

            void *getUserData() const override;

            void setUserData( void *userData ) override;

            bool getKinematicMode() const override;
            void setKinematicMode( bool val ) override;

            void *getUserDataById( u32 id ) const override;
            void setUserDataById( u32 id, void *userData ) override;

        protected:
            physx::PxCapsuleController *mController;
            physx::PxControllerManager *mControllerManager;
            physx::PxVec3 mControllerInitialPosition;
            Vector3F m_moveVector;
            bool m_kinematicMode;
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxCharacterController_h__
