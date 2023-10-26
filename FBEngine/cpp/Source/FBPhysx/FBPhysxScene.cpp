#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxScene.h>
#include <FBPhysx/FBPhysxRigidDynamic.h>
#include <FBPhysx/FBPhysxRigidStatic.h>
#include <FBPhysx/PhysxUtil.h>
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>
#include <FBPhysx/FBPhysxThread.h>

#include "FBPhysx/FBPhysxManager.h"

namespace fb
{
    namespace physics
    {
        using namespace physx;

        PhysxScene::PhysxScene()
        {
        }

        PhysxScene::~PhysxScene()
        {
            unload( nullptr );
        }

        void PhysxScene::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                using namespace physx;

                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto physicsManager =
                    fb::static_pointer_cast<PhysxManager>( applicationManager->getPhysicsManager() );
                auto physics = physicsManager->getPhysics();

                auto gravity = Vector3<real_Num>( 0.0f, -9.81f, 0.0f );
                // properties->getPropertyAsVector3D("gravity",
                // Vector3<real_Num>(0.0f, -9.8f, 0.0f));

                PxSceneDesc sceneDesc( physics->getTolerancesScale() );
                sceneDesc.gravity = PxVec3( gravity.X(), gravity.Y(), gravity.Z() );
                sceneDesc.broadPhaseType = PxBroadPhaseType::eMBP;
                sceneDesc.frictionOffsetThreshold = 0.05f;
                sceneDesc.maxNbContactDataBlocks = 1024;
                sceneDesc.ccdMaxPasses = 0;
                sceneDesc.solverBatchSize = 2;
                sceneDesc.dynamicStructure = PxPruningStructure::Enum::eNONE;
                sceneDesc.staticStructure = PxPruningStructure::Enum::eDYNAMIC_AABB_TREE;
                sceneDesc.dynamicTreeRebuildRateHint = 1000;

                PxSceneLimits limits;
                limits.maxNbActors = 256;
                limits.maxNbBodies = 256;
                limits.maxNbStaticShapes = 256;
                limits.maxNbDynamicShapes = 256;
                limits.maxNbAggregates = 256;
                limits.maxNbConstraints = 256;
                limits.maxNbRegions = 256;
                limits.maxNbObjectsPerRegion = 256;

                sceneDesc.limits = limits;

                int numThreads = Thread::physical_concurrency();
                if( numThreads <= 4 )
                {
                    numThreads = 0;
                }

                if( numThreads > 4 )
                {
                    numThreads = 4;
                }

                numThreads = 0;

                m_cpuDispatcher = PxDefaultCpuDispatcherCreate( numThreads );
                if( !m_cpuDispatcher )
                {
                    FB_LOG_ERROR( "PxDefaultCpuDispatcherCreate failed!" );
                }

                sceneDesc.cpuDispatcher = m_cpuDispatcher;

                if( !sceneDesc.filterShader )
                {
                    //sceneDesc.filterShader = PxDefaultSimulationFilterShader;
                    sceneDesc.filterShader = PhysxScene::simulationFilterShader;
                }

                sceneDesc.frictionType = PxFrictionType::ePATCH;

                m_collisionCallback = new CollisionCallback;
                sceneDesc.filterCallback = m_collisionCallback;

                m_simulationEventCallback = new SimulationEventCallback;
                sceneDesc.simulationEventCallback = m_simulationEventCallback;

                m_contactModificationCallback = new ContactModificationCallback;
                sceneDesc.contactModifyCallback = m_contactModificationCallback;

                sceneDesc.flags = PxSceneFlag::eENABLE_PCM;
                sceneDesc.frictionOffsetThreshold = 0.05f;

                sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;

#ifdef PX_WINDOWS
                // if(!sceneDesc.gpuDispatcher && mCudaContextManager)
                // {
                //	sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
                // }
#endif

                // sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
                // sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
                // sceneDesc.flags |= physx::PxSceneFlag::eADAPTIVE_FORCE;
                sceneDesc.flags |= PxSceneFlag::eENABLE_AVERAGE_POINT;

                //#if !FB_FINAL
                //		sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;
                //#endif

                sceneDesc.broadPhaseType = PxBroadPhaseType::eSAP;

