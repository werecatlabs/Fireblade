#include <GameEditorPCH.h>
#include <Application.h>
#include <editor/EditorManager.h>
#include <jobs/JobRendererSetup.h>
#include <jobs/PlaymodeJob.h>
#include "jobs/LeavePlaymodeJob.h"
#include <ui/ProjectWindow.h>
#include <ui/UIManager.h>
#include <FBCore/FBCore.h>
#include <FBApplication/FBApplication.h>
#include <FBFileSystem/FBFileSystem.h>
#include <FBAnimation/FBAnimation.h>
#include <FBGraphics/FBGraphics.h>
#include <FBOISInput/FBOISInput.h>
#include <FBAssimp/FBAssimp.h>

#if FB_GRAPHICS_SYSTEM_OGRENEXT
#    include <FBGraphicsOgreNext/FBGraphicsOgreNext.h>
#elif FB_GRAPHICS_SYSTEM_OGRE
#    include <FBGraphicsOgre/FBGraphicsOgre.h>
#endif

#include <FBRenderUI/FBRenderUI.h>

#include <FBParticleSystem/FBParticleSystem.h>
#include <FBApplication/FBApplication.h>
#include <FBProcedural/FBProcedural.h>
#include <editor/Project.h>

#include <GameEditorTypes.h>

#include <FBVehicle/CVehicleManager.h>
#include <FBCPlusPlus/FBCPlusPlus.h>

#if FB_BUILD_PHYSICS3
#    include "FBPhysics3/FBPhysics3.h"
#endif

#if FB_BUILD_PHYSX
#    include "FBPhysx/FBPhysx.h"
#endif

#if FB_BUILD_IMGUI
#    include <FBImGui/FBImGui.h>
#endif

#ifdef _FB_STATIC_LIB_
#    include <FBFileSystem/FBFileSystem.h>
#    include <FBSQLite/FBSQLite.h>
#endif

#ifdef _FB_STATIC_LIB_
#    if FB_ENABLE_LUA
#        include <FBLua/FBLua.h>
#    elif FB_ENABLE_PYTHON
#        include <FBPython/FBPython.h>
#    endif
#endif

namespace fb
{
    namespace editor
    {
        Application::Application() : m_editorManager( nullptr ), m_isRunning( true )
        {
            auto &gc = GarbageCollector::instance();
            //gc.setMode( GarbageCollector::Mode::Balanced );
            //gc.setMode( GarbageCollector::Mode::Deferred );
            //gc.setMode( GarbageCollector::Mode::LowConsumption );
        }

