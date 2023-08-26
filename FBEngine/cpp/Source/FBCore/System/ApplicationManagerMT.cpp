#include <FBCore/FBCorePCH.h>
#include <FBCore/System/ApplicationManagerMT.h>
#include <FBCore/Core/DebugTrace.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/Scene/IActor.h>
#include <FBCore/Interface/Scene/ICameraManager.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Interface/Scene/IScene.h>
#include <FBCore/Interface/Scene/IPrefabManager.h>
#include <FBCore/Interface/System/ILogManager.h>
#include <FBCore/Interface/System/ICommandManager.h>
#include <FBCore/Interface/Database/IDatabase.h>
#include <FBCore/Interface/Database/IDatabaseManager.h>
#include <FBCore/Interface/FSM/IFSM.h>
#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/Interface/IApplicationClient.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/Input/IInput.h>
#include <FBCore/Interface/Input/IInputDeviceManager.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/Mesh/IMeshLoader.h>
#include <FBCore/Interface/Particle/IParticleManager.h>
#include <FBCore/Interface/Physics/IPhysicsManager2D.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsScene2.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/System/IPluginInterface.h>
#include <FBCore/Interface/Procedural/IProceduralEngine.h>
#include <FBCore/Interface/Procedural/IProceduralManager.h>
#include <FBCore/Interface/Resource/IResourceDatabase.h>
#include <FBCore/Interface/Resource/IMeshManager.h>
#include <FBCore/Interface/Script/IScriptManager.h>
#include <FBCore/Interface/Sound/ISoundManager.h>
#include <FBCore/Interface/System/IAsyncOperation.h>
#include <FBCore/Interface/System/IConsole.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Interface/System/IJobQueue.h>
#include <FBCore/Interface/System/IProfiler.h>
#include <FBCore/Interface/System/IProcessManager.h>
#include <FBCore/Interface/System/ISelectionManager.h>
#include <FBCore/Interface/System/IStateContext.h>
#include <FBCore/Interface/System/IStateManager.h>
#include <FBCore/Interface/System/ISystemManager.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/System/ITransformRoot.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUIWindow.h>
#include <FBCore/Interface/Vehicle/IVehicleManager.h>
#include <FBCore/Interface/Video/IVideoManager.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <algorithm>
#include <cstdio>

namespace fb
{
    namespace core
    {
        FB_CLASS_REGISTER_DERIVED( fb, ApplicationManagerMT, IApplicationManager );

        ApplicationManagerMT::ApplicationManagerMT()
        {
        }

        ApplicationManagerMT::~ApplicationManagerMT()
        {
            unload( nullptr );
        }

        void ApplicationManagerMT::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto numTasks = (size_t)Thread::Task::Count;
                auto fsmManagers = fb::make_shared<Array<SmartPtr<IFSMManager>>>();
                fsmManagers->resize( numTasks );
                m_fsmManagers = fsmManagers;

#if defined FB_PLATFORM_WIN32
                setProjectPath( "" );

                const auto fileCachePath = String( ".FBCache/" );
                setCachePath( fileCachePath );

                const auto settingsCachePath = String( ".FBSettingsCache/" );
                setSettingsCachePath( settingsCachePath );
#elif defined FB_PLATFORM_APPLE
                auto path = Path::getAbsolutePath( Path::macBundlePath(), "../" );
                Path::setWorkingDirectory( path );

                setProjectPath( path );

                const auto fileCachePath = String( ".FBCache/" );
                setCachePath( fileCachePath );

                const auto settingsCachePath = String( ".FBSettingsCache/" );
                setSettingsCachePath( settingsCachePath );
#endif

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void ApplicationManagerMT::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    setApplication( nullptr );

                    if( auto taskManager = getTaskManager() )
                    {
                        taskManager->unload( nullptr );
                        setTaskManager( nullptr );
                    }

                    if( auto stateContext = getStateObject() )
                    {
                        if( auto stateManager = getStateManager() )
                        {
                            stateManager->removeStateObject( stateContext );
                        }

                        setStateObject( nullptr );
                    }