                auto pScene = physics->createScene( sceneDesc );
                setScene( pScene );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PhysxScene::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    if( auto scene = getScene() )
                    {
                        auto numActors =
                            scene->getNbActors( physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC );
                        FB_ASSERT( numActors == 0 );

                        auto numStaticActors =
                            scene->getNbActors( physx::PxActorTypeSelectionFlag::eRIGID_STATIC );
                        FB_ASSERT( numStaticActors == 0 );
                    }

                    clear();

                    if( auto scene = getScene() )
                    {
                        auto numActors =
                            scene->getNbActors( physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC );
                        FB_ASSERT( numActors == 0 );

                        auto numStaticActors =
                            scene->getNbActors( physx::PxActorTypeSelectionFlag::eRIGID_STATIC );
                        FB_ASSERT( numStaticActors == 0 );

                        scene->release();
                        setScene( nullptr );
                    }

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PhysxScene::preUpdate()
        {
        }

        void PhysxScene::update()
        {
            if( auto scene = getScene() )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto timer = applicationManager->getTimer();
                FB_ASSERT( timer );

                auto t = timer->getTime();
                auto dt = timer->getDeltaTime();

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                FB_PXSCENE_WRITE_LOCK( scene );

                FB_ASSERT( Math<time_interval>::isFinite( t ) );
                FB_ASSERT( Math<time_interval>::isFinite( dt ) );
                FB_ASSERT( dt > 0.0 );

                const auto maxDT = 1.0 / 50.0;
                if( dt > maxDT )
                {
                    dt = maxDT;
                }

                if( applicationManager->isPlaying() )
                {
                    scene->simulate( static_cast<f32>( dt ) );
                    scene->fetchResults( true );

                    if( auto p = getRigidBodiesPtr() )
                    {
                        auto &rigidBodies = *p;
                        for( auto rigidBody : rigidBodies )
                        {
                            auto pRigid = fb::dynamic_pointer_cast<PhysxRigidDynamic>( rigidBody );
                            if( pRigid )
                            {
                                auto pxBody = pRigid->getActor();
                                auto pxTransform = pxBody->getGlobalPose();
                                auto transform = PhysxUtil::toFB( pxTransform );
                                pRigid->setActiveTransform( transform );
                            }
                        }
                    }
                }
                else
                {
#if FB_PHYSX_DEBUG
                    scene->simulate( 1.0 / 100000.0 );  //todo work around for visualization in debug
                    scene->fetchResults();
#endif
                }

                //auto numTransforms = static_cast<u32>( 0 );
                //auto transforms = scene->getActiveTransforms( numTransforms );
                //for( size_t i = 0; i < numTransforms; ++i )
                //{
                //    const auto &activeTransform = transforms[i];
                //    auto rigidBody = static_cast<PhysxRigidDynamic *>( activeTransform.userData );
                //    if( rigidBody )
                //    {
                //        auto &pxTransform = activeTransform.actor2World;
                //        FB_ASSERT( pxTransform.isSane() );
                //        FB_ASSERT( pxTransform.isValid() );

                //        auto transform = PhysxUtil::toFB( pxTransform );
                //        FB_ASSERT( transform.isSane() );
                //        FB_ASSERT( transform.isValid() );

                //        rigidBody->setActiveTransform( transform );
                //    }
                //}
            }
        }