        Application::~Application()
        {
            try
            {
                unload( nullptr );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Application::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto currentThreadId = Thread::ThreadId::Primary;
                Thread::setCurrentThreadId( currentThreadId );

                auto task = Thread::Task::Primary;
                Thread::setCurrentTask( task );

                auto applicationManager = fb::make_ptr<core::ApplicationManagerMT>();
                core::IApplicationManager::setInstance( applicationManager );

                applicationManager->load( nullptr );
                applicationManager->setEditor( true );
                applicationManager->setEditorCamera( true );

                auto playerSettings = fb::make_ptr<Properties>();
                applicationManager->setPlayerSettings( playerSettings );

                auto editorSettingsDataStr = Path::readAllText( "editor.settings" );
                if( !StringUtil::isNullOrEmpty( editorSettingsDataStr ) )
                {
                    auto editorSettings = fb::make_ptr<Properties>();
                    DataUtil::parse( editorSettingsDataStr, editorSettings.get() );
                    applicationManager->setEditorSettings( editorSettings );
                }

                auto stateRoot = fb::make_ptr<CTransformRoot>();
                ITransformRoot::setInstance( stateRoot );

                auto meshManager = fb::make_ptr<MeshManager>();
                applicationManager->setMeshManager( meshManager );

                auto commandMgr = fb::make_ptr<CommandManager>();
                //auto commandMgr = fb::make_ptr<CommandManagerMT>();
                applicationManager->setCommandManager( commandMgr );

                auto prefabManager = fb::make_ptr<scene::PrefabManager>();
                applicationManager->setPrefabManager( prefabManager );

                auto selectionManager = fb::make_ptr<SelectionManager>();
                applicationManager->setSelectionManager( selectionManager );

                auto meshLoader = FBAssimp::createAssimpLoader();
                applicationManager->setMeshLoader( meshLoader );

                CApplicationClient::load( data );

                loadScripts();

                ApplicationUtil::createDefaultMaterialUI();
                ApplicationUtil::createDefaultMaterial();
                ApplicationUtil::createDefaultMaterials();
                ApplicationUtil::createDefaultFont();

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                m_stateObject = stateManager->addStateObject();

                auto applicationStateListener = factoryManager->make_ptr<ApplicationStateListener>();
                applicationStateListener->setOwner( this );
                m_stateListener = applicationStateListener;
                m_stateObject->addStateListener( m_stateListener );

                auto jobQueue = fb::make_ptr<JobQueue>();
                applicationManager->setJobQueue( jobQueue );

                m_editorManager = fb::make_ptr<EditorManager>();
                EditorManager::setSingletonPtr( m_editorManager );

                auto projectManager = fb::make_ptr<ProjectManager>();
                m_editorManager->setProjectManager( projectManager );

                auto pProject = fb::make_ptr<Project>();
                m_editorManager->setProject( pProject );

                auto renderUI = ui::FBRenderUI::createUIManager();
                applicationManager->setRenderUI( renderUI );

                auto editorUiManager = fb::make_ptr<UIManager>();
                m_editorManager->setUI( editorUiManager );
                editorUiManager->load( nullptr );

                if( auto inputManager = applicationManager->getInputDeviceManager() )
                {
                    auto listener = fb::make_ptr<ApplicationInputListener>( this );
                    inputManager->addListener( listener );
                }

                auto translateManipulator = factoryManager->make_ptr<TranslateManipulator>();
                translateManipulator->load( nullptr );
                m_editorManager->setTranslateManipulator( translateManipulator );

                auto rotateManipulator = factoryManager->make_ptr<RotateManipulator>();
                rotateManipulator->load( nullptr );
                m_editorManager->setRotateManipulator( rotateManipulator );

                auto scaleManipulator = factoryManager->make_ptr<ScaleManipulator>();
                scaleManipulator->load( nullptr );
                m_editorManager->setScaleManipulator( scaleManipulator );

                if( auto sceneManager = applicationManager->getSceneManager() )
                {
                    auto system = fb::make_ptr<scene::CanvasTransformSystem>();
                    sceneManager->addSystem( scene::LayoutTransform::typeInfo(), system );
                }

                ApplicationUtil::createDefaultSky();
                ApplicationUtil::createDirectionalLight();
                // ApplicationUtil::createDefaultCube();
                //ApplicationUtil::createDefaultTerrain();
                //  ApplicationUtil::createDefaultPlane();
                //  ApplicationUtil::createDefaultVehicle();
                //  ApplicationUtil::createProceduralTest();
                //  ApplicationUtil::createOverlayPanelTest();

                // auto currentScene = sceneManager->getCurrentScene();
                // auto prefab = prefabManager->loadPrefab("car.prefab");
                // auto carActor = prefab->getActor();
                // if (carActor)
                //{
                //	auto groundPosition = Vector3F::unitY() * 5.0f;
                //	auto groundScale = Vector3F::unit() * 1.0f;

                //	auto localTransform = carActor->getLocalTransform();
                //	if (localTransform)
                //	{
                //		localTransform->setPosition(groundPosition);
                //		localTransform->setScale(groundScale);
                //	}

                //	carActor->updateTransform();
                //	currentScene->addActor(carActor);
                //	currentScene->registerAllUpdates(carActor);
                //}

                editorUiManager->rebuildSceneTree();

                auto sceneManager = applicationManager->getSceneManager();
                if( sceneManager )
                {
                    sceneManager->edit();
                }

                auto cameraManager = applicationManager->getCameraManager();
                if( cameraManager )
                {
                    cameraManager->edit();
                }

                auto taskManager = applicationManager->getTaskManager();
                taskManager->setState( ITaskManager::State::FreeStep );

                auto threadPool = applicationManager->getThreadPool();
                threadPool->setState( IThreadPool::State::Start );

                auto projectWindow = editorUiManager->getProjectWindow();
                if( projectWindow )
                {
                    projectWindow->buildTree();
                }

                if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
                {
                    auto window = graphicsSystem->getDefaultWindow();
                    if( window )
                    {
                        auto windowName = String( "Fireblade" );
                        window->setTitle( windowName );
                        window->maximize();
                    }
                }

                auto resourceDatabase = applicationManager->getResourceDatabase();
                resourceDatabase->build();

                if( m_sphericalCameraActor )
                {
                    auto sphericalCamera =
                        m_sphericalCameraActor->getComponent<scene::SphericalCameraController>();
                    sphericalCamera->setUiWindow( m_renderWindow );
                }

                if( auto uiManager = applicationManager->getUI() )
                {
                    uiManager->setMainWindow( m_renderWindow );
                }

                applicationManager->setSceneRenderWindow( m_renderWindow );

                if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
                {
                    if( auto renderUI = applicationManager->getRenderUI() )
                    {
                        graphicsSystem->loadObject( renderUI );
                    }
                }

                Array<String> ignoreList;
                ignoreList.push_back( "UnityEngine.Transform" );
                sceneManager->setComponentFactoryIgnoreList( ignoreList );

                Map<String, String> componentMap;
                componentMap["saracen.ApplicationManager"] = "SimulatorApplication";
                componentMap["FB.FBFiniteStateMachine"] = "FiniteStateMachine";
                componentMap["UnityEngine.Canvas"] = "CanvasComponent";
                componentMap["UnityEngine.RectTransform"] = "CanvasTransform";
                componentMap["UnityEngine.UI.Text"] = "TextComponent";
                componentMap["UnityEngine.UI.Image"] = "ImageComponent";
                componentMap["UnityEngine.UI.Button"] = "ButtonComponent";
                componentMap["Unitycoding.UIWidgets.TooltipTrigger"] = "Tooltip";
                componentMap["UI.Tables.TableLayout"] = "TableLayout";
                componentMap["UnityEngine.UI.VerticalLayoutGroup"] = "TableLayout";
                componentMap["saracen.StartMenu"] = "StartMenu";

                componentMap["CameraComponent"] = "Camera";
                componentMap["MeshComponent"] = "Mesh";
                componentMap["LightComponent"] = "Light";
                componentMap["MaterialComponent"] = "Material";
                componentMap["RigidbodyComponent"] = "Rigidbody";
                componentMap["SkyboxComponent"] = "Skybox";
                componentMap["Canvas"] = "Layout";
                componentMap["CanvasTransform"] = "LayoutTransform";
                componentMap["TextComponent"] = "Text";
                componentMap["ImageComponent"] = "Image";

                // unity map
                componentMap["5f7201a12d95ffc409449d95f23cf332"] = "Text";
                componentMap["dc42784cf147c0c48a680349fa168899"] = "Layout";
                componentMap["fe87c0e1cc204ed48ad3b37840f39efc"] = "Image";
                componentMap["4e29b1a8efbd4b44bb3f3716e73f07ff"] = "Button";

                sceneManager->setComponentFactoryMap( componentMap );

                setLoadingState( LoadingState::Loaded );
            }
            catch( Exception &e )
            {
                FB_LOG_EXCEPTION( e );
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
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    if( auto taskManager = applicationManager->getTaskManager() )
                    {
                        taskManager->stop();
                    }

                    if( auto threadPool = applicationManager->getThreadPool() )
                    {
                        threadPool->stop();
                    }

                    if( auto manipulator = m_editorManager->getTranslateManipulator() )
                    {
                        manipulator->unload( nullptr );
                        m_editorManager->setTranslateManipulator( nullptr );
                    }

                    if( auto manipulator = m_editorManager->getRotateManipulator() )
                    {
                        manipulator->unload( nullptr );
                        m_editorManager->setRotateManipulator( nullptr );
                    }

                    if( auto manipulator = m_editorManager->getScaleManipulator() )
                    {
                        manipulator->unload( nullptr );
                        m_editorManager->setScaleManipulator( nullptr );
                    }

                    applicationManager->setWindow( nullptr );

                    if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
                    {
                        if( auto textureManager = graphicsSystem->getTextureManager() )
                        {
                            if( m_renderTarget )
                            {
                                if( auto renderTarget = m_renderTarget->getRenderTarget() )
                                {
                                    if( auto vp = getRttViewport() )
                                    {
                                        vp->setCamera( nullptr );

                                        renderTarget->removeViewport( vp );
                                        setRttViewport( nullptr );
                                    }
                                }
                            }
                        }
                    }

                    if( auto ui = applicationManager->getUI() )
                    {
                        if( m_application )
                        {
                            ui->removeApplication( m_application );
                            m_application = nullptr;
                        }

                        if( m_renderWindow )
                        {
                            m_renderWindow->setWindow( nullptr );
                            m_renderWindow->setRenderTexture( nullptr );

                            ui->removeElement( m_renderWindow );
                            m_renderWindow = nullptr;
                        }
                    }

                    const auto editorSettings = applicationManager->getEditorSettings();

                    const auto dataStr = DataUtil::toString( editorSettings.get(), true );
                    Path::writeAllText( "editor.settings", dataStr );

                    if( auto cameraManager = applicationManager->getCameraManager() )
                    {
                        cameraManager->unload( nullptr );
                        applicationManager->setCameraManager( nullptr );
                    }

                    m_sphericalCamera = nullptr;

                    if( auto stateObject = getStateObject() )
                    {
                        if( auto stateListener = getStateListener() )
                        {
                            stateObject->removeStateListener( stateListener );
                            setStateListener( nullptr );
                        }

                        if( auto stateManager = applicationManager->getStateManager() )
                        {
                            stateManager->removeStateObject( stateObject );
                        }

                        setStateObject( nullptr );
                    }

                    if( auto sceneManager = applicationManager->getSceneManager() )
                    {
                        if( m_sphericalCameraActor )
                        {
                            sceneManager->destroyActor( m_sphericalCameraActor );
                            m_sphericalCameraActor = nullptr;
                        }
                    }

                    if( auto camera = getUiCamera() )
                    {
                        if( auto owner = camera->getOwner() )
                        {
                            owner->detachObject( camera );
                        }
                    }

                    if( auto camera = getCamera() )
                    {
                        if( auto owner = camera->getOwner() )
                        {
                            owner->detachObject( camera );
                        }
                    }

                    if( auto node = getUiCameraSceneNode() )
                    {
                        if( auto creator = node->getCreator() )
                        {
                            creator->removeSceneNode( node );
                        }
                    }

                    if( auto node = getCameraSceneNode() )
                    {
                        if( auto creator = node->getCreator() )
                        {
                            creator->removeSceneNode( node );
                        }
                    }

                    if( auto sceneManager = applicationManager->getSceneManager() )
                    {
                        if( auto currentScene = sceneManager->getCurrentScene() )
                        {
                            currentScene->clear();
                        }

                        if( sceneManager )
                        {
                            sceneManager->unload( nullptr );
                        }
                    }

                    if( m_editorManager )
                    {
                        m_editorManager->unload( nullptr );
                        m_editorManager = nullptr;
                    }

                    EditorManager::setSingletonPtr( nullptr );

                    if( m_camera )
                    {
                        if( m_camera->isAttached() )
                        {
                            if( auto owner = m_camera->getOwner() )
                            {
                                owner->detachObject( m_camera );
                            }
                        }

                        m_camera = nullptr;
                    }

                    if( m_cameraSceneNode )
                    {
                        auto creator = m_cameraSceneNode->getCreator();
                        creator->removeSceneNode( m_cameraSceneNode );
                        m_cameraSceneNode = nullptr;
                    }

                    if( m_uiCamera )
                    {
                        if( m_uiCamera->isAttached() )
                        {
                            if( auto owner = m_uiCamera->getOwner() )
                            {
                                owner->detachObject( m_uiCamera );
                            }
                        }

                        m_uiCamera = nullptr;
                    }

                    if( m_uiCameraSceneNode )
                    {
                        if( auto creator = m_uiCameraSceneNode->getCreator() )
                        {
                            creator->removeSceneNode( m_uiCameraSceneNode );
                        }

                        m_uiCameraSceneNode = nullptr;
                    }

                    if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
                    {
                        if( m_viewport )
                        {
                            if( auto window = graphicsSystem->getDefaultWindow() )
                            {
                                m_viewport->setCamera( nullptr );
                                window->removeViewport( m_viewport );
                            }

                            m_viewport = nullptr;
                        }

                        if( m_uiViewport )
                        {
                            if( auto window = graphicsSystem->getDefaultWindow() )
                            {
                                m_uiViewport->setCamera( nullptr );
                                window->removeViewport( m_uiViewport );
                            }

                            m_uiViewport = nullptr;
                        }
                    }

                    if( auto graphicsSystem = applicationManager->getGraphicsSystem() )
                    {
                        if( auto textureManager = graphicsSystem->getTextureManager() )
                        {
                            if( m_renderTarget )
                            {
                                textureManager->destroyRenderTexture( m_renderTarget );
                                m_renderTarget = nullptr;
                            }
                        }
                    }

                    applicationManager->unload( nullptr );
                    core::IApplicationManager::setInstance( nullptr );

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
            auto currentThreadId = Thread::ThreadId::Primary;
            Thread::setCurrentThreadId( currentThreadId );

            auto task = Thread::Task::Primary;
            Thread::setCurrentTask( task );

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto fsmManager = applicationManager->getFsmManager();
            FB_ASSERT( fsmManager );

            auto applicationFSM = getFSM();
            timer->reset();

            while( applicationManager->isRunning() )
            {
                try
                {
                    if( auto taskManager = applicationManager->getTaskManager() )
                    {
                        taskManager->update();
                    }

                    Thread::yield();
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }
        }

        void Application::iterate()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto fsmManager = applicationManager->getFsmManager();
            FB_ASSERT( fsmManager );

            auto applicationFSM = getFSM();

            try
            {
                if( auto taskManager = applicationManager->getTaskManager() )
                {
                    taskManager->update();
                }

                Thread::yield();
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Application::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto timer = applicationManager->getTimer();
            FB_ASSERT( timer );

            auto fsmManager = applicationManager->getFsmManager();
            FB_ASSERT( fsmManager );

            auto applicationFSM = getFSM();

            auto task = Thread::getCurrentTask();
            auto t = timer->getTime();
            auto dt = timer->getDeltaTime();

            auto editorManager = EditorManager::getSingletonPtr();

            auto sceneManager = applicationManager->getSceneManager();
            if( sceneManager )
            {
                sceneManager->preUpdate();
            }

            auto inputManager = applicationManager->getInputDeviceManager();
            if( inputManager )
            {
                inputManager->preUpdate();
            }

            auto stateManager = applicationManager->getStateManager();
            if( stateManager )
            {
                stateManager->preUpdate();
            }

            auto cameraManager = applicationManager->getCameraManager();
            if( cameraManager )
            {
                cameraManager->update();
            }

            if( stateManager )
            {
                stateManager->update();
            }

            if( sceneManager )
            {
                sceneManager->update();
            }

            if( auto scriptManager = applicationManager->getScriptManager() )
            {
                scriptManager->update();
            }

            switch( task )
            {
            case Thread::Task::GarbageCollect:
            {
                auto &gc = GarbageCollector::instance();
                gc.update();
            }
            break;
            case Thread::Task::Primary:
            {
                auto jobQueue = applicationManager->getJobQueue();
                if( jobQueue )
                {
                    jobQueue->preUpdate();
                }

                if( jobQueue )
                {
                    jobQueue->update();
                }

                if( auto inputManager = applicationManager->getInputDeviceManager() )
                {
                    inputManager->update();
                }

                auto &gc = GarbageCollector::instance();
                gc.update();

                timer->update();
                auto dt = timer->getTimeInterval();
                auto time = timer->getTime();

                if( time > 3.0 )
                {
                    if( applicationFSM->getState<State>() != State::Editor )
                    {
                        applicationFSM->setState( State::Editor );
                    }
                }

                fsmManager->update();

                gc.update();

                m_editorManager->update( time, dt );

                if( jobQueue )
                {
                    jobQueue->postUpdate();
                }

                if( applicationManager->getQuit() )
                {
                    applicationManager->setRunning( false );
                }
            }
            break;
            case Thread::Task::Render:
            {
                try
                {
                    auto graphicsSystem = applicationManager->getGraphicsSystem();
                    FB_ASSERT( graphicsSystem );

                    if( auto window = graphicsSystem->getDefaultWindow() )
                    {
                        if( !window->isClosed() )
                        {
                            if( stateManager )
                            {
                                stateManager->update();
                            }

                            if( graphicsSystem )
                            {
                                graphicsSystem->update();
                            }

                            // auto renderer = graphicsSystem->getRenderer();
                            // if (renderer)
                            //{
                            //	renderer->swapAllRenderTargetBuffers();
                            // }

                            // auto window = graphicsSystem->getDefaultWindow();
                            // if(window)
                            //{
                            //     window->update();
                            // }

                            m_editorManager->update( t, dt );
                        }
                    }

                    graphicsSystem->messagePump();
                    if( applicationManager->getQuit() )
                    {
                        applicationManager->setRunning( false );
                    }

                    auto &gc = GarbageCollector::instance();
                    gc.update();
                }
                catch( std::exception &e )
                {
                    FB_LOG_EXCEPTION( e );
                }
            }
            break;
            case Thread::Task::Physics:
            {
                if( timer->getTimeSinceLevelLoad() > 0.1 )
                {
                    if( auto vehicleManager = applicationManager->getVehicleManager() )
                    {
                        vehicleManager->preUpdate();
                        vehicleManager->update();
                        vehicleManager->postUpdate();
                    }

                    if( auto physicsManager = applicationManager->getPhysicsManager() )
                    {
                        physicsManager->preUpdate();
                        physicsManager->update();
                        physicsManager->postUpdate();
                    }

                    if( auto physicsScene = applicationManager->getPhysicsScene() )
                    {
                        physicsScene->update();
                    }
                }

                if( editorManager )
                {
                    editorManager->update( t, dt );
                }

                auto &gc = GarbageCollector::instance();
                gc.update();
            }
            break;
            case Thread::Task::Application:
            {
                timer->addTimeSinceLevelLoad( dt );

                auto cameraManager = applicationManager->getCameraManager();
                if( cameraManager )
                {
                    cameraManager->update();
                }

                auto translateManipulator = editorManager->getTranslateManipulator();
                if( translateManipulator )
                {
                    translateManipulator->preUpdate();
                    translateManipulator->update();
                    translateManipulator->postUpdate();
                }

                auto rotateManipulator = editorManager->getRotateManipulator();
                if( rotateManipulator )
                {
                    rotateManipulator->preUpdate();
                    rotateManipulator->update();
                    rotateManipulator->postUpdate();
                }

                m_editorManager->update( t, dt );

                auto &gc = GarbageCollector::instance();
                gc.update();
            }
            break;
            default:
            {
                m_editorManager->update( t, dt );

                auto &gc = GarbageCollector::instance();
                gc.update();
            }
            }

            if( sceneManager )
            {
                sceneManager->postUpdate();
            }
        }

        bool Application::inputEvent( SmartPtr<IInputEvent> event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();

            auto editorManager = EditorManager::getSingletonPtr();

            if( m_application )
            {
                m_application->handleInputEvent( event );
            }

            if( auto translateManipulator = editorManager->getTranslateManipulator() )
            {
                if( translateManipulator->OnEvent( event ) )
                {
                    return true;
                }
            }

            if( auto rotateManipulator = editorManager->getRotateManipulator() )
            {
                if( rotateManipulator->OnEvent( event ) )
                {
                    return true;
                }
            }

            if( m_sphericalCamera )
            {
                if( m_sphericalCamera->handleEvent( event ) )
                {
                    return true;
                }
            }

            return false;
        }

        IFSM::ReturnType Application::handleEvent( u32 state, IFSM::Event eventType )
        {
            switch( eventType )
            {
            case IFSM::Event::Change:
            {
            }
            break;
            case IFSM::Event::Enter:
            {
                switch( auto eState = static_cast<State>( state ) )
                {
                case State::Editor:
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto fileSystem = applicationManager->getFileSystem();

                    auto editorSettings = applicationManager->getEditorSettings();
                    FB_ASSERT( editorSettings );

                    auto editorManager = EditorManager::getSingletonPtr();
                    FB_ASSERT( editorManager );

                    auto projectFilePath = String();
                    if( editorSettings->getPropertyValue( "Project Path", projectFilePath ) )
                    {
                        if( !StringUtil::isNullOrEmpty( projectFilePath ) )
                        {
                            
                            if( Path::isExistingFile( projectFilePath ) )
                            {
                                editorManager->loadProject( projectFilePath );
                            }
                        }
                    }
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Leave:
            {
                auto eState = static_cast<State>( state );
                switch( eState )
                {
                case State::Editor:
                {
                }
                break;
                default:
                {
                }
                }
            }
            break;
            case IFSM::Event::Pending:
            {
            }
            break;
            case IFSM::Event::Complete:
            {
            }
            break;
            case IFSM::Event::NewState:
            {
            }
            break;
            case IFSM::Event::WaitForChange:
            {
            }
            break;
            default:
            {
            }
            break;
            }

            return IFSM::ReturnType::Ok;
        }

        size_t Application::getWindowHandle() const
        {
            return m_windowHandle;
        }

        void Application::setWindowHandle( size_t val )
        {
            m_windowHandle = val;
        }

        void Application::enterPlayMode()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto jobQueue = applicationManager->getJobQueue();

            auto job = fb::make_ptr<PlaymodeJob>();
            jobQueue->addJob( job );
        }

        void Application::stopPlayMode()
        {
            auto applicationManager = core::IApplicationManager::instance();
            auto jobQueue = applicationManager->getJobQueue();

            auto job = fb::make_ptr<LeavePlaymodeJob>();
            jobQueue->addJob( job );
        }

        void Application::createLogManager()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto logManager = fb::make_ptr<LogManager>();
            applicationManager->setLogManager( logManager );
            logManager->open( "Editor.log" );

            FB_ASSERT( logManager->isValid() );
            FB_ASSERT( applicationManager->isValid() );
        }

        void Application::createFactoryManager()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = fb::make_ptr<FactoryManager>();
            applicationManager->setFactoryManager( factoryManager );
            FB_ASSERT( applicationManager->getFactoryManager() );

            ApplicationUtil::createFactories();

#ifdef _FB_STATIC_LIB_
            auto corePlugin = fb::make_ptr<FBCore>();
            applicationManager->addPlugin( corePlugin );
#endif

#ifdef _FB_STATIC_LIB_
            auto fileSystemPlugin = fb::make_ptr<FBFileSystem>();
            applicationManager->addPlugin( fileSystemPlugin );
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
            auto imguiPlugin = fb::make_ptr<ui::FBImGui>();
            applicationManager->addPlugin( imguiPlugin );
#endif

#ifdef _FB_STATIC_LIB_
#    if FB_ENABLE_LUA
            auto lua = fb::make_ptr<FBLua>();
            applicationManager->addPlugin( lua );
#    elif FB_ENABLE_PYTHON
            auto python = fb::make_ptr<FBPython>();
            applicationManager->addPlugin( python );
#    endif
#endif
        }

        void Application::createTimer()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto timer = fb::make_ptr<TimerMT>();
            applicationManager->setTimer( timer );
        }

