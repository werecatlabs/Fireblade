#include "SampleVehicle.h"
#include "Types.h"
#include <FBCore/FBCore.h>
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

            auto applicationManager = fb::make_ptr<core::ApplicationManager>();
            core::ApplicationManager::setInstance( applicationManager );

            Application::load( data );

            m_inputListener = fb::make_ptr<InputListener>();
            m_inputListener->setOwner( this );

            auto inputManager = applicationManager->getInputDeviceManager();
            inputManager->addListener( m_inputListener );

            auto taskManager = applicationManager->getTaskManager();
            auto physicsTask = taskManager->getTask( Thread::Task::Physics );
            physicsTask->setTargetFPS( 300.0 );

            auto sceneManager = applicationManager->getSceneManager();
            sceneManager->play();

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
        m_boxGround = nullptr;
        m_terrain = nullptr;

        m_cameraActor = nullptr;
        m_vehicleActor = nullptr;
        m_vehicleMesh = nullptr;
        m_vehicleController = nullptr;

        m_boxes.clear();

        Application::unload( data );
    }

    void SampleVehicle::reset()
    {
        if( m_vehicleActor )
        {
            m_vehicleActor->setPosition( Vector3<real_Num>::unitY() * 5.0f );            
        }
    }

    void SampleVehicle::createPlugins()
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

            ApplicationUtil::createFactories();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void SampleVehicle::createScene()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        auto sceneManager = applicationManager->getSceneManager();
        auto prefabManager = applicationManager->getPrefabManager();
        auto scene = sceneManager->getCurrentScene();

        auto factory = applicationManager->getFactoryManager();
        auto physicsMgr = applicationManager->getPhysicsManager();

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto smgr = graphicsSystem->getGraphicsScene( "GameSceneManager" );
        smgr->setAmbientLight( ColourF::White );

        ApplicationUtil::createDefaultSky();
        m_terrain = ApplicationUtil::createDefaultTerrain();
        //m_boxGround = ApplicationUtil::createDefaultGround();

        m_vehicleActor = ApplicationUtil::createDefaultVehicle();

        m_vehicleActor->setPosition( Vector3<real_Num>::unitY() * 5.0f );

        m_cameraActor = sceneManager->createActor();
        auto camera = m_cameraActor->addComponent<scene::Camera>();
        camera->setActive( true );

        auto cameraController = m_cameraActor->addComponent<scene::VehicleCameraController>();
        cameraController->setDistance( 8.0f );

        scene->addActor( m_cameraActor );
        scene->registerAllUpdates( m_cameraActor );
    }

    void SampleVehicle::InputListener::unload( SmartPtr<ISharedObject> data )
    {
        m_owner = nullptr;
    }

    Parameter SampleVehicle::InputListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                         const Array<Parameter> &arguments,
                                                         SmartPtr<ISharedObject> sender,
                                                         SmartPtr<ISharedObject> object,
                                                         SmartPtr<IEvent> event )
    {
        if( eventValue == IEvent::inputEvent )
        {
            auto result = inputEvent( event );
            return Parameter( result );
        }

        return Parameter();
    }

    bool SampleVehicle::InputListener::inputEvent( SmartPtr<IInputEvent> event )
    {
        if( auto owner = getOwner() )
        {
            switch( auto eventType = event->getEventType() )
            {
            case IInputEvent::EventType::Key:
            {
                if( auto keyboardState = event->getKeyboardState() )
                {
                    if( keyboardState->isPressedDown() )
                    {
                        if( keyboardState->getKeyCode() == static_cast<u32>( KeyCodes::KEY_KEY_R ) )
                        {
                            owner->reset();
                            return true;
                        }
                    }
                }
            }
            };
        }

        return false;
    }

    bool SampleVehicle::InputListener::updateEvent( const SmartPtr<IInputEvent> &event )
    {
        return false;
    }

    void SampleVehicle::InputListener::setPriority( s32 priority )
    {
        m_priority = priority;
    }

    s32 SampleVehicle::InputListener::getPriority() const
    {
        return m_priority;
    }

    SmartPtr<SampleVehicle> SampleVehicle::InputListener::getOwner() const
    {
        return m_owner;
    }

    void SampleVehicle::InputListener::setOwner( SmartPtr<SampleVehicle> owner )
    {
        m_owner = owner;
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
        const auto threads = Thread::hardware_concurrency();
        app.setActiveThreads( threads );
        //app.setActiveThreads( 0 );

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