                    if( auto meshLoader = getMeshLoader() )
                    {
                        meshLoader->unload( nullptr );
                        setMeshLoader( nullptr );
                    }

                    if( auto scriptManager = getScriptManager() )
                    {
                        scriptManager->unload( nullptr );
                        setScriptManager( nullptr );
                    }

                    if( auto window = getWindow() )
                    {
                        setWindow( nullptr );
                    }

                    if( auto window = getSceneRenderWindow() )
                    {
                        setSceneRenderWindow( nullptr );
                    }

                    if( auto input = getInput() )
                    {
                        input->unload( nullptr );
                        setInput( nullptr );
                    }

                    if( auto inputDeviceManager = getInputDeviceManager() )
                    {
                        inputDeviceManager->unload( nullptr );
                        setInputDeviceManager( nullptr );
                    }

                    if( auto selectionManager = getSelectionManager() )
                    {
                        selectionManager->unload( nullptr );
                        setSelectionManager( nullptr );
                    }

                    if( auto prefabManager = getPrefabManager() )
                    {
                        prefabManager->unload( nullptr );
                        setPrefabManager( nullptr );
                    }

                    if( auto sceneManager = getSceneManager() )
                    {
                        sceneManager->unload( nullptr );
                        setSceneManager( nullptr );
                    }

                    if( auto database = getDatabase() )
                    {
                        database->unload( nullptr );
                        setDatabase( nullptr );
                    }

                    if( auto proceduralManager = getProceduralManager() )
                    {
                        proceduralManager->unload( nullptr );
                        setProceduralManager( nullptr );
                    }

                    if( auto systemManager = getSystemSettings() )
                    {
                        systemManager->unload( nullptr );
                        setSystemSettings( nullptr );
                    }

                    if( auto commandManager = getCommandManager() )
                    {
                        commandManager->unload( nullptr );
                        setCommandManager( nullptr );
                    }

                    if( auto resourceDatabase = getResourceDatabase() )
                    {
                        resourceDatabase->unload( nullptr );
                        setResourceDatabase( nullptr );
                    }

                    if( auto resourceManager = getResourceManager() )
                    {
                        resourceManager->unload( nullptr );
                        setResourceManager( nullptr );
                    }

                    if( auto processManager = getProcessManager() )
                    {
                        processManager->unload( nullptr );
                        setProcessManager( nullptr );
                    }

                    if( auto meshLoader = getMeshLoader() )
                    {
                        meshLoader->unload( nullptr );
                        meshLoader = nullptr;
                    }

                    if( auto meshManager = getMeshManager() )
                    {
                        meshManager->unload( nullptr );
                        setMeshManager( nullptr );
                    }

                    if( auto ui = getUI() )
                    {
                        ui->unload( nullptr );
                        setUI( nullptr );
                    }

                    if( auto ui = getRenderUI() )
                    {
                        ui->unload( nullptr );
                        setRenderUI( nullptr );
                    }

                    if( auto vehicleManager = getVehicleManager() )
                    {
                        vehicleManager->unload( nullptr );
                        setVehicleManager( nullptr );
                    }

                    if( auto physicsManager2 = getPhysicsManager2D() )
                    {
                        physicsManager2->unload( nullptr );
                        setPhysicsManager2D( nullptr );
                    }

                    if( auto physicsScene = getPhysicsScene() )
                    {
                        physicsScene->clear();
                        physicsScene->unload( nullptr );
                        setPhysicsScene( nullptr );
                    }

                    if( auto controlsScene = getControlsScene() )
                    {
                        controlsScene->unload( nullptr );
                        setControlsScene( nullptr );
                    }

                    if( auto raycastScene = getRaycastScene() )
                    {
                        raycastScene->unload( nullptr );
                        setRaycastScene( nullptr );
                    }

                    if( auto objectsScene = getObjectsScene() )
                    {
                        objectsScene->unload( nullptr );
                        setObjectsScene( nullptr );
                    }