        void Application::createFsmManager()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto fsmManager = fb::make_ptr<FSMManager>();
            fsmManager->load( nullptr );
            FB_ASSERT( fsmManager->isLoaded() );
            FB_ASSERT( fsmManager->isValid() );

            applicationManager->setFsmManager( fsmManager );
            FB_ASSERT( applicationManager->isValid() );
        }

        void Application::createFsm()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto fsmManager = applicationManager->getFsmManager();
            FB_ASSERT( fsmManager );

            auto fsm = fsmManager->createFSM();
            setFSM( fsm );

            auto applicationFSMListener = fb::make_ptr<ApplicationFSMListener>();
            applicationFSMListener->setOwner( this );
            fsm->addListener( applicationFSMListener );
        }

        void Application::createTaskManager()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto taskManager = fb::make_ptr<TaskManager>();
            applicationManager->setTaskManager( taskManager );

            taskManager->load( nullptr );
        }

        void Application::createThreadPool()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto numThreads = getActiveThreads();

            auto maxThreads = Thread::hardware_concurrency();
            if( numThreads > maxThreads )
            {
                numThreads = maxThreads;
            }

            auto threadPool = fb::make_ptr<ThreadPool>();
            applicationManager->setThreadPool( threadPool );

            threadPool->setNumThreads( numThreads );
            threadPool->load( nullptr );

            for( u32 i = 0; i < numThreads; ++i )
            {
                auto workerThread = threadPool->getThread( i );
                workerThread->setTargetFPS( 75.0 );
            }
        }

        void Application::createStateManager()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto stateManager = fb::make_ptr<StateManagerStandard>();
            applicationManager->setStateManager( stateManager );

            auto stateObject = stateManager->addStateObject();
            applicationManager->setStateObject( stateObject );
        }

        void Application::createSceneManager()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = fb::make_ptr<scene::SceneManager>();
            applicationManager->setSceneManager( sceneManager );
            sceneManager->load( nullptr );

            auto scene = fb::make_ptr<scene::Scene>();
            scene->load( nullptr );
            scene->setName( "Untitled" );
            sceneManager->setCurrentScene( scene );
        }

        void Application::createFileSystem()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto fileSystem = factoryManager->make_object<IFileSystem>();
                FB_ASSERT( fileSystem );

                applicationManager->setFileSystem( fileSystem );

                auto workingDirectory = Path::getWorkingDirectory();

                auto projectPath = applicationManager->getProjectPath();
                fileSystem->addFolder( projectPath );

                fileSystem->addFolder( workingDirectory );

                auto cachePath = applicationManager->getCachePath();
                auto cacheAbsolutePath = Path::getAbsolutePath( workingDirectory, cachePath );
                fileSystem->addFolder( cacheAbsolutePath );

                auto settingsCachePath = applicationManager->getSettingsCachePath();
                auto settingsCacheAbsolutePath =
                    Path::getAbsolutePath( workingDirectory, settingsCachePath );
                fileSystem->addFolder( settingsCacheAbsolutePath );

                // add archives
                Array<String> fileNames;
                fileSystem->getFileNamesInFolder( String( "./" ), fileNames );
                for( u32 i = 0; i < fileNames.size(); ++i )
                {
                    const auto &fileName = fileNames[i];
                    auto ext = Path::getFileExtension( fileName );
                    if( ext == ".fba" )
                    {
                        fileSystem->addFileArchive( fileName, true, true,
                                                    IFileSystem::ArchiveType::ObfuscatedZip, "" );
                    }
                }

                auto mediaPath = String( "" );
                auto scriptsPath = String( "" );

