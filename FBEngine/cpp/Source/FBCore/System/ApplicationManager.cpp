#include <FBCore/FBCorePCH.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/Core/DebugTrace.h>
#include <FBCore/Core/Path.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Memory/MemoryTracker.h>
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
#include <FBCore/Interface/IApplication.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IParticleManager.h>
#include <FBCore/Interface/Graphics/IVideoManager.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/Input/IInput.h>
#include <FBCore/Interface/Input/IInputDeviceManager.h>
#include <FBCore/Interface/IO/IFileSystem.h>
#include <FBCore/Interface/Mesh/IMeshLoader.h>
#include <FBCore/Interface/Physics/IPhysicsManager2D.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>
#include <FBCore/Interface/Physics/IPhysicsScene2.h>
#include <FBCore/Interface/Physics/IPhysicsScene3.h>
#include <FBCore/Interface/System/IPluginManager.h>
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
#include <FBCore/Interface/System/ISystemSettings.h>
#include <FBCore/Interface/System/ITaskManager.h>
#include <FBCore/Interface/System/ITask.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUIWindow.h>
#include <FBCore/Interface/Vehicle/IVehicleManager.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <algorithm>
#include <cstdio>

#if FB_ENABLE_MEMORY_TRACKER
#include <boost/stacktrace/stacktrace.hpp>
#endif

namespace fb::core
{
    FB_CLASS_REGISTER_DERIVED( fb, ApplicationManager, ApplicationManager );
    SmartPtr<ApplicationManager> ApplicationManager::m_instance;

    ApplicationManager::ApplicationManager() = default;

    ApplicationManager::~ApplicationManager()
    {
        unload( nullptr );
    }

    void ApplicationManager::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Loading );

            auto numTasks = static_cast<size_t>( Thread::Task::Count );
            auto fsmManagers = fb::make_shared<Array<SmartPtr<IFSMManager>>>();
            fsmManagers->resize( numTasks );
            m_fsmManagers = fsmManagers;

#if defined FB_PLATFORM_WIN32
            setProjectPath( "" );

            const auto fileCachePath = String( ".FBCache/" );
            setCachePath( fileCachePath );

            const auto settingsCachePath = String( ".FBSettingsCache/" );
            setSettingsPath( settingsCachePath );
#elif defined FB_PLATFORM_APPLE
            auto macBundlePath = Path::macBundlePath();
            //auto path = Path::getAbsolutePath( macBundlePath, "../" );
            //Path::setWorkingDirectory( path );

            auto projectPath = String();
            setProjectPath( projectPath );

            const auto fileCachePath = String( ".FBCache/" );
            setCachePath( fileCachePath );

            const auto settingsCachePath = String( ".FBSettingsCache/" );
            setSettingsPath( settingsCachePath );
#endif

#if !FB_FINAL
            auto mediaPath = String( "" );

#    if defined FB_PLATFORM_WIN32
            mediaPath = String( "../../../../../Media" );
#    elif defined FB_PLATFORM_APPLE
            mediaPath = String( "../../Media" );
#    else
            mediaPath = String( "../../Media" );
#    endif

            setMediaPath( mediaPath );