                    if( auto physicsManager = getPhysicsManager() )
                    {
                        physicsManager->unload( nullptr );
                        setPhysicsManager( nullptr );
                    }

                    if( auto fileSystem = getFileSystem() )
                    {
                        fileSystem->unload( nullptr );
                        setFileSystem( nullptr );
                    }

                    if( auto graphicsSystem = getGraphicsSystem() )
                    {
                        graphicsSystem->unload( nullptr );
                        setGraphicsSystem( nullptr );
                    }

                    ITransformRoot::setInstance( nullptr );

                    if( auto threadPool = getThreadPool() )
                    {
                        threadPool->unload( nullptr );
                        setThreadPool( nullptr );
                    }

                    if( auto logManager = getLogManager() )
                    {
                        logManager->unload( nullptr );
                        setLogManager( nullptr );
                    }

                    if( auto timer = getTimer() )
                    {
                        timer->unload( nullptr );
                        setTimer( nullptr );
                    }

                    if( auto stateManager = getStateManager() )
                    {
                        stateManager->unload( nullptr );
                        setStateManager( nullptr );
                    }

                    for( auto plugin : m_plugins )
                    {
                        plugin->unload( nullptr );
                    }

                    m_plugins.clear();

                    if( auto fsmManager = getFsmManager() )
                    {
                        fsmManager->unload( nullptr );
                        setFsmManager( nullptr );
                    }

                    if( auto factoryManager = getFactoryManager() )
                    {
                        factoryManager->unload( nullptr );
                        setFactoryManager( nullptr );
                    }

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void ApplicationManagerMT::setEditorSettings( SmartPtr<Properties> properties )
        {
            m_editorSettings = properties;
        }

        SmartPtr<Properties> ApplicationManagerMT::getPlayerSettings() const
        {
            return m_playerSettings;
        }

        void ApplicationManagerMT::setPlayerSettings( SmartPtr<Properties> properties )
        {
            m_playerSettings = properties;
        }

        SmartPtr<ILogManager> ApplicationManagerMT::getLogManager() const
        {
            return m_logManager;
        }

        void ApplicationManagerMT::setLogManager( SmartPtr<ILogManager> logManager )
        {
            m_logManager = logManager;
        }

        SmartPtr<IFactoryManager> ApplicationManagerMT::getFactoryManager() const
        {
            return m_factoryManager;
        }

        void ApplicationManagerMT::setFactoryManager( SmartPtr<IFactoryManager> factoryManager )
        {
            m_factoryManager = factoryManager;
        }

        SmartPtr<IProcessManager> ApplicationManagerMT::getProcessManager() const
        {
            return m_processManager;
        }

        void ApplicationManagerMT::setProcessManager( SmartPtr<IProcessManager> processManager )
        {
            m_processManager = processManager;
        }

        SmartPtr<IApplicationClient> ApplicationManagerMT::getApplication() const
        {
            return m_application;
        }

        void ApplicationManagerMT::setApplication( SmartPtr<IApplicationClient> val )
        {
            m_application = val;
        }

        SmartPtr<IFileSystem> ApplicationManagerMT::getFileSystem() const
        {
            return m_fileSystem;
        }

        void ApplicationManagerMT::setFileSystem( SmartPtr<IFileSystem> fileSystem )
        {
            m_fileSystem = fileSystem;
        }

        SmartPtr<ITimer> ApplicationManagerMT::getTimer() const
        {
            return m_timer;
        }

        void ApplicationManagerMT::setTimer( SmartPtr<ITimer> timer )
        {
            m_timer = timer;
        }

        SmartPtr<IFSMManager> ApplicationManagerMT::getFsmManager() const
        {
            return m_fsmManager;
        }

        void ApplicationManagerMT::setFsmManager( SmartPtr<IFSMManager> fsmManager )
        {
            m_fsmManager = fsmManager;
        }

        SmartPtr<IFSMManager> ApplicationManagerMT::getFsmManagerByTask( Thread::Task task ) const
        {
            if( auto p = getFSMManagersPtr() )
            {
                auto &fsmManagers = *p;
                return fsmManagers[(u32)task];
            }

            return nullptr;
        }

