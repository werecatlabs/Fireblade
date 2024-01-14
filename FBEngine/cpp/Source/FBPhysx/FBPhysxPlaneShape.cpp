#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxPlaneShape.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBPhysx/FBPhysxRigidDynamic.h>
#include <FBPhysx/FBPhysxRigidStatic.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>

namespace fb::physics
{

    void PhysxPlaneShape::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto physicsManager =
                fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );
            FB_ASSERT( physicsManager );

            auto physics = physicsManager->getPhysics();
            FB_ASSERT( physics );

            ISharedObject::ScopedLock lock( physicsManager );

            createShape();

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void PhysxPlaneShape::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = applicationManager->getStateManager();

            auto physicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsManager );

            ISharedObject::ScopedLock lock( physicsManager );

            if( auto stateContext = getStateContext() )
            {
                if( m_stateListener )
                {
                    stateContext->removeStateListener( m_stateListener );
                }

                stateManager->removeStateObject( m_stateContext );
                setStateContext( nullptr );
            }

            if( m_stateListener )
            {
                m_stateListener->unload( nullptr );
                m_stateListener = nullptr;
            }

            if( auto shape = getShape() )
            {
                if( m_pxActor )
                {
                    m_pxActor->detachShape( *shape, false );
                }
            }

            setShape( nullptr );

            PhysxShape<IPlaneShape3>::unload( data );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto PhysxPlaneShape::getExtents() const -> Vector3<real_Num>
    {
        return m_extents;
    }

    void PhysxPlaneShape::setExtents( const Vector3<real_Num> &extents )
    {
        m_extents = extents;
    }

    void PhysxPlaneShape::setAABB( const AABB3<real_Num> &box )
    {
        m_extents = box.getExtent();
    }

    auto PhysxPlaneShape::getAABB() const -> AABB3<real_Num>
    {
        AABB3<real_Num> box;
        return box;
    }

    auto PhysxPlaneShape::getDistance() const -> real_Num
    {
        return m_plane.getDistance();
    }

    void PhysxPlaneShape::setDistance( real_Num distance )
    {
        m_plane.setDistance( distance );
    }

    auto PhysxPlaneShape::getNormal() const -> Vector3<real_Num>
    {
        return m_plane.getNormal();
    }

    void PhysxPlaneShape::setNormal( const Vector3<real_Num> &normal )
    {
        m_plane.setNormal( normal );
    }

    auto PhysxPlaneShape::getPlane() const -> Plane3<real_Num>
    {
        return m_plane;
    }

    void PhysxPlaneShape::createShape()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto physicsManager =
            fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );
        FB_ASSERT( physicsManager );

        ScopedLock lock( physicsManager );

        auto physics = physicsManager->getPhysics();
        FB_ASSERT( physics );

        auto n = m_plane.getNormal();
        auto d = m_plane.getDistance();

        physx::PxPlane testPlane;
        testPlane.n = physx::PxVec3( n.X(), n.Y(), n.Z() );
        testPlane.d = 0.0;  // d;
        physx::PxTransform localTransform = PxTransformFromPlaneEquation( testPlane );

        auto material = getMaterial();
        auto physicsMaterial = fb::static_pointer_cast<PhysxMaterial>( material );
        FB_ASSERT( physicsMaterial );

        auto m = physicsMaterial->getMaterial();

        auto planeShape = physics->createShape( physx::PxPlaneGeometry(), *m );
        planeShape->setLocalPose( localTransform );

        if( !isTrigger() )
        {
            auto attachedBody = getActor();

            physx::PxFilterData filterData;
            physx::PxU32 filterGroup = attachedBody->getCollisionType();
            physx::PxU32 filterMask = attachedBody->getCollisionMask();
            filterData.word0 = filterGroup;
            filterData.word1 = filterMask;

            physx::PxFilterData queryFilterData;
            // FourWheeledVehicle::SetupDrivableShapeQueryFilterData(&queryFilterData);

            planeShape->setSimulationFilterData( filterData );
            planeShape->setQueryFilterData( queryFilterData );
        }
    }

    void PhysxPlaneShape::destroyShape()
    {
        if( auto shape = getShape() )
        {
            if( m_pxActor )
            {
                m_pxActor->detachShape( *shape, false );
            }
        }
    }

}  // namespace fb::physics
