#include <FBPhysx/FBPhysxPCH.h>
#include <FBPhysx/FBPhysxManager.h>
#include <FBPhysx/FBPhysxAllocator.h>
#include <FBPhysx/FBPhysxPoolAllocator.h>
#include <FBPhysx/FBPhysxErrorOutput.h>
#include <FBPhysx/FBPhysxBoxShape.h>
#include <FBPhysx/FBPhysxPlaneShape.h>
#include <FBPhysx/FBPhysxCharacterController.h>
#include <FBPhysx/FBPhysxRigidDynamic.h>
#include <FBPhysx/FBPhysxRigidStatic.h>
#include <FBPhysx/FBPhysxTerrain.h>
#include <FBPhysx/FBPhysxMeshShape.h>
#include <FBPhysx/FBPhysxSphereShape.h>
#include <FBPhysx/FBPhysxScene.h>
#include <FBPhysx/FBPhysxConstraintD6.h>
#include <FBPhysx/FBPhysxConstraintFixed3.h>
#include <FBPhysx/FBPhysxCooker.h>
#include <FBPhysx/FBPhysxMaterial.h>
#include <FBPhysx/PhysxUtil.h>
#include <FBCore/FBCore.h>
#include <PxPhysicsAPI.h>
#include <PxExtensionsAPI.h>

#ifdef FB_PHYSX_DEBUG
#    include "extensions/PxVisualDebuggerExt.h"
#endif

namespace fb
{
    namespace physics
    {
        FB_CLASS_REGISTER_DERIVED( fb, PhysxManager, SharedObject<IPhysicsManager> );

        PxVehicleTelemetryData *mTelemetryData4W;

        SharedPtr<PxAllocatorCallback> PhysxManager::m_allocator;
        SharedPtr<PxErrorCallback> PhysxManager::m_errorOutput;

        ///////////////////////////////////////////////////////////////////////////////

        enum Word3
        {
            SWEPT_INTEGRATION_LINEAR = 1,
        };

        PxFilterFlags SampleVehicleFilterShader( PxFilterObjectAttributes attributes0,
                                                 PxFilterData filterData0,
                                                 PxFilterObjectAttributes attributes1,
                                                 PxFilterData filterData1, PxPairFlags &pairFlags,
                                                 const void *constantBlock, PxU32 constantBlockSize )
        {
            // let triggers through
            if( PxFilterObjectIsTrigger( attributes0 ) || PxFilterObjectIsTrigger( attributes1 ) )
            {
                pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
                return PxFilterFlags();
            }

            // use a group-based mechanism for all other pairs:
            // - Objects within the default group (mask 0) always collide
            // - By default, objects of the default group do not collide
            //   with any other group. If they should collide with another
            //   group then this can only be specified through the filter
            //   data of the default group objects (objects of a different
            //   group can not choose to do so)
            // - For objects that are not in the default group, a bitmask
            //   is used to define the groups they should collide with
            if( ( filterData0.word0 != 0 || filterData1.word0 != 0 ) &&
                !( filterData0.word0 & filterData1.word1 || filterData1.word0 & filterData0.word1 ) )
                return PxFilterFlag::eSUPPRESS;

            pairFlags = PxPairFlag::eCONTACT_DEFAULT;

            // enable CCD stuff -- for now just for everything or nothing.
            // if((filterData0.word3|filterData1.word3) & SWEPT_INTEGRATION_LINEAR)
            //	pairFlags |= PxPairFlag::eSWEPT_INTEGRATION_LINEAR;

            // The pairFlags for each object are stored in word2 of the filter data. Combine them.
            pairFlags |= PxPairFlags( static_cast<PxU16>( filterData0.word2 | filterData1.word2 ) );
            return PxFilterFlags();
        }

        PhysxManager::PhysxManager()
        {
            m_physics = nullptr;
            m_cooking = nullptr;
            m_defaultMaterial = nullptr;
        }

        PhysxManager::~PhysxManager()
        {
            unload( nullptr );
        }

        void PhysxManager::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                if( !m_allocator )
                {
                    m_allocator = fb::make_shared<PhysxAllocator>();

                    // Create a custom allocator callback object with a pool size of 512 MB
                    //m_allocator = fb::make_shared<PhysxPoolAllocator>( 512 * 1024 * 1024 );
                }

                if( !m_errorOutput )
                {
                    m_errorOutput = fb::make_shared<PhysxErrorOutput>();
                }

                m_foundation = PxCreateFoundation( PX_PHYSICS_VERSION, *m_allocator, *m_errorOutput );
                if( !m_foundation )
                {
                    auto &foundation = PxGetFoundation();
                    m_foundation = &foundation;
                    FB_LOG( "Getting existing px foundation." );
                }

                auto scale = PxTolerancesScale();
                auto physics = PxCreatePhysics( PX_PHYSICS_VERSION, *m_foundation, scale );
                if( !physics )
                {
                    FB_LOG_MESSAGE( "Physics", "Error: SDK initialisation failed." );
                }

                auto params = PxCookingParams( scale );
                m_cooking = PxCreateCooking( PX_PHYSICS_VERSION, *m_foundation, params );
                if( !m_cooking )
                {
                    FB_LOG_MESSAGE( "Physics", "[OgrePhysX] Error: Cooking initialisation failed." );
                }

                if( !PxInitExtensions( *physics ) )
                {
                    FB_LOG_MESSAGE( "Physics", "PxInitExtensions failed!" );
                }

