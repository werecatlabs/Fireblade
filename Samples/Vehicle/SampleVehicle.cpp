#include "SampleVehicle.h"
#include "Types.h"
#include <FBCore/FBCore.h>
#include <FBFileSystem/FBFileSystem.h>
#include <FBApplication/FBApplication.h>
#include <FBVehicle/FBVehicle.h>
#include <FBOISInput/FBOISInput.h>
#include <FBSQLite/FBSQLite.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT
#    include <FBGraphicsOgreNext/FBGraphicsOgreNext.h>
#elif FB_GRAPHICS_SYSTEM_OGRE
#    include <FBGraphicsOgre/FBGraphicsOgre.h>
#endif

#if FB_BUILD_PHYSX

#    include <FBPhysx/FBPhysx.h>

#elif FB_BUILD_ODE
#    include <FBODE3/CPhysicsManagerODE.h>
#endif

namespace fb
{

    SampleVehicle::SampleVehicle()
    {
    }

    SampleVehicle::~SampleVehicle()
    {
        unload( nullptr );
    }

    void SampleVehicle::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto task = Thread::Task::Primary;
            Thread::setCurrentTask( task );

            auto threadId = Thread::ThreadId::Primary;
            Thread::setCurrentThreadId( threadId );

            auto applicationManager = fb::make_ptr<core::ApplicationManagerMT>();
            core::IApplicationManager::setInstance( applicationManager );

            CApplicationClient::load( data );

            //auto factoryManager = applicationManager->getFactoryManager();

            // auto logManager = LogManager::instance();
            // logManager->open("Sample.log");

            //auto sceneManager = fb::make_ptr<scene::CSceneManager>();
            //applicationManager->setSceneManager( sceneManager );
            //sceneManager->load( nullptr );

            //auto scene = fb::make_ptr<scene::CScene>();
            //sceneManager->setCurrentScene( scene );
            //scene->load( nullptr );

            //m_cameraActor = ApplicationUtil::createCamera();

            //auto physicsManager = applicationManager->getPhysicsManager();

            //auto prefabManager = fb::make_ptr<scene::CPrefabManager>();
            //applicationManager->setPrefabManager( prefabManager );

            //auto vehicleManager = fb::make_ptr<CVehicleManager>();
            //applicationManager->setVehicleManager( vehicleManager );

            //createScene();
            //createVehicle();

            /*
            for (size_t i=0; i<40; ++i)
            {
                auto newVehicle = prefabManager->createInstance(m_vehicleActor);
                scene->registerAllUpdates(newVehicle);
                scene->addActor(newVehicle);
            }
            */

            /*
            auto prefab = factoryManager->make_ptr<CActor>();
            auto prefabMesh = factoryManager->make_ptr<CActor>();
            prefab->addChild(prefabMesh);

            auto meshComponent = prefab->addComponentObject<component::MeshComponent>();
            if (meshComponent)
            {
                meshComponent->setMeshPath("F40f40.fbmeshbin");
            }

            auto meshRenderer = prefab->addComponentObject<component::MeshRenderer>();
            auto material = prefab->addComponentObject<component::MaterialComponent>();
            if (material)
            {
                //material->setDiffuse(ColourF::Red);
            }

            auto boxCollider = prefab->addComponentObject<component::CollisionBox>();
            auto rigidBody = prefab->addComponentObject<component::Rigidbody>();

            prefab->setLocalScale(Vector3<real_Num>::unit() * 0.01);

            auto offset = Vector3<real_Num>::unit() * 2.0;
            auto numBoxes = (size_t)4;
            for (size_t x = 0; x < numBoxes; ++x)
            {
                for (size_t y = 0; y < numBoxes; ++y)
                {
                    for (size_t z = 0; z < numBoxes; ++z)
                    {
                        auto newBox = prefabManager->createInstance(prefab);

                        auto position = Vector3<real_Num>::zero();
                        position += Vector3<real_Num>::unitX() * (x * offset.X());
                        position += Vector3<real_Num>::unitY() * ((y * offset.Y()) + 1);
                        position += Vector3<real_Num>::unitZ() * (z * offset.Z());

                        newBox->setPosition(position);
                        m_boxes.push_back(newBox);

                        scene->registerAllUpdates(newBox);
                        scene->addActor(newBox);
                    }
                }
            }
            */

            //auto graphicsSystem = applicationManager->getGraphicsSystem();
            // graphicsSystem->setupRenderer(m_sceneMgr, m_window, m_camera, "", true);