        void PhysxScene::postUpdate()
        {
            if( auto scene = getScene() )
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto timer = applicationManager->getTimer();
                FB_ASSERT( timer );

                auto t = timer->getTime();
                auto dt = timer->getDeltaTime();

                auto task = Thread::getCurrentTask();

                auto physicsManager = applicationManager->getPhysicsManager();
                FB_ASSERT( physicsManager );

                FB_PXSCENE_WRITE_LOCK( scene );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                // auto sceneState = factoryManager->make_ptr<PhysicsSceneState>();
                // sceneState->setTime(t);

                // auto count = 0;
                // for (auto& ridigBody : m_rigidBodies)
                //{
                //	auto& bodyState = sceneState->m_rigidBodies[count];

                //	auto transform = ridigBody->getTransform();
                //	FB_ASSERT(transform.isValid());

                //	bodyState.position = transform.getPosition();
                //	bodyState.orientation = transform.getOrientation();
                //	bodyState.linearVelocity = ridigBody->getLinearVelocity();
                //	bodyState.angularVelocity = ridigBody->getAngularVelocity();

                //	count++;
                //}

                // auto stateObject = getStateObject();
                // if (stateObject)
                //{
                //	stateObject->addOutputState(sceneState);
                // }
            }
        }

        void PhysxScene::clear()
        {
            if( auto scene = getScene() )
            {
                FB_PXSCENE_WRITE_LOCK( scene );

                if( auto p = getRigidBodiesPtr() )
                {
                    std::unique_lock lock( m_rigidBodiesMutex );

                    auto &rigidBodies = *p;
                    for( auto body : rigidBodies )
                    {
                        if( body )
                        {
                            body->unload( nullptr );
                        }
                    }

                    setRigidBodiesPtr( nullptr );
                }
            }
        }

        void PhysxScene::setSize( const Vector3F &size )
        {
            m_size = size;
        }

        Vector3F PhysxScene::getSize() const
        {
            return m_size;
        }

        bool PhysxScene::rayTest( const Vector3F &start, const Vector3F &direction, Vector3F &hitPos,
                                  Vector3F &hitNormal, u32 collisionType, u32 collisionMask )
        {
            return false;
        }

        bool PhysxScene::intersects( const Vector3F &start, const Vector3F &end, Vector3F &hitPos,
                                     Vector3F &hitNormal, SmartPtr<ISharedObject> &object,
                                     u32 collisionType, u32 collisionMask )
        {
            return false;
        }

        physx::PxScene *PhysxScene::getScene() const
        {
            return m_scene.load();
        }

        void PhysxScene::setScene( physx::PxScene *scene )
        {
            m_scene = scene;
        }

        void PhysxScene::setGravity( const Vector3<real_Num> &vec )
        {
            if( auto scene = getScene() )
            {
                auto pxGravity = physx::PxVec3( vec.X(), vec.Y(), vec.Z() );
                scene->setGravity( pxGravity );
            }
        }

        Vector3<real_Num> PhysxScene::getGravity() const
        {
            return Vector3<real_Num>::zero();
        }

        void PhysxScene::simulate( real_Num elapsedTime, void *scratchMemBlock, u32 scratchMemBlockSize,
                                   bool controlSimulation )
        {
            if( auto scene = getScene() )
            {
                scene->simulate( elapsedTime, nullptr, scratchMemBlock, scratchMemBlockSize,
                                 controlSimulation );
            }
        }

        bool PhysxScene::fetchResults( bool block, u32 *errorState )
        {
            if( auto scene = getScene() )
            {
                return scene->fetchResults( block, errorState );
            }

            return false;
        }

        void PhysxScene::addActor( SmartPtr<IPhysicsBody3> body )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto physicsManager = applicationManager->getPhysicsManager();

                if( auto scene = getScene() )
                {
                    FB_PXSCENE_WRITE_LOCK( scene );

                    FB_ASSERT( body );

                    if( body )
                    {
                        addRigidBodiesPtr( body );

                        auto pRigidbody = fb::dynamic_pointer_cast<PhysxRigidDynamic>( body );
                        if( pRigidbody )
                        {
                            if( !pRigidbody->isLoaded() )
                            {
                                pRigidbody->load( nullptr );
                            }

                            if( auto actor = pRigidbody->getActor() )
                            {
                                //FB_ASSERT( actor->getNbShapes() > 0 );

                                auto currentScene = actor->getScene();
                                if( currentScene == nullptr )
                                {
                                    scene->addActor( *actor );
                                }
                            }
                        }

                        auto pRigidStatic = fb::dynamic_pointer_cast<PhysxRigidStatic>( body );
                        if( pRigidStatic )
                        {
                            if( !pRigidStatic->isLoaded() )
                            {
                                pRigidStatic->load( nullptr );
                            }

                            if( auto actor = pRigidStatic->getRigidStatic() )
                            {
                                //FB_ASSERT( actor->getNbShapes() > 0 );

                                auto currentScene = actor->getScene();
                                if( currentScene == nullptr )
                                {
                                    scene->addActor( *actor );
                                }
                            }
                        }
                    }
                }

                body->setScene( this );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PhysxScene::removeActor( SmartPtr<IPhysicsBody3> body )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto physicsManager = applicationManager->getPhysicsManager();
                FB_PXSCENE_WRITE_LOCK( getScene() );

                if( body )
                {
                    removeRigidBodiesPtr( body );

                    auto pRigidbody = fb::dynamic_pointer_cast<PhysxRigidDynamic>( body );
                    if( pRigidbody )
                    {
                        auto actor = pRigidbody->getActor();
                        if( actor )
                        {
                            if( auto scene = getScene() )
                            {
                                scene->removeActor( *actor );
                            }
                        }
                    }

                    auto pRigidStatic = fb::dynamic_pointer_cast<PhysxRigidStatic>( body );
                    if( pRigidStatic )
                    {
                        auto actor = pRigidStatic->getRigidStatic();
                        if( actor )
                        {
                            if( auto scene = getScene() )
                            {
                                scene->removeActor( *actor );
                            }
                        }
                    }
                }

                body->setScene( nullptr );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool PhysxScene::castRay( const Vector3<real_Num> &origin, const Vector3<real_Num> &dir,
                                  Array<SmartPtr<IRaycastHit>> &hits )
        {
            using namespace physx;

            auto rorigin = PxVec3( origin.X(), origin.Y(), origin.Z() );  // [in] Ray origin
            auto unitDir = PxVec3( dir.X(), dir.Y(), dir.Z() );  // [in] Normalized ray direction
            PxReal maxDistance = 100000;  //-vec.y;       // [in] Raycast max distance

            RaycastCallback raycastCallback;

            PxHitFlags hitFlags( PxHitFlag::eDEFAULT );
            PxQueryFilterData pxQueryFilterData;

            if( auto scene = getScene() )
            {
                bool status = scene->raycast( rorigin, unitDir, maxDistance, raycastCallback, hitFlags,
                                              pxQueryFilterData );
                if( status )
                {
                    float distance = raycastCallback.m_hit.distance >= fabs( 100000.0f )
                                         ? 0.0f
                                         : raycastCallback.m_hit.distance;

                    auto hitPos = origin + ( dir * distance );

                    auto hit = fb::make_ptr<RaycastHit>();
                    hit->setDistance( distance );
                    hit->setPoint( hitPos );
                    hits.push_back( hit );

                    return true;
                }
            }

            return false;
        }

        bool PhysxScene::castRay( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit )
        {
            using namespace physx;

            FB_ASSERT( MathUtil<real_Num>::isFinite( ray.getOrigin() ) );
            FB_ASSERT( MathUtil<real_Num>::isFinite( ray.getDirection() ) );

            auto applicationManager = core::IApplicationManager::instance();
            auto physicsManager = applicationManager->getPhysicsManager();
            FB_PXSCENE_WRITE_LOCK( getScene() );

            const Vector3<real_Num> &origin = ray.getOrigin();
            const Vector3<real_Num> &dir = ray.getDirection();

            auto rorigin = PxVec3( origin.X(), origin.Y(), origin.Z() );  // [in] Ray origin
            auto unitDir = PxVec3( dir.X(), dir.Y(), dir.Z() );  // [in] Normalized ray direction
            const f32 maxDistance = 100000.0f;  //-vec.y;       // [in] Raycast max distance

            // const PxSceneQueryFlags outputFlags = PxSceneQueryFlag::eDISTANCE |
            // PxSceneQueryFlag::eIMPACT | PxSceneQueryFlag::eNORMAL; const PxSceneQueryFlags outputFlags
            // = PxSceneQueryFlag::eDISTANCE;

            // PxSceneQueryFilterData filterData(PxSceneQueryFilterFlag::eSTATIC);
            // filterData.flags |= PxQueryFlag::eANY_HIT;

            PxHitFlags hitFlags( PxHitFlag::eDEFAULT );
            PxQueryFilterData pxQueryFilterData( PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC );

            if( auto scene = getScene() )
            {
                RaycastCallback raycastCallback;
                bool status = scene->raycast( rorigin, unitDir, maxDistance, raycastCallback, hitFlags,
                                              pxQueryFilterData );
                if( status )
                {
                    auto d = (real_Num)raycastCallback.m_hit.distance;
                    if( d < maxDistance )
                    {
                        const auto &n = raycastCallback.m_hit.normal;

                        hit->setDistance( d );
                        hit->setPoint( origin + ( dir * d ) );
                        hit->setNormal( Vector3<real_Num>( n.x, n.y, n.z ));

                        FB_ASSERT( MathF::isFinite( d ) );
                        FB_ASSERT( MathUtil<real_Num>::isFinite( hit->getPoint() ) );
                        FB_ASSERT( MathUtil<real_Num>::isFinite( hit->getNormal() ) );

                        return true;
                    }
                }
            }

            return false;
        }

        SmartPtr<IStateContext> PhysxScene::getStateObject() const
        {
            return m_stateObject;
        }

        void PhysxScene::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        SharedPtr<Array<SmartPtr<IRigidBody3>>> PhysxScene::getRigidBodiesPtr() const
        {
            return m_rigidBodies;
        }

        void PhysxScene::setRigidBodiesPtr( SharedPtr<Array<SmartPtr<IRigidBody3>>> ptr )
        {
            m_rigidBodies = ptr;
        }

        void PhysxScene::addRigidBodiesPtr( SmartPtr<IRigidBody3> body )
        {
            auto p = getRigidBodiesPtr();
            if( !p )
            {
                p = fb::make_shared<Array<SmartPtr<IRigidBody3>>>();
                setRigidBodiesPtr( p );
            }

            if( p )
            {
                std::unique_lock lock( m_rigidBodiesMutex );
                auto &rigidBodies = *p;
                rigidBodies.push_back( body );
            }
        }

        void PhysxScene::removeRigidBodiesPtr( SmartPtr<IRigidBody3> body )
        {
            if( auto p = getRigidBodiesPtr() )
            {
                std::unique_lock lock( m_rigidBodiesMutex );
                auto &rigidBodies = *p;
                auto it = std::find( rigidBodies.begin(), rigidBodies.end(), body );
                if( it != rigidBodies.end() )
                {
                    rigidBodies.erase( it );
                }
            }
        }

        bool PhysxScene::castRayDynamic( const Ray3<real_Num> &ray, SmartPtr<IRaycastHit> hit )
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto physicsManager = applicationManager->getPhysicsManager();
            FB_PXSCENE_READ_LOCK( getScene() );

            using namespace physx;

            auto origin = ray.getOrigin();
            auto dir = ray.getDirection();

            auto rorigin = PxVec3( origin.X(), origin.Y(), origin.Z() );  // [in] Ray origin
            auto unitDir = PxVec3( dir.X(), dir.Y(), dir.Z() );  // [in] Normalized ray direction
            PxReal maxDistance = 100000;  //-vec.y;       // [in] Raycast max distance

            // const physx::PxSceneQueryFlags outputFlags = physx::PxSceneQueryFlag::eDISTANCE |
            // physx::PxSceneQueryFlag::eIMPACT | physx::PxSceneQueryFlag::eNORMAL; const
            // physx::PxSceneQueryFlags outputFlags = physx::PxSceneQueryFlag::eDISTANCE;

            // physx::PxSceneQueryFilterData filterData(physx::PxSceneQueryFilterFlag::eSTATIC);
            // filterData.flags |= physx::PxQueryFlag::eANY_HIT;

            RaycastCallback raycastCallback;

            PxHitFlags hitFlags( PxHitFlag::eDEFAULT );
            PxQueryFilterData pxQueryFilterData( PxQueryFlag::eSTATIC | PxQueryFlag::eDYNAMIC );

            if( auto scene = getScene() )
            {
                bool status = scene->raycast( rorigin, unitDir, maxDistance, raycastCallback, hitFlags,
                                              pxQueryFilterData );
                if( status )
                {
                    float distance = raycastCallback.m_hit.distance >= fabs( 100000.0f )
                                         ? 0.0f
                                         : raycastCallback.m_hit.distance;

                    auto hitPos = origin + ( dir * distance );

                    hit->setDistance( distance );
                    hit->setPoint( hitPos );

                    return true;
                }
            }

            return false;
        }

        class QueryFilterCallback : public physx::PxQueryFilterCallback
        {
        public:
            QueryFilterCallback()
            {
            }
            ~QueryFilterCallback()
            {
            }

            virtual PxQueryHitType::Enum preFilter( const PxFilterData &filterData, const PxShape *shape,
                                                    const PxRigidActor *actor, PxHitFlags &queryFlags )
            {
                return PxQueryHitType::eBLOCK;
            }

            virtual PxQueryHitType::Enum postFilter( const PxFilterData &filterData,
                                                     const PxQueryHit &hit )
            {
                return PxQueryHitType::eBLOCK;
            }
        };

        physx::PxAgain PhysxScene::RaycastCallback::processTouches( const physx::PxRaycastHit *buffer,
                                                                    u32 nbHits )
        {
            using namespace physx;

            for( u32 i = 0; i < nbHits; ++i )
            {
                const PxRaycastHit &hit = buffer[i];
                if( hit.distance < m_closestHit )
                {
                    auto actor = hit.actor;
                    auto shape = hit.shape;

                    auto staticActor = actor->isRigidStatic();
                    auto dynamicActor = actor->isRigidDynamic();

                    if( m_checkStatic && staticActor )
                    {
                        // if( shape->getFlags().isSet( PxShapeFlag::eSIMULATION_SHAPE ) )
                        {
                            m_hit = hit;
                            m_closestHit = hit.distance;
                        }
                    }
                    else if( m_checkDynamic && dynamicActor )
                    {
                        // if( dynamicActor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC )
                        {
                            m_hit = hit;
                            m_closestHit = hit.distance;
                        }
                    }
                }
            }

            return false;
        }

        PhysxScene::RaycastCallback::RaycastCallback() :
            physx::PxRaycastCallback( buffer, 10 ),
            m_closestHit( static_cast<f32>( 1e10 ) )
        {
        }

        physx::PxFilterFlags PhysxScene::simulationFilterShader(
            PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
            PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
            PxPairFlags &pairFlags, const void *constantBlock, PxU32 constantBlockSize )
        {
            // let triggers through
            if( PxFilterObjectIsTrigger( attributes0 ) || PxFilterObjectIsTrigger( attributes1 ) )
            {
                pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
                return PxFilterFlags();
            }

            // if ((filterData0.word0 & filterData1.word1) == 0 &&
            //	(filterData1.word0 & filterData0.word1) == 0)
            //{
            //	return physx::PxFilterFlag::eSUPPRESS;
            // }

            pairFlags = PxPairFlag::eCONTACT_DEFAULT;
            return PxFilterFlag::eDEFAULT | PxFilterFlag::eNOTIFY;
        }

        PhysxScene::CollisionCallback::CollisionCallback()
        {
        }

        PhysxScene::CollisionCallback::~CollisionCallback()
        {
        }

        physx::PxFilterFlags PhysxScene::CollisionCallback::pairFound(
            PxU32 pairID, PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
            const PxActor *a0, const PxShape *s0, PxFilterObjectAttributes attributes1,
            physx::PxFilterData filterData1, const PxActor *a1, const PxShape *s1,
            PxPairFlags &pairFlags )
        {
            return PxFilterFlag::eDEFAULT | PxFilterFlag::eNOTIFY;
        }

        bool PhysxScene::CollisionCallback::statusChange( PxU32 &pairID, PxPairFlags &pairFlags,
                                                          PxFilterFlags &filterFlags )
        {
            pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND |
                        PxPairFlag::eNOTIFY_TOUCH_LOST;
            filterFlags = PxFilterFlag::eDEFAULT | PxFilterFlag::eNOTIFY;

            return false;
        }

        void PhysxScene::CollisionCallback::pairLost( PxU32 pairID, PxFilterObjectAttributes attributes0,
                                                      physx::PxFilterData filterData0,
                                                      PxFilterObjectAttributes attributes1,
                                                      physx::PxFilterData filterData1,
                                                      bool objectDeleted )
        {
        }

        void PhysxScene::SimulationEventCallback::onConstraintBreak( PxConstraintInfo *constraints,
                                                                     PxU32 count )
        {
            int stop = 0;
            stop = 0;
        }

        void PhysxScene::SimulationEventCallback::onSleep( PxActor **actors, PxU32 count )
        {
            int stop = 0;
            stop = 0;
        }

        void PhysxScene::SimulationEventCallback::onWake( PxActor **actors, PxU32 count )
        {
            int stop = 0;
            stop = 0;
        }

        void PhysxScene::SimulationEventCallback::onContact( const PxContactPairHeader &pairHeader,
                                                             const PxContactPair *pairs, PxU32 nbPairs )
        {
        }

        void PhysxScene::SimulationEventCallback::onTrigger( PxTriggerPair *pairs, PxU32 count )
        {
        }

        void PhysxScene::SimulationEventCallback::checkBreakage( PxRigidDynamic *rb0, PxShape *shape )
        {
            int stop = 0;
            stop = 0;
        }

        PhysxScene::SimulationEventCallback::~SimulationEventCallback()
        {
        }

        PhysxScene::SimulationEventCallback::SimulationEventCallback()
        {
        }

        void PhysxScene::ContactModificationCallback::onContactModify( PxContactModifyPair *const pairs,
                                                                       PxU32 count )
        {
        }

    }  // end namespace physics
}  // end namespace fb
