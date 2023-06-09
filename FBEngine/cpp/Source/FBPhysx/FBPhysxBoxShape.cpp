#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxBoxShape.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBPhysx/PhysxUtil.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBPhysx/FBPhysxRigidDynamic.h>
#include <FBPhysx/FBPhysxRigidStatic.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>

namespace fb
{
    namespace physics
    {

        PhysxBoxShape::PhysxBoxShape()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();
            FB_ASSERT( stateManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto physicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsManager );

            m_stateObject = stateManager->addStateObject();

            auto sceneNodeStateListener = factoryManager->make_ptr<BoxShapeStateListener>();
            sceneNodeStateListener->setOwner( this );
            m_stateListener = sceneNodeStateListener;
            m_stateObject->addStateListener( m_stateListener );

            auto state = factoryManager->make_ptr<BoxShapeState>();
            m_stateObject->setState( state );

            auto physicsTask = physicsManager->getPhysicsTask();
            state->setTaskId( physicsTask );
        }

        PhysxBoxShape::~PhysxBoxShape()
        {
            unload( nullptr );
        }

        void PhysxBoxShape::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                ISharedObject::ScopedLock lock( physicsManager );

                if( auto body = getActor() )
                {
                    if( body->isExactly<PhysxRigidDynamic>() )
                    {
                        auto rigidDynamic = fb::static_pointer_cast<PhysxRigidDynamic>( body );

                        auto pRigidDynamic = rigidDynamic->getActor();
                        setPxActor( pRigidDynamic );
                    }

                    if( body->isExactly<PhysxRigidStatic>() )
                    {
                        auto rigidStatic = fb::static_pointer_cast<PhysxRigidStatic>( body );

                        auto pRigidStatic = rigidStatic->getRigidStatic();
                        setPxActor( pRigidStatic );
                    }
                }

                createShape();

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PhysxBoxShape::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto stateManager = applicationManager->getStateManager();
                    FB_ASSERT( stateManager );

                    auto physicsManager = applicationManager->getPhysicsManager();
                    FB_ASSERT( physicsManager );

                    ISharedObject::ScopedLock lock( physicsManager );

                    if( auto stateObject = getStateObject() )
                    {
                        if( auto state = stateObject->getState() )
                        {
                            if( stateObject )
                            {
                                stateObject->setState( nullptr );
                            }

                            state->unload( nullptr );
                            state = nullptr;
                        }

                        if( m_stateListener )
                        {
                            stateObject->removeStateListener( m_stateListener );
                        }

                        stateManager->removeStateObject( m_stateObject );
                        setStateObject( nullptr );
                    }

                    if( m_stateListener )
                    {
                        m_stateListener->unload( nullptr );
                        m_stateListener = nullptr;
                    }

                    auto shape = getShape();
                    if( shape )
                    {
                        if( m_pxActor )
                        {
                            m_pxActor->detachShape( *shape, false );
                        }
                    }

                    setShape( nullptr );