        void ApplicationManagerMT::setFsmManagerByTask( Thread::Task task,
                                                        SmartPtr<IFSMManager> fsmManager )
        {
            auto p = getFSMManagersPtr();
            if( !p )
            {
                p = fb::make_shared<Array<SmartPtr<IFSMManager>>>();

                auto numTasks = (size_t)Thread::Task::Count;
                p->resize( numTasks );

                setFSMManagersPtr( p );
            }

            if( p )
            {
                auto &fsmManagers = *p;

                auto iTask = (u32)task;
                fsmManagers[iTask] = fsmManager;
            }
        }

        SmartPtr<procedural::IProceduralEngine> ApplicationManagerMT::getProceduralEngine() const
        {
            return m_proceduralEngine;
        }

        void ApplicationManagerMT::setProceduralEngine( SmartPtr<procedural::IProceduralEngine> val )
        {
            m_proceduralEngine = val;
        }

        SmartPtr<procedural::IProceduralManager> ApplicationManagerMT::getProceduralManager() const
        {
            return m_proceduralManager;
        }

        void ApplicationManagerMT::setProceduralManager( SmartPtr<procedural::IProceduralManager> val )
        {
            m_proceduralManager = val;
        }

        void ApplicationManagerMT::setPhysicsManager2D(
            SmartPtr<physics::IPhysicsManager2D> physicsManager )
        {
            m_physicsManager2 = physicsManager;
        }

        SmartPtr<physics::IPhysicsManager2D> ApplicationManagerMT::getPhysicsManager2D() const
        {
            return m_physicsManager2;
        }

        void ApplicationManagerMT::setPhysicsManager( SmartPtr<physics::IPhysicsManager> physicsManager )
        {
            m_physicsManager3 = physicsManager;
        }

        SmartPtr<physics::IPhysicsManager> ApplicationManagerMT::getPhysicsManager() const
        {
            return m_physicsManager3;
        }

        bool ApplicationManagerMT::isEditor() const
        {
            return m_isEditor;
        }

        void ApplicationManagerMT::setEditor( bool editor )
        {
            m_isEditor = editor;
        }

        bool ApplicationManagerMT::isEditorCamera() const
        {
            return m_isEditorCamera;
        }

        void ApplicationManagerMT::setEditorCamera( bool editorCamera )
        {
            m_isEditorCamera = editorCamera;
        }

        bool ApplicationManagerMT::isPlaying() const
        {
            return m_isPlaying;
        }

        void ApplicationManagerMT::setPlaying( bool playing )
        {
            m_isPlaying = playing;
        }

        bool ApplicationManagerMT::isPaused() const
        {
            return m_isPaused;
        }

        void ApplicationManagerMT::setPaused( bool paused )
        {
            m_isPaused = paused;
        }

        bool ApplicationManagerMT::isRunning() const
        {
            return m_isRunning;
        }

        void ApplicationManagerMT::setRunning( bool running )
        {
            m_isRunning = running;
        }

        bool ApplicationManagerMT::getQuit() const
        {
            return m_quit;
        }

        void ApplicationManagerMT::setQuit( bool quit )
        {
            m_quit = quit;
        }

        bool ApplicationManagerMT::hasTasks() const
        {
            if( const auto taskManager = getTaskManager() )
            {
                return taskManager->getNumTasks() > 0;
            }

            return false;
        }

        SmartPtr<IFSM> ApplicationManagerMT::getFSM() const
        {
            return m_fsm;
        }

        void ApplicationManagerMT::setFSM( SmartPtr<IFSM> fsm )
        {
            m_fsm = fsm;
        }

        SmartPtr<Properties> ApplicationManagerMT::getEditorSettings() const
        {
            return m_editorSettings;
        }

        SmartPtr<render::IGraphicsSystem> ApplicationManagerMT::getGraphicsSystem() const
        {
            return m_graphicsSystem;
        }

