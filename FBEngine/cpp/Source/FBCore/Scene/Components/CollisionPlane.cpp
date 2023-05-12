#include <FBCore/FBCorePCH.h>
#include <FBCore/Scene/Components/CollisionPlane.h>
#include <FBCore/Scene/Components/Rigidbody.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, CollisionPlane, Collision );

        CollisionPlane::CollisionPlane()
        {
        }

        CollisionPlane::~CollisionPlane()
        {
            unload( nullptr );
        }

        void CollisionPlane::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                Collision::load( data );

                auto applicationManager = core::IApplicationManager::instance();
                auto physicsManager = applicationManager->getPhysicsManager();
                m_material = physicsManager->createMaterial();

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CollisionPlane::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    Collision::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CollisionPlane::createPlane( const data::collider &collider )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                auto actor = getActor();
                FB_ASSERT( actor );

                auto pPlaneCollider = (data::plane_collider *)&collider;

                auto rigidbody = actor->getComponent<Rigidbody>();
                FB_ASSERT( rigidbody );

                auto material = getMaterial();
                // setupMaterial( collider );

                auto n = Vector3<real_Num>( pPlaneCollider->normal.x, pPlaneCollider->normal.y,
                                            pPlaneCollider->normal.z );
                auto d = -pPlaneCollider->distance;

                auto planeShape = physicsManager->createCollisionShape<physics::IPlaneShape3>();
                planeShape->setNormal( n );
                planeShape->setDistance( d );

                auto plane = planeShape->getPlane();
                auto localTransform = MathUtil<real_Num>::transformFromPlaneEquation( plane );

                auto rigidStatic = rigidbody->getRigidStatic();
                if( rigidStatic )
                {
                    rigidStatic->addShape( planeShape );
                }

                // actor->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);
                // actor->userData = this;

                // if (!collider.isTrigger)
                //{
                //	physx::PxFilterData filterData;
                //	physx::PxU32 filterGroup = getGroupMask();
                //	physx::PxU32 filterMask = getCollisionMask();
                //	filterData.word0 = filterGroup;
                //	filterData.word1 = filterMask;

                //	physx::PxFilterData queryFilterData;
                //	FourWheeledVehicle::SetupDrivableShapeQueryFilterData(&queryFilterData);

                //	planeShape->setSimulationFilterData(filterData);
                //	planeShape->setQueryFilterData(queryFilterData);
                //}

                // externalCollision->setShape(planeShape);
                // externalCollision->setTrigger(collider.isTrigger);

                // m_collisionObjects.push_back(externalCollision);
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
    }  // namespace scene
}  // end namespace fb
