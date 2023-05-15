#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxCharacterController.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBCore/FBCore.h>
#include <characterkinematic/PxControllerManager.h>
#include <characterkinematic/PxBoxController.h>
#include <characterkinematic/PxCapsuleController.h>

namespace fb
{
    namespace physics
    {

        PhysxCharacterController::PhysxCharacterController() : m_kinematicMode( false )
        {
        }

        PhysxCharacterController::~PhysxCharacterController()
        {
        }

        void *PhysxCharacterController::getUserDataById( u32 id ) const
        {
            return nullptr;
        }

        void PhysxCharacterController::setUserDataById( u32 id, void *userData )
        {
        }

        void PhysxCharacterController::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            physx::PxVec3 moveVec( m_moveVector.X(), m_moveVector.Y(), m_moveVector.Z() );
            const physx::PxU32 moveFlags = mController->move( moveVec, 0.01f, static_cast<f32>( dt ),
                                                              physx::PxControllerFilters() );

            physx::PxVec3 gravityVec( 0.0f, -10.0f * static_cast<f32>( dt ), 0.0f );
            const physx::PxU32 gravityFlags = mController->move(
                gravityVec, 0.01f, static_cast<f32>( dt ), physx::PxControllerFilters() );
        }

        void PhysxCharacterController::initialise( SmartPtr<scene::IDirector> objectTemplate )
        {
            // auto engine = core::IApplicationManager::instance();
            // SmartPtr<IPhysicsManager3> pPhysicsManager = engine->getPhysicsManager3();
            // SmartPtr<PhysxManager> physicsManager =
            // fb::static_pointer_cast<PhysxManager>(pPhysicsManager);

            // using namespace physx;

            ////mControllerManager =
            /// PxCreateControllerManager(physicsManager->getPhysics()->getFoundation());

            //// use ray casting to position the CCT on the terrain
            // PxScene& scene = *physicsManager->getScene();
            // PxRaycastHit hit;
            // scene.raycastSingle(PxVec3(0,100,0), PxVec3(0,-1,0), 500.0f,
            // PxSceneQueryFlags(0xffffffff), hit);
            ////mControllerInitialPosition = hit.impact + PxVec3(0.0f, 1.8f, 0.0f);

            //// create and fill in the descriptor for the capsule controller
            // physx::PxCapsuleControllerDesc cDesc;

            // cDesc.height				= 2.0f;
            // cDesc.radius				= 0.5f;
            // cDesc.material				= physicsManager->getDefaultMaterial();
            // cDesc.position				= PxExtendedVec3(mControllerInitialPosition.x,
            // mControllerInitialPosition.y, mControllerInitialPosition.z); cDesc.slopeLimit			=
            // 0.1f; cDesc.contactOffset			= 0.01f; cDesc.stepOffset			= 0.1f;
            // cDesc.invisibleWallHeight	= 0.0f;
            // cDesc.maxJumpHeight			= 2.0f;
            // cDesc.scaleCoeff            = 0.9f;
            // cDesc.climbingMode          = PxCapsuleClimbingMode::eEASY;
            ////cDesc.callback              = this;

            // mController = static_cast<PxCapsuleController*>(
            //	mControllerManager->createController(*physicsManager->getPhysics(),
            // physicsManager->getScene(), cDesc));

            //
        }

        void PhysxCharacterController::initialise( SmartPtr<scene::IDirector> objectTemplate,
                                                   SmartPtr<Properties> instanceProperties )
        {
        }

        void PhysxCharacterController::setPosition( const Vector3F &position )
        {
            mController->setPosition(
                physx::PxExtendedVec3( position.X(), position.Y(), position.Z() ) );
        }

        Vector3F PhysxCharacterController::getPosition() const
        {
            auto vec = mController->getPosition();
            return Vector3F( (real_Num)vec.x, (real_Num)vec.y, (real_Num)vec.z );
        }

        void PhysxCharacterController::setOrientation( const QuaternionF &orientation )
        {
        }

        QuaternionF PhysxCharacterController::getOrientation() const
        {
            return QuaternionF::identity();
        }

        f32 PhysxCharacterController::getMoveSpeed() const
        {
            return 0.f;
        }

        void PhysxCharacterController::setMoveSpeed( f32 moveSpeed )
        {
        }

        bool PhysxCharacterController::getJump() const
        {
            return false;
        }

        void PhysxCharacterController::setJump( bool jump )
        {
        }

        bool PhysxCharacterController::isGrounded() const
        {
            return true;
        }

        void PhysxCharacterController::_getObject( void **ppObject )
        {
        }

        void PhysxCharacterController::setWalkVector( const Vector3F &vector )
        {
            m_moveVector = vector;
        }

        void PhysxCharacterController::stop()
        {
            m_moveVector = Vector3F::zero();
        }

        real_Num PhysxCharacterController::getMass() const
        {
            return 0.f;
        }

        void PhysxCharacterController::setMass( real_Num mass )
        {
        }

        void PhysxCharacterController::setCollisionType( u32 type )
        {
        }

        u32 PhysxCharacterController::getCollisionType() const
        {
            return 0;
        }

        void PhysxCharacterController::setCollisionMask( u32 mask )
        {
        }

        u32 PhysxCharacterController::getCollisionMask() const
        {
            return 0;
        }

        void PhysxCharacterController::setEnabled( bool enabled )
        {
        }

        bool PhysxCharacterController::isEnabled() const
        {
            return true;
        }

        void *PhysxCharacterController::getUserData() const
        {
            return nullptr;
        }

        void PhysxCharacterController::setUserData( void *userData )
        {
        }

        bool PhysxCharacterController::getKinematicMode() const
        {
            return m_kinematicMode;
        }

        void PhysxCharacterController::setKinematicMode( bool kinematicMode )
        {
            m_kinematicMode = kinematicMode;
        }
    }  // end namespace physics
}  // end namespace fb
