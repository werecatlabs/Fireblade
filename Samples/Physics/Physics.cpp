#include "Physics.h"
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include "FBOISInput/FBOISInput.h"

#ifdef _FB_STATIC_LIB_
#    include <FBFileSystem/FBFileSystem.h>
#endif

#ifdef _FB_STATIC_LIB_
#    include <FBSQLite/FBSQLite.h>
#endif

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


    Physics::Physics()
    {
    }

    Physics::~Physics()
    {
        unload( nullptr );
    }

    void Physics::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            const auto task = Thread::Task::Primary;
            Thread::setCurrentTask( task );

            const auto threadId = Thread::ThreadId::Primary;
            Thread::setCurrentThreadId( threadId );

            auto applicationManager = fb::make_ptr<core::ApplicationManagerMT>();
            FB_ASSERT( applicationManager );

            core::IApplicationManager::setInstance( applicationManager );
            FB_ASSERT( core::IApplicationManager::instance() );

            applicationManager->setLoadingState( LoadingState::Loading );
            FB_ASSERT( applicationManager->isValid() );

            CApplicationClient::load( data );

            FB_ASSERT( applicationManager->isValid() );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            graphicsSystem->setupRenderer( m_sceneMgr, m_window, m_camera, "", true );

            auto sceneManager = applicationManager->getSceneManager();
            sceneManager->play();

            setLoadingState( LoadingState::Loaded );
            applicationManager->setLoadingState( LoadingState::Loaded );

            FB_ASSERT( sceneManager->isValid() );
            FB_ASSERT( applicationManager->isValid() );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Physics::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            const auto &loadingState = getLoadingState();
            if( loadingState != LoadingState::Unloaded )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );
                FB_ASSERT( applicationManager->isValid() );

                applicationManager->setLoadingState( LoadingState::Unloading );

                m_boxGround = nullptr;
                m_boxes.clear();

                if( m_frameStatistics )
                {
                    m_frameStatistics->unload( nullptr );
                    m_frameStatistics = nullptr;
                }

                CApplicationClient::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Physics::createScene()
    {
        // ApplicationUtil::create();

        auto &gc = GarbageCollector::instance();

        auto sceneIdx = 3;

        switch( sceneIdx )
        {
        case 0:
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto sceneManager = graphicsSystem->getSceneManager( "GameSceneManager" );
            FB_ASSERT( sceneManager );

            auto physicsManager = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsManager );

            auto resourceDatabase = applicationManager->getResourceDatabase();
            // resourceDatabase->importAssets();
            FB_ASSERT( resourceDatabase );

            auto physicsScene = physicsManager->createScene();
            FB_ASSERT( physicsScene );

            applicationManager->setPhysicsScene( physicsScene );

            sceneManager->setAmbientLight( ColourF::White );

            m_boxGround = ApplicationUtil::createDefaultGround();
            FB_ASSERT( m_boxGround );

            gc.update();
        }
        break;
        case 1:
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto prefabManager = applicationManager->getPrefabManager();
            FB_ASSERT( prefabManager );

            auto physicsMgr = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsMgr );

            auto physicsScene = physicsMgr->createScene();
            applicationManager->setPhysicsScene( physicsScene );

            auto sceneManager = applicationManager->getSceneManager();
            auto scene = sceneManager->getCurrentScene();

            m_boxGround = ApplicationUtil::createDefaultGround();

            auto prefab = sceneManager->createActor();
            auto meshComponent = prefab->addComponent<scene::Mesh>();
            if( meshComponent )
            {
                meshComponent->setMeshPath( "cube.fbmeshbin" );
            }

            auto meshRenderer = prefab->addComponent<scene::MeshRenderer>();
            auto boxCollider = prefab->addComponent<scene::CollisionBox>();
            auto rigidBody = prefab->addComponent<scene::Rigidbody>();

            auto material = prefab->addComponent<scene::Material>();
            if( material )
            {
                material->setMaterialPath( "Standard.mat" );
            }

            auto numBoxes = static_cast<size_t>( 10 );
            for( size_t x = 0; x < numBoxes; ++x )
            {
                for( size_t y = 0; y < numBoxes; ++y )
                {
                    for( size_t z = 0; z < numBoxes; ++z )
                    {
                        auto newBox = prefabManager->createInstance( prefab );

                        auto position = Vector3<real_Num>::zero();
                        position += Vector3<real_Num>::unitX() * x;
                        position += Vector3<real_Num>::unitY() * ( y + 1 );
                        position += Vector3<real_Num>::unitZ() * z;

                        newBox->setPosition( position );
                        m_boxes.push_back( newBox );

                        // scene->registerAllUpdates(newBox); // todo remove
                        scene->addActor( newBox );
                    }
                }
            }
        }
        break;
        case 2:
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto prefabManager = applicationManager->getPrefabManager();
            FB_ASSERT( prefabManager );

            auto physicsMgr = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsMgr );

            auto physicsScene = physicsMgr->createScene();
            applicationManager->setPhysicsScene( physicsScene );

            auto sceneManager = applicationManager->getSceneManager();
            auto scene = sceneManager->getCurrentScene();

            m_boxGround = ApplicationUtil::createDefaultGround();

            auto prefab = sceneManager->createActor();
            auto meshComponent = prefab->addComponent<scene::Mesh>();
            if( meshComponent )
            {
                meshComponent->setMeshPath( "cube.fbmeshbin" );
            }

            auto meshRenderer = prefab->addComponent<scene::MeshRenderer>();
            auto boxCollider = prefab->addComponent<scene::CollisionBox>();
            auto rigidBody = prefab->addComponent<scene::Rigidbody>();

            auto material = prefab->addComponent<scene::Material>();
            if( material )
            {
                material->setMaterialPath( "Standard.mat" );
            }

            u32 seed = 1;
            Math<s32>::SetRandomSeed( seed );

            auto numBoxes = static_cast<size_t>( 10 );
            for( size_t x = 0; x < numBoxes; ++x )
            {
                for( size_t y = 0; y < numBoxes; ++y )
                {
                    for( size_t z = 0; z < numBoxes; ++z )
                    {
                        auto newBox = prefabManager->createInstance( prefab );

                        auto position = Vector3<real_Num>::zero();
                        position += Vector3<real_Num>::unitX() * (real_Num)x *
                                    Math<real_Num>::RangedRandom( -10.0f, 10.0f );
                        position +=
                            Vector3<real_Num>::unitY() * y * Math<real_Num>::RangedRandom( 1.0f, 10.0f );
                        position += Vector3<real_Num>::unitZ() * (real_Num)z *
                                    Math<real_Num>::RangedRandom( -10.0f, 10.0f );

                        newBox->setPosition( position );
                        m_boxes.push_back( newBox );

                        // scene->registerAllUpdates(newBox); // todo remove
                        scene->addActor( newBox );
                    }
                }
            }
        }
        break;
        case 3:
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto prefabManager = applicationManager->getPrefabManager();
            FB_ASSERT( prefabManager );

            auto physicsMgr = applicationManager->getPhysicsManager();
            FB_ASSERT( physicsMgr );

            auto physicsScene = physicsMgr->createScene();
            applicationManager->setPhysicsScene( physicsScene );

            auto sceneManager = applicationManager->getSceneManager();
            auto scene = sceneManager->getCurrentScene();

            m_boxGround = ApplicationUtil::createDefaultGround();

            auto prefab = sceneManager->createActor();
            auto meshComponent = prefab->addComponent<scene::Mesh>();
            if( meshComponent )
            {
                meshComponent->setMeshPath( "cube.fbmeshbin" );
            }

            auto meshRenderer = prefab->addComponent<scene::MeshRenderer>();
            auto boxCollider = prefab->addComponent<scene::CollisionBox>();
            auto rigidBody = prefab->addComponent<scene::Rigidbody>();

            auto material = prefab->addComponent<scene::Material>();
            if( material )
            {
                material->setMaterialPath( "Standard.mat" );
            }

            u32 seed = 1;
            Math<s32>::SetRandomSeed( seed );

            auto numBoxes = static_cast<size_t>( 4 );
            for( size_t x = 0; x < numBoxes; ++x )
            {
                for( size_t y = 0; y < numBoxes; ++y )
                {
                    for( size_t z = 0; z < numBoxes; ++z )
                    {
                        auto newBox = prefabManager->createInstance( prefab );

                        // auto position = Vector3<real_Num>::zero();
                        // position += Vector3<real_Num>::unitX() * x *
                        // Math<real_Num>::RangedRandom(-10.0f, 10.0f); position +=
                        // Vector3<real_Num>::unitY() * y * Math<real_Num>::RangedRandom(-10.0f, 10.0f);
                        // position += Vector3<real_Num>::unitZ() * z *
                        // Math<real_Num>::RangedRandom(-10.0f, 10.0f);

                        auto position = Vector3<real_Num>::zero();
                        position += Vector3<real_Num>::unitX() * (real_Num)x;
                        position += Vector3<real_Num>::unitY() * (real_Num)( y + 1 );
                        position += Vector3<real_Num>::unitZ() * (real_Num)z;

                        newBox->setPosition( position );
                        m_boxes.push_back( newBox );

                        scene->addActor( newBox );
                    }
                }
            }
        }
        break;
        default:
        {
        }
        }

        gc.update();
    }

    void Physics::createPlugins()
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

}  // namespace fb

int main( int argc, char *argv[] )
{
    using namespace fb;

    // Create application object
    Physics app;

#if 1
    try
    {
        app.setActiveThreads( 4 );
        app.load( nullptr );
        app.run();
        app.unload( nullptr );
    }
    catch( Exception &e )
    {
        MessageBoxUtil::show( e.what() );
    }
    catch( std::exception &e )
    {
        MessageBoxUtil::show( e.what() );
    }
#else
    app.load( nullptr );
    app.run();
    app.unload( nullptr );
#endif

    return 0;
}