        void ApplicationManagerMT::setGraphicsSystem( SmartPtr<render::IGraphicsSystem> graphicsSystem )
        {
            m_graphicsSystem = graphicsSystem;
        }

        SmartPtr<IVideoManager> ApplicationManagerMT::getVideoManager() const
        {
            return m_videoManager;
        }

        void ApplicationManagerMT::setVideoManager( SmartPtr<IVideoManager> videoManager )
        {
            m_videoManager = videoManager;
        }

        SmartPtr<ITaskManager> ApplicationManagerMT::getTaskManager() const
        {
            return m_taskManager;
        }

        void ApplicationManagerMT::setTaskManager( SmartPtr<ITaskManager> taskManager )
        {
            m_taskManager = taskManager;
        }

        bool ApplicationManagerMT::isPauseMenuActive() const
        {
            return m_isPauseMenuActive;
        }

        void ApplicationManagerMT::setPauseMenuActive( bool pauseMenuActive )
        {
            m_isPauseMenuActive = pauseMenuActive;
        }

        SmartPtr<IProfiler> ApplicationManagerMT::getProfiler() const
        {
            return m_profiler;
        }

        void ApplicationManagerMT::setProfiler( SmartPtr<IProfiler> profiler )
        {
            m_profiler = profiler;
        }

        SmartPtr<ISystemManager> ApplicationManagerMT::getSystemSettings() const
        {
            return m_systemManager;
        }

        void ApplicationManagerMT::setSystemSettings( SmartPtr<ISystemManager> val )
        {
            m_systemManager = val;
        }

        SmartPtr<IJobQueue> ApplicationManagerMT::getJobQueue() const
        {
            return m_jobQueue;
        }

        void ApplicationManagerMT::setJobQueue( SmartPtr<IJobQueue> val )
        {
            m_jobQueue = val;
        }

        SmartPtr<render::IParticleManager> ApplicationManagerMT::getParticleManager() const
        {
            return m_particleManager;
        }

        void ApplicationManagerMT::setParticleManager(
            SmartPtr<render::IParticleManager> particleManager )
        {
            m_particleManager = particleManager;
        }

        void ApplicationManagerMT::setScriptManager( SmartPtr<IScriptManager> scriptManager )
        {
            m_scriptManager = scriptManager;
        }

        SmartPtr<IScriptManager> ApplicationManagerMT::getScriptManager() const
        {
            return m_scriptManager;
        }

        SmartPtr<IInput> ApplicationManagerMT::getInput() const
        {
            return m_input;
        }

        void ApplicationManagerMT::setInput( SmartPtr<IInput> input )
        {
            m_input = input;
        }

        void ApplicationManagerMT::setInputDeviceManager( SmartPtr<IInputDeviceManager> inputManager )
        {
            m_inputManager = inputManager;
        }

        SmartPtr<IInputDeviceManager> ApplicationManagerMT::getInputDeviceManager() const
        {
            return m_inputManager;
        }

        void ApplicationManagerMT::setThreadPool( SmartPtr<IThreadPool> threadPool )
        {
            m_threadPool = threadPool;
        }

        SmartPtr<IThreadPool> ApplicationManagerMT::getThreadPool() const
        {
            return m_threadPool;
        }

        void ApplicationManagerMT::setConsole( SmartPtr<IConsole> console )
        {
            m_console = console;
        }

        SmartPtr<IConsole> ApplicationManagerMT::getConsole() const
        {
            return m_console;
        }

        SmartPtr<scene::ICameraManager> ApplicationManagerMT::getCameraManager() const
        {
            return m_cameraManager;
        }

        void ApplicationManagerMT::setCameraManager( SmartPtr<scene::ICameraManager> cameraManager )
        {
            m_cameraManager = cameraManager;
        }

        void ApplicationManagerMT::setStateManager( SmartPtr<IStateManager> stateManager )
        {
            m_stateManager = stateManager;
        }

        SmartPtr<IStateManager> ApplicationManagerMT::getStateManager() const
        {
            return m_stateManager;
        }