                // if (mPxPhysics->getPvdConnectionManager())
                //	PxExtensionVisualDebugger::connect(mPxPhysics->getPvdConnectionManager(),
                //"localhost", 5425, 500, true);

                // create default material
                m_defaultMaterial = physics->createMaterial( 0.5f, 0.5f, 0.1f );

                setPhysics( physics );

#if FB_PHYSX_DEBUG
                FB_LOG( "PhysicsManager::initialise PvdConnectionManager" );

                // connect to PVD
                auto cm = physics->getPvdConnectionManager();
                if( cm )
                {
                    if( cm->isConnected() )
                    {
                        cm->disconnect();
                    }
                    else
                    {
                        bool mUseFullPvdConnection = true;
                        // The connection flags state overall what data is to be sent to PVD.  Currently
                        // the Debug connection flag requires support from the implementation (don't send
                        // the data when debug isn't set) but the other two flags, profile and memory
                        // are taken care of by the PVD SDK.

                        // Use these flags for a clean profile trace with minimal overhead
                        // PVD::TConnectionFlagsType theConnectionFlags( PVD::PvdConnectionType::Profile
                        // )
                        debugger::TConnectionFlagsType theConnectionFlags(
                            debugger::PvdConnectionType::eDEBUG | debugger::PvdConnectionType::ePROFILE |
                            debugger::PvdConnectionType::eMEMORY );
                        if( !mUseFullPvdConnection )
                        {
                            theConnectionFlags =
                                debugger::TConnectionFlagsType( debugger::PvdConnectionType::ePROFILE );
                        }

                        // Create a pvd connection that writes data straight to the filesystem.  This is
                        // the fastest connection on windows for various reasons.  First, the transport
                        // is quite fast as pvd writes data in blocks and filesystems work well with that
                        // abstraction. Second, you don't have the PVD application parsing data and using
                        // CPU and memory bandwidth while your application is running.
                        // PxExtensionVisualDebugger::connect(mPhysics->getPvdConnectionManager(),"c:\\temp.pxd2",
                        // PxDebuggerConnectionFlags( (PxU32)theConnectionFlags));

                        // The normal way to connect to pvd.  PVD needs to be running at the time this
                        // function is called. We don't worry about the return value because we are
                        // already registered as a listener for connections and thus our onPvdConnected
                        // call will take care of setting up our basic connection state.
                        // PxExtensionVisualDebugger::connect(pPhysics->getPvdConnectionManager(),
                        // "127.0.0.1", 5425, 10, PxDebuggerConnectionFlags( (PxU32)theConnectionFlags)
                        // );

                        PxVisualDebuggerExt::createConnection( cm, "localhost", 5425, 500 );
                    }
                }

                // connect to PVD
#endif

                setLoadingState( LoadingState::Loaded );
                FB_LOG_MESSAGE( "Physics", "PhysX physics created." );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PhysxManager::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    m_loadQueue.clear();

                    for( auto box : m_boxShapes )
                    {
                        if( box )
                        {
                            box->unload( data );
                        }
                    }

                    for( auto mesh : m_meshShapes )
                    {
                        if( mesh )
                        {
                            mesh->unload( data );
                        }
                    }

                    for( auto plane : m_planeShapes )
                    {
                        if( plane )
                        {
                            plane->unload( data );
                        }
                    }

                    for( auto constraint : m_constraints )
                    {
                        if( constraint )
                        {
                            constraint->unload( data );
                        }
                    }

                    for( auto body : m_staticBodies )
                    {
                        if( body )
                        {
                            body->unload( data );
                        }
                    }

                    for( auto body : m_rigidBodies )
                    {
                        if( body )
                        {
                            body->unload( data );
                        }
                    }

                    m_constraints.clear();
                    m_boxShapes.clear();
                    m_meshShapes.clear();
                    m_planeShapes.clear();

                    m_rigidBodies.clear();
                    m_staticBodies.clear();

                    for( auto scene : m_scenes )
                    {
                        scene->clear();
                        scene->unload( nullptr );
                    }

                    m_scenes.clear();

                    if( m_cpuDispatcher )
                    {
                        m_cpuDispatcher->release();
                        m_cpuDispatcher = nullptr;
                    }

                    if( m_cooking )
                    {
                        m_cooking->release();
                        m_cooking = nullptr;
                    }

                    if( auto physics = getPhysics() )
                    {
                        PxCloseExtensions();

                        physics->release();
                        setPhysics( nullptr );
                    }

                    if( m_foundation )
                    {
                        m_foundation->release();
                        m_foundation = nullptr;
                    }