#if !FB_FINAL
#    if defined FB_PLATFORM_WIN32
#        if FB_GRAPHICS_SYSTEM_OGRENEXT
                mediaPath = String( "../../../../Media/2.0" );
#        elif FB_GRAPHICS_SYSTEM_OGRE
                mediaPath = String( "../../../../Media/1.0" );
#        endif

                scriptsPath = String( "../../../../Media/Scripts/" );
#    elif defined FB_PLATFORM_APPLE
#        if FB_GRAPHICS_SYSTEM_OGRENEXT
                mediaPath = String( "../../../../../MediaOgreNext" );
#        elif FB_GRAPHICS_SYSTEM_OGRE
                mediaPath = String( "../../Media/1.0" );
#        endif

                scriptsPath = String( "../../Media/Scripts/" );
#    else
#        if FB_GRAPHICS_SYSTEM_OGRENEXT
                mediaPath = String( "../../MediaOgreNext/" );
#        elif FB_GRAPHICS_SYSTEM_OGRE
                mediaPath = String( "../../Media/" );
#        endif
#    endif

                applicationManager->setMediaPath( mediaPath );

                fileSystem->getFileNamesInFolder( mediaPath, fileNames );
                for( u32 i = 0; i < fileNames.size(); ++i )
                {
                    const auto &fileName = fileNames[i];
                    auto ext = Path::getFileExtension( fileName );
                    if( ext == ".fba" )
                    {
                        fileSystem->addFileArchive( String( "../Media/" ) + fileName, true, true,
                                                    IFileSystem::ArchiveType::ObfuscatedZip, "" );
                    }
                }

                auto packs = fileSystem->getFiles( mediaPath + "/packs" );
                for( auto &pack : packs )
                {
                    fileSystem->addFileArchive( pack, true, true, IFileSystem::ArchiveType::Zip );
                }

                auto absoluteScriptsPath = Path::lexically_normal( workingDirectory, scriptsPath );
                fileSystem->addFolder( scriptsPath, true );
                fileSystem->addFolder( mediaPath, true );
