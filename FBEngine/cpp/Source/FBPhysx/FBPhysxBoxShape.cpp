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

namespace fb::physics
{

    PhysxBoxShape::PhysxBoxShape()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto physicsManager = applicationManager->getPhysicsManager();
        FB_ASSERT( physicsManager );

        m_stateContext = stateManager->addStateObject();

        auto sceneNodeStateListener = factoryManager->make_ptr<BoxShapeStateListener>();
        sceneNodeStateListener->setOwner( this );
        m_stateListener = sceneNodeStateListener;
        m_stateContext->addStateListener( m_stateListener );

        auto state = factoryManager->make_ptr<BoxShapeState>();
        m_stateContext->addState( state );

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

            auto applicationManager = core::ApplicationManager::instance();
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

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                ISharedObject::ScopedLock lock( physicsManager );

                PhysxShape<IBoxShape3>::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto PhysxBoxShape::getExtents() const -> Vector3<real_Num>
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<BoxShapeState>() )
            {
                return state->getExtents();
            }
        }

        return Vector3<real_Num>::zero();
    }

    void PhysxBoxShape::setExtents( const Vector3<real_Num> &extents )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<BoxShapeState>() )
            {
                state->setExtents( extents );
            }
        }
    }

    void PhysxBoxShape::setAABB( const AABB3<real_Num> &box )
    {
        auto extents = box.getExtent();
        setExtents( extents );
    }

    auto PhysxBoxShape::getAABB() const -> AABB3<real_Num>
    {
        return AABB3<real_Num>( m_extents * static_cast<real_Num>( -0.5 ),
                                m_extents * static_cast<real_Num>( 0.5 ) );
    }

    auto PhysxBoxShape::isValid() const -> bool
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
        auto applicationManager = core::ApplicationManager::instance();
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

        auto physics = physicsManager->getPhysics();
        FB_ASSERT( physics );

        auto halfExtents = getExtents() / static_cast<real_Num>( 2.0 );

        physx::PxVec3 dimensions( ( halfExtents.X() ), ( halfExtents.Y() ), ( halfExtents.Z() ) );
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

        //u32 GROUP_MASK = std::numeric_limits<u32>::max();
        //u32 COLLISION_MASK = std::numeric_limits<u32>::max();

        //physx::PxFilterData filterData;
        //filterData.word0 = GROUP_MASK;
        //filterData.word1 = COLLISION_MASK;

        //if( shape )
        //{
        //    shape->setSimulationFilterData( filterData );
        //}
    }

    auto PhysxBoxShape::createGeometry() -> physx::PxBoxGeometry
    {
        auto extents = getExtents() / static_cast<real_Num>( 2.0 );
        auto dimensions = physx::PxVec3( extents.X(), extents.Y(), extents.Z() );
        return { dimensions };
    }

    PhysxBoxShape::BoxShapeStateListener::~BoxShapeStateListener() = default;

    void PhysxBoxShape::BoxShapeStateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        if( auto owner = getOwner() )
        {
            if( owner->isLoaded() )
            {
                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                ISharedObject::ScopedLock lock( physicsManager );

                if( auto boxShapeState = fb::static_pointer_cast<BoxShapeState>( state ) )
                {
                    if( auto shape = owner->getShape() )
                    {
                        auto actor = owner->getPxActor();
                        if( actor )
                        {
                            actor->detachShape( *shape );
                        }

                        auto transform = owner->getLocalPose();
                        auto extents = boxShapeState->getExtents() / static_cast<real_Num>( 2.0 );
                        auto dimensions = PhysxUtil::toPx( extents * transform.getScale() );

                        auto geometry = physx::PxBoxGeometry( dimensions );
                        shape->setGeometry( geometry );

                        physx::PxTransform localPose = PhysxUtil::toPx( transform );
                        shape->setLocalPose( localPose );

                        if( actor )
                        {
                            actor->attachShape( *shape );
                        }
                    }

                    state->setDirty( false );
                }
            }
        }
    }

    void PhysxBoxShape::BoxShapeStateListener::handleStateChanged(
        const SmartPtr<IStateMessage> &message )
    {
        if( auto owner = getOwner() )
        {
            auto type = message->getType();
            if( type == CREATE_SHAPE_HASH )
            {
                owner->createShape();
            }
        }
    }

    void PhysxBoxShape::BoxShapeStateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto PhysxBoxShape::BoxShapeStateListener::getOwner() const -> SmartPtr<PhysxBoxShape>
    {
        return m_owner;
    }

    void PhysxBoxShape::BoxShapeStateListener::setOwner( SmartPtr<PhysxBoxShape> owner )
    {
        m_owner = owner;
    }
}  // namespace fb::physics