                    auto &gc = GarbageCollector::instance();
                    gc.update();

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void PhysxManager::initialise( const SmartPtr<Properties> &properties )
        {
            if( !m_allocator )
            {
                m_allocator = fb::make_shared<PhysxAllocator>();
            }

            if( !m_errorOutput )
            {
                m_errorOutput = fb::make_shared<PhysxErrorOutput>();
            }

            m_foundation = PxCreateFoundation( PX_PHYSICS_VERSION, *m_allocator, *m_errorOutput );
            if( !m_foundation )
            {
                auto &foundation = PxGetFoundation();
                m_foundation = &foundation;
                FB_LOG( "Getting existing px foundation." );
            }

            auto scale = PxTolerancesScale();
            auto physics = PxCreatePhysics( PX_PHYSICS_VERSION, *m_foundation, scale );
            if( !physics )
            {
                FB_LOG_MESSAGE( "Physics", "Error: SDK initialisation failed." );
            }

            auto params = PxCookingParams( scale );
            m_cooking = PxCreateCooking( PX_PHYSICS_VERSION, physics->getFoundation(), params );
            if( !m_cooking )
            {
                FB_LOG_MESSAGE( "Physics", "[OgrePhysX] Error: Cooking initialisation failed." );
            }

            if( !PxInitExtensions( *physics ) )
            {
                FB_LOG_MESSAGE( "Physics", "PxInitExtensions failed!" );
            }

            // if (mPxPhysics->getPvdConnectionManager())
            //	PxExtensionVisualDebugger::connect(mPxPhysics->getPvdConnectionManager(), "localhost",
            // 5425, 500, true);

            // create default material
            m_defaultMaterial = physics->createMaterial( 0.5f, 0.5f, 0.1f );

            setPhysics( physics );

#if FB_PHYSX_DEBUG
            FB_LOG( "PhysicsManager::initialise PvdConnectionManager" );

            // connect to PVD
            debugger::comm::PvdConnectionManager *cm = physics->getPvdConnectionManager();
            if( cm )
            {
                if( cm->isConnected() )
                {
                    physics->getPvdConnectionManager()->disconnect();
                }
                else
                {
                    bool mUseFullPvdConnection = true;
                    // The connection flags state overall what data is to be sent to PVD.  Currently
                    // the Debug connection flag requires support from the implementation (don't send
                    // the data when debug isn't set) but the other two flags, profile and memory
                    // are taken care of by the PVD SDK.

                    // Use these flags for a clean profile trace with minimal overhead
                    // PVD::TConnectionFlagsType theConnectionFlags( PVD::PvdConnectionType::Profile )
                    debugger::TConnectionFlagsType theConnectionFlags(
                        debugger::PvdConnectionType::eDEBUG | debugger::PvdConnectionType::ePROFILE |
                        debugger::PvdConnectionType::eMEMORY );
                    if( !mUseFullPvdConnection )
                    {
                        theConnectionFlags =
                            debugger::TConnectionFlagsType( debugger::PvdConnectionType::ePROFILE );
                    }

                    // Create a pvd connection that writes data straight to the filesystem.  This is
                    // the fastest connection on windows for various reasons.  First, the transport is
                    // quite fast as pvd writes data in blocks and filesystems work well with that
                    // abstraction. Second, you don't have the PVD application parsing data and using CPU
                    // and memory bandwidth while your application is running.
                    // PxExtensionVisualDebugger::connect(mPhysics->getPvdConnectionManager(),"c:\\temp.pxd2",
                    // PxDebuggerConnectionFlags( (PxU32)theConnectionFlags));

                    // The normal way to connect to pvd.  PVD needs to be running at the time this
                    // function is called. We don't worry about the return value because we are already
                    // registered as a listener for connections and thus our onPvdConnected call will
                    // take care of setting up our basic connection state.
                    // PxExtensionVisualDebugger::connect(pPhysics->getPvdConnectionManager(),
                    // "127.0.0.1", 5425, 10, PxDebuggerConnectionFlags( (PxU32)theConnectionFlags) );

                    PxVisualDebuggerExt::createConnection( physics->getPvdConnectionManager(),
                                                           "localhost", 5425, 500 );
                }
            }

            // connect to PVD
#endif

            FB_LOG_MESSAGE( "Physics", "[OgrePhysX] SDK created." );
        }

        void PhysxManager::initialise( SmartPtr<scene::IDirector> objectTemplate )
        {
            m_allocator = fb::make_shared<PhysxAllocator>();
            m_errorOutput = fb::make_shared<PhysxErrorOutput>();

            auto mFoundation = PxCreateFoundation( PX_PHYSICS_VERSION, *m_allocator, *m_errorOutput );
            if( !mFoundation )
            {
                FB_LOG_MESSAGE( "Physics", "Error: PxCreateFoundation failed!" );
            }

            auto scale = PxTolerancesScale();
            auto physics = PxCreatePhysics( PX_PHYSICS_VERSION, *mFoundation, scale );
            if( !physics )
            {
                FB_LOG_MESSAGE( "Physics", "Error: SDK initialisation failed." );
            }

            PxCookingParams params( scale );
            // m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, m_physics->getFoundation(), params);
            if( !m_cooking )
            {
                FB_LOG_MESSAGE( "Physics", "[OgrePhysX] Error: Cooking initialisation failed." );
            }

            if( !PxInitExtensions( *physics ) )
            {
                FB_LOG_MESSAGE( "Physics", "PxInitExtensions failed!" );
            }

#if FB_PHYSX_DEBUG
            if( physics->getPvdConnectionManager() )
                PxVisualDebuggerExt::createConnection( physics->getPvdConnectionManager(), "localhost",
                                                       5425, 500 );
#endif

            // create default material
            m_defaultMaterial = physics->createMaterial( 0.5f, 0.5f, 0.1f );

            setPhysics( physics );

            FB_LOG_MESSAGE( "Physics", "[OgrePhysX] SDK created." );

            m_cooker = new PhysxCooker;
        }

        void PhysxManager::setDefaultMaterial( PxMaterial *defaultMaterial )
        {
            m_defaultMaterial = defaultMaterial;
        }

        PhysxVehicleManager *PhysxManager::getVehicleManager() const
        {
            return m_vehicleManager;
        }