#else
                applicationManager->setMediaPath( "./" );
                mediaPath = String( "./Media" );
                fileSystem->addFolder( mediaPath, true );

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
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto uiManager = ui::FBImGui::createUI();
            FB_ASSERT( uiManager );
            applicationManager->setUI( uiManager );

            auto application = uiManager->addApplication();
            uiManager->setApplication( application );
            m_application = application;
        }

        bool Application::createGraphicsSystem()
        {
#if FB_GRAPHICS_SYSTEM_OGRENEXT
            auto graphicsSystem = render::FBGraphicsOgreNext::createGraphicsOgre();
#elif FB_GRAPHICS_SYSTEM_OGRE
            auto graphicsSystem = render::FBGraphicsOgre::createGraphicsOgre();
#endif

#if FB_GRAPHICS_SYSTEM_OGRENEXT
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            applicationManager->setGraphicsSystem( graphicsSystem );

            graphicsSystem->load( nullptr );

            auto configuration = graphicsSystem->createConfiguration();
            FB_ASSERT( configuration );

            configuration->setCreateWindow( false );
            // configuration->setCreateWindow( true );

            graphicsSystem->configure( configuration );

            SmartPtr<Properties> properties( new Properties );

            if( m_windowHandle != 0 )
            {
                properties->setProperty( "WindowHandle", m_windowHandle );
            }

            auto window =
                graphicsSystem->createRenderWindow( "Editor App", 400, 400, false, properties );
            applicationManager->setWindow( window );

            auto resourceGroupManager = graphicsSystem->getResourceGroupManager();
            FB_ASSERT( resourceGroupManager );
            resourceGroupManager->load( nullptr );

            auto resourceDatabase = fb::make_ptr<ResourceDatabase>();
            applicationManager->setResourceDatabase( resourceDatabase );
            resourceDatabase->load( nullptr );

#elif FB_GRAPHICS_SYSTEM_OGRE
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            applicationManager->setGraphicsSystem( graphicsSystem );

            auto task = Thread::getCurrentTask();
            auto renderTask = graphicsSystem->getRenderTask();

            graphicsSystem->load( nullptr );

            auto configuration = graphicsSystem->createConfiguration();
            configuration->setCreateWindow( false );
            graphicsSystem->configure( configuration );

            SmartPtr<Properties> properties = fb::make_ptr<Properties>();

            if( m_windowHandle != 0 )
            {
                properties->setProperty( "WindowHandle", m_windowHandle );
            }

            auto window =
                graphicsSystem->createRenderWindow( "Editor App", 400, 400, false, properties );
            applicationManager->setWindow( window );

            auto resourceGroupManager = graphicsSystem->getResourceGroupManager();
            FB_ASSERT( resourceGroupManager );
            resourceGroupManager->load( nullptr );

            auto resourceDatabase = fb::make_ptr<ResourceDatabase>();
            applicationManager->setResourceDatabase( resourceDatabase );
            resourceDatabase->load( nullptr );

#endif

            return true;
        }

        void Application::createRenderWindow()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto ui = applicationManager->getUI();
            FB_ASSERT( ui );

            m_renderWindow = ui->addElementByType<ui::IUIRenderWindow>();

            if( m_renderTarget )
            {
                m_renderWindow->setRenderTexture( m_renderTarget );
            }
        }

        void Application::createCamera()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto sceneManager = applicationManager->getSceneManager();
            FB_ASSERT( sceneManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto smgr = graphicsSystem->getGraphicsScene();

            FB_ASSERT( smgr );
            FB_ASSERT( smgr->isValid() );

            auto rootNode = smgr->getRootSceneNode();
            FB_ASSERT( rootNode );

            const auto uiCameraName = String( "UICamera" );
            m_uiCamera = smgr->addCamera( uiCameraName );
            FB_ASSERT( m_uiCamera );

            m_uiCameraSceneNode = rootNode->addChildSceneNode( uiCameraName );
            FB_ASSERT( m_uiCameraSceneNode );

            m_uiCameraSceneNode->attachObject( m_uiCamera );
            m_uiCamera->setVisible( true );
            m_uiCamera->setRenderUI( true );

            /*
            const auto cameraName = String("DefaultCamera");
            m_camera = smgr->addCamera(cameraName);
            FB_ASSERT(m_camera);

            m_cameraSceneNode = rootNode->addChildSceneNode(cameraName);
            FB_ASSERT(m_cameraSceneNode);

            m_cameraSceneNode->attachObject(m_camera);

            auto cameraPosition = Vector3<real_Num>::zero();
            cameraPosition += Vector3F::unitY() * 5.0;
            cameraPosition += Vector3F::unitZ() * 20.0;
            m_cameraSceneNode->setPosition(cameraPosition);

            m_camera->setNearClipDistance(0.01f);
            m_camera->setFarClipDistance(1000.0f);

            FB_ASSERT(smgr->isValid());
            FB_ASSERT(m_cameraSceneNode->isValid());
            FB_ASSERT(m_camera->isValid());

            auto textureManager = graphicsSystem->getTextureManager();
            FB_ASSERT(textureManager);

            m_renderTarget = textureManager->createRenderTexture();
            FB_ASSERT(m_renderTarget);

            FB_ASSERT(m_camera);
            m_camera->setTargetTexture(m_renderTarget);
            */

            auto textureManager = graphicsSystem->getTextureManager();
            FB_ASSERT( textureManager );

            auto cameraMgr = fb::make_ptr<scene::CameraManager>();
            applicationManager->setCameraManager( cameraMgr );

            m_renderTarget = textureManager->createRenderTexture();
            FB_ASSERT( m_renderTarget );

            cameraMgr->setEditorTexture( m_renderTarget );

            // auto fpsCamera = factoryManager->make_ptr<AppFPSCameraCtrl>();
            // fpsCamera->setName("FPSCameraCtrl");
            // fpsCamera->setRotationSpeed(10.0f);
            // fpsCamera->setPosition((Vector3F::UNIT_Z * 100.f) + (Vector3F::UNIT_Y * 100.0f));
            // fpsCamera->setTargetPosition(Vector3F::ZERO);
            // cameraMgr->addCamera(fpsCamera);
            // cameraMgr->setCurrentCamera(fpsCamera);
            // m_fpsCamera = fpsCamera;

            m_sphericalCameraActor = sceneManager->createActor();
            m_sphericalCameraActor->setFlag( scene::IActor::ActorFlagIsEditor, true );
            // auto sphericalCamera = factoryManager->make_ptr<scene::SphericalCamera>();
            auto sphericalCamera =
                m_sphericalCameraActor->addComponent<scene::SphericalCameraController>();
            // sphericalCamera->setName( "SphericalCamera" );
            // cameraMgr->addCamera( m_sphericalCameraActor );

            sphericalCamera->setSphericalCoords( Vector3<real_Num>( 5.0, 0.0, 2.0 ) );
            m_sphericalCamera = sphericalCamera;

            auto cameraComponent = m_sphericalCameraActor->addComponent<scene::Camera>();

            FB_ASSERT( cameraComponent );
            cameraComponent->setTargetTexture( m_renderTarget );

            auto renderCamera = cameraComponent->getCamera();

            /*
            auto rtt = m_renderTarget->getRenderTarget();
            auto rttVP = rtt->addViewport( 0, renderCamera, 0 );
            rttVP->setOverlaysEnabled( true );
            rttVP->setUiEnabled( false );
            // rttVP->setAutoUpdated( true );

            auto uiViewportColour = ColourF::Blue * 0.33f;
            rttVP->setBackgroundColour( uiViewportColour );

            setRttViewport( rttVP );
            */

            //m_sphericalCameraActor->addComponentInstance(sphericalCamera);
            //cameraMgr->setCurrentCamera( m_sphericalCameraActor );
            //m_sphericalCameraActor->setState( scene::IActor::State::Edit );
        }

        void Application::createViewports()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto window = graphicsSystem->getDefaultWindow();
            FB_ASSERT( window );

            auto mainVP = window->addViewport( 100, m_uiCamera );
            FB_ASSERT( mainVP );

            auto uiViewportColour = ColourF::White * 0.33f;
            mainVP->setBackgroundColour( uiViewportColour );

            mainVP->setEnableSceneRender( false );
            mainVP->setClearEveryFrame( true );
            mainVP->setOverlaysEnabled( false );
            mainVP->setEnableUI( true );
            mainVP->setAutoUpdated( true );

            m_uiViewport = mainVP;

            if( m_sphericalCameraActor )
            {
                auto cameraComponent = m_sphericalCameraActor->getComponent<scene::Camera>();
                auto camera = cameraComponent->getCamera();

                FB_ASSERT( camera );
                camera->setAutoAspectRatio( true );
            }

            // auto vp = window->addViewport( 200, camera );
            // FB_ASSERT( vp );

            // auto viewportColour = ColourF( 0.1f, 0.1f, 0.5f, 1.0f );
            // vp->setBackgroundColour( viewportColour );

            // auto actualWidth = static_cast<f32>(vp->getActualWidth());
            // auto actualHeight = static_cast<f32>(vp->getActualHeight());

            // auto aspectRatio = 1.0f;
            // if(actualHeight > 0.0f)
            //{
            //     aspectRatio = actualWidth / actualHeight;
            // }

            // FB_ASSERT( MathF::isFinite( aspectRatio ) );
            // FB_ASSERT( aspectRatio > 0.0f );

            // camera->setAspectRatio( aspectRatio );
            // FB_ASSERT( camera->isValid() );

            // vp->setClearEveryFrame( true );
            // vp->setOverlaysEnabled( true );
            // vp->setUiEnabled( false );
            // vp->setAutoUpdated( true );
            // m_viewport = vp;

            // FB_ASSERT( m_viewport );
            // FB_ASSERT( m_viewport->isValid() );
        }

        void Application::createRenderInitJob()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto taskManager = applicationManager->getTaskManager();
            auto jobQueue = applicationManager->getJobQueue();

            auto jobRendererSetup = fb::make_ptr<JobRendererSetup>();

            auto renderTask = taskManager->getTask( Thread::Task::Render );
            if( renderTask )
            {
                renderTask->queueJob( jobRendererSetup );
            }
            else
            {
                jobQueue->addJob( jobRendererSetup );
            }
        }

        void Application::loadGraphicsResources()
        {
            //auto applicationManager = core::IApplicationManager::instance();
            //FB_ASSERT( applicationManager );

            //auto graphicsSystem = applicationManager->getGraphicsSystem();
            //FB_ASSERT( graphicsSystem );

            //auto resourceGroupManager = graphicsSystem->getResourceGroupManager();
            //FB_ASSERT( resourceGroupManager );
            //// resourceGroupManager->load( nullptr );
            //graphicsSystem->loadObject( resourceGroupManager, true );
        }

        void Application::createPhysics()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto properties = fb::make_ptr<Properties>();

