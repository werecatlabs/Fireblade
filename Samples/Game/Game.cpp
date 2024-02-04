#include "Game.h"
#include "Types.h"
#include <FBCore/FBCore.h>
#include <FBAero/FBAero.h>
#include <FBOISInput/FBOISInput.h>
#include "FBSQLite/FBSQLite.h"

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

    Game::Game()
    {
    }

    Game::~Game()
    {
        unload( nullptr );
    }

    void Game::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto task = Thread::Task::Primary;
            Thread::setCurrentTask( task );

            auto threadId = Thread::ThreadId::Primary;
            Thread::setCurrentThreadId( threadId );

            auto applicationManager = fb::make_ptr<core::ApplicationManager>();
            core::ApplicationManager::setInstance( applicationManager );

            Application::load( data );

            auto factoryManager = applicationManager->getFactoryManager();

            // auto logManager = LogManager::instance();
            // logManager->open("Sample.log");

            //auto physicsManager = applicationManager->getPhysicsManager();

            //auto sceneManager = fb::make_ptr<scene::CSceneManager>();
            //applicationManager->setSceneManager( sceneManager );
            //sceneManager->load( nullptr );

            //auto scene = fb::make_ptr<scene::CScene>();
            //sceneManager->setCurrentScene( scene );
            //scene->load( nullptr );

            //auto prefabManager = fb::make_ptr<scene::CPrefabManager>();
            //applicationManager->setPrefabManager( prefabManager );

            // m_cameraActor = ApplicationUtil::createCamera();

            // auto vehicleManager = fb::make_ptr<CAeroManager>();
            // applicationManager->setAeroManager(vehicleManager);

            // createScene();
            // createAero();

            /*
            for (size_t i=0; i<40; ++i)
            {
                auto newAero = prefabManager->createInstance(m_vehicleActor);
                scene->registerAllUpdates(newAero);
                scene->addActor(newAero);
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

            // auto graphicsSystem = applicationManager->getGraphicsSystem();
            // graphicsSystem->setupRenderer(m_sceneMgr, m_window, m_camera, "", true);

            setLoadingState( LoadingState::Loaded );
            applicationManager->setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Game::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            applicationManager->unload( nullptr );
            core::ApplicationManager::setInstance( nullptr );
            applicationManager = nullptr;
            FB_ASSERT( core::ApplicationManager::instance() == nullptr );

            setLoadingState( LoadingState::Unloaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Game::createPlugins()
    {
        try
        {
            FB_DEBUG_TRACE;

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

#ifdef _FB_STATIC_LIB_
            auto corePlugin = fb::make_ptr<FBCore>();
            corePlugin->load( nullptr );
#endif

#ifdef _FB_STATIC_LIB_
            //auto applicationPlugin = fb::make_ptr<ApplicationPlugin>();
            //applicationManager->addPlugin( applicationPlugin );
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

            //ApplicationUtil::createFactories();
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
    Game app;

    try
    {
        app.load( nullptr );
        app.run();
        app.unload( nullptr );
    }
    catch( Exception &e )
    {
        std::cout << e.what() << std::endl;
    }

    return 1;
}