        void PhysxManager::setVehicleManager( PhysxVehicleManager *vehicleManager )
        {
            m_vehicleManager = vehicleManager;
        }

        PhysxCooker *PhysxManager::getCooker() const
        {
            return m_cooker;
        }

        void PhysxManager::setCooker( PhysxCooker *cooker )
        {
            m_cooker = cooker;
        }

        SmartPtr<IPhysicsMaterial3> PhysxManager::createMaterial()
        {
            auto material = fb::make_ptr<PhysxMaterial>();

            if( auto physics = getPhysics() )
            {
                auto m = physics->createMaterial( 0.5f, 0.5f, 0.1f );
                material->setMaterial( m );
            }

            return material;
        }

        void PhysxManager::destroyMaterial( SmartPtr<IPhysicsMaterial3> material )
        {
        }

        SmartPtr<ISphereShape3> PhysxManager::createSphere()
        {
            try
            {
                ISharedObject::ScopedLock lock( this );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto sphere = factoryManager->make_ptr<PhysxSphereShape>();

                auto material = createMaterial();
                sphere->setMaterial( material );

                m_sphereShapes.push_back( sphere );
                return sphere;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IBoxShape3> PhysxManager::createBox()
        {
            try
            {
                ISharedObject::ScopedLock lock( this );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto box = factoryManager->make_ptr<PhysxBoxShape>();

                auto material = createMaterial();
                box->setMaterial( material );

                m_boxShapes.push_back( box );
                return box;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IPlaneShape3> PhysxManager::createPlane()
        {
            try
            {
                ISharedObject::ScopedLock lock( this );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto plane = factoryManager->make_ptr<PhysxPlaneShape>();

                auto material = createMaterial();
                plane->setMaterial( material );

                m_planeShapes.push_back( plane );
                return plane;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<IMeshShape> PhysxManager::createMesh()
        {
            try
            {
                ISharedObject::ScopedLock lock( this );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto mesh = factoryManager->make_ptr<PhysxMeshShape>();
                m_meshShapes.push_back( mesh );
                return mesh;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        SmartPtr<ITerrainShape> PhysxManager::createTerrain()
        {
            try
            {
                ISharedObject::ScopedLock lock( this );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto terrain = factoryManager->make_ptr<PhysxTerrain>();
                m_terrainShapes.push_back( terrain );
                return terrain;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void PhysxManager::preUpdate()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto timer = applicationManager->getTimer();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            ISharedObject::ScopedLock lock( this );

            if( dt < MathF::epsilon() )
            {
                return;
            }

            if( !m_loadQueue.empty() )
            {
                SmartPtr<ISharedObject> loadObject;
                while( m_loadQueue.try_pop( loadObject ) )
                {
                    loadObject->load( nullptr );
                }
            }

            if( m_vehicleManager )
            {
                // if (m_scene)
                //{
                //	m_vehicleManager->suspensionRaycasts(m_scene);
                // }
            }

            // for (auto c : m_constraints)
            //{
            //	if (c)
            //	{
            //		c->update();
            //	}
            // }

            // for (u32 i = 0; i < m_rigidBodies.size(); ++i)
            //{
            //	auto& rigidBody = m_rigidBodies[i];
            //	if (rigidBody)
            //	{
            //		rigidBody->update();
            //	}
            // }

            // for (u32 i = 0; i < m_characters.size(); ++i)
            //{
            //	auto& character = m_characters[i];
            //	if (character)
            //	{
            //		character->update();
            //	}
            // }
        }

        void PhysxManager::update()
        {
        }

        void PhysxManager::postUpdate()
        {
        }

        SmartPtr<IPhysicsScene3> PhysxManager::createScene()
        {
            try
            {
                auto scene = fb::make_ptr<PhysxScene>();
                scene->load( nullptr );

                m_scenes.push_back( scene );
                return scene;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        void PhysxManager::destroyScene( SmartPtr<IPhysicsScene3> scene )
        {
            if( scene )
            {
                scene->unload( nullptr );
                scene = nullptr;
            }
        }

        bool PhysxManager::getEnableDebugDraw() const
        {
            return m_enableDebugDraw;
        }

        void PhysxManager::setEnableDebugDraw( bool enableDebugDraw )
        {
            m_enableDebugDraw = enableDebugDraw;
        }

        SmartPtr<IPhysicsShape3> PhysxManager::createCollisionShapeByType( hash64 type,
                                                                           SmartPtr<ISharedObject> data )
        {
            auto typeManager = TypeManager::instance();
            FB_ASSERT( typeManager );

            const auto sphereTypeInfo = ISphereShape3::typeInfo();
            const auto boxTypeInfo = IBoxShape3::typeInfo();
            const auto meshTypeInfo = IMeshShape::typeInfo();
            const auto terrainTypeInfo = ITerrainShape::typeInfo();

            const auto sphereType = typeManager->getHash( sphereTypeInfo );
            const auto boxType = typeManager->getHash( boxTypeInfo );
            const auto meshType = typeManager->getHash( meshTypeInfo );
            const auto terrainType = typeManager->getHash( terrainTypeInfo );

            if( type == sphereType )
            {
                return createSphere();
            }
            if( type == boxType )
            {
                return createBox();
            }
            if( type == meshType )
            {
                try
                {
                    ISharedObject::ScopedLock lock( this );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto factoryManager = applicationManager->getFactoryManager();
                    FB_ASSERT( factoryManager );

                    auto mesh = factoryManager->make_ptr<PhysxMeshShape>();
                    mesh->load( data );
                    m_meshShapes.push_back( mesh );
                    return mesh;
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }

                return nullptr;
            }
            if( type == terrainType )
            {
                return createTerrain();
            }

            return nullptr;
        }

        SmartPtr<IRigidBody3> PhysxManager::createRigidBody()
        {
            return createRigidBody( nullptr, nullptr );
        }

        SmartPtr<IRigidBody3> PhysxManager::createRigidBody( SmartPtr<IPhysicsShape3> collisionShape )
        {
            return createRigidBody( collisionShape, nullptr );
        }

        SmartPtr<IRigidBody3> PhysxManager::createRigidBody( SmartPtr<IPhysicsShape3> collisionShape,
                                                             SmartPtr<Properties> properties )
        {
            ISharedObject::ScopedLock lock( this );

            PxTransform transform = PxTransform::createIdentity();

            if( auto physics = getPhysics() )
            {
                PxRigidDynamic *actor = physics->createRigidDynamic( transform );

                if( collisionShape )
                {
                    /*
                    hash32 type = collisionShape->getType();
                    if (type == StringUtil::getHash("sphere"))
                    {
                    }
                    else if (type == StringUtil::getHash("box"))
                    {
                        auto boxShape = fb::dynamic_pointer_cast<IBoxShape3>(collisionShape);
                        if (boxShape)
                        {
                            auto extents = boxShape->getExtents() / real_Num(2.0);

                            PxVec3 dimensions(extents.X(), extents.Y(), extents.Z());
                            PxBoxGeometry geometry(dimensions);
                            PxTransform localPose = PxTransform::createIdentity();

                            auto shape = actor->createShape(geometry, *m_defaultMaterial, localPose);

                            //shape->setSimulationFilterData(simFilterData);
                            //shape->setQueryFilterData(qryFilterData);
                        }
                    }
                    */
                }

                SmartPtr<PhysxRigidDynamic> rigidBody( new PhysxRigidDynamic );
                rigidBody->setActor( actor );
                PxRigidBodyExt::updateMassAndInertia( *actor, 1.0f );
                m_rigidBodies.push_back( rigidBody );
                loadObject( rigidBody );
                return rigidBody;
            }

            return nullptr;
        }

        SmartPtr<IRigidBody3> PhysxManager::createRigidBody( const Transform3<real_Num> &transform )
        {
            auto rigidBody = createRigidBody( nullptr, nullptr );
            rigidBody->setTransform( transform );
            return rigidBody;
        }

        SmartPtr<IRigidStatic3> PhysxManager::createRigidStatic(
            SmartPtr<IPhysicsShape3> collisionShape )
        {
            return createRigidStatic( collisionShape, nullptr );
        }

        SmartPtr<IRigidStatic3> PhysxManager::createRigidStatic( SmartPtr<IPhysicsShape3> collisionShape,
                                                                 SmartPtr<Properties> properties )
        {
            ISharedObject::ScopedLock lock( this );

            PxTransform transform = PxTransform::createIdentity();

            if( auto physics = getPhysics() )
            {
                PxRigidStatic *actor = physics->createRigidStatic( transform );

                /*
                hash32 type = collisionShape->getType();
                if (type == StringUtil::getHash("sphere"))
                {
                }
                else if (type == StringUtil::getHash("box"))
                {
                    SmartPtr<IBoxShape3> boxShape = collisionShape;
                    Vector3<real_Num> extents = boxShape->getExtents() / 2.0;

                    PxVec3 dimensions(extents.X(), extents.Y(), extents.Z());
                    PxBoxGeometry geometry(dimensions);
                    PxTransform localPose = PxTransform::createIdentity();

                    PxMaterial* mat = mStandardMaterials[0];
                    PxShape* shape = actor->createShape(geometry, *mat, localPose);

                    //shape->setSimulationFilterData(simFilterData);
                    //shape->setQueryFilterData(qryFilterData);
                }
                */

                // SmartPtr<PhysxRigidStatic> rigidBody(new PhysxRigidStatic);
                // rigidBody->setRigidStatic(actor);

                ////PxRigidBodyExt::updateMassAndInertia(*actor, 1.0f);
                ////m_scene->addActor(*actor);

                // m_staticBodies.push_back(rigidBody);
            }

            // return rigidBody;
            return nullptr;
        }

        SmartPtr<IRigidStatic3> PhysxManager::createRigidStatic( const Transform3<real_Num> &transform )
        {
            auto rigidStatic = fb::make_ptr<PhysxRigidStatic>();

            rigidStatic->setTransform( transform );
            m_staticBodies.push_back( rigidStatic );
            loadObject( rigidStatic );
            return rigidStatic;
        }

        SmartPtr<IRigidDynamic3> PhysxManager::createRigidDynamic(
            const Transform3<real_Num> &transform )
        {
            auto rigidDynamic = fb::make_ptr<PhysxRigidDynamic>();
            rigidDynamic->setTransform( transform );
            m_rigidBodies.push_back( rigidDynamic );
            loadObject( rigidDynamic );
            return rigidDynamic;
        }

        //
        // SmartPtr<IRigidBody3> PhysxManager::createRigidBody(const String& filePath,
        // SmartPtr<IPhysicsShape3> collisionShape)
        //{
        //	return nullptr;
        // }

        SmartPtr<IPhysicsSoftBody3> PhysxManager::createSoftBody( const String &filePath )
        {
            return nullptr;
        }

        PxU32 gNumVehicleAdded = 0;

        ////////////////////////////////////////////////////////////////
        // VEHICLE SETUP DATA
        ////////////////////////////////////////////////////////////////

        PxF32 gChassisMass = 1500.0f;
        PxF32 gSuspensionShimHeight = 0.125f;

        ////////////////////////////////////////////////////////////////
        // RENDER USER DATA TO ASSOCIATE EACH RENDER MESH WITH A
        // VEHICLE PHYSICS COMPONENT
        ////////////////////////////////////////////////////////////////

        enum
        {
            CAR_PART_FRONT_LEFT_WHEEL = 0,
            CAR_PART_FRONT_RIGHT_WHEEL,
            CAR_PART_REAR_LEFT_WHEEL,
            CAR_PART_REAR_RIGHT_WHEEL,
            CAR_PART_CHASSIS,
            CAR_PART_WINDOWS,
            NUM_CAR4W_RENDER_COMPONENTS
        };

        static char gCarPartNames[NUM_CAR4W_RENDER_COMPONENTS][64] = {
            "frontwheelleftshape", "frontwheelrightshape", "backwheelleftshape",
            "backwheelrightshape", "car_02_visshape",      "car_02_windowsshape"
        };

        struct CarRenderUserData
        {
            PxU8 carId;
            PxU8 carPart;
            PxU8 carPartDependency;
            PxU8 pad;
        };

        static CarRenderUserData gCar4WRenderUserData[NUM_CAR4W_RENDER_COMPONENTS] = {
            // wheel fl		wheel fr		wheel rl		wheel rl		chassis			windows
            { 0, 0, 255 }, { 0, 1, 255 }, { 0, 2, 255 }, { 0, 3, 255 }, { 0, 4, 255 }, { 0, 4, 4 }
        };

        CarRenderUserData gVehicleRenderUserData[NUM_PLAYER_CARS + NUM_NONPLAYER_4W_VEHICLES]
                                                [NUM_CAR4W_RENDER_COMPONENTS];

        static PxVec3 g4WCarPartDependencyOffsets[NUM_CAR4W_RENDER_COMPONENTS] = {
            PxVec3( 0, 0, 0 ), PxVec3( 0, 0, 0 ), PxVec3( 0, 0, 0 ),
            PxVec3( 0, 0, 0 ), PxVec3( 0, 0, 0 ), PxVec3( 0, 0, 0 )
        };

        //	RenderMeshActor*
        // gRenderMeshActors[NUM_CAR4W_RENDER_COMPONENTS]={NULL,NULL,NULL,NULL,NULL,NULL};

        ////////////////////////////////////////////////////////////////
        // TRANSFORM APPLIED TO CHASSIS RENDER MESH VERTS
        // THAT IS REQUIRED TO PLACE AABB OF CHASSIS RENDER MESH AT ORIGIN
        // AT CENTRE-POINT OF WHEELS.
        ////////////////////////////////////////////////////////////////

        static PxVec3 gChassisMeshTransform( 0, 0, 0 );

        ////////////////////////////////////////////////////////////////
        // WHEEL CENTRE OFFSETS FROM CENTRE OF CHASSIS RENDER MESH AABB
        // OF 4-WHEELED VEHICLE
        ////////////////////////////////////////////////////////////////

        static PxVec3 gWheelCentreOffsets4[4];

        ////////////////////////////////////////////////////////////////
        // CONVEX HULL OF RENDER MESH FOR CHASSIS AND WHEELS OF
        // 4-WHEELED VEHICLE
        ////////////////////////////////////////////////////////////////

        static PxConvexMesh *gChassisConvexMesh = nullptr;
        static PxConvexMesh *gWheelConvexMeshes4[4] = { nullptr, nullptr, nullptr, nullptr };

        SmartPtr<IPhysicsVehicle3> PhysxManager::addVehicle( SmartPtr<IRigidBody3> chassis,
                                                             const SmartPtr<Properties> &properties )
        {
            // SmartPtr<PhysxVehicle3> vehicle(new PhysxVehicle3);

            // Ogre::MeshPtr chassisMesh = Ogre::MeshManager::getSingletonPtr()->load("4x4chassis.mesh",
            // Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME); chassisMesh->load(nullptr);
            // if(chassisMesh->isLoaded())
            //	gChassisConvexMesh = m_cooker->createPxConvexMesh(chassisMesh.get());

            // Ogre::MeshPtr wheelMeshFL =
            // Ogre::MeshManager::getSingletonPtr()->getByName("4x4WheelLeftFront.mesh");
            // wheelMeshFL->load(nullptr);
            // gWheelConvexMeshes4[0] = m_cooker->createPxConvexMesh(wheelMeshFL.get());

            // Ogre::MeshPtr wheelMeshFR =
            // Ogre::MeshManager::getSingletonPtr()->getByName("4x4WheelLeftFront.mesh");
            // wheelMeshFR->load(nullptr);
            // gWheelConvexMeshes4[1] = m_cooker->createPxConvexMesh(wheelMeshFR.get());

            // Ogre::MeshPtr wheelMeshRL =
            // Ogre::MeshManager::getSingletonPtr()->getByName("4x4WheelLeftFront.mesh");
            // wheelMeshRL->load(nullptr);
            // gWheelConvexMeshes4[2] = m_cooker->createPxConvexMesh(wheelMeshRL.get());

            // Ogre::MeshPtr wheelMeshRR =
            // Ogre::MeshManager::getSingletonPtr()->getByName("4x4WheelLeftFront.mesh");
            // wheelMeshRR->load(nullptr);
            // gWheelConvexMeshes4[3] = m_cooker->createPxConvexMesh(wheelMeshRR.get());

            // vehicle->setVehicle(pVehicle);

            // m_vehicles.push_back(vehicle);

            // return vehicle;
            return nullptr;
        }

        SmartPtr<IPhysicsVehicle3> PhysxManager::addVehicle( SmartPtr<scene::IDirector> pTemplate )
        {
            // SmartPtr<VehicleTemplate> vehicleTemplate; // = pTemplate;

            // SmartPtr<PhysxVehicle3> vehicle(new PhysxVehicle3);

            // PhysxCooker::Params vehicleParams;
            // vehicleParams.scale(Vector3F::unit() * 0.2f);

            // auto engine = core::IApplicationManager::instance();
            // SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();
            // SmartPtr<IMeshManager> meshMgr = graphicsSystem->getMeshManager();

            // SmartPtr<IMesh> chassisMesh = meshMgr->loadMesh("4x4chassis.mesh");
            // gChassisConvexMesh = m_cooker->createPxConvexMesh(chassisMesh, vehicleParams);

            // SmartPtr<IMesh> wheelMeshFL = meshMgr->loadMesh("4x4WheelLeftFront.mesh");
            // gWheelConvexMeshes4[0] = m_cooker->createPxConvexMesh(wheelMeshFL, vehicleParams);

            // SmartPtr<IMesh> wheelMeshFR = meshMgr->loadMesh("4x4WheelLeftFront.mesh");
            // gWheelConvexMeshes4[1] = m_cooker->createPxConvexMesh(wheelMeshFR, vehicleParams);

            // SmartPtr<IMesh> wheelMeshRL = meshMgr->loadMesh("4x4WheelLeftFront.mesh");
            // gWheelConvexMeshes4[2] = m_cooker->createPxConvexMesh(wheelMeshRL, vehicleParams);

            // SmartPtr<IMesh> wheelMeshRR = meshMgr->loadMesh("4x4WheelLeftFront.mesh");
            // gWheelConvexMeshes4[3] = m_cooker->createPxConvexMesh(wheelMeshRR, vehicleParams);

            // Array<SmartPtr<VehicleWheelTemplate>> wheels = vehicleTemplate->getWheels();
            // for (u32 i = 0; i < wheels.size(); ++i)
            //	gWheelCentreOffsets4[i] = PxVec3(wheels[i]->getOffset().X(), wheels[i]->getOffset().Y(),
            //	                                 wheels[i]->getOffset().Z());

            // PxTransform transform = PxTransform::createIdentity();
            // PxVehicleDrive4W* pVehicle = m_vehicleManager->create4WVehicle(*getScene(), *getPhysics(),
            // *getCooking(),
            //                                                                *mStandardMaterials[SURFACE_TYPE_TARMAC],
            //                                                                gChassisMass,
            //                                                                gWheelCentreOffsets4,
            //                                                                gChassisConvexMesh,
            //                                                                gWheelConvexMeshes4,
            //                                                                transform, true);

            // vehicle->setVehicle(pVehicle);

            // m_vehicles.push_back(vehicle);

            // return vehicle;

            return nullptr;
        }

        bool PhysxManager::destroyCollisionShape( SmartPtr<IPhysicsShape3> collisionShape )
        {
            if( collisionShape )
            {
                collisionShape->unload( nullptr );

                if( collisionShape->isDerived<IBoxShape3>() )
                {
                    auto it = std::find( m_boxShapes.begin(), m_boxShapes.end(), collisionShape );
                    if( it != m_boxShapes.end() )
                    {
                        m_boxShapes.erase( it );
                    }
                }
                else if( collisionShape->isDerived<IMeshShape>() )
                {
                    auto it = std::find( m_meshShapes.begin(), m_meshShapes.end(), collisionShape );
                    if( it != m_meshShapes.end() )
                    {
                        m_meshShapes.erase( it );
                    }
                }

                return true;
            }

            return false;
        }

        bool PhysxManager::destroyPhysicsBody( SmartPtr<IRigidBody3> body )
        {
            return false;
        }

        bool PhysxManager::destroyVehicle( SmartPtr<IPhysicsVehicle3> vehicle )
        {
            return false;
        }

        SmartPtr<ICharacterController3> PhysxManager::addCharacter()
        {
            // SmartPtr<PhysxCharacterController> character(new PhysxCharacterController);
            // character->initialise(objectTemplate);
            // m_characters.push_back(character);
            // return character;
            return nullptr;
        }

        SmartPtr<ITerrainShape> PhysxManager::createTerrain( SmartPtr<scene::IDirector> objectTemplate )
        {
            SmartPtr<ITerrainShape> terrain( new PhysxTerrain );
            // terrain->initialise(objectTemplate);
            return terrain;
        }

        bool PhysxManager::rayTest( const Vector3<real_Num> &start, const Vector3<real_Num> &direction,
                                    Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                                    u32 collisionType, u32 collisionMask )
        {
            return false;
        }

        bool PhysxManager::intersects( const Vector3<real_Num> &start, const Vector3<real_Num> &end,
                                       Vector3<real_Num> &hitPos, Vector3<real_Num> &hitNormal,
                                       SmartPtr<ISharedObject> &object, u32 collisionType /*= 0*/,
                                       u32 collisionMask /*= 0 */ )
        {
            return false;
        }

        PxPhysics *PhysxManager::getPhysics() const
        {
            auto p = m_physics.load();
            return p;
        }

        void PhysxManager::setPhysics( PxPhysics *physics )
        {
            m_physics = physics;
        }

        PxCooking *PhysxManager::getCooking() const
        {
            return m_cooking;
        }

        void PhysxManager::setCooking( PxCooking *cooking )
        {
            m_cooking = cooking;
        }

        PxMaterial *PhysxManager::getDefaultMaterial() const
        {
            return m_defaultMaterial;
        }

        PxMaterial *PhysxManager::getStandardMaterials() const
        {
            return mStandardMaterials[0];
        }

        void PhysxManager::lock()
        {
            PhysxMutex.lock();
        }

        void PhysxManager::unlock()
        {
            PhysxMutex.unlock();
        }

        SmartPtr<IConstraintD6> PhysxManager::d6JointCreate( SmartPtr<IPhysicsBody3> actor0,
                                                             const Transform3<real_Num> &localFrame0,
                                                             SmartPtr<IPhysicsBody3> actor1,
                                                             const Transform3<real_Num> &localFrame1 )
        {
            auto physics = getPhysics();
            auto j = fb::make_ptr<PhysxConstraintD6>();

            RawPtr<PxRigidDynamic> pxActor0;
            RawPtr<PxRigidDynamic> pxActor1;

            if( actor0 )
            {
                auto pActor0 = fb::static_pointer_cast<PhysxRigidDynamic>( actor0 );
                pxActor0 = pActor0->getActor();
            }

            if( actor1 )
            {
                auto pActor1 = fb::static_pointer_cast<PhysxRigidDynamic>( actor1 );
                pxActor1 = pActor1->getActor();
            }

            auto pxLocalFrame0 = PhysxUtil::toPx( localFrame0 );
            auto pxLocalFrame1 = PhysxUtil::toPx( localFrame1 );

            auto d6joint = PxD6JointCreate( *physics, pxActor0, pxLocalFrame0, pxActor1, pxLocalFrame1 );
            j->setJoint( d6joint );

            m_constraints.push_back( j );
            return j;
        }

        SmartPtr<IConstraintFixed3> PhysxManager::fixedJointCreate(
            SmartPtr<IPhysicsBody3> actor0, const Transform3<real_Num> &localFrame0,
            SmartPtr<IPhysicsBody3> actor1, const Transform3<real_Num> &localFrame1 )
        {
            auto physics = getPhysics();
            auto j = fb::make_ptr<PhysxConstraintFixed3>();

            RawPtr<PxRigidDynamic> pxActor0;
            RawPtr<PxRigidDynamic> pxActor1;

            if( actor0 )
            {
                auto pActor0 = fb::static_pointer_cast<PhysxRigidDynamic>( actor0 );
                pxActor0 = pActor0->getActor();
            }

            if( actor1 )
            {
                auto pActor1 = fb::static_pointer_cast<PhysxRigidDynamic>( actor1 );
                pxActor1 = pActor1->getActor();
            }

            auto pxLocalFrame0 = PhysxUtil::toPx( localFrame0 );
            auto pxLocalFrame1 = PhysxUtil::toPx( localFrame1 );

            auto fixedJoint =
                PxFixedJointCreate( *physics, pxActor0, pxLocalFrame0, pxActor1, pxLocalFrame1 );
            j->setJoint( fixedJoint );

            m_constraints.push_back( j );
            return j;
        }

        SmartPtr<IConstraintDrive> PhysxManager::createConstraintDrive()
        {
            return nullptr;
        }

        SmartPtr<IConstraintLinearLimit> PhysxManager::createConstraintLinearLimit(real_Num extent, real_Num contactDist)
        {
            physx::PxJointLinearLimit;
            return nullptr;
        }

        SmartPtr<IRaycastHit> PhysxManager::createRaycastHitData()
        {
            return nullptr;
        }

        Thread::Task PhysxManager::getPhysicsTask() const
        {
            auto applicationManager = core::IApplicationManager::instance();

            // if (isUpdating())
            //{
            //	return Thread::getCurrentTask();
            // }

            auto taskManager = applicationManager->getTaskManager();
            if( taskManager )
            {
                auto task = taskManager->getTask( Thread::Task::Physics );
                if( task )
                {
                    if( task->isExecuting() )
                    {
                        return Thread::Task::Physics;
                    }
                    if( task->isPrimary() )
                    {
                        return Thread::Task::Primary;
                    }
                }
            }

            return applicationManager->hasTasks() ? Thread::Task::Physics : Thread::Task::Primary;
        }

        void PhysxManager::loadObject( SmartPtr<ISharedObject> object, bool forceQueue )
        {
#if 1
            ScopedLock lock( this );

            if( !object->isLoaded() )
            {
                object->load( nullptr );
            }
#else
            auto task = Thread::getCurrentTask();
            auto physicsTask = getPhysicsTask();

            if( task != physicsTask )
            {
                m_loadQueue.push( object );
            }
            else
            {
                object->load( nullptr );
            }
#endif
        }

        void PhysxManager::unloadObject( SmartPtr<ISharedObject> object, bool forceQueue )
        {
            object->unload( nullptr );
        }

        Thread::Task PhysxManager::getStateTask() const
        {
            return Thread::Task::Physics;
        }

    }  // end namespace physics
}  // end namespace fb