            setLoadingState( LoadingState::Loaded );
            applicationManager->setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void SampleVehicle::unload( SmartPtr<ISharedObject> data )
    {
        setLoadingState( LoadingState::Unloading );

        auto applicationManager = core::IApplicationManager::instance();

        applicationManager->unload( nullptr );
        core::IApplicationManager::setInstance( nullptr );
        applicationManager = nullptr;
        FB_ASSERT( core::IApplicationManager::instance() == nullptr );

        setLoadingState( LoadingState::Unloaded );
    }

    void SampleVehicle::createPlugins()
    {
        try
        {
            FB_DEBUG_TRACE;

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

#ifdef _FB_STATIC_LIB_
            auto corePlugin = fb::make_ptr<FBCore>();
            corePlugin->load( nullptr );
#endif

#ifdef _FB_STATIC_LIB_
            auto applicationPlugin = fb::make_ptr<ApplicationPlugin>();
            applicationManager->addPlugin( applicationPlugin );
#endif

#ifdef _FB_STATIC_LIB_
            auto fileSystemPlugin = fb::make_ptr<FBFileSystem>();
            fileSystemPlugin->load( nullptr );
#endif

#ifdef _FB_STATIC_LIB_
            auto databasePlugin = fb::make_ptr<SQLitePlugin>();
            applicationManager->addPlugin( databasePlugin );
#endif

#ifdef _FB_STATIC_LIB_
            auto inputPlugin = fb::make_ptr<OISInput>();
            applicationManager->addPlugin( inputPlugin );
#endif

#ifdef _FB_STATIC_LIB_
#    if FB_GRAPHICS_SYSTEM_CLAW
            auto graphicsPlugin = fb::make_ptr<render::FBGraphicsClaw>();
            applicationManager->addPlugin( graphicsPlugin );
#    elif FB_GRAPHICS_SYSTEM_OGRENEXT
            auto graphicsPlugin = fb::make_ptr<render::FBGraphicsOgreNext>();
            applicationManager->addPlugin( graphicsPlugin );
#    elif FB_GRAPHICS_SYSTEM_OGRE
            auto graphicsPlugin = fb::make_ptr<render::FBGraphicsOgre>();
            applicationManager->addPlugin( graphicsPlugin );
#    endif
#endif

#ifdef _FB_STATIC_LIB_
#    if FB_BUILD_PHYSX
            auto physxPlugin = fb::make_ptr<physics::FBPhysx>();
            applicationManager->addPlugin( physxPlugin );
#    elif FB_BUILD_ODE
#    endif
#endif

            ApplicationUtil::createFactories();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }
    
    void SampleVehicle::createScene()
    {
        auto applicationManager = core::IApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factory = applicationManager->getFactoryManager();
        auto physicsMgr = applicationManager->getPhysicsManager();

        auto physicsScene = physicsMgr->createScene();
        applicationManager->setPhysicsScene( physicsScene );

        // auto fpsCamera = factory->createObject<AppFPSCameraCtrl>();
        // fpsCamera->setName("FPSCameraCtrl");
        // fpsCamera->setRotationSpeed(10.0f);
        // fpsCamera->setPosition((Vector3F::UNIT_Z * 100.f) + (Vector3F::UNIT_Y * 100.0f));
        // fpsCamera->setTargetPosition(Vector3F::zero());
        // fpsCamera->addCamera(m_camera);
        // fpsCamera->setInvert(true);
        // m_cameraController = fpsCamera;

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto smgr = graphicsSystem->getSceneManager( "GameSceneManager" );
        smgr->setAmbientLight( ColourF::White );
        //smgr->setSkyBox( true, "Examples/SceneSkyBox1" );

        m_boxGround = ApplicationUtil::createDefaultGround();

        auto light = smgr->addLight( "MainLight" );
        auto lightNode = smgr->getRootSceneNode()->addChildSceneNode( "MainLight" );
        lightNode->attachObject( light );
        // light->setDirection();

        // m_cameraSceneNode->setPosition(Vector3F(0, 100, 100));
        // m_cameraSceneNode->lookAt(Vector3F::zero());
        // m_cameraSceneNode->setFixedYawAxis(true, Vector3F::UNIT_Y);

        // m_terrain = ApplicationUtil::createDefaultTerrain();

        m_boxGround = ApplicationUtil::createDefaultGround();

        m_vehicleActor = ApplicationUtil::createDefaultCar();

        // SmartPtr<TerrainTemplate> terrainTemplate(new TerrainTemplate);
        // terrainTemplate->setHeightData(terrainData);

        // auto physicsTerrain = physicsMgr->createTerrain(terrainTemplate);
        ////physicsTerrain->setData(terrainData);
        ////physicsTerrain->setHeightScale(60.0f/255.f);
        ////physicsTerrain->setGridSpacing(2000.0f / 513.f);
        // physicsTerrain->build();
        // m_physicsTerrain = physicsTerrain;

        // m_vehicle = factory->create("CarStandard");

        //SmartPtr<VehicleTemplate> vehicleTemplate( new VehicleTemplate );
        // m_vehicle->initialise(vehicleTemplate);
        // m_vehicle->setPosition(Vector3F::UNIT_Y * 45.0);

        // auto terrainRayResult = m_terrain->intersects(Ray3F(Vector3F::UNIT_Y * 45.0,
        // -Vector3F::UNIT_Y)); if (terrainRayResult)
        //{
        //	//m_vehicle->setPosition(terrainRayResult->getPosition() + (Vector3F::UNIT_Y * 5.0f));
        // }

        physicsMgr->setEnableDebugDraw( true );
    }

    void SampleVehicle::createVehicle()
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            auto sceneManager = applicationManager->getSceneManager();
            auto prefabManager = applicationManager->getPrefabManager();

            FB_ASSERT( fileSystem );
            FB_ASSERT( sceneManager );
            FB_ASSERT( prefabManager );

            m_vehicleActor = sceneManager->createActor();
            FB_ASSERT( m_vehicleActor );

            auto scene = sceneManager->getCurrentScene();
            FB_ASSERT( scene );

            if( scene )
            {
                scene->addActor( m_vehicleActor );
                scene->registerAllUpdates( m_vehicleActor );
            }

            f32 m_length = 4.405f;
            f32 m_width = 1.810f;
            f32 m_height = 1.170f;

            auto boxPrefab = prefabManager->loadPrefab( "cube.fbmeshbin" );
            if( boxPrefab )
            {
                auto box = boxPrefab->createActor();
                m_vehicleActor->addChild( box );

                box->setLocalScale( Vector3<real_Num>( m_width, m_height, m_length ) );
            }

            /*
            auto prefab = prefabManager->loadPrefab("F40f40.fbmeshbin");
            if (prefab)
            {
                m_vehicleMesh = prefab->createActor();
                m_vehicleMesh->setOrientation(Quaternion<real_Num>::angleAxis(-90,
            Vector3<real_Num>::unitX())); m_vehicleMesh->setLocalScale(Vector3<real_Num>::unit() * 0.01);

                auto material = m_vehicleMesh->getComponent<component::MaterialComponent>();
                if (!material)
                {
                    material = m_vehicleMesh->addComponentObject<component::MaterialComponent>();
                }

                if (material)
                {
                    //material->setMaterialName("f40");
                    //material->setDiffuse(ColourF::Blue);
                }

                m_vehicleActor->addChild(m_vehicleMesh);
            }
            */

            auto c = m_vehicleActor->addComponent<scene::CollisionBox>();
            FB_ASSERT( c );

            if( c )
            {
                c->setExtents( Vector3<real_Num>( m_width, m_height, m_length ) );
            }

            auto b = m_vehicleActor->addComponent<scene::Rigidbody>();
            FB_ASSERT( b );

            auto carcontroller = m_vehicleActor->addComponent<scene::CarController>();
            FB_ASSERT( carcontroller );

            data::actor_data vehicleData;
            vehicleData.name = "F40";

            data::actor_data vehicleDynamicsData;
            vehicleDynamicsData.name = "Dynamics";

            for( u32 i = 0; i < 4; ++i )
            {
                data::actor_data wheelPrefabData;
                wheelPrefabData.name = "Wheel";

                data::aircraft_wheel_data wheelData;
                auto pProperties = PropertiesUtil::getProperties( &wheelData );
                auto pData = pProperties->toData();
                auto propertiesData = pData->getDataAsType<data::properties>();

                data::component_data componentData;
                componentData.properties_ = *propertiesData;
                wheelPrefabData.components.push_back( componentData );

                vehicleDynamicsData.children.push_back( wheelPrefabData );
            }

            vehicleData.children.push_back( vehicleDynamicsData );

            m_vehicleActor->setPosition( Vector3<real_Num>::unitY() * 5.0f );

            auto jsonStr = DataUtil::toString( &vehicleData, true );
            fileSystem->writeAllText( "vehicle.prefab", jsonStr );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

}  // end namespace fb

int main( int argc, char *argv[] )
{
    using namespace fb;

    // Create application object
    SampleVehicle app;

#if 1
    try
    {
        app.load( nullptr );
        app.run();
        app.unload( nullptr );
    }
    catch( Exception &e )
    {
        FB_LOG_EXCEPTION( e );
    }

#else
    app.load( nullptr );
    app.run();
    app.unload( nullptr );
#endif

    return 1;
}