        void ApplicationManagerMT::setCommandManager( SmartPtr<ICommandManager> commandManager )
        {
            m_commandManager = commandManager;
        }

        SmartPtr<ICommandManager> ApplicationManagerMT::getCommandManager() const
        {
            return m_commandManager;
        }

        void ApplicationManagerMT::setPrefabManager( SmartPtr<scene::IPrefabManager> prefabManager )
        {
            m_prefabManager = prefabManager;
        }

        SmartPtr<scene::IPrefabManager> ApplicationManagerMT::getPrefabManager() const
        {
            return m_prefabManager;
        }

        SmartPtr<IMeshLoader> ApplicationManagerMT::getMeshLoader() const
        {
            return m_meshLoader;
        }

        void ApplicationManagerMT::setMeshLoader( SmartPtr<IMeshLoader> meshLoader )
        {
            m_meshLoader = meshLoader;
        }

        SmartPtr<IResourceDatabase> ApplicationManagerMT::getResourceDatabase() const
        {
            return m_resourceDatabase;
        }

        void ApplicationManagerMT::setResourceDatabase( SmartPtr<IResourceDatabase> resourceDatabase )
        {
            m_resourceDatabase = resourceDatabase;
        }

        void ApplicationManagerMT::setSceneManager( SmartPtr<scene::ISceneManager> sceneManager )
        {
            m_sceneManager = sceneManager;
        }

        SmartPtr<scene::ISceneManager> ApplicationManagerMT::getSceneManager() const
        {
            return m_sceneManager;
        }

        void ApplicationManagerMT::setSelectionManager( SmartPtr<ISelectionManager> selectionManager )
        {
            m_selectionManager = selectionManager;
        }

        void ApplicationManagerMT::setResourceManager( SmartPtr<IResourceManager> resourceManager )
        {
            m_resourceManager = resourceManager;
        }

        SmartPtr<IResourceManager> ApplicationManagerMT::getResourceManager() const
        {
            return m_resourceManager;
        }

        SmartPtr<ISelectionManager> ApplicationManagerMT::getSelectionManager() const
        {
            return m_selectionManager;
        }

        SmartPtr<ISoundManager> ApplicationManagerMT::getSoundManager() const
        {
            return m_soundManager;
        }

        void ApplicationManagerMT::setSoundManager( SmartPtr<ISoundManager> soundManager )
        {
            m_soundManager = soundManager;
        }

        SmartPtr<physics::IPhysicsScene3> ApplicationManagerMT::getPhysicsScene() const
        {
            return m_physicsScene;
        }

        void ApplicationManagerMT::setPhysicsScene( SmartPtr<physics::IPhysicsScene3> physicsScene )
        {
            m_physicsScene = physicsScene;
        }

        SmartPtr<physics::IPhysicsScene3> ApplicationManagerMT::getObjectsScene() const
        {
            return m_objectsScene;
        }

        void ApplicationManagerMT::setObjectsScene( SmartPtr<physics::IPhysicsScene3> objectsScene )
        {
            m_objectsScene = objectsScene;
        }

        SmartPtr<physics::IPhysicsScene3> ApplicationManagerMT::getRaycastScene() const
        {
            return m_raycastScene;
        }

        void ApplicationManagerMT::setRaycastScene( SmartPtr<physics::IPhysicsScene3> raycastScene )
        {
            m_raycastScene = raycastScene;
        }

        SmartPtr<physics::IPhysicsScene3> ApplicationManagerMT::getControlsScene() const
        {
            return m_controlsScene;
        }

        void ApplicationManagerMT::setControlsScene( SmartPtr<physics::IPhysicsScene3> controlsScene )
        {
            m_controlsScene = controlsScene;
        }

        SmartPtr<IPluginInterface> ApplicationManagerMT::getPluginInterface() const
        {
            return m_pluginInterface;
        }

        void ApplicationManagerMT::setPluginInterface( SmartPtr<IPluginInterface> pluginInterface )
        {
            m_pluginInterface = pluginInterface;
        }

