#include "HelloWorld.h"
#include <FBCore/FBCore.h>
#include <FBSQLite/FBSQLite.h>
#include "FBRenderUI/FBRenderUI.h"

#if FB_GRAPHICS_SYSTEM_CLAW
#    include <FBGraphicsClaw/FBGraphicsClaw.h>
#elif FB_GRAPHICS_SYSTEM_OGRENEXT
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


    HelloWorld::HelloWorld()
    {
    }

    HelloWorld::~HelloWorld()
    {
        unload( nullptr );
    }

    void HelloWorld::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto task = Thread::Task::Primary;
            Thread::setCurrentTask( task );

            auto threadId = Thread::ThreadId::Primary;
            Thread::setCurrentThreadId( threadId );

            auto taskFlags = std::numeric_limits<u32>::max();
            Thread::setTaskFlags( taskFlags );

            auto applicationManager = fb::make_ptr<core::ApplicationManager>();
            applicationManager->load( data );
            core::ApplicationManager::setInstance( applicationManager );

            applicationManager->setApplication( this );

            Application::load( data );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            auto debug = graphicsSystem->getDebug();

            debug->drawText( 0, Vector2F::unit() * 0.5f, "Hello world!", 0 );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void HelloWorld::createScene()
    {
        // auto applicationManager = Engine::getSingletonPtr();
        // SmartPtr<IGraphicsSystem> graphicsSystem = applicationManager->getGraphicsSystem();
        // SmartPtr<render::IGraphicsSceneManager> smgr = graphicsSystem->getSceneManager("GameSceneManager");
        // smgr->setAmbientLight(ColourF::White);

        // m_box = smgr->addMesh("box.mesh");
        // m_node = smgr->getRootSceneNode()->addChildSceneNode();
        // m_node->attachObject(m_box);
        // m_node->setScale(Vector3F::unit() * 10.0f);

        // m_cameraSceneNode->setPosition(Vector3F(0,200,0));
        // m_cameraSceneNode->lookAt(Vector3F::zero());
    }

    void HelloWorld::createPlugins()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

#ifdef _FB_STATIC_LIB_
        auto corePlugin = fb::make_ptr<FBCore>();
        applicationManager->addPlugin( corePlugin );
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
    }

}  // namespace fb
