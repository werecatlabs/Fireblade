#include <FBPhysx/FBPhysxPCH.h>
#include "FBPhysx/FBPhysxPlaneShape.h"
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBPhysx/FBPhysxRigidDynamic.h>
#include <FBPhysx/FBPhysxRigidStatic.h>
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>

namespace fb
{
    namespace physics
    {
        void PhysxPlaneShape::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                ISharedObject::ScopedLock lock( physicsManager );

                if( m_body->isExactly<PhysxRigidDynamic>() )
                {
                    auto rigidDynamic = fb::static_pointer_cast<PhysxRigidDynamic>( m_body );
                    auto pRigidDynamic = rigidDynamic->getActor();
                    createShape( pRigidDynamic );
                }

                if( m_body->isExactly<PhysxRigidStatic>() )
                {
                    auto rigidStatic = fb::static_pointer_cast<PhysxRigidStatic>( m_body );
                    auto pRigidStatic = rigidStatic->getRigidStatic();
                    createShape( pRigidStatic );
                }

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

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                ISharedObject::ScopedLock lock( physicsManager );

                PhysxShape<IPlaneShape3>::unload(data);

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        Vector3<real_Num> PhysxPlaneShape::getExtents() const
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

        AABB3<real_Num> PhysxPlaneShape::getAABB() const
        {
            AABB3<real_Num> box;
            return box;
        }

        real_Num PhysxPlaneShape::getDistance() const
        {
            return m_plane.getDistance();
        }

        void PhysxPlaneShape::setDistance( real_Num distance )
        {
            m_plane.setDistance( distance );
        }

        Vector3<real_Num> PhysxPlaneShape::getNormal() const
        {
            return m_plane.getNormal();
        }

        void PhysxPlaneShape::setNormal( const Vector3<real_Num> &normal )
        {
            m_plane.setNormal( normal );
        }

        Plane3<real_Num> PhysxPlaneShape::getPlane() const
        {
            return m_plane;
        }

        void PhysxPlaneShape::createShape( void *actor )
        {
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

            auto pActor = static_cast<physx::PxRigidActor *>( actor );
            auto planeShape = pActor->createShape( physx::PxPlaneGeometry(), *m );
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

            pActor->setActorFlag( physx::PxActorFlag::eVISUALIZATION, true );
        }

        void PhysxPlaneShape::destroyShape( void *actor )
        {
            if( auto shape = getShape() )
            {
                auto pActor = static_cast<physx::PxRigidActor *>( actor );
                pActor->detachShape( *shape );
                shape->release();
            }
        }
    }  // end namespace physics
}  // end namespace fb