        SmartPtr<scene::IScene> ApplicationManagerMT::getScene() const
        {
            auto sceneManager = getSceneManager();
            return sceneManager->getCurrentScene();
        }

        bool ApplicationManagerMT::getEnableRenderer() const
        {
            return m_enableRenderer;
        }

        void ApplicationManagerMT::setEnableRenderer( bool enable )
        {
            m_enableRenderer = enable;
        }

        SmartPtr<ui::IUIManager> ApplicationManagerMT::getUI() const
        {
            return m_ui;
        }

        void ApplicationManagerMT::setUI( SmartPtr<ui::IUIManager> ui )
        {
            m_ui = ui;
        }

        SmartPtr<ui::IUIManager> ApplicationManagerMT::getRenderUI() const
        {
            return m_renderUiManager;
        }

        void ApplicationManagerMT::setRenderUI( SmartPtr<ui::IUIManager> renderUI )
        {
            m_renderUiManager = renderUI;
        }

        SmartPtr<IDatabaseManager> ApplicationManagerMT::getDatabase() const
        {
            return m_database;
        }

        void ApplicationManagerMT::setDatabase( SmartPtr<IDatabaseManager> val )
        {
            m_database = val;
        }

        SmartPtr<Properties> ApplicationManagerMT::getProperties() const
        {
            return m_properties;
        }

        void ApplicationManagerMT::setProperties( SmartPtr<Properties> val )
        {
            m_properties = val;
        }

        SmartPtr<IVehicleManager> ApplicationManagerMT::getVehicleManager() const
        {
            return m_vehicleManager;
        }

        void ApplicationManagerMT::setVehicleManager( SmartPtr<IVehicleManager> vehicleManager )
        {
            m_vehicleManager = vehicleManager;
        }

        String ApplicationManagerMT::getCachePath() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_cachePath;
        }