#endif

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void ApplicationManager::unload( SmartPtr<ISharedObject> data )
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

                if( auto stateContext = getStateContext() )
                {
                    if( auto stateManager = getStateManager() )
                    {
                        stateManager->removeStateObject( stateContext );
                    }

                    setStateContext( nullptr );
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

                if( auto soundManager = getSoundManager() )
                {
                    soundManager->unload( nullptr );
                    setSoundManager( nullptr );
                }

                if( auto graphicsSystem = getGraphicsSystem() )
                {
                    graphicsSystem->unload( nullptr );
                    setGraphicsSystem( nullptr );
                }

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

    void ApplicationManager::setEditorSettings( SmartPtr<Properties> properties )
    {
        m_editorSettings = properties;
    }

    auto ApplicationManager::getPlayerSettings() const -> SmartPtr<Properties>
    {
        return m_playerSettings;
    }

    void ApplicationManager::setPlayerSettings( SmartPtr<Properties> properties )
    {
        m_playerSettings = properties;
    }

    auto ApplicationManager::getLogManager() const -> SmartPtr<ILogManager>
    {
        return m_logManager;
    }

    void ApplicationManager::setLogManager( SmartPtr<ILogManager> logManager )
    {
        m_logManager = logManager;
    }

    void ApplicationManager::setFactoryManager( SmartPtr<IFactoryManager> factoryManager )
    {
        m_factoryManager = factoryManager;
    }

    auto ApplicationManager::getProcessManager() const -> SmartPtr<IProcessManager>
    {
        return m_processManager;
    }

    void ApplicationManager::setProcessManager( SmartPtr<IProcessManager> processManager )
    {
        m_processManager = processManager;
    }

    auto ApplicationManager::getApplication() const -> SmartPtr<IApplication>
    {
        return m_application;
    }

    void ApplicationManager::setApplication( SmartPtr<IApplication> val )
    {
        m_application = val;
    }

    void ApplicationManager::setFileSystem( SmartPtr<IFileSystem> fileSystem )
    {
        m_fileSystem = fileSystem;
    }

    void ApplicationManager::setTimer( SmartPtr<ITimer> timer )
    {
        m_timer = timer;
    }

    void ApplicationManager::setFsmManager( SmartPtr<IFSMManager> fsmManager )
    {
        m_fsmManager = fsmManager;
    }

    auto ApplicationManager::getFsmManagerByTask( Thread::Task task ) const -> SmartPtr<IFSMManager>
    {
        if( auto p = getFSMManagersPtr() )
        {
            auto &fsmManagers = *p;
            return fsmManagers[static_cast<u32>( task )];
        }

        return nullptr;
    }

    void ApplicationManager::setFsmManagerByTask( Thread::Task task, SmartPtr<IFSMManager> fsmManager )
    {
        auto p = getFSMManagersPtr();
        if( !p )
        {
            p = fb::make_shared<Array<SmartPtr<IFSMManager>>>();

            auto numTasks = static_cast<size_t>( Thread::Task::Count );
            p->resize( numTasks );

            setFSMManagersPtr( p );
        }

        if( p )
        {
            auto &fsmManagers = *p;

            auto iTask = static_cast<u32>( task );
            fsmManagers[iTask] = fsmManager;
        }
    }

    auto ApplicationManager::getProceduralEngine() const -> SmartPtr<procedural::IProceduralEngine>
    {
        return m_proceduralEngine;
    }

    void ApplicationManager::setProceduralEngine( SmartPtr<procedural::IProceduralEngine> val )
    {
        m_proceduralEngine = val;
    }

    auto ApplicationManager::getProceduralManager() const -> SmartPtr<procedural::IProceduralManager>
    {
        return m_proceduralManager;
    }

    void ApplicationManager::setProceduralManager( SmartPtr<procedural::IProceduralManager> val )
    {
        m_proceduralManager = val;
    }

    void ApplicationManager::setPhysicsManager2D( SmartPtr<physics::IPhysicsManager2D> physicsManager )
    {
        m_physicsManager2 = physicsManager;
    }

    auto ApplicationManager::getPhysicsManager2D() const -> SmartPtr<physics::IPhysicsManager2D>
    {
        return m_physicsManager2;
    }

    void ApplicationManager::setPhysicsManager( SmartPtr<physics::IPhysicsManager> physicsManager )
    {
        m_physicsManager3 = physicsManager;
    }

    auto ApplicationManager::getPhysicsManager() const -> SmartPtr<physics::IPhysicsManager>
    {
        return m_physicsManager3;
    }

    auto ApplicationManager::isEditor() const -> bool
    {
        return m_isEditor;
    }

    void ApplicationManager::setEditor( bool editor )
    {
        m_isEditor = editor;
    }

    auto ApplicationManager::isEditorCamera() const -> bool
    {
        return m_isEditorCamera;
    }

    void ApplicationManager::setEditorCamera( bool editorCamera )
    {
        m_isEditorCamera = editorCamera;
    }

    auto ApplicationManager::isPlaying() const -> bool
    {
        return m_isPlaying;
    }

    void ApplicationManager::setPlaying( bool playing )
    {
        m_isPlaying = playing;
    }

    auto ApplicationManager::isPaused() const -> bool
    {
        return m_isPaused;
    }

    void ApplicationManager::setPaused( bool paused )
    {
        m_isPaused = paused;
    }

    auto ApplicationManager::isRunning() const -> bool
    {
        return m_isRunning;
    }

    void ApplicationManager::setRunning( bool running )
    {
        m_isRunning = running;
    }

    auto ApplicationManager::getQuit() const -> bool
    {
        return m_quit;
    }

    void ApplicationManager::setQuit( bool quit )
    {
        m_quit = quit;
    }

    auto ApplicationManager::hasTasks() const -> bool
    {
        if( const auto taskManager = getTaskManager() )
        {
            return taskManager->getNumTasks() > 0;
        }

        return false;
    }

    auto ApplicationManager::getFSM() const -> SmartPtr<IFSM>
    {
        return m_fsm;
    }

    void ApplicationManager::setFSM( SmartPtr<IFSM> fsm )
    {
        m_fsm = fsm;
    }

    auto ApplicationManager::getEditorSettings() const -> SmartPtr<Properties>
    {
        return m_editorSettings;
    }

    void ApplicationManager::setGraphicsSystem( SmartPtr<render::IGraphicsSystem> graphicsSystem )
    {
        m_graphicsSystem = graphicsSystem;
    }

    auto ApplicationManager::getVideoManager() const -> SmartPtr<render::IVideoManager>
    {
        return m_videoManager;
    }

    void ApplicationManager::setVideoManager( SmartPtr<render::IVideoManager> videoManager )
    {
        m_videoManager = videoManager;
    }

    auto ApplicationManager::getTaskManager() const -> SmartPtr<ITaskManager>
    {
        return m_taskManager;
    }

    void ApplicationManager::setTaskManager( SmartPtr<ITaskManager> taskManager )
    {
        m_taskManager = taskManager;
    }

    auto ApplicationManager::isPauseMenuActive() const -> bool
    {
        return m_isPauseMenuActive;
    }

    void ApplicationManager::setPauseMenuActive( bool pauseMenuActive )
    {
        m_isPauseMenuActive = pauseMenuActive;
    }

    auto ApplicationManager::getProfiler() const -> SmartPtr<IProfiler>
    {
        return m_profiler;
    }

    void ApplicationManager::setProfiler( SmartPtr<IProfiler> profiler )
    {
        m_profiler = profiler;
    }

    auto ApplicationManager::getSystemSettings() const -> SmartPtr<ISystemSettings>
    {
        return m_systemManager;
    }

    void ApplicationManager::setSystemSettings( SmartPtr<ISystemSettings> val )
    {
        m_systemManager = val;
    }

    void ApplicationManager::setJobQueue( SmartPtr<IJobQueue> jobQueue )
    {
        m_jobQueue = jobQueue;
    }

    auto ApplicationManager::getParticleManager() const -> SmartPtr<render::IParticleManager>
    {
        return m_particleManager;
    }

    void ApplicationManager::setParticleManager( SmartPtr<render::IParticleManager> particleManager )
    {
        m_particleManager = particleManager;
    }

    void ApplicationManager::setScriptManager( SmartPtr<IScriptManager> scriptManager )
    {
        m_scriptManager = scriptManager;
    }

    auto ApplicationManager::getScriptManager() const -> SmartPtr<IScriptManager>
    {
        return m_scriptManager;
    }

    auto ApplicationManager::getInput() const -> SmartPtr<IInput>
    {
        return m_input;
    }

    void ApplicationManager::setInput( SmartPtr<IInput> input )
    {
        m_input = input;
    }

    void ApplicationManager::setInputDeviceManager( SmartPtr<IInputDeviceManager> inputManager )
    {
        m_inputManager = inputManager;
    }

    auto ApplicationManager::getInputDeviceManager() const -> SmartPtr<IInputDeviceManager>
    {
        return m_inputManager;
    }

    void ApplicationManager::setThreadPool( SmartPtr<IThreadPool> threadPool )
    {
        m_threadPool = threadPool;
    }

    void ApplicationManager::setConsole( SmartPtr<IConsole> console )
    {
        m_console = console;
    }

    auto ApplicationManager::getConsole() const -> SmartPtr<IConsole>
    {
        return m_console;
    }

    auto ApplicationManager::getCameraManager() const -> SmartPtr<scene::ICameraManager>
    {
        return m_cameraManager;
    }

    void ApplicationManager::setCameraManager( SmartPtr<scene::ICameraManager> cameraManager )
    {
        m_cameraManager = cameraManager;
    }

    void ApplicationManager::setStateManager( SmartPtr<IStateManager> stateManager )
    {
        m_stateManager = stateManager;
    }

    auto ApplicationManager::getStateManager() -> SmartPtr<IStateManager> &
    {
        return m_stateManager;
    }

    auto ApplicationManager::getStateManager() const -> const SmartPtr<IStateManager> &
    {
        return m_stateManager;
    }

    void ApplicationManager::setCommandManager( SmartPtr<ICommandManager> commandManager )
    {
        m_commandManager = commandManager;
    }

    auto ApplicationManager::getCommandManager() const -> SmartPtr<ICommandManager>
    {
        return m_commandManager;
    }

    void ApplicationManager::setPrefabManager( SmartPtr<scene::IPrefabManager> prefabManager )
    {
        m_prefabManager = prefabManager;
    }

    auto ApplicationManager::getPrefabManager() const -> SmartPtr<scene::IPrefabManager>
    {
        return m_prefabManager;
    }

    auto ApplicationManager::getMeshLoader() const -> SmartPtr<IMeshLoader>
    {
        return m_meshLoader;
    }

    void ApplicationManager::setMeshLoader( SmartPtr<IMeshLoader> meshLoader )
    {
        m_meshLoader = meshLoader;
    }

    auto ApplicationManager::getResourceDatabase() const -> SmartPtr<IResourceDatabase>
    {
        return m_resourceDatabase;
    }

    void ApplicationManager::setResourceDatabase( SmartPtr<IResourceDatabase> resourceDatabase )
    {
        m_resourceDatabase = resourceDatabase;
    }

    void ApplicationManager::setSceneManager( SmartPtr<scene::ISceneManager> sceneManager )
    {
        m_sceneManager = sceneManager;
    }

    auto ApplicationManager::getSceneManager() const -> SmartPtr<scene::ISceneManager>
    {
        return m_sceneManager;
    }

    void ApplicationManager::setSelectionManager( SmartPtr<ISelectionManager> selectionManager )
    {
        m_selectionManager = selectionManager;
    }

    void ApplicationManager::setResourceManager( SmartPtr<IResourceManager> resourceManager )
    {
        m_resourceManager = resourceManager;
    }

    auto ApplicationManager::getResourceManager() const -> SmartPtr<IResourceManager>
    {
        return m_resourceManager;
    }

    auto ApplicationManager::getSelectionManager() const -> SmartPtr<ISelectionManager>
    {
        return m_selectionManager;
    }

    auto ApplicationManager::getSoundManager() const -> SmartPtr<ISoundManager>
    {
        return m_soundManager;
    }

    void ApplicationManager::setSoundManager( SmartPtr<ISoundManager> soundManager )
    {
        m_soundManager = soundManager;
    }

    auto ApplicationManager::getPhysicsScene() const -> SmartPtr<physics::IPhysicsScene3>
    {
        return m_physicsScene;
    }

    void ApplicationManager::setPhysicsScene( SmartPtr<physics::IPhysicsScene3> physicsScene )
    {
        m_physicsScene = physicsScene;
    }

    auto ApplicationManager::getObjectsScene() const -> SmartPtr<physics::IPhysicsScene3>
    {
        return m_objectsScene;
    }

    void ApplicationManager::setObjectsScene( SmartPtr<physics::IPhysicsScene3> objectsScene )
    {
        m_objectsScene = objectsScene;
    }

    auto ApplicationManager::getRaycastScene() const -> SmartPtr<physics::IPhysicsScene3>
    {
        return m_raycastScene;
    }

    void ApplicationManager::setRaycastScene( SmartPtr<physics::IPhysicsScene3> raycastScene )
    {
        m_raycastScene = raycastScene;
    }

    auto ApplicationManager::getControlsScene() const -> SmartPtr<physics::IPhysicsScene3>
    {
        return m_controlsScene;
    }

    void ApplicationManager::setControlsScene( SmartPtr<physics::IPhysicsScene3> controlsScene )
    {
        m_controlsScene = controlsScene;
    }

    auto ApplicationManager::getScene() const -> SmartPtr<scene::IScene>
    {
        auto sceneManager = getSceneManager();
        return sceneManager->getCurrentScene();
    }

    auto ApplicationManager::getEnableRenderer() const -> bool
    {
        return m_enableRenderer;
    }

    void ApplicationManager::setEnableRenderer( bool enable )
    {
        m_enableRenderer = enable;
    }

    auto ApplicationManager::getUI() const -> SmartPtr<ui::IUIManager>
    {
        return m_ui;
    }

    void ApplicationManager::setUI( SmartPtr<ui::IUIManager> ui )
    {
        m_ui = ui;
    }

    auto ApplicationManager::getRenderUI() const -> SmartPtr<ui::IUIManager>
    {
        return m_renderUiManager;
    }

    void ApplicationManager::setRenderUI( SmartPtr<ui::IUIManager> renderUI )
    {
        m_renderUiManager = renderUI;
    }

    auto ApplicationManager::getDatabase() const -> SmartPtr<IDatabaseManager>
    {
        return m_database;
    }

    void ApplicationManager::setDatabase( SmartPtr<IDatabaseManager> val )
    {
        m_database = val;
    }

    auto ApplicationManager::getProperties() const -> SmartPtr<Properties>
    {
        return m_properties;
    }

    void ApplicationManager::setProperties( SmartPtr<Properties> val )
    {
        m_properties = val;
    }

    auto ApplicationManager::getVehicleManager() const -> SmartPtr<IVehicleManager>
    {
        return m_vehicleManager;
    }

    void ApplicationManager::setVehicleManager( SmartPtr<IVehicleManager> vehicleManager )
    {
        m_vehicleManager = vehicleManager;
    }

    auto ApplicationManager::getCachePath() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_cachePath;
    }

    void ApplicationManager::setCachePath( const String &cachePath )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_cachePath = cachePath;
    }

    auto ApplicationManager::getProjectPath() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_projectPath;
    }

    void ApplicationManager::setProjectPath( const String &projectPath )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_projectPath = projectPath;
    }

    auto ApplicationManager::getProjectLibraryName() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_projectLibraryName;
    }

    void ApplicationManager::setProjectLibraryName( const String &projectLibraryName )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_projectLibraryName = projectLibraryName;
    }

    void ApplicationManager::setSettingsPath( const String &val )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_settingsCachePath = val;
    }

    auto ApplicationManager::getBuildConfig() const -> String
    {
#if _DEBUG
        return String( "Debug" );
#else
        return String( "Release" );
#endif
    }

    auto ApplicationManager::getProjectLibraryExtension() const -> String
    {
        return ".dll";
        // return ".lib";
    }

    auto ApplicationManager::getProjectLibraryPath() const -> String
    {
        const auto cachePath = getCachePath();
        const auto libraryName = getProjectLibraryName();
        const auto libraryExt = getProjectLibraryExtension();
        const auto config = getBuildConfig();

        return cachePath + "/project/" + config + "/" + libraryName + libraryExt;
    }

    auto ApplicationManager::getMediaPath() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_mediaPath;
    }

    void ApplicationManager::setMediaPath( const String &mediaPath )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_mediaPath = mediaPath;
    }

    auto ApplicationManager::getRenderMediaPath() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_renderMediaPath;
    }

    void ApplicationManager::setRenderMediaPath( const String &renderMediaPath )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        m_renderMediaPath = renderMediaPath;
    }

    auto ApplicationManager::getSettingsPath() const -> String
    {
        RecursiveMutex::ScopedLock lock( m_mutex );
        return m_settingsCachePath;
    }

    auto ApplicationManager::getStateTask() const -> Thread::Task
    {
        return hasTasks() ? Thread::Task::Application : Thread::Task::Primary;
    }

    auto ApplicationManager::getApplicationTask() const -> Thread::Task
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

    auto ApplicationManager::getMeshManager() const -> SmartPtr<IMeshManager>
    {
        return m_meshManager;
    }

    void ApplicationManager::setMeshManager( SmartPtr<IMeshManager> val )
    {
        m_meshManager = val;
    }

    auto ApplicationManager::getLoadProgress() const -> s32
    {
        return m_loadProgress;
    }

    void ApplicationManager::setLoadProgress( s32 loadProgress )
    {
        m_loadProgress = loadProgress;
    }

    void ApplicationManager::addLoadProgress( s32 loadProgress )
    {
        m_loadProgress += loadProgress;
    }

    auto ApplicationManager::getActors() const -> Array<SmartPtr<scene::IActor>>
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

        return {};
    }

    auto ApplicationManager::getPluginManager() const -> SmartPtr<IPluginManager>
    {
        return m_pluginManager;
    }

    void ApplicationManager::setPluginManager( SmartPtr<IPluginManager> pluginManager )
    {
        m_pluginManager = pluginManager;
    }

    void ApplicationManager::addPlugin( SmartPtr<ISharedObject> plugin )
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

    void ApplicationManager::removePlugin( SmartPtr<ISharedObject> plugin )
    {
        FB_DEBUG_TRACE;

        RecursiveMutex::ScopedLock lock( m_mutex );
        m_plugins.erase( std::remove( m_plugins.begin(), m_plugins.end(), plugin ), m_plugins.end() );
    }

    auto ApplicationManager::getStateContext() const -> SmartPtr<IStateContext>
    {
        return m_stateContext;
    }

    void ApplicationManager::setStateContext( SmartPtr<IStateContext> stateContext )
    {
        m_stateContext = stateContext;
    }

    auto ApplicationManager::getWindow() const -> SmartPtr<render::IWindow>
    {
        return m_window;
    }

    void ApplicationManager::setWindow( SmartPtr<render::IWindow> window )
    {
        m_window = window;
    }

    auto ApplicationManager::getSceneRenderWindow() const -> SmartPtr<ui::IUIWindow>
    {
        return m_sceneRenderWindow;
    }

    void ApplicationManager::setSceneRenderWindow( SmartPtr<ui::IUIWindow> sceneRenderWindow )
    {
        m_sceneRenderWindow = sceneRenderWindow;
    }

    auto ApplicationManager::getActorComponent( SmartPtr<scene::IActor> actor, u32 typeId ) const
        -> SmartPtr<scene::IComponent>
    {
        const auto &typeManager = TypeManager::instance();

        auto components = actor->getComponents();
        for( auto &component : components )
        {
            if( typeManager->isDerived( typeId, component->getTypeInfo() ) )
            {
                return component;
            }
        }

        auto children = actor->getChildren();
        for( auto child : children )
        {
            auto component = getActorComponent( child, typeId );
            if( component )
            {
                return component;
            }
        }

        return nullptr;
    }

    auto ApplicationManager::getComponentByType( u32 typeId ) const -> SmartPtr<scene::IComponent>
    {
        const auto &typeManager = TypeManager::instance();

        auto actors = getActors();
        for( auto &actor : actors )
        {
            auto result = getActorComponent( actor, typeId );
            if( result )
            {
                return result;
            }
        }

        return nullptr;
    }

    auto ApplicationManager::triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                           const Array<Parameter> &arguments,
                                           SmartPtr<ISharedObject> sender,
                                           SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        if( auto stateContext = getStateContext() )
        {
            return stateContext->triggerEvent( eventType, eventValue, arguments, sender, object, event );
        }

        return {};
    }

    auto ApplicationManager::getTypeManager() const -> RawPtr<TypeManager>
    {
        return m_typeManager.load();
    }

    void ApplicationManager::setTypeManager( RawPtr<TypeManager> typeManager )
    {
        m_typeManager = typeManager;
    }

    auto ApplicationManager::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        Array<SmartPtr<ISharedObject>> objects;
        objects.reserve( 32 );

        objects.emplace_back( getApplication() );
        objects.emplace_back( getTaskManager() );
        objects.emplace_back( getStateManager() );
        objects.emplace_back( getCommandManager() );
        objects.emplace_back( getDatabase() );
        objects.emplace_back( getFileSystem() );
        objects.emplace_back( getGraphicsSystem() );
        objects.emplace_back( getCameraManager() );
        return objects;
    }

    auto ApplicationManager::allocateMemory( size_t size ) -> void *
    {
        auto ptr = Memory::ScalableAlignedMalloc( size, FB_ALIGNMENT );
        if( !ptr )
        {
            FB_ASSERT( false );
            return nullptr;
        }

#if FB_ENABLE_MEMORY_TRACKER
        const char *file = __FILE__;
        int line = __LINE__;
        const char *func = __FUNCTION__;

        std::stringstream backtraceMsg;
        backtraceMsg << boost::stacktrace::stacktrace();
        auto msg = backtraceMsg.str();
        MemoryTracker::get()._recordAlloc( ptr, size, 0, file, line, msg.c_str() );
#endif

        return ptr;
    }

    void ApplicationManager::freeMemory( void *ptr )
    {
        Memory::ScalableAlignedFree( ptr );

#if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#endif
    }

    auto ApplicationManager::getFSMManagersPtr() const -> SharedPtr<Array<SmartPtr<IFSMManager>>>
    {
        return m_fsmManagers;
    }

    void ApplicationManager::setFSMManagersPtr( SharedPtr<Array<SmartPtr<IFSMManager>>> fsmManagers )
    {
        m_fsmManagers = fsmManagers;
    }

    void ApplicationManager::setInstance( SmartPtr<ApplicationManager> instance )
    {
        m_instance = instance;
        IApplicationManager::setInstance( instance );
    }

}  // namespace fb::core
