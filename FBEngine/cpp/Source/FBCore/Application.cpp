#include <FBCore/FBCorePCH.h>
#include <FBCore/Application.h>
#include <FBCore/FBCore.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Scene/Director.h>
#include <FBCore/Scene/Components/CarController.h>
#include <FBCore/Scene/Components/ParticleSystem.h>
#include <FBCore/Scene/Components/WheelController.h>
#include <FBCore/System/PluginManager.h>

namespace fb::core
{

    FB_CLASS_REGISTER_DERIVED( fb::editor, core::Application, core::IApplication );

    Application::Application()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "Application" );
        }
    }

    Application::~Application()
    {
        unload( nullptr );
    }

    void Application::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto currentThreadId = Thread::ThreadId::Primary;
            Thread::setCurrentThreadId( currentThreadId );

            auto task = Thread::Task::Primary;
            Thread::setCurrentTask( task );

            auto taskFlags = std::numeric_limits<u32>::max();
            Thread::setTaskFlags( taskFlags );

            auto typeManager = new TypeManager();
            TypeManager::setInstance( typeManager );

            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            applicationManager->setTypeManager( typeManager );

            createLogManager();
            FB_ASSERT( applicationManager->isValid() );

            FB_DEBUG_TRACE;

            createFactoryManager();
            FB_ASSERT( applicationManager->isValid() );

            createPluginManager();
            FB_ASSERT( applicationManager->isValid() );

            createPlugins();
            FB_ASSERT( applicationManager->isValid() );

            createFsmManager();
            FB_LOG( "Fsm manager created." );
            FB_ASSERT( applicationManager->isValid() );

            createFsm();
            FB_ASSERT( applicationManager->isValid() );

            createComponentsContainer();
            FB_ASSERT( applicationManager->isValid() );

            createPlatformManager();
            FB_ASSERT( applicationManager->isValid() );

            createProcessManager();

            createStateManager();
            FB_ASSERT( applicationManager->isValid() );

            createOutputManager();
            FB_ASSERT( applicationManager->isValid() );

            createTimer();
            FB_ASSERT( applicationManager->isValid() );

            createProfiler();
            FB_ASSERT( applicationManager->isValid() );

            createThreadPool();
            FB_ASSERT( applicationManager->isValid() );

            createTaskManager();
            FB_ASSERT( applicationManager->isValid() );

            createTasks();
            FB_ASSERT( applicationManager->isValid() );

            createJobQueue();
            FB_ASSERT( applicationManager->isValid() );

            createFileSystem();
            FB_LOG( "FileSystem created." );
            FB_ASSERT( applicationManager->isValid() );

            createSoundManager();
            FB_LOG( "SoundManager created." );
            FB_ASSERT( applicationManager->isValid() );

            createScriptManager();
            FB_LOG( "ScriptManager created." );
            FB_ASSERT( applicationManager->isValid() );

            createSceneManager();
            FB_LOG( "Create scene manager." );
            FB_ASSERT( applicationManager->isValid() );

            if( !createGraphicsSystem() )
            {
                FB_LOG( "GraphicsSystem not created." );
                FB_ASSERT( applicationManager->isValid() );
            }
            else
            {
                FB_LOG( "GraphicsSystem created." );
                FB_ASSERT( applicationManager->isValid() );
            }

            loadGraphicsResources();
            FB_LOG( "Resources loaded." );
            FB_ASSERT( applicationManager->isValid() );

            createGraphicsSceneManager();
            FB_LOG( "SceneManager(s) created." );
            FB_ASSERT( applicationManager->isValid() );

            createCamera();
            FB_LOG( "Cameras created." );
            FB_ASSERT( applicationManager->isValid() );

            createViewports();
            FB_LOG( "Viewports created." );
            FB_ASSERT( applicationManager->isValid() );

            createInputSystem();
            FB_LOG( "Create input system." );
            FB_ASSERT( applicationManager->isValid() );

            createUI();
            FB_LOG( "UI created." );
            FB_ASSERT( applicationManager->isValid() );

            createRenderUI();
            FB_LOG( "RenderUI created." );
            FB_ASSERT( applicationManager->isValid() );

            setupRenderpipeline();
            FB_LOG( "Renderpipeline created." );
            FB_ASSERT( applicationManager->isValid() );

            createPhysics();
            FB_LOG( "Physics created." );
            FB_ASSERT( applicationManager->isValid() );

            createRenderWindow();
            FB_LOG( "RenderWindow created." );
            FB_ASSERT( applicationManager->isValid() );

            FB_LOG( "Finished creating base components." );

            createPrefabManager();

            createDefaultMaterialUI();
            createDefaultMaterial();
            createDefaultMaterials();
            createDefaultFont();

            loadScripts();

            createScene();
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Application::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( isLoaded() )
            {
                FB_DEBUG_TRACE;
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = ApplicationManager::instance();
                FB_ASSERT( applicationManager );
                FB_ASSERT( applicationManager->isValid() );

                m_fsm = nullptr;

                m_window = nullptr;
                m_sceneMgr = nullptr;
                m_camera = nullptr;
                m_cameraSceneNode = nullptr;
                m_viewport = nullptr;

                m_frameStatistics = nullptr;

                applicationManager->unload( nullptr );
                applicationManager->setLoadingState( LoadingState::Unloaded );
                ApplicationManager::setInstance( nullptr );
                applicationManager = nullptr;
                FB_ASSERT( core::ApplicationManager::instance() == nullptr );

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Application::run()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto threadPool = applicationManager->getThreadPool();
        auto taskManager = applicationManager->getTaskManager();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto stateManager = applicationManager->getStateManager();
        auto physicsManager = applicationManager->getPhysicsManager();
        auto inputManager = applicationManager->getInputDeviceManager();

        auto fsmManager = applicationManager->getFsmManager();
        FB_ASSERT( fsmManager );

        auto timer = applicationManager->getTimer();

        applicationManager->setPlaying( true );

        auto task = Thread::Task::Primary;
        Thread::setCurrentTask( task );

        auto threadId = Thread::ThreadId::Primary;
        Thread::setCurrentThreadId( threadId );

        timer->reset();
        timer->setTimeSinceLevelLoad( 0.0 );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        scene->setState( scene::IScene::State::Play );
        taskManager->setState( ITaskManager::State::FreeStep );
        threadPool->setState( IThreadPool::State::Start );

        while( applicationManager->isRunning() )
        {
            taskManager->update();
            Thread::yield();
        }

        // clean up
        destroyScene();
    }

    void Application::update()
    {
        auto pApplicationManager = ApplicationManager::instance();
        auto applicationManager = pApplicationManager.get();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        FB_ASSERT( stateManager );

        auto taskManager = applicationManager->getTaskManager();
        FB_ASSERT( taskManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto fsmManager = applicationManager->getFsmManager();
        FB_ASSERT( fsmManager );

        auto timer = applicationManager->getTimer();
        FB_ASSERT( timer );

        if( sceneManager )
        {
            sceneManager->preUpdate();
        }

        stateManager->preUpdate();
        stateManager->update();
        stateManager->postUpdate();

        if( m_frameStatistics )
        {
            m_frameStatistics->update();
        }

        if( sceneManager )
        {
            sceneManager->update();
        }

        switch( auto task = Thread::getCurrentTask() )
        {
        case Thread::Task::Ai:
        {
        }
        break;
        case Thread::Task::Animation:
        {
        }
        break;
        case Thread::Task::Application:
        {
        }
        break;
        case Thread::Task::Collision:
        {
        }
        break;
        case Thread::Task::Controls:
        {
        }
        break;
        case Thread::Task::Dynamics:
        {
        }
        break;
        case Thread::Task::GarbageCollect:
        {
            try
            {
                auto timer = applicationManager->getTimer();

                auto dt = timer->getDeltaTime();
                auto t = timer->getTime();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
        break;
        case Thread::Task::Physics:
        {
            try
            {
                auto physicsManager = applicationManager->getPhysicsManager();
                //FB_ASSERT( physicsManager );

                auto dt = timer->getDeltaTime();
                FB_ASSERT( Math<time_interval>::isFinite( dt ) );

                auto t = timer->getTime();
                FB_ASSERT( Math<time_interval>::isFinite( t ) );

                if( timer->getTimeSinceLevelLoad() > 3.0 )
                {
                    if( physicsManager )
                    {
                        physicsManager->preUpdate();
                        physicsManager->update();
                        physicsManager->postUpdate();
                    }

                    auto physicsScene = applicationManager->getPhysicsScene();
                    if( physicsScene )
                    {
                        physicsScene->preUpdate();
                        physicsScene->update();
                        physicsScene->postUpdate();
                    }

                    if( auto vehicleManager = applicationManager->getVehicleManager() )
                    {
                        vehicleManager->preUpdate();
                        vehicleManager->update();
                        vehicleManager->postUpdate();
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
        break;
        case Thread::Task::Primary:
        {
            try
            {
                auto fsmManager = applicationManager->getFsmManager();
                FB_ASSERT( fsmManager );

                auto dt = timer->getDeltaTime();
                FB_ASSERT( Math<time_interval>::isFinite( dt ) );

                auto t = timer->getTime();
                FB_ASSERT( Math<time_interval>::isFinite( t ) );

                if( auto inputManager = applicationManager->getInputDeviceManager() )
                {
                    inputManager->update();
                }

                timer->addTimeSinceLevelLoad( dt );

                fsmManager->update();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
        break;
        case Thread::Task::Render:
        {
            try
            {
                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                graphicsSystem->update();

                graphicsSystem->messagePump();
                if( applicationManager->getQuit() )
                {
                    applicationManager->setRunning( false );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }
        break;
        case Thread::Task::Sound:
        {
        }
        break;
        default:
        {
        }
        break;
        }

        if( sceneManager )
        {
            sceneManager->postUpdate();
        }
    }

    auto Application::getFSM() const -> SmartPtr<IFSM>
    {
        return m_fsm;
    }

    void Application::setFSM( SmartPtr<IFSM> fsm )
    {
        m_fsm = fsm;
    }

    auto Application::createGraphicsSystem() -> bool
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );
            FB_ASSERT( factoryManager->isValid() );

            if( auto graphicsSystem = factoryManager->make_object<render::IGraphicsSystem>() )
            {
                FB_ASSERT( graphicsSystem );
                FB_ASSERT( graphicsSystem->isValid() );

                applicationManager->setGraphicsSystem( graphicsSystem );
                FB_ASSERT( applicationManager->getGraphicsSystem() );
                FB_ASSERT( applicationManager->isValid() );

                graphicsSystem->load( nullptr );

                FB_ASSERT( applicationManager->isValid() );

                if( !graphicsSystem->configure( nullptr ) )
                {
                    return false;
                }

                FB_ASSERT( applicationManager->isValid() );

                auto window = graphicsSystem->getDefaultWindow();
                FB_ASSERT( window );
                applicationManager->setWindow( window );

                FB_ASSERT( applicationManager->isValid() );

                // todo refactor
                auto resourceGroupManager = graphicsSystem->getResourceGroupManager();
                FB_ASSERT( resourceGroupManager );
                resourceGroupManager->load( nullptr );

                auto resourceDatabase = fb::make_ptr<ResourceDatabase>();
                applicationManager->setResourceDatabase( resourceDatabase );
                resourceDatabase->load( nullptr );

                createDefaultFont();

                m_frameStatistics = fb::make_ptr<FrameStatistics>();
                m_frameStatistics->load( nullptr );

                return true;
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return false;
    }

    void Application::createDefaultFont()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();

        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto director = fb::make_ptr<scene::Director>();
        director->load( nullptr );

        auto fontFileName = "cuckoo.ttf";
        auto directorProperties = director->getProperties();
        directorProperties->setProperty( "font_type", String( "arial" ) );
        directorProperties->setProperty( "font_source", String( fontFileName ) );
        directorProperties->setProperty( "font_size", 12 );
        directorProperties->setProperty( "font_resolution", 96 );

        if( fileSystem->isExistingFile( fontFileName, true, true ) )
        {
            auto fontName = String( "default" );
            auto result =
                resourceDatabase->createOrRetrieveFromDirector<render::IFont>( fontName, director );
            if( result.first && result.second )
            {
                auto font = result.first;
                font->setProperties( directorProperties );
                font->load( nullptr );
            }
        }
    }

    void Application::createRenderWindow()
    {
    }

    void Application::createGraphicsSceneManager()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
        {
            FB_ASSERT( graphicsSystem );

            auto name = String( "DefaultSceneManager" );
            auto type = String( "GameSceneManager" );

            auto smgr = graphicsSystem->addGraphicsScene( name, type );
            FB_ASSERT( smgr );

            m_sceneMgr = smgr;
        }
    }

    void Application::loadGraphicsResources()
    {
    }

    void Application::setupRenderpipeline()
    {
    }

    void Application::createPhysics()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );
        FB_ASSERT( factoryManager->isValid() );

        auto physicsManager = factoryManager->make_object<physics::IPhysicsManager>();
        if( physicsManager )
        {
            FB_ASSERT( physicsManager->isValid() );

            applicationManager->setPhysicsManager( physicsManager );

            physicsManager->load( nullptr );
        }
    }

    void Application::createInputSystem()
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            if( auto inputManager = factoryManager->make_object<IInputDeviceManager>() )
            {
                applicationManager->setInputDeviceManager( inputManager );

                if( auto window = graphicsSystem->getDefaultWindow() )
                {
                    inputManager->setWindow( window );

#if defined FB_PLATFORM_WIN32
                    inputManager->setCreateMouse( true );
                    inputManager->setCreateKeyboard( true );
#elif defined FB_PLATFORM_APPLE
                    inputManager->setCreateMouse( false );
                    inputManager->setCreateKeyboard( false );
#endif

                    inputManager->load( nullptr );

                    auto hash = StringUtil::getHash( "gameInput0" );
                    auto gameInput0 = inputManager->addGameInput( hash );
                    gameInput0->setPlayerIndex( 0 );
                }
                else
                {
                    FB_LOG( "Could not initialise input system. No window found." );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Application::createSceneManager()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );
        FB_ASSERT( factoryManager->isValid() );

        auto sceneManager = factoryManager->make_object<scene::ISceneManager>();
        FB_ASSERT( sceneManager );
        FB_ASSERT( sceneManager->isValid() );

        applicationManager->setSceneManager( sceneManager );
        sceneManager->load( nullptr );
        FB_ASSERT( sceneManager->isValid() );

        auto scene = factoryManager->make_object<scene::IScene>();
        scene->load( nullptr );
        scene->setLabel( "Application" );
        sceneManager->setCurrentScene( scene );
        FB_ASSERT( scene->isValid() );
    }

    void Application::createCamera()
    {
        if( m_sceneMgr )
        {
            FB_ASSERT( m_sceneMgr );
            FB_ASSERT( m_sceneMgr->isValid() );

            const auto cameraName = String( "DefaultCamera" );
            m_camera = m_sceneMgr->addCamera( cameraName );
            FB_ASSERT( m_camera );

            auto rootNode = m_sceneMgr->getRootSceneNode();
            FB_ASSERT( rootNode );

            m_cameraSceneNode = rootNode->addChildSceneNode( cameraName );
            FB_ASSERT( m_cameraSceneNode );

            m_cameraSceneNode->attachObject( m_camera );

            auto cameraPosition = Vector3<real_Num>::zero();
            cameraPosition += Vector3<real_Num>::unitY() * 5.0;
            cameraPosition += Vector3<real_Num>::unitZ() * 20.0;
            m_cameraSceneNode->setPosition( cameraPosition );

            m_camera->setNearClipDistance( 0.01f );
            m_camera->setFarClipDistance( 1000.0f );

            m_camera->setVisible( true );

            FB_ASSERT( m_sceneMgr->isValid() );
            FB_ASSERT( m_cameraSceneNode->isValid() );
            FB_ASSERT( m_camera->isValid() );
        }
    }

    auto Application::createDefaultCamera( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto actor = sceneManager->createActor();

        auto name = String( "Camera" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::Camera>();
        FB_ASSERT( c );

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    void Application::createRigidStaticMesh()
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                    createRigidStaticMesh( actor, true );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Application::createRigidDynamicMesh()
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto selectionManager = applicationManager->getSelectionManager();
            FB_ASSERT( selectionManager );

            auto selection = selectionManager->getSelection();
            for( auto selected : selection )
            {
                if( selected->isDerived<scene::IActor>() )
                {
                    auto actor = fb::static_pointer_cast<scene::IActor>( selected );
                    createRigidDynamicMesh( actor, true );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Application::getResourceType( const String &resourceTypeName ) -> IResource::ResourceType
    {
        if( resourceTypeName == "None" )
        {
            return IResource::ResourceType::None;
        }
        if( resourceTypeName == "Material" )
        {
            return IResource::ResourceType::Material;
        }
        if( resourceTypeName == "Object" )
        {
            return IResource::ResourceType::Object;
        }
        if( resourceTypeName == "Actor" )
        {
            return IResource::ResourceType::Actor;
        }
        if( resourceTypeName == "Component" )
        {
            return IResource::ResourceType::Component;
        }
        if( resourceTypeName == "Texture" )
        {
            return IResource::ResourceType::Texture;
        }

        return IResource::ResourceType::None;
    }

    auto Application::getResourceTypeByName( IResource::ResourceType resourceType ) -> String
    {
        switch( resourceType )
        {
        case IResource::ResourceType::None:
            return "None";
        case IResource::ResourceType::Material:
            return "Material";
        case IResource::ResourceType::Object:
            return "Object";
        case IResource::ResourceType::Actor:
            return "Actor";
        case IResource::ResourceType::Component:
            return "Component";
        case IResource::ResourceType::Texture:
            return "Texture";
        }

        return "";
    }

    auto Application::importScene( const String &filePath ) -> SmartPtr<Properties>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto fileSystem = applicationManager->getFileSystem();
        FB_ASSERT( fileSystem );

        auto dataStr = fileSystem->readAllText( filePath );

        auto pScene = fb::make_ptr<Properties>();

        DataUtil::parse( dataStr, pScene.get() );

        return pScene;
    }

    auto Application::createDefaultCubemap( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );

            auto scene = sceneManager->getCurrentScene();
            FB_ASSERT( scene );
            FB_ASSERT( scene->isValid() );

            auto actor = sceneManager->createActor();
            FB_ASSERT( actor );

            auto name = String( "Cubemap" );
            actor->setName( name );

            auto c = actor->addComponent<scene::CollisionBox>();
            FB_ASSERT( c );

            auto cubemap = actor->addComponent<scene::Cubemap>();
            FB_ASSERT( cubemap );

            auto meshComponent = actor->addComponent<scene::Mesh>();
            FB_ASSERT( meshComponent );
            meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

            auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
            FB_ASSERT( meshRenderer );

            auto material = actor->addComponent<scene::Material>();
            FB_ASSERT( material );
            FB_ASSERT( material->isValid() );

            if( material )
            {
                material->setMaterialPath( "Standard.mat" );
            }

            if( addToScene )
            {
                scene->addActor( actor );
                FB_ASSERT( scene->isValid() );
            }

            return actor;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto Application::createDefaultCube( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );

            auto scene = sceneManager->getCurrentScene();
            FB_ASSERT( scene );
            FB_ASSERT( scene->isValid() );

            auto actor = sceneManager->createActor();
            FB_ASSERT( actor );

            const auto name = String( "Cube" );
            actor->setName( name );

            auto c = actor->addComponent<scene::CollisionBox>();
            FB_ASSERT( c );

            auto rigidbody = actor->addComponent<scene::Rigidbody>();
            FB_ASSERT( rigidbody );

            auto meshComponent = actor->addComponent<scene::Mesh>();
            FB_ASSERT( meshComponent );
            meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

            auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
            FB_ASSERT( meshRenderer );

            auto material = actor->addComponent<scene::Material>();
            FB_ASSERT( material );
            FB_ASSERT( material->isValid() );

            if( material )
            {
                material->setMaterialPath( "Standard.mat" );
            }

            if( addToScene )
            {
                scene->addActor( actor );
                FB_ASSERT( scene->isValid() );
            }

            return actor;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto Application::createDefaultCubeMesh( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );
            FB_ASSERT( sceneManager->isValid() );

            auto scene = sceneManager->getCurrentScene();
            FB_ASSERT( scene );
            FB_ASSERT( scene->isValid() );

            auto actor = sceneManager->createActor();
            FB_ASSERT( actor );

            const auto name = String( "Cube Mesh" );
            actor->setName( name );

            auto c = actor->addComponent<scene::CollisionMesh>();
            FB_ASSERT( c );

            auto rigidbody = actor->addComponent<scene::Rigidbody>();
            FB_ASSERT( rigidbody );

            auto meshComponent = actor->addComponent<scene::Mesh>();
            FB_ASSERT( meshComponent );
            meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

            auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
            FB_ASSERT( meshRenderer );

            auto material = actor->addComponent<scene::Material>();
            FB_ASSERT( material );
            FB_ASSERT( material->isValid() );

            if( material )
            {
                material->setMaterialPath( "Standard.mat" );
            }

            if( addToScene )
            {
                scene->addActor( actor );
                FB_ASSERT( scene->isValid() );
            }

            return actor;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return nullptr;
    }

    auto Application::createDefaultGround( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        actor->setStatic( true );

        auto name = String( "Ground" );
        actor->setName( name );

        auto collisionBox = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( collisionBox );
        if( collisionBox )
        {
            collisionBox->setExtents( Vector3<real_Num>::unit() * 500.0f );
        }

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
        FB_ASSERT( meshRenderer );

        auto material = actor->addComponent<scene::Material>();
        FB_ASSERT( material );

        if( material )
        {
            material->setMaterialPath( "Standard.mat" );
            material->updateMaterial();
        }

        auto groundPosition = Vector3<real_Num>::unitY() * static_cast<real_Num>( -250.0 );
        // groundPosition += Vector3<real_Num>::unitZ() * -250.0f;

        auto groundScale = Vector3<real_Num>::unit() * static_cast<real_Num>( 500.0 );

        auto transform = actor->getTransform();
        if( transform )
        {
            transform->setLocalPosition( groundPosition );
            transform->setLocalScale( groundScale );
        }

        if( addToScene )
        {
            scene->addActor( actor );
            FB_ASSERT( scene->isValid() );
        }

        return actor;
    }

    auto Application::createDefaultTerrain( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        actor->setStatic( true );

        auto name = String( "Terrain" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        // todo remove
        // for test the renderer
        auto terrain = actor->addComponent<scene::TerrainSystem>();

        auto terrainCollision = actor->addComponent<scene::CollisionTerrain>();

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        return actor;
    }

    auto Application::createDefaultConstraint() -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        actor->setStatic( true );

        auto name = String( "Constraint" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto terrain = actor->addComponent<scene::Constraint>();

        scene->addActor( actor );
        scene->registerAllUpdates( actor );

        return actor;
    }

    auto Application::createDirectionalLight( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Directional Light" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        // todo remove
        // for test the renderer
        auto light = actor->addComponent<scene::Light>();
        if( light )
        {
            light->setLightType( render::ILight::LightTypes::LT_DIRECTIONAL );
        }

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    auto Application::createPointLight( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Main Light" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        // todo remove
        // for test the renderer
        auto light = actor->addComponent<scene::Light>();
        if( light )
        {
            light->setLightType( render::ILight::LightTypes::LT_POINT );
        }

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    auto Application::createDefaultPlane( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Plane" );
        actor->setName( name );

        auto localOrientation = Quaternion<real_Num>::angleAxis(
            Math<real_Num>::DegToRad( static_cast<real_Num>( -90.0 ) ), Vector3<real_Num>::unitX() );
        actor->setLocalOrientation( localOrientation );
        actor->setLocalScale( Vector3<real_Num>::unit() * static_cast<real_Num>( 100.0 ) );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( c );

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( "plane.fbmeshbin" );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();

        auto material = actor->addComponent<scene::Material>();
        FB_ASSERT( material );

        if( material )
        {
            material->setMaterialPath( "Standard.mat" );
            material->updateMaterial();
        }

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    auto Application::createDefaultVehicle( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto prefabManager = applicationManager->getPrefabManager();
        FB_ASSERT( prefabManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        f32 wheelBase = 2.530f;
        f32 length = 4.405f;
        f32 width = 1.810f;
        f32 height = 1.170f;
        f32 mass = 1370.0f;

        auto actor = sceneManager->createActor();

        auto name = String( "Vehicle" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( c );

        auto bodyDimensions = Vector3<real_Num>( length, height, width ) * 0.5f;
        c->setExtents( bodyDimensions );

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto vehicle = actor->addComponent<scene::CarController>();
        FB_ASSERT( vehicle );

        auto cubeMeshPath = String( "cube_internal.fbmeshbin" );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( cubeMeshPath );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
        FB_ASSERT( meshRenderer );

        auto material = actor->addComponent<scene::Material>();
        FB_ASSERT( material );

        if( material )
        {
            material->setMaterialPath( "Standard.mat" );
            material->updateMaterial();
        }

        auto meshActor = sceneManager->createActor();

        auto meshActorName = String( "mesh" );
        meshActor->setName( meshActorName );

        actor->addChild( meshActor );

        Array<Vector3<real_Num>> wheelPositions;
        wheelPositions.resize( 4 );

        auto wheelOffset = -1.0f;
        wheelPositions[static_cast<u32>( scene::CarController::Wheels::FRONT_LEFT )] =
            Vector3<real_Num>( -width / 2.0f, wheelOffset, wheelBase / 2.0f );
        wheelPositions[static_cast<u32>( scene::CarController::Wheels::FRONT_RIGHT )] =
            Vector3<real_Num>( width / 2.0f, wheelOffset, wheelBase / 2.0f );
        wheelPositions[static_cast<u32>( scene::CarController::Wheels::REAR_LEFT )] =
            Vector3<real_Num>( -width / 2.0f, wheelOffset, -wheelBase / 2.0f );
        wheelPositions[static_cast<u32>( scene::CarController::Wheels::REAR_RIGHT )] =
            Vector3<real_Num>( width / 2.0f, wheelOffset, -wheelBase / 2.0f );

        auto wheelMeshSize = Vector3<real_Num>( 0.1f, 0.3f, 0.3f );

        for( u32 i = 0; i < 4; ++i )
        {
            auto wheelMeshActor = sceneManager->createActor();

            auto wheelMeshActorName = String( "wheel_" ) + StringUtil::toString( i );
            wheelMeshActor->setName( wheelMeshActorName );

            auto wheelMeshComponent = wheelMeshActor->addComponent<scene::Mesh>();
            FB_ASSERT( wheelMeshComponent );
            wheelMeshComponent->setMeshPath( cubeMeshPath );

            auto meshRenderer = wheelMeshActor->addComponent<scene::MeshRenderer>();
            FB_ASSERT( meshRenderer );

            auto material = wheelMeshActor->addComponent<scene::Material>();
            FB_ASSERT( material );

            meshActor->addChild( wheelMeshActor );

            wheelMeshActor->setLocalPosition( wheelPositions[i] );
            wheelMeshActor->setLocalScale( wheelMeshSize );
        }

        auto dynamicsActor = sceneManager->createActor();

        auto dynamicsName = String( "dynamics" );
        dynamicsActor->setName( dynamicsName );

        // auto dynamicsId = StringUtil::getHash(dynamicsName);
        // dynamicsActor->setId(dynamicsId);

        // auto dynamicsUuid = StringUtil::getUUID();
        // dynamicsActor->setUUID(dynamicsUuid);

        actor->addChild( dynamicsActor );

        for( u32 i = 0; i < 4; ++i )
        {
            auto wheelActor = sceneManager->createActor();
            dynamicsActor->addChild( wheelActor );

            auto wheelName = String( "Wheel" );
            wheelActor->setName( wheelName );

            auto wheel = wheelActor->addComponent<scene::WheelController>();
            FB_ASSERT( wheel );
        }

        // auto prefab = prefabManager->loadPrefab("f40.fbx");
        // if (prefab)
        //{
        //	auto vehicleMesh = prefab->createActor();
        //
        //	//auto vehicleMeshTransform = vehicleMesh->getLocalTransform();
        //	//if (vehicleMeshTransform)
        //	//{
        //	//	vehicleMeshTransform->setScale(Vector3<real_Num>::unit() * 0.0254f);
        //	//}

        //	vehicleMesh->setName("F40");
        //	actor->addChild(vehicleMesh);
        //}

        auto groundPosition = Vector3<real_Num>::unitY() * static_cast<real_Num>( 5.0 );
        auto groundScale = Vector3<real_Num>::unit() * static_cast<real_Num>( 1.0 );

        auto transform = actor->getTransform();
        if( transform )
        {
            transform->setLocalPosition( groundPosition );
            transform->setLocalScale( groundScale );
        }

        actor->updateTransform();

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        return actor;
    }

    auto Application::createDefaultCar( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto prefabManager = applicationManager->getPrefabManager();
        FB_ASSERT( prefabManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Car" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( c );

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto vehicle = actor->addComponent<scene::CarController>();
        FB_ASSERT( vehicle );

        auto cubeMeshPath = String( "cube_internal.fbmeshbin" );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( cubeMeshPath );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
        FB_ASSERT( meshRenderer );

        auto material = actor->addComponent<scene::Material>();
        FB_ASSERT( material );

        if( material )
        {
            material->setMaterialPath( "Standard.mat" );
            material->updateMaterial();
        }

        auto dynamicsActor = sceneManager->createActor();

        auto dynamicsName = String( "dynamics" );
        dynamicsActor->setName( dynamicsName );

        // auto dynamicsId = StringUtil::getHash(dynamicsName);
        // dynamicsActor->setId(dynamicsId);

        // auto dynamicsUuid = StringUtil::getUUID();
        // dynamicsActor->setUUID(dynamicsUuid);

        actor->addChild( dynamicsActor );

        for( u32 i = 0; i < 4; ++i )
        {
            auto wheelActor = sceneManager->createActor();
            dynamicsActor->addChild( wheelActor );

            auto wheelName = String( "Wheel" );
            wheelActor->setName( wheelName );

            auto wheel = wheelActor->addComponent<scene::WheelController>();
            FB_ASSERT( wheel );
        }

        auto rigActor = sceneManager->createActor();

        auto rigName = String( "rig" );
        rigActor->setName( rigName );
        actor->addChild( rigActor );

        //auto prefab = prefabManager->loadPrefab( "f40.fbx" );
        //if( prefab )
        //{
        //    auto vehicleMesh = prefab->createActor();

        //    // auto vehicleMeshTransform = vehicleMesh->getLocalTransform();
        //    // if (vehicleMeshTransform)
        //    //{
        //    //	vehicleMeshTransform->setScale(Vector3<real_Num>::unit() * 0.0254f);
        //    // }

        //    vehicleMesh->setName( "F40" );
        //    rigActor->addChild( vehicleMesh );
        //}

        auto groundPosition = Vector3<real_Num>::unitY() * 5.0f;
        auto groundScale = Vector3<real_Num>::unit() * 1.0f;

        auto transform = actor->getTransform();
        if( transform )
        {
            transform->setLocalPosition( groundPosition );
            transform->setLocalScale( groundScale );
        }

        actor->updateTransform();

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        return actor;
    }

    auto Application::createDefaultTruck( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto prefabManager = applicationManager->getPrefabManager();
        FB_ASSERT( prefabManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();

        auto name = String( "Truck" );
        actor->setName( name );

        // auto id = StringUtil::getHash(name);
        // actor->setId(id);

        // auto uuid = StringUtil::getUUID();
        // actor->setUUID(uuid);

        auto c = actor->addComponent<scene::CollisionBox>();
        FB_ASSERT( c );

        auto rigidbody = actor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        auto vehicle = actor->addComponent<scene::CarController>();
        FB_ASSERT( vehicle );

        auto meshComponent = actor->addComponent<scene::Mesh>();
        FB_ASSERT( meshComponent );
        meshComponent->setMeshPath( "cube_internal.fbmeshbin" );

        auto meshRenderer = actor->addComponent<scene::MeshRenderer>();
        FB_ASSERT( meshRenderer );

        auto material = actor->addComponent<scene::Material>();
        FB_ASSERT( material );

        if( material )
        {
            material->setMaterialPath( "Standard.mat" );
            material->updateMaterial();
        }

        auto dynamicsActor = sceneManager->createActor();

        auto dynamicsName = String( "dynamics" );
        dynamicsActor->setName( dynamicsName );

        // auto dynamicsId = StringUtil::getHash(dynamicsName);
        // dynamicsActor->setId(dynamicsId);

        // auto dynamicsUuid = StringUtil::getUUID();
        // dynamicsActor->setUUID(dynamicsUuid);

        actor->addChild( dynamicsActor );

        for( u32 i = 0; i < 4; ++i )
        {
            auto wheelActor = sceneManager->createActor();
            dynamicsActor->addChild( wheelActor );

            auto wheelName = String( "Wheel" );
            wheelActor->setName( wheelName );

            auto wheel = wheelActor->addComponent<scene::WheelController>();
            FB_ASSERT( wheel );
        }

        auto rigActor = sceneManager->createActor();

        auto rigName = String( "rig" );
        rigActor->setName( rigName );
        actor->addChild( rigActor );

        auto prefab = prefabManager->loadPrefab( "f40.fbx" );
        if( prefab )
        {
            auto vehicleMesh = prefab->createActor();

            // auto vehicleMeshTransform = vehicleMesh->getLocalTransform();
            // if (vehicleMeshTransform)
            //{
            //	vehicleMeshTransform->setScale(Vector3<real_Num>::unit() * 0.0254f);
            // }

            vehicleMesh->setName( "F40" );
            rigActor->addChild( vehicleMesh );
        }

        auto groundPosition = Vector3<real_Num>::unitY() * 5.0f;
        auto groundScale = Vector3<real_Num>::unit() * 1.0f;

        auto transform = actor->getTransform();
        if( transform )
        {
            transform->setLocalPosition( groundPosition );
            transform->setLocalScale( groundScale );
        }

        actor->updateTransform();

        if( addToScene )
        {
            scene->addActor( actor );
            scene->registerAllUpdates( actor );
        }

        return actor;
    }

    auto Application::createVehicleFromDatabase( s32 id ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto vehicleActor = sceneManager->createActor();

        auto name = String( "Vehicle" );
        vehicleActor->setName( name );

        //auto modelConfig = vehicleActor->getComponent<VehicleDirector>();
        //if( !modelConfig )
        //{
        //    modelConfig = vehicleActor->addComponent<VehicleDirector>();
        //}

        String sNewModelId = StringUtil::toString( id );
        //modelConfig->loadFromDB( sNewModelId, false );

        auto rigidbody = vehicleActor->addComponent<scene::Rigidbody>();
        FB_ASSERT( rigidbody );

        //auto vehicle = vehicleActor->addComponent<scene::VehicleCar>();

        return vehicleActor;
    }

    auto Application::createDefaultParticleSystem( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        FB_ASSERT( sceneManager );

        auto scene = sceneManager->getCurrentScene();
        FB_ASSERT( scene );

        auto actor = sceneManager->createActor();
        FB_ASSERT( actor );

        auto particleSystem = actor->addComponent<scene::ParticleSystem>();
        FB_ASSERT( particleSystem );

        return actor;
    }

    auto Application::loadVehicleDataFromDatabase( s32 id ) -> SmartPtr<IData>
    {
        return nullptr;
    }

    auto Application::createDefaultMaterialUI() -> SmartPtr<render::IMaterial>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto materialManager = graphicsSystem->getMaterialManager();
        FB_ASSERT( materialManager );

        auto materialName = String( "DefaultUI" );
        SmartPtr<render::IMaterial> defaultMat = materialManager->loadFromFile( materialName + ".mat" );
        if( !defaultMat )
        {
            static const auto uuid =
                String( "2f261ebe-5db8-11ed-9b6a-0242ac120002" );  //StringUtil::getUUID();

            auto resource = materialManager->createOrRetrieve( uuid, materialName, "material" );
            auto material = fb::static_pointer_cast<render::IMaterial>( resource.first );
            if( material )
            {
                material->setMaterialType( render::IMaterial::MaterialType::UI );
                graphicsSystem->loadObject( material );

                auto techniques = material->getTechniques();
                SmartPtr<render::IMaterialTechnique> technique;

                if( !techniques.empty() )
                {
                    technique = techniques[0];
                }

                if( !technique )
                {
                    technique = material->createTechnique();
                }

                if( technique )
                {
                    auto passes = technique->getPasses();
                    SmartPtr<render::IMaterialPass> pass;

                    if( !passes.empty() )
                    {
                        pass = passes[0];
                    }

                    if( !pass )
                    {
                        pass = technique->createPass();
                    }

                    if( pass )
                    {
                        auto textures = pass->getTextureUnits();
                        if( !textures.empty() )
                        {
                            //textures[0];
                        }
                    }
                }
            }

            return material;
        }

        return defaultMat;
    }

    auto Application::createDefaultMaterial() -> SmartPtr<render::IMaterial>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();
        FB_ASSERT( graphicsSystem );

        auto materialManager = graphicsSystem->getMaterialManager();
        FB_ASSERT( materialManager );

        auto resource = materialManager->create( StringUtil::getUUID() );
        auto material = fb::static_pointer_cast<render::IMaterial>( resource );
        if( material )
        {
            auto techniques = material->getTechniques();
            SmartPtr<render::IMaterialTechnique> technique;

            if( !techniques.empty() )
            {
                technique = techniques[0];
            }

            if( !technique )
            {
                technique = material->createTechnique();
            }

            if( technique )
            {
                auto passes = technique->getPasses();
                SmartPtr<render::IMaterialPass> pass;

                if( !passes.empty() )
                {
                    pass = passes[0];
                }

                if( !pass )
                {
                    pass = technique->createPass();
                }

                if( pass )
                {
                }
            }
        }

        return material;
    }

    void Application::createDefaultMaterials()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto graphicsSystem = applicationManager->getGraphicsSystem();

        auto resourceDatabase = applicationManager->getResourceDatabase();

        auto filePath = String( "default" );
        auto result = resourceDatabase->createOrRetrieveByType<render::IMaterial>( filePath );
        if( result.first && result.second )
        {
            auto material = result.first;
            graphicsSystem->loadObject( material );
        }
    }

    void Application::createViewports()
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
            {
                if( auto window = graphicsSystem->getDefaultWindow() )
                {
                    if( m_camera )
                    {
                        m_camera->setAutoAspectRatio( true );

                        auto vp = window->addViewport( 0, m_camera );
                        FB_ASSERT( vp );

                        auto viewportColour = ColourF::Blue * 0.2f;
                        vp->setBackgroundColour( viewportColour );

                        //auto actualWidth = static_cast<f32>( vp->getActualWidth() );
                        //auto actualHeight = static_cast<f32>( vp->getActualHeight() );

                        //auto aspectRatio = 1.0f;
                        //if( actualHeight > 0.0f )
                        //{
                        //    aspectRatio = actualWidth / actualHeight;
                        //}

                        //FB_ASSERT( MathF::isFinite( aspectRatio ) );
                        //FB_ASSERT( aspectRatio > 0.0f );

                        //m_camera->setAspectRatio( aspectRatio );
                        //FB_ASSERT( m_camera->isValid() );

                        vp->setClearEveryFrame( true );
                        vp->setOverlaysEnabled( true );
                        vp->setEnableUI( true );
                        vp->setEnableSceneRender( true );
                        vp->setAutoUpdated( true );
                        m_viewport = vp;

                        FB_ASSERT( m_viewport );
                        FB_ASSERT( m_viewport->isValid() );
                    }
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Application::createScriptManager() -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto scriptManager = factoryManager->make_object<IScriptManager>();
        if( scriptManager )
        {
            scriptManager->load( nullptr );
            applicationManager->setScriptManager( scriptManager );
        }

        return scriptManager != nullptr;
    }

    auto Application::createSoundManager() -> bool
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();

        if( auto soundManager = factoryManager->make_object<ISoundManager>() )
        {
            soundManager->load( nullptr );
            applicationManager->setSoundManager( soundManager );
        }

        return true;
    }

    void Application::createPluginManager()
    {
        auto applicationManager = ApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        auto pluginManager = factoryManager->make_ptr<PluginManager>();
        applicationManager->setPluginManager( pluginManager );
    }

    void Application::createPlugins()
    {
    }

    void Application::createProcessManager()
    {
        auto applicationManager = ApplicationManager::instance();

        auto processManager = fb::make_ptr<ProcessManager>();
        applicationManager->setProcessManager( processManager );
    }

    void Application::destroyScene()
    {
    }

    void Application::loadScripts()
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fileSystem = applicationManager->getFileSystem();
            auto scriptManager = applicationManager->getScriptManager();
            if( scriptManager )
            {
                String scriptExt;

#if FB_ENABLE_LUA
                scriptExt = ".lua";
#elif FB_ENABLE_PYTHON
                scriptExt = ".py";
#endif

                auto scripts = fileSystem->getFileNamesWithExtension( scriptExt );
                std::sort( scripts.begin(), scripts.end() );
                scriptManager->loadScripts( scripts );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto Application::handleEvent( u32 state, IFSM::Event eventType ) -> IFSM::ReturnType
    {
        return IFSM::ReturnType::Ok;
    }

    void Application::createLogManager()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto logManager = fb::make_ptr<LogManagerDefault>();
        FB_ASSERT( logManager );
        FB_ASSERT( logManager->isValid() );

        applicationManager->setLogManager( logManager );
        logManager->open( "Application.log" );

        FB_ASSERT( logManager->isValid() );
    }

    void Application::createFactoryManager()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = fb::make_ptr<FactoryManager>();
        applicationManager->setFactoryManager( factoryManager );
        FB_ASSERT( applicationManager->getFactoryManager() );
    }

    void Application::createStateManager()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );
        FB_ASSERT( factoryManager->isValid() );

        auto stateManager = factoryManager->make_object<IStateManager>();
        applicationManager->setStateManager( stateManager );
    }

    void Application::createComponentsContainer()
    {
    }

    void Application::createPlatformManager()
    {
    }

    void Application::createFileSystem()
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            auto fileSystem = factoryManager->make_object<IFileSystem>();
            applicationManager->setFileSystem( fileSystem );

            auto workingDirectory = Path::getWorkingDirectory();
            fileSystem->addFolder( workingDirectory );

#if !FB_FINAL
            auto mediaPath = String( "" );

#    if defined FB_PLATFORM_WIN32
            mediaPath = String( "../../../../../Media" );
#    elif defined FB_PLATFORM_APPLE
            mediaPath = String( "../../Media" );
#    else
            mediaPath = String( "../../Media" );
#    endif

            auto absolutePath = Path::lexically_normal( workingDirectory, mediaPath );

            auto packs = fileSystem->getFiles( mediaPath + "/packs" );
            for( auto &pack : packs )
            {
                fileSystem->addFileArchive( pack, true, true, IFileSystem::ArchiveType::Zip );
            }

#    if defined FB_PLATFORM_WIN32
            fileSystem->addFolder( mediaPath, true );
            applicationManager->setMediaPath( mediaPath );
#    else
            auto absoluteMediaPath = Path::lexically_normal( workingDirectory, mediaPath );
            fileSystem->addFolder( absoluteMediaPath, true );
            applicationManager->setMediaPath( absoluteMediaPath );
#    endif
#else
            fileSystem->addFileArchive( "Media.zip", false, false, IFileSystem::ArchiveType::Zip );
#endif

            fileSystem->addFileArchive( ".FBCache", true, true, IFileSystem::ArchiveType::Folder );
            fileSystem->addFileArchive( "./", true, true, IFileSystem::ArchiveType::Folder );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Application::createUI()
    {
    }

    void Application::createRenderUI()
    {
        //auto renderUI = ui::FBRenderUI::createUIManager();
        //applicationManager->setRenderUI( renderUI );
        //graphicsSystem->loadObject( renderUI );
    }

    void Application::createScene()
    {
    }

    void Application::createTimer()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto timer = fb::make_ptr<TimerMT>();
        applicationManager->setTimer( timer );
    }

    void Application::createPrefabManager()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto prefabManager = fb::make_ptr<scene::PrefabManager>();
        applicationManager->setPrefabManager( prefabManager );
    }

    void Application::createProfiler()
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto profiler = factoryManager->make_object<IProfiler>();
        applicationManager->setProfiler( profiler );
    }

    void Application::createThreadPool()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto numThreads = static_cast<u32>( getActiveThreads() );

        auto maxThreads = Thread::hardware_concurrency();
        if( numThreads > maxThreads )
        {
            numThreads = maxThreads;
        }

        auto threadPool = factoryManager->make_ptr<ThreadPool>();
        applicationManager->setThreadPool( threadPool );

        threadPool->setNumThreads( numThreads );
        threadPool->load( nullptr );

        for( u32 i = 0; i < numThreads; ++i )
        {
            auto workerThread = threadPool->getThread( i );
            workerThread->setTargetFPS( 100.0 );
        }
    }

    void Application::createTaskManager()
    {
        auto applicationManager = ApplicationManager::instance();

        auto taskManager = fb::make_ptr<TaskManager>();
        applicationManager->setTaskManager( taskManager );

        taskManager->load( nullptr );
    }

    void Application::createTasks()
    {
        try
        {
            auto applicationManager = ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto taskManager = applicationManager->getTaskManager();
            FB_ASSERT( taskManager );

            auto primaryTask = taskManager->getTask( Thread::Task::Primary );
            primaryTask->setTask( Thread::Task::Primary );
            primaryTask->setThreadTaskFlags( Thread::Primary_Flag );
            primaryTask->setPrimary( true );
            primaryTask->setEnabled( true );
            primaryTask->setOwner( this );
            primaryTask->setTargetFPS( 1000.0 );

            auto applicationTask = taskManager->getTask( Thread::Task::Application );
            applicationTask->setTask( Thread::Task::Application );
            applicationTask->setThreadTaskFlags( Thread::Application_Flag );
            applicationTask->setPrimary( false );
            applicationTask->setEnabled( true );
            applicationTask->setOwner( this );
            applicationTask->setTargetFPS( 100.0 );

            auto renderTask = taskManager->getTask( Thread::Task::Render );
            renderTask->setTask( Thread::Task::Render );
            renderTask->setThreadTaskFlags( Thread::Render_Flag );
            renderTask->setPrimary( true );
            renderTask->setEnabled( true );
            renderTask->setOwner( this );
            renderTask->setTargetFPS( 1000.0 );

            auto physicsTask = taskManager->getTask( Thread::Task::Physics );
            physicsTask->setTask( Thread::Task::Physics );
            physicsTask->setThreadTaskFlags( Thread::Physics_Flag );
            physicsTask->setPrimary( false );
            physicsTask->setEnabled( true );
            physicsTask->setOwner( this );
            physicsTask->setTargetFPS( 100.0 );

            auto garbageCollectTask = taskManager->getTask( Thread::Task::GarbageCollect );
            garbageCollectTask->setTask( Thread::Task::GarbageCollect );
            garbageCollectTask->setThreadTaskFlags( Thread::GarbageCollect_Flag );
            garbageCollectTask->setPrimary( false );
            garbageCollectTask->setEnabled( true );
            garbageCollectTask->setOwner( this );
            garbageCollectTask->setTargetFPS( 60.0 );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Application::createJobQueue()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );
        FB_ASSERT( applicationManager->isValid() );

        auto factoryManager = applicationManager->getFactoryManager();

        auto jobQueue = factoryManager->make_ptr<JobQueue>();
        applicationManager->setJobQueue( jobQueue );
    }

    void Application::createFsmManager()
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );
        FB_ASSERT( factoryManager->isValid() );

        auto fsmManager = factoryManager->make_object<IFSMManager>();
        FB_ASSERT( fsmManager );
        FB_ASSERT( fsmManager->isValid() );

        fsmManager->load( nullptr );

        applicationManager->setFsmManager( fsmManager );
        FB_ASSERT( applicationManager->getFsmManager() );
    }

    void Application::createFsm()
    {
    }

    void Application::createOutputManager()
    {
    }

    auto Application::getActiveThreads() const -> size_t
    {
        return m_activeThreads;
    }

    void Application::setActiveThreads( size_t activeThreads )
    {
        m_activeThreads = activeThreads;
    }

    auto Application::createDefaultSky( bool addToScene ) -> SmartPtr<scene::IActor>
    {
        auto applicationManager = ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto sceneManager = applicationManager->getSceneManager();
        auto scene = sceneManager->getCurrentScene();

        auto actor = sceneManager->createActor();

        auto name = String( "Skybox" );
        actor->setName( name );

        auto skybox = actor->addComponent<scene::Skybox>();

        auto material = actor->addComponent<scene::Material>();
        if( material )
        {
            material->setMaterialPath( "DefaultSkybox.mat" );
        }

        if( addToScene )
        {
            scene->addActor( actor );
        }

        auto uniqueId = 0;  // StringUtil::getHash(uuid);
        return actor;
    }

    void Application::createRigidStaticMesh( SmartPtr<scene::IActor> actor, bool recursive )
    {
        try
        {
            actor->setStatic( true );

            auto meshComponent = actor->getComponent<scene::Mesh>();
            if( meshComponent )
            {
                auto collisionMesh = actor->getComponent<scene::CollisionMesh>();
                if( !collisionMesh )
                {
                    collisionMesh = actor->addComponent<scene::CollisionMesh>();
                }

                if( collisionMesh )
                {
                    auto meshPath = meshComponent->getMeshPath();
                    collisionMesh->setMeshPath( meshPath );
                }

                auto rigidbody = actor->getComponent<scene::Rigidbody>();
                if( !rigidbody )
                {
                    rigidbody = actor->addComponent<scene::Rigidbody>();
                }
            }

            if( auto p = actor->getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    createRigidStaticMesh( child, recursive );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Application::createRigidDynamicMesh( SmartPtr<scene::IActor> actor, bool recursive )
    {
        try
        {
            actor->setStatic( false );

            auto meshComponent = actor->getComponent<scene::Mesh>();
            if( meshComponent )
            {
                auto collisionMesh = actor->getComponent<scene::CollisionMesh>();
                if( !collisionMesh )
                {
                    collisionMesh = actor->addComponent<scene::CollisionMesh>();
                }

                if( collisionMesh )
                {
                    auto meshPath = meshComponent->getMeshPath();
                    collisionMesh->setMeshPath( meshPath );
                }

                auto rigidbody = actor->getComponent<scene::Rigidbody>();
                if( !rigidbody )
                {
                    rigidbody = actor->addComponent<scene::Rigidbody>();
                }
            }

            if( auto p = actor->getChildrenPtr() )
            {
                auto &children = *p;
                for( auto child : children )
                {
                    createRigidDynamicMesh( child, recursive );
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }
}  // namespace fb::core