                    PhysxShape<IBoxShape3>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Vector3<real_Num> PhysxBoxShape::getExtents() const
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto state = stateObject->getStateByType<BoxShapeState>() )
                {
                    return state->getExtents();
                }
            }

            return Vector3<real_Num>::zero();
        }

        void PhysxBoxShape::setExtents( const Vector3<real_Num> &extents )
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto state = stateObject->getStateByType<BoxShapeState>() )
                {
                    state->setExtents( extents );
                }
            }

            //auto applicationManager = core::IApplicationManager::instance();
            //auto physicsManager = applicationManager->getPhysicsManager();

            //auto task = Thread::getCurrentTask();
            //auto physicsTask = physicsManager->getPhysicsTask();

            //const auto &loadingState = getLoadingState();

            //if( loadingState == LoadingState::Loaded && task == physicsTask )
            //{
            //    auto shape = getShape();
            //    if( shape )
            //    {
            //        auto halfExtents = extents / static_cast<real_Num>( 2.0 );
            //        physx::PxVec3 dimensions( ( halfExtents.X() ), ( halfExtents.Y() ),
            //                                  ( halfExtents.Z() ) );
            //        physx::PxBoxGeometry geometry( dimensions );
            //        shape->setGeometry( geometry );
            //    }

            //    m_state->setDirty( false );
            //}
        }

        void PhysxBoxShape::setAABB( const AABB3<real_Num> &box )
        {
            m_extents = box.getExtent();
        }

        AABB3<real_Num> PhysxBoxShape::getAABB() const
        {
            return AABB3<real_Num>( m_extents * static_cast<real_Num>( -0.5 ),
                                    m_extents * static_cast<real_Num>( 0.5 ) );
        }

        bool PhysxBoxShape::isValid() const
        {
            auto valid = false;

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                if( auto shape = getShape() )
                {
                    valid = true;
                }
            }
            else if( loadingState == LoadingState::Unloaded )
            {
                auto shape = getShape();
                if( !shape )
                {
                    valid = true;
                }
            }

            return valid;
        }

        void PhysxBoxShape::createShape()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager =
                fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );
            FB_ASSERT( physicsManager );

            ScopedLock lock( physicsManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto task = Thread::getCurrentTask();
            auto physicsTask = physicsManager->getPhysicsTask();

            const auto &loadingState = getLoadingState();

            //if( ( loadingState == LoadingState::Loading || loadingState == LoadingState::Loaded ) &&
            //    task == physicsTask )
            //{
            auto physics = physicsManager->getPhysics();
            FB_ASSERT( physics );

            auto extents = getExtents() / static_cast<real_Num>( 2.0 );

            physx::PxVec3 dimensions( ( extents.X() ), ( extents.Y() ), ( extents.Z() ) );
            physx::PxBoxGeometry geometry( dimensions );
            physx::PxTransform localPose = PhysxUtil::toPx( getLocalPose() );

            auto material = getMaterial();
            FB_ASSERT( material );

            auto physicsMaterial = fb::static_pointer_cast<PhysxMaterial>( material );
            FB_ASSERT( physicsMaterial );

            auto m = physicsMaterial->getMaterial();

            physx::PxShape *shape = physics->createShape( geometry, *m );
            setShape( shape );

            if( shape )
            {
                shape->setLocalPose( localPose );
            }

            // shape->setSimulationFilterData(simFilterData);
            // shape->setQueryFilterData(qryFilterData);
            //}
            //else
            //{
            //    auto stateMessage = factoryManager->make_ptr<StateMessage>();
            //    stateMessage->setType( CREATE_SHAPE_HASH );

            //    if( auto stateContext = getStateObject() )
            //    {
            //        stateContext->addMessage( physicsTask, stateMessage );
            //    }
            //}
        }

        physx::PxBoxGeometry PhysxBoxShape::createGeometry()
        {
            auto extents = getExtents() / static_cast<real_Num>( 2.0 );
            auto dimensions = physx::PxVec3( extents.X(), extents.Y(), extents.Z() );
            return physx::PxBoxGeometry( dimensions );
        }

        PhysxBoxShape::BoxShapeStateListener::~BoxShapeStateListener()
        {
        }

        void PhysxBoxShape::BoxShapeStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            if( auto boxShapeState = fb::static_pointer_cast<BoxShapeState>( state ) )
            {
                if( auto shape = m_owner->getShape() )
                {
                    auto extents = boxShapeState->getExtents() / static_cast<real_Num>( 2.0 );
                    auto dimensions = PhysxUtil::toPx( extents );

                    auto geometry = physx::PxBoxGeometry( dimensions );
                    shape->setGeometry( geometry );
                }

                state->setDirty( false );
            }
        }

        void PhysxBoxShape::BoxShapeStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            auto owner = getOwner();
            auto type = message->getType();
            if( type == CREATE_SHAPE_HASH )
            {
                owner->createShape();
            }
        }

        void PhysxBoxShape::BoxShapeStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        PhysxBoxShape *PhysxBoxShape::BoxShapeStateListener::getOwner() const
        {
            return m_owner;
        }

        void PhysxBoxShape::BoxShapeStateListener::setOwner( PhysxBoxShape *owner )
        {
            m_owner = owner;
        }
    }  // namespace physics
}  // end namespace fb