#if FB_BUILD_PHYSX
                auto physicsManager = physics::createPhysxManager( properties );
                applicationManager->setPhysicsManager( physicsManager );
#elif FB_BUILD_ODE
                auto physicsManager = fb::make_ptr<physics::CPhysicsManagerODE>();
                applicationManager->setPhysicsManager( physicsManager );
#elif FB_BUILD_PHYSICS3
                auto physicsManager = physics::Physics3::createPhysicsManager();
                applicationManager->setPhysicsManager( physicsManager );
#endif

                auto physicsScene = physicsManager->createScene();
                applicationManager->setPhysicsScene( physicsScene );

                auto vehicleManager = fb::make_ptr<CVehicleManager>();
                applicationManager->setVehicleManager( vehicleManager );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool Application::createScriptManager()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto scriptManager = factoryManager->make_object<IScriptManager>();
            scriptManager->load( nullptr );
            applicationManager->setScriptManager( scriptManager );

            return scriptManager != nullptr;
        }

        void Application::createInputSystem()
        {
            try
            {
#if defined FB_PLATFORM_APPLE
                return;
#endif

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                if( auto window = graphicsSystem->getDefaultWindow() )
                {
                    auto inputManager = factoryManager->make_object<IInputDeviceManager>();
                    applicationManager->setInputDeviceManager( inputManager );

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

                    auto gameInputMap0 = gameInput0->getGameInputMap();

                    ////Keyboard
                    // gameInputMap0->setKeyboardAction(Types::INPUT_UP, "UP", "");
                    // gameInputMap0->setKeyboardAction(Types::INPUT_DOWN, "DOWN", "");
                    // gameInputMap0->setKeyboardAction(Types::INPUT_RIGHT, "RIGHT", "");
                    // gameInputMap0->setKeyboardAction(Types::INPUT_LEFT, "LEFT", "");
                }
                else
                {
                    FB_LOG( "Could not create input system. No window found." );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Application::createGraphicsSceneManager()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto smgr = graphicsSystem->addGraphicsScene( "DefaultSceneManager", "ViewSM" );

                auto color = ColourF::White * 0.5f;
                smgr->setAmbientLight( color );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void Application::createTasks()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto taskManager = applicationManager->getTaskManager();
                FB_ASSERT( taskManager );

                // todo move
                auto profiler = fb::make_ptr<Profiler>();
                applicationManager->setProfiler( profiler );

                if( auto primaryTask = taskManager->getTask( Thread::Task::Primary ) )
                {
                    primaryTask->setTask( Thread::Task::Primary );
                    primaryTask->setPrimary( true );
                    primaryTask->setEnabled( true );
                    primaryTask->setOwner( this );
                    primaryTask->setTargetFPS( 60.0 );

                    auto profile = profiler->addProfile();
                    profile->setLabel( "Primary" );
                    primaryTask->setProfile( profile );
                }

                if( auto applicationTask = taskManager->getTask( Thread::Task::Application ) )
                {
                    applicationTask->setTask( Thread::Task::Application );
                    applicationTask->setPrimary( true );  // todo hack should not be primary
                    applicationTask->setEnabled( true );
                    applicationTask->setOwner( this );
                    applicationTask->setTargetFPS( 60.0 );

                    auto profile = profiler->addProfile();
                    profile->setLabel( "Application" );
                    applicationTask->setProfile( profile );
                }

                if( auto renderTask = taskManager->getTask( Thread::Task::Render ) )
                {
                    renderTask->setTask( Thread::Task::Render );
                    renderTask->setPrimary( true );
                    renderTask->setEnabled( true );
                    renderTask->setOwner( this );
                    renderTask->setTargetFPS( 60.0 );

                    auto profile = profiler->addProfile();
                    profile->setLabel( "Render" );
                    renderTask->setProfile( profile );
                }

                if( auto physicsTask = taskManager->getTask( Thread::Task::Physics ) )
                {
                    physicsTask->setTask( Thread::Task::Physics );
                    physicsTask->setPrimary( false );
                    physicsTask->setEnabled( true );
                    physicsTask->setOwner( this );
                    physicsTask->setTargetFPS( 120.0 );

                    auto profile = profiler->addProfile();
                    profile->setLabel( "Physics" );
                    physicsTask->setProfile( profile );
                }

                if( auto garbageCollectTask = taskManager->getTask( Thread::Task::GarbageCollect ) )
                {
                    garbageCollectTask->setTask( Thread::Task::GarbageCollect );
                    garbageCollectTask->setPrimary( false );
                    garbageCollectTask->setEnabled( true );
                    garbageCollectTask->setOwner( this );
                    garbageCollectTask->setTargetFPS( 30.0 );

                    auto profile = profiler->addProfile();
                    profile->setLabel( "Garbage Collect" );
                    garbageCollectTask->setProfile( profile );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<render::ICamera> Application::getUiCamera() const
        {
            return m_uiCamera;
        }

        void Application::setUiCamera( SmartPtr<render::ICamera> camera )
        {
            m_uiCamera = camera;
        }

        SmartPtr<render::ISceneNode> Application::getUiCameraSceneNode() const
        {
            return m_uiCameraSceneNode;
        }

        void Application::setUiCameraSceneNode( SmartPtr<render::ISceneNode> sceneNode )
        {
            m_uiCameraSceneNode = sceneNode;
        }

        SmartPtr<render::ICamera> Application::getCamera() const
        {
            return m_camera;
        }

        void Application::setCamera( SmartPtr<render::ICamera> camera )
        {
            m_camera = camera;
        }

        SmartPtr<render::ISceneNode> Application::getCameraSceneNode() const
        {
            return m_cameraSceneNode;
        }

        void Application::setCameraSceneNode( SmartPtr<render::ISceneNode> sceneNode )
        {
            m_cameraSceneNode = sceneNode;
        }

        SmartPtr<IStateContext> Application::getStateObject() const
        {
            return m_stateObject;
        }

        void Application::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        SmartPtr<IStateListener> Application::getStateListener() const
        {
            return m_stateListener;
        }

        void Application::setStateListener( SmartPtr<IStateListener> stateListener )
        {
            m_stateListener = stateListener;
        }

        SmartPtr<render::IViewport> Application::getRttViewport() const
        {
            return m_rttViewport;
        }

        void Application::setRttViewport( SmartPtr<render::IViewport> rttViewport )
        {
            m_rttViewport = rttViewport;
        }

        Application::ApplicationInputListener::ApplicationInputListener( Application *app ) :
            m_application( app )
        {
        }

        Application::ApplicationInputListener::~ApplicationInputListener()
        {
        }

        Parameter Application::ApplicationInputListener::handleEvent(
            IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
            SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        {
            if( eventValue == IEvent::inputEvent )
            {
                inputEvent( event );
            }

            return Parameter();
        }

        bool Application::ApplicationInputListener::inputEvent( SmartPtr<IInputEvent> event )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto smgr = graphicsSystem->getGraphicsScene( "ViewSM" );
            FB_ASSERT( smgr );

            if( m_application->inputEvent( event ) )
            {
                return true;
            }

            switch( event->getEventType() )
            {
            case IInputEvent::EventType::Mouse:
            {
                if( auto mouseState = event->getMouseState() )
                {
                    auto window = static_cast<render::IWindow *>( event->getWindow() );
                    if( !window )
                    {
                        return false;
                    }

                    auto windowSize = window->getSize();
                    Vector2F mousePosition = mouseState->getAbsolutePosition();
                    //Vector2F mouseCoords = mousePosition / windowSize;

                    if( mouseState->getEventType() == IMouseState::Event::LeftPressed )
                    {
                        // RoadManagerPtr roadManager = applicationManager->getRoadManager();

                        // if ( roadManager )
                        //{
                        //	if ( roadManager->getCreateRoadNode() )
                        //	{
                        //		//SmartPtr<ISceneView> meshSceneView =
                        // applicationManager->getMeshSceneView();
                        //		//SmartPtr<render::IGraphicsSceneManager> smgr =
                        // meshSceneView->getSceneManager();

                        //		// create road node
                        //		SmartPtr<ISceneNode> boxNode;
                        //		GraphicsMeshPtr boxMesh;

                        //		boxNode = smgr->getRootSceneNode()->addChildSceneNode();
                        //		boxMesh = smgr->addMesh("4x4chassis.mesh");
                        //		boxMesh->setMaterialName("Box/SphereMappedRustySteel");
                        //		boxNode->attachObject(boxMesh);
                        //		boxNode->setPosition(Vector3F(0, 0, 0));
                        //		boxNode->setScale(Vector3F::UNIT * 2.0f);

                        //		m_app->boxNode = boxNode;
                        //		m_app->boxMesh = boxMesh;

                        //		SphericalCameraPtr sphericalCamera = m_app->m_sphericalCamera;
                        //		Array<SmartPtr<ICamera>> cameras = sphericalCamera->getCameras();
                        //		for ( u32 i = 0; i < cameras.size(); ++i )
                        //		{
                        //			SmartPtr<ICamera> camera = cameras[i];

                        //			Ray3F ray = camera->getRay(mouseCoords.X(), mouseCoords.Y());

                        //			Vector3F intersectionPoint = Vector3F::ZERO;

                        //			Plane3F groundPlane(Vector3F::ZERO, Vector3F::UNIT_Y);
                        //			if ( groundPlane.getIntersectionWithLine(ray.getOrigin(),
                        // ray.getDirection(), intersectionPoint) )
                        //			{
                        //				boxNode->setPosition(intersectionPoint);
                        //			}
                        //		}
                        //	}
                        //}
                    }

                    // auto viewWindow = static_cast<ui::wxViewWindow*>(event->getUserData());
                    // auto camera = viewWindow->getCamera();
                    // if (!camera)
                    //{
                    //	return false;
                    // }

                    // Ray3F ray = camera->getRay(mouseCoords.X(), mouseCoords.Y());

                    // if (mouseState->getEventType() == IMouseState::MOUSE_EVENT_LEFT_BUTTON_PRESSED)
                    //{
                    //	//SmartPtr<ISceneView> meshSceneView = applicationManager->getMeshSceneView();

                    //	//Vector3F meshHitPosition;

                    //	//SmartPtr<render::IGraphicsSceneManager> sceneManager =
                    // meshSceneView->getSceneManager();
                    //	//if (sceneManager->castRay(ray, meshHitPosition))
                    //	//{
                    //	//	m_application->m_sphericalCamera->setTargetPosition(meshHitPosition);
                    //	//}
                    //}

                    //						SmartPtr<ITerrain> terrain =
                    // applicationManager->getTerrain(); 						if ( terrain &&
                    // terrain->isLoaded()
                    // )
                    //						{
                    //							TerrainRayResultPtr result = terrain->intersects(ray);
                    //							if ( result && result->hasIntersected() )
                    //							{
                    //								Vector3F hitPosition = result->getPosition();
                    //
                    //								SmartPtr<IDecalCursor> decalCursor =
                    // applicationManager->getDecalCursor(); 								if (
                    // decalCursor
                    // )
                    //								{
                    //									decalCursor->setPosition(hitPosition);
                    //								}
                    //
                    //								if(mouseState->getEventType() ==
                    // IMouseState::MOUSE_EVENT_LEFT_BUTTON_PRESSED)
                    //								{
                    //									if(applicationManager->getEditFoliage())
                    //									{
                    //#if 0
                    //										auto applicationManager =
                    // IApplicationManager::instance();
                    // SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();
                    // SmartPtr<render::IGraphicsSceneManager> smgr = graphicsSystem->getSceneManager("ViewSM");
                    //
                    //										GraphicsMeshPtr treeMesh =
                    // smgr->addMesh("Palm.mesh"); SmartPtr<ISceneNode> sceneNode =
                    // smgr->getRootSceneNode()->addChildSceneNode(); sceneNode->attachObject(treeMesh);
                    // sceneNode->setPosition(hitPosition);
                    //
                    //										QuaternionF orientation;
                    //										orientation.fromAngleAxis(MathF::DegToRad(-90.0f),
                    // Vector3F::UNIT_X);
                    // sceneNode->setOrientation(orientation); #else
                    //
                    //										ApplicationManager* editorManager =
                    // IApplicationManager::instance(); FoliageManagerPtr foliageMgr =
                    // editorManager->getFoliageManager();
                    //
                    //										Vector3F terrrainSpacePosition =
                    // terrain->getTerrainSpacePosition(result->getPosition());
                    //										foliageMgr->add(terrrainSpacePosition);
                    //
                    //#endif
                    //								}
                    //									else
                    //									{
                    //										Vector3F terrrainSpacePosition =
                    // terrain->getTerrainSpacePosition(result->getPosition());
                    // TerrainBlendMapPtr
                    // blendMap = terrain->getBlendMap(1); 										if (
                    // blendMap
                    // )
                    //										{
                    //											f32 blendMapSize =
                    // terrain->getLayerBlendMapSize(); 											u32
                    // brushSize = 64; Vector3F blendMapCoords(terrrainSpacePosition.X() * blendMapSize,
                    // terrrainSpacePosition.Y()
                    //* blendMapSize, 0.0f);
                    //
                    //											Vector2I minCoord(blendMapCoords.X() -
                    // brushSize, blendMapCoords.Y() - brushSize);
                    // Vector2I maxCoord(blendMapCoords.X() + brushSize, blendMapCoords.Y() + brushSize);
                    //
                    //											for ( u32 y = minCoord.Y(); y <
                    // maxCoord.Y();
                    //++y
                    //)
                    //											{
                    //												for ( u32 x = minCoord.X(); x <
                    // maxCoord.X();
                    //++x
                    //)
                    //												{
                    //													blendMap->setBlendValue(x, y,
                    // MathF::RangedRandom(0.0, 1.0));
                    //												}
                    //											}
                    //
                    //											blendMap->updateModifications();
                    //										}
                    //									}
                    //							}
                    //								else if ( mouseState->getEventType() ==
                    // IMouseState::MOUSE_EVENT_RIGHT_BUTTON_PRESSED )
                    //								{
                    //
                    //								}
                    //						}
                    //					}
                }
            }
            break;
            default:
            {
            }
            }

            return false;
        }

        bool Application::ApplicationInputListener::updateEvent( const SmartPtr<IInputEvent> &event )
        {
            return false;
        }

        void Application::ApplicationInputListener::setPriority( s32 priority )
        {
        }

        s32 Application::ApplicationInputListener::getPriority() const
        {
            return 0;
        }

        Application::ApplicationStateListener::~ApplicationStateListener()
        {
            setOwner( nullptr );
        }

        void Application::ApplicationStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void Application::ApplicationStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            // if (message->isExactly<StateMessage>())
            //{
            //	static const auto RENDERER_READY_HASH = StringUtil::getHash("RendererReady");

            //	auto type = message->getType();
            //	if (type == RENDERER_READY_HASH)
            //	{
            //		auto editorManager = EditorManager::getSingletonPtr();
            //		auto uiManager = editorManager->getUI();
            //		auto renderWindow = uiManager->getRenderWindow();
            //		renderWindow->setRenderer();

            //		auto applicationManager = core::IApplicationManager::instance();
            //		auto graphicsSystem = applicationManager->getGraphicsSystem();
            //		graphicsSystem->loadResources();
            //	}
            //}
        }

        void Application::ApplicationStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        Application *Application::ApplicationStateListener::getOwner() const
        {
            return m_owner;
        }

        void Application::ApplicationStateListener::setOwner( Application *owner )
        {
            m_owner = owner;
        }

        IFSM::ReturnType Application::ApplicationFSMListener::handleEvent( u32 state,
                                                                           IFSM::Event eventType )
        {
            return m_owner->handleEvent( state, eventType );
        }

        Application *Application::ApplicationFSMListener::getOwner() const
        {
            return m_owner;
        }

        void Application::ApplicationFSMListener::setOwner( Application *owner )
        {
            m_owner = owner;
        }

        Parameter Application::EventListener::handleEvent( IEvent::Type eventType, hash_type eventValue,
                                                           const Array<Parameter> &arguments,
                                                           SmartPtr<ISharedObject> sender,
                                                           SmartPtr<ISharedObject> object,
                                                           SmartPtr<IEvent> event )
        {
            return Parameter();
        }
    }  // end namespace editor
}  // end namespace fb
