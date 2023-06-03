#include <FBCore/FBCorePCH.h>
#include <FBCore/System/ApplicationManager.h>
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
#include <FBCore/Interface/System/IThreadPool.h>
#include <FBCore/Interface/UI/IUIManager.h>
#include <FBCore/Interface/UI/IUIWindow.h>
#include <FBCore/Interface/Vehicle/IVehicleManager.h>
#include <algorithm>
#include <cstdio>

namespace fb
{
    namespace core
    {
        FB_CLASS_REGISTER_DERIVED( fb, ApplicationManager, SharedObject<IApplicationManager> );

        ApplicationManager::ApplicationManager() : SharedObject<IApplicationManager>()
        {
        }

        ApplicationManager::~ApplicationManager()
        {
            unload( nullptr );
        }

        void ApplicationManager::load( SmartPtr<ISharedObject> data )
        {
            try
            {
#if defined FB_PLATFORM_WIN32
                auto fileCachePath = String( ".FBCache/" );
                setCachePath( fileCachePath );

                auto settingsCachePath = String( ".FBSettingsCache/" );
                setSettingsCachePath( settingsCachePath );
#endif
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

                    auto &gc = GarbageCollector::instance();
                    gc.update();

                    if( m_selectionManager )
                    {
                        m_selectionManager->unload( nullptr );
                        m_selectionManager = nullptr;
                    }

                    if( m_sceneManager )
                    {
                        m_sceneManager->unload( nullptr );
                        m_sceneManager = nullptr;
                    }

                    gc.update();

                    if( m_database )
                    {
                        m_database->unload( nullptr );
                        m_database = nullptr;
                    }

                    if( m_proceduralManager )
                    {
                        m_proceduralManager->unload( nullptr );
                        m_proceduralManager = nullptr;
                    }

                    if( m_systemManager )
                    {
                        m_systemManager->unload( nullptr );
                        m_systemManager = nullptr;
                    }

                    if( m_commandManager )
                    {
                        m_commandManager->unload( nullptr );
                        m_commandManager = nullptr;
                    }

                    if( m_resourceDatabase )
                    {
                        m_resourceDatabase->unload( nullptr );
                        m_resourceDatabase = nullptr;
                    }

                    if( m_resourceManager )
                    {
                        m_resourceManager->unload( nullptr );
                        m_resourceManager = nullptr;
                    }

                    if( m_processManager )
                    {
                        m_processManager->unload( nullptr );
                        m_processManager = nullptr;
                    }

                    if( m_meshLoader )
                    {
                        m_meshLoader->unload( nullptr );
                        m_meshLoader = nullptr;
                    }

                    if( m_meshManager )
                    {
                        m_meshManager->unload( nullptr );
                        m_meshManager = nullptr;
                    }

                    if( m_physicsScene )
                    {
                        m_physicsScene->clear();
                        m_physicsScene->unload( nullptr );
                        m_physicsScene = nullptr;
                    }

                    if( m_controlsScene )
                    {
                        m_controlsScene->clear();
                        m_controlsScene->unload( nullptr );
                        m_controlsScene = nullptr;
                    }

                    if( m_softbodyScene )
                    {
                        m_controlsScene->clear();
                        m_softbodyScene->unload( nullptr );
                        m_softbodyScene = nullptr;
                    }

                    if( m_particleScene )
                    {
                        m_particleScene->clear();
                        m_particleScene->unload( nullptr );
                        m_particleScene = nullptr;
                    }

                    if( m_raycastScene )
                    {
                        m_raycastScene->clear();
                        m_raycastScene->unload( nullptr );
                        m_raycastScene = nullptr;
                    }

                    if( m_objectsScene )
                    {
                        m_objectsScene->clear();
                        m_objectsScene->unload( nullptr );
                        m_objectsScene = nullptr;
                    }

                    if( m_scene )
                    {
                        m_scene->unload( nullptr );
                        m_scene = nullptr;
                    }

                    gc.update();

                    if( m_ui )
                    {
                        m_ui->unload( nullptr );
                        m_ui = nullptr;
                    }

                    if( m_vehicleManager )
                    {
                        m_vehicleManager->unload( nullptr );
                        m_vehicleManager = nullptr;
                    }

                    if( m_physicsManager2 )
                    {
                        m_physicsManager2->unload( nullptr );
                        m_physicsManager2 = nullptr;
                    }

                    if( m_physicsScene )
                    {
                        m_physicsScene->unload( nullptr );
                        m_physicsScene = nullptr;
                    }

                    if( m_controlsScene )
                    {
                        m_controlsScene->unload( nullptr );
                        m_controlsScene = nullptr;
                    }

                    if( m_softbodyScene )
                    {
                        m_softbodyScene->unload( nullptr );
                        m_softbodyScene = nullptr;
                    }

                    if( m_particleScene )
                    {
                        m_particleScene->unload( nullptr );
                        m_particleScene = nullptr;
                    }

                    if( m_raycastScene )
                    {
                        m_raycastScene->unload( nullptr );
                        m_raycastScene = nullptr;
                    }

                    if( m_objectsScene )
                    {
                        m_objectsScene->unload( nullptr );
                        m_objectsScene = nullptr;
                    }

                    if( m_physicsManager3 )
                    {
                        m_physicsManager3->unload( nullptr );
                        m_physicsManager3 = nullptr;
                    }

                    gc.update();

                    if( m_fileSystem )
                    {
                        m_fileSystem->unload( nullptr );
                        m_fileSystem = nullptr;
                    }

                    if( m_graphicsSystem )
                    {
                        m_graphicsSystem->unload( nullptr );
                        m_graphicsSystem = nullptr;
                    }

                    gc.update();

                    if( m_taskManager )
                    {
                        m_taskManager->unload( nullptr );
                        m_taskManager = nullptr;
                    }

                    auto threadPool = getThreadPool();
                    if( threadPool )
                    {
                        threadPool->unload( nullptr );
                        threadPool = nullptr;
                        m_threadPool = nullptr;
                    }

                    auto logManager = getLogManager();
                    if( logManager )
                    {
                        logManager->unload( nullptr );
                        setLogManager( nullptr );
                    }

                    auto timer = getTimer();
                    if( timer )
                    {
                        timer->unload( nullptr );
                        setTimer( nullptr );
                    }

                    if( m_stateManager )
                    {
                        m_stateManager->unload( nullptr );
                        m_stateManager = nullptr;
                    }

                    auto factoryManager = getFactoryManager();
                    if( factoryManager )
                    {
                        factoryManager->unload( nullptr );
                        setFactoryManager( nullptr );
                    }

                    gc.update();

                    setGarbageCollected( true );
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
        }

        SmartPtr<Properties> ApplicationManager::getPlayerSettings() const
        {
            return nullptr;
        }

        void ApplicationManager::setPlayerSettings( SmartPtr<Properties> properties )
        {
        }

        SmartPtr<IFactoryManager> ApplicationManager::getFactoryManager() const
        {
            return m_factoryManager;
        }

        void ApplicationManager::setLogManager( SmartPtr<ILogManager> logManager )
        {
            m_logManager = logManager;
        }

        void ApplicationManager::setFactoryManager( SmartPtr<IFactoryManager> factoryManager )
        {
            m_factoryManager = factoryManager;
        }

        SmartPtr<IProcessManager> ApplicationManager::getProcessManager() const
        {
            return m_processManager;
        }

        void ApplicationManager::setProcessManager( SmartPtr<IProcessManager> processManager )
        {
            m_processManager = processManager;
        }

        SmartPtr<IApplicationClient> ApplicationManager::getApplication() const
        {
            return m_application;
        }

        void ApplicationManager::setApplication( SmartPtr<IApplicationClient> val )
        {
            m_application = val;
        }

        SmartPtr<IFileSystem> ApplicationManager::getFileSystem() const
        {
            return m_fileSystem;
        }

        void ApplicationManager::setFileSystem( SmartPtr<IFileSystem> fileSystem )
        {
            m_fileSystem = fileSystem;
        }

        SmartPtr<ITimer> ApplicationManager::getTimer() const
        {
            return m_timer;
        }

        void ApplicationManager::setTimer( SmartPtr<ITimer> timer )
        {
            m_timer = timer;
        }

        SmartPtr<IFSMManager> ApplicationManager::getFsmManager() const
        {
            return m_fsmManager;
        }

        void ApplicationManager::setFsmManager( SmartPtr<IFSMManager> fsmManager )
        {
            m_fsmManager = fsmManager;
        }

        SmartPtr<procedural::IProceduralEngine> ApplicationManager::getProceduralEngine() const
        {
            return m_proceduralEngine;
        }

        void ApplicationManager::setProceduralEngine( SmartPtr<procedural::IProceduralEngine> val )
        {
            m_proceduralEngine = val;
        }

        SmartPtr<procedural::IProceduralManager> ApplicationManager::getProceduralManager() const
        {
            return m_proceduralManager;
        }

        void ApplicationManager::setProceduralManager(
            SmartPtr<procedural::IProceduralManager> processManager )
        {
            m_proceduralManager = processManager;
        }

        void ApplicationManager::setPhysicsManager2D(
            SmartPtr<physics::IPhysicsManager2D> physicsManager )
        {
            m_physicsManager2 = physicsManager;
        }

        SmartPtr<physics::IPhysicsManager2D> ApplicationManager::getPhysicsManager2D() const
        {
            return m_physicsManager2;
        }

        void ApplicationManager::setPhysicsManager( SmartPtr<physics::IPhysicsManager> physicsManager )
        {
            m_physicsManager3 = physicsManager;
        }

        SmartPtr<physics::IPhysicsManager> ApplicationManager::getPhysicsManager() const
        {
            return m_physicsManager3;
        }

        bool ApplicationManager::isEditor() const
        {
            return m_isEditor;
        }

        void ApplicationManager::setEditor( bool editor )
        {
            m_isEditor = editor;
        }

        bool ApplicationManager::isEditorCamera() const
        {
            return m_isEditorCamera;
        }

        void ApplicationManager::setEditorCamera( bool editorCamera )
        {
            m_isEditorCamera = editorCamera;
        }

        bool ApplicationManager::isPlaying() const
        {
            return m_isPlaying;
        }

        void ApplicationManager::setPlaying( bool playing )
        {
            m_isPlaying = playing;
        }

        bool ApplicationManager::isPaused() const
        {
            return m_isPaused;
        }

        void ApplicationManager::setPaused( bool paused )
        {
            m_isPaused = paused;
        }

        bool ApplicationManager::isRunning() const
        {
            return m_isRunning;
        }

        void ApplicationManager::setRunning( bool running )
        {
            m_isRunning = running;
        }

        bool ApplicationManager::getQuit() const
        {
            return m_quit;
        }

        void ApplicationManager::setQuit( bool quit )
        {
            m_quit = quit;
        }

        bool ApplicationManager::hasTasks() const
        {
            if( m_taskManager )
            {
                return m_taskManager->getNumTasks() > 0;
            }

            return false;
        }

        SmartPtr<IFSM> ApplicationManager::getFSM() const
        {
            return m_fsm;
        }

        void ApplicationManager::setFSM( SmartPtr<IFSM> fsm )
        {
            m_fsm = fsm;
        }

        SmartPtr<Properties> ApplicationManager::getEditorSettings() const
        {
            return nullptr;
        }

        SmartPtr<render::IGraphicsSystem> ApplicationManager::getGraphicsSystem() const
        {
            return m_graphicsSystem;
        }

        void ApplicationManager::setGraphicsSystem( SmartPtr<render::IGraphicsSystem> graphicsSystem )
        {
            m_graphicsSystem = graphicsSystem;
        }

        SmartPtr<ITaskManager> ApplicationManager::getTaskManager() const
        {
            return m_taskManager;
        }

        void ApplicationManager::setTaskManager( SmartPtr<ITaskManager> val )
        {
            m_taskManager = val;
        }

        bool ApplicationManager::isPauseMenuActive() const
        {
            return m_isPauseMenuActive;
        }

        void ApplicationManager::setPauseMenuActive( bool pauseMenuActive )
        {
            m_isPauseMenuActive = pauseMenuActive;
        }

        SmartPtr<IProfiler> ApplicationManager::getProfiler() const
        {
            return m_profiler;
        }

        void ApplicationManager::setProfiler( SmartPtr<IProfiler> profiler )
        {
            m_profiler = profiler;
        }

        SmartPtr<ISystemManager> ApplicationManager::getSystemSettings() const
        {
            return m_systemManager;
        }

        void ApplicationManager::setSystemSettings( SmartPtr<ISystemManager> val )
        {
            m_systemManager = val;
        }

        SmartPtr<IJobQueue> ApplicationManager::getJobQueue() const
        {
            return m_jobQueue;
        }

        void ApplicationManager::setJobQueue( SmartPtr<IJobQueue> val )
        {
            m_jobQueue = val;
        }

        SmartPtr<render::IParticleManager> ApplicationManager::getParticleManager() const
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

        SmartPtr<IScriptManager> ApplicationManager::getScriptManager() const
        {
            return m_scriptManager;
        }

        void ApplicationManager::setInputDeviceManager( SmartPtr<IInputDeviceManager> inputManager )
        {
            m_inputManager = inputManager;
        }

        SmartPtr<IInputDeviceManager> ApplicationManager::getInputDeviceManager() const
        {
            return m_inputManager;
        }

        void ApplicationManager::setThreadPool( SmartPtr<IThreadPool> threadPool )
        {
            m_threadPool = threadPool;
        }

        SmartPtr<IThreadPool> ApplicationManager::getThreadPool() const
        {
            return m_threadPool;
        }

        void ApplicationManager::setConsole( SmartPtr<IConsole> console )
        {
            m_console = console;
        }

        SmartPtr<IConsole> ApplicationManager::getConsole() const
        {
            return m_console;
        }

        SmartPtr<scene::ICameraManager> ApplicationManager::getCameraManager() const
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

        SmartPtr<IStateManager> ApplicationManager::getStateManager() const
        {
            return m_stateManager;
        }

        void ApplicationManager::setCommandManager( SmartPtr<ICommandManager> commandManager )
        {
            m_commandManager = commandManager;
        }

        SmartPtr<ICommandManager> ApplicationManager::getCommandManager() const
        {
            return m_commandManager;
        }

        void ApplicationManager::setPrefabManager( SmartPtr<scene::IPrefabManager> prefabManager )
        {
            m_prefabManager = prefabManager;
        }

        SmartPtr<scene::IPrefabManager> ApplicationManager::getPrefabManager() const
        {
            return m_prefabManager;
        }

        SmartPtr<IMeshLoader> ApplicationManager::getMeshLoader() const
        {
            return m_meshLoader;
        }

        void ApplicationManager::setMeshLoader( SmartPtr<IMeshLoader> meshLoader )
        {
            m_meshLoader = meshLoader;
        }

        SmartPtr<IResourceDatabase> ApplicationManager::getResourceDatabase() const
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

        SmartPtr<scene::ISceneManager> ApplicationManager::getSceneManager() const
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

        SmartPtr<IResourceManager> ApplicationManager::getResourceManager() const
        {
            return m_resourceManager;
        }

        SmartPtr<ISelectionManager> ApplicationManager::getSelectionManager() const
        {
            return m_selectionManager;
        }

        SmartPtr<ISoundManager> ApplicationManager::getSoundManager() const
        {
            return m_soundManager;
        }

        void ApplicationManager::setSoundManager( SmartPtr<ISoundManager> soundManager )
        {
            m_soundManager = soundManager;
        }

        SmartPtr<physics::IPhysicsScene3> ApplicationManager::getPhysicsScene() const
        {
            return m_physicsScene;
        }

        void ApplicationManager::setPhysicsScene( SmartPtr<physics::IPhysicsScene3> val )
        {
            m_physicsScene = val;
        }

        SmartPtr<physics::IPhysicsScene3> ApplicationManager::getObjectsScene() const
        {
            return m_objectsScene;
        }

        void ApplicationManager::setObjectsScene( SmartPtr<physics::IPhysicsScene3> val )
        {
            m_objectsScene = val;
        }

        SmartPtr<physics::IPhysicsScene3> ApplicationManager::getRaycastScene() const
        {
            return m_raycastScene;
        }

        void ApplicationManager::setRaycastScene( SmartPtr<physics::IPhysicsScene3> val )
        {
            m_raycastScene = val;
        }

        SmartPtr<physics::IPhysicsScene3> ApplicationManager::getControlsScene() const
        {
            return m_controlsScene;
        }

        void ApplicationManager::setControlsScene( SmartPtr<physics::IPhysicsScene3> val )
        {
            m_controlsScene = val;
        }

        SmartPtr<IPluginInterface> ApplicationManager::getPluginInterface() const
        {
            return m_pluginInterface;
        }

        void ApplicationManager::setPluginInterface( SmartPtr<IPluginInterface> val )
        {
            m_pluginInterface = val;
        }

        void ApplicationManager::setScene( SmartPtr<scene::IScene> scene )
        {
            m_scene = scene;
        }

        SmartPtr<scene::IScene> ApplicationManager::getScene() const
        {
            return m_scene;
        }

        bool ApplicationManager::getEnableRenderer() const
        {
            return m_enableRenderer;
        }

        void ApplicationManager::setEnableRenderer( bool enable )
        {
            m_enableRenderer = enable;
        }

        SmartPtr<ui::IUIManager> ApplicationManager::getUI() const
        {
            return m_ui;
        }

        void ApplicationManager::setUI( SmartPtr<ui::IUIManager> ui )
        {
            m_ui = ui;
        }

        SmartPtr<ui::IUIManager> ApplicationManager::getRenderUI() const
        {
            return m_renderUiManager;
        }

        void ApplicationManager::setRenderUI( SmartPtr<ui::IUIManager> renderUI )
        {
            m_renderUiManager = renderUI;
        }

        SmartPtr<IDatabaseManager> ApplicationManager::getDatabase() const
        {
            return m_database;
        }

        void ApplicationManager::setDatabase( SmartPtr<IDatabaseManager> val )
        {
            m_database = val;
        }

        SmartPtr<Properties> &ApplicationManager::getProperties()
        {
            return m_properties;
        }

        const SmartPtr<Properties> &ApplicationManager::getProperties() const
        {
            return m_properties;
        }

        void ApplicationManager::setProperties( SmartPtr<Properties> val )
        {
            m_properties = val;
        }

        SmartPtr<IVehicleManager> ApplicationManager::getVehicleManager() const
        {
            return m_vehicleManager;
        }

        void ApplicationManager::setVehicleManager( SmartPtr<IVehicleManager> val )
        {
            m_vehicleManager = val;
        }

        String ApplicationManager::getCachePath() const
        {
            return m_cachePath;
        }

        void ApplicationManager::setCachePath( const String &cachePath )
        {
            m_cachePath = cachePath;
        }

        String ApplicationManager::getProjectPath() const
        {
            return m_projectPath;
        }

        void ApplicationManager::setProjectPath( const String &projectPath )
        {
            m_projectPath = projectPath;
        }

        String ApplicationManager::getProjectLibraryName() const
        {
            return m_projectLibraryName;
        }

        void ApplicationManager::setProjectLibraryName( const String &projectLibraryName )
        {
            m_projectLibraryName = projectLibraryName;
        }

        void ApplicationManager::setSettingsCachePath( const String &val )
        {
            m_settingsCachePath = val;
        }

        String ApplicationManager::getBuildConfig() const
        {
#if _DEBUG
            return String( "Debug" );
#else
            return String( "Release" );
#endif
        }

        String ApplicationManager::getProjectLibraryExtension() const
        {
            return String( ".dll" );
            // return String(".lib");
        }

        String ApplicationManager::getProjectLibraryPath() const
        {
            auto cachePath = getCachePath();
            auto libraryName = getProjectLibraryName();
            auto libraryExt = getProjectLibraryExtension();
            auto config = getBuildConfig();

            return cachePath + "/project/" + config + "/" + libraryName + libraryExt;
        }

        String ApplicationManager::getMediaPath() const
        {
            return m_mediaPath;
        }

        void ApplicationManager::setMediaPath( const String &mediaPath )
        {
            m_mediaPath = mediaPath + "/";
            m_mediaPath = StringUtil::cleanupPath( m_mediaPath );
        }

        String ApplicationManager::getSettingsCachePath() const
        {
            return m_settingsCachePath;
        }

        Thread::Task ApplicationManager::getStateTask() const
        {
            auto applicationManager = instance();
            return applicationManager->hasTasks() ? Thread::Task::Application : Thread::Task::Primary;
        }

        Thread::Task ApplicationManager::getApplicationTask() const
        {
            auto applicationManager = instance();
            // auto threadPool = applicationManager->getThreadPool();

            // if (isUpdating())
            //{
            //	return Thread::getCurrentTask();
            // }

            auto taskManager = getTaskManager();
            if( taskManager )
            {
                auto task = taskManager->getTask( Thread::Task::Application );
                if( task )
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

            return applicationManager->hasTasks() ? Thread::Task::Application : Thread::Task::Primary;
        }

        SmartPtr<IMeshManager> ApplicationManager::getMeshManager() const
        {
            return m_meshManager;
        }

        void ApplicationManager::setMeshManager( SmartPtr<IMeshManager> val )
        {
            m_meshManager = val;
        }

        s32 ApplicationManager::getLoadProgress() const
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

        void ApplicationManager::addPlugin( SmartPtr<ISharedObject> plugin )
        {
        }

        void ApplicationManager::setSceneRenderWindow( SmartPtr<ui::IUIWindow> sceneRenderWindow )
        {
            m_sceneRenderWindow = sceneRenderWindow;
        }

        bool ApplicationManager::isValid() const
        {
            auto taskManager = getTaskManager();
            if( taskManager )
            {
                if( !taskManager->isValid() )
                {
                    return false;
                }
            }

            auto threadPool = getThreadPool();
            if( threadPool )
            {
                if( !threadPool->isValid() )
                {
                    return false;
                }
            }

            return true;
        }

        void ApplicationManager::removePlugin( SmartPtr<ISharedObject> plugin )
        {
        }

        SmartPtr<IStateContext> ApplicationManager::getStateObject() const
        {
            return m_stateObject;
        }

        void ApplicationManager::setStateObject( SmartPtr<IStateContext> stateObject )
        {
            m_stateObject = stateObject;
        }

        SmartPtr<render::IWindow> ApplicationManager::getWindow() const
        {
            return nullptr;
        }

        void ApplicationManager::setWindow( SmartPtr<render::IWindow> window )
        {
        }

        SmartPtr<ui::IUIWindow> ApplicationManager::getSceneRenderWindow() const
        {
            return m_sceneRenderWindow;
        }

        Parameter ApplicationManager::triggerEvent( IEvent::Type eventType, hash_type eventValue,
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

        SmartPtr<ILogManager> ApplicationManager::getLogManager() const
        {
            return m_logManager;
        }
    }  // namespace core
}  // namespace fb