        void ApplicationManagerMT::setCachePath( const String &cachePath )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_cachePath = cachePath;
        }

        String ApplicationManagerMT::getProjectPath() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_projectPath;
        }

        void ApplicationManagerMT::setProjectPath( const String &projectPath )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_projectPath = projectPath;
        }

        String ApplicationManagerMT::getProjectLibraryName() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_projectLibraryName;
        }

        void ApplicationManagerMT::setProjectLibraryName( const String &projectLibraryName )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_projectLibraryName = projectLibraryName;
        }

        void ApplicationManagerMT::setSettingsCachePath( const String &val )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_settingsCachePath = val;
        }

        String ApplicationManagerMT::getBuildConfig() const
        {
#if _DEBUG
            return String( "Debug" );
#else
            return String( "Release" );
#endif
        }

        String ApplicationManagerMT::getProjectLibraryExtension() const
        {
            return ".dll";
            // return ".lib";
        }

        String ApplicationManagerMT::getProjectLibraryPath() const
        {
            const auto cachePath = getCachePath();
            const auto libraryName = getProjectLibraryName();
            const auto libraryExt = getProjectLibraryExtension();
            const auto config = getBuildConfig();

            return cachePath + "/project/" + config + "/" + libraryName + libraryExt;
        }

        String ApplicationManagerMT::getMediaPath() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_mediaPath;
        }

        void ApplicationManagerMT::setMediaPath( const String &mediaPath )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            m_mediaPath = mediaPath;
        }

        String ApplicationManagerMT::getSettingsCachePath() const
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            return m_settingsCachePath;
        }

        Thread::Task ApplicationManagerMT::getStateTask() const
        {
            return hasTasks() ? Thread::Task::Application : Thread::Task::Primary;
        }

        Thread::Task ApplicationManagerMT::getApplicationTask() const
        {
            if( auto taskManager = getTaskManager() )
            {
                if( auto task = taskManager->getTask( Thread::Task::Application ) )
                {
                    if( task->isExecuting() )
                    {
                        return Thread::Task::Application;
                    }
                    if( task->isPrimary() )
                    {
                        return Thread::Task::Primary;
                    }
                }
            }

            return hasTasks() ? Thread::Task::Application : Thread::Task::Primary;
        }

        SmartPtr<IMeshManager> ApplicationManagerMT::getMeshManager() const
        {
            return m_meshManager;
        }

        void ApplicationManagerMT::setMeshManager( SmartPtr<IMeshManager> val )
        {
            m_meshManager = val;
        }

        s32 ApplicationManagerMT::getLoadProgress() const
        {
            return m_loadProgress;
        }

        void ApplicationManagerMT::setLoadProgress( s32 loadProgress )
        {
            m_loadProgress = loadProgress;
        }

        void ApplicationManagerMT::addLoadProgress( s32 loadProgress )
        {
            m_loadProgress += loadProgress;
        }

        Array<SmartPtr<scene::IActor>> ApplicationManagerMT::getActors() const
        {
            FB_ASSERT( isValid() );

            if( auto sceneManager = getSceneManager() )
            {
                FB_ASSERT( sceneManager->isValid() );

                if( auto scene = sceneManager->getCurrentScene() )
                {
                    FB_ASSERT( scene->isValid() );
                    return scene->getActors();
                }
            }

            return Array<SmartPtr<scene::IActor>>();
        }

        void ApplicationManagerMT::addPlugin( SmartPtr<ISharedObject> plugin )
        {
            FB_DEBUG_TRACE;

            FB_ASSERT( plugin );

            RecursiveMutex::ScopedLock lock( m_mutex );

            if( plugin )
            {
                plugin->load( nullptr );
                m_plugins.push_back( plugin );
            }
        }

        void ApplicationManagerMT::removePlugin( SmartPtr<ISharedObject> plugin )
        {
            FB_DEBUG_TRACE;

            RecursiveMutex::ScopedLock lock( m_mutex );
            m_plugins.erase( std::remove( m_plugins.begin(), m_plugins.end(), plugin ),
                             m_plugins.end() );
        }

        SmartPtr<IStateContext> ApplicationManagerMT::getStateObject() const
        {
            return m_stateObject;
        }

        void ApplicationManagerMT::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        SmartPtr<render::IWindow> ApplicationManagerMT::getWindow() const
        {
            return m_window;
        }

        void ApplicationManagerMT::setWindow( SmartPtr<render::IWindow> window )
        {
            m_window = window;
        }

        SmartPtr<ui::IUIWindow> ApplicationManagerMT::getSceneRenderWindow() const
        {
            return m_sceneRenderWindow;
        }

        void ApplicationManagerMT::setSceneRenderWindow( SmartPtr<ui::IUIWindow> sceneRenderWindow )
        {
            m_sceneRenderWindow = sceneRenderWindow;
        }

        SmartPtr<scene::IComponent> ApplicationManagerMT::getComponentByType( u32 typeId ) const
        {
            const auto &typeManager = TypeManager::instance();

            auto actors = getActors();
            for( auto &actor : actors )
            {
                auto components = actor->getComponents();
                for( auto &component : components )
                {
                    if( typeManager->isDerived( typeId, component->getTypeInfo() ) )
                    {
                        return component;
                    }
                }
            }

            return nullptr;
        }

        Parameter ApplicationManagerMT::triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                                      const Array<Parameter> &arguments,
                                                      SmartPtr<ISharedObject> sender,
                                                      SmartPtr<ISharedObject> object,
                                                      SmartPtr<IEvent> event )
        {
            if( auto stateContext = getStateObject() )
            {
                return stateContext->triggerEvent( eventType, eventValue, arguments, sender, object,
                                                   event );
            }

            return Parameter();
        }

        SharedPtr<Array<SmartPtr<IFSMManager>>> ApplicationManagerMT::getFSMManagersPtr() const
        {
            return m_fsmManagers;
        }

        void ApplicationManagerMT::setFSMManagersPtr(
            SharedPtr<Array<SmartPtr<IFSMManager>>> fsmManagers )
        {
            m_fsmManagers = fsmManagers;
        }

    }  // namespace core
}  // namespace fb
