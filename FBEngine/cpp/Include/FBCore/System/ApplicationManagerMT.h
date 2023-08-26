#ifndef __ApplicationManagerMT_h__
#define __ApplicationManagerMT_h__

#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/IApplicationClient.h>
#include <FBCore/Core/ConcurrentQueue.h>

namespace fb
{
    namespace core
    {

        /** Implementation of the IApplicationManager interface. */
        class ApplicationManagerMT : public IApplicationManager
        {
        public:
            /** Constructor. */
            ApplicationManagerMT();

            /** Destructor. */
            ~ApplicationManagerMT() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IApplicationManager::isEditor */
            bool isEditor() const override;

            /** @copydoc IApplicationManager::setEditor */
            void setEditor( bool editor ) override;

            /** @copydoc IApplicationManager::isEditorCamera */
            bool isEditorCamera() const;

            /** @copydoc IApplicationManager::setEditorCamera */
            void setEditorCamera( bool editor );

            bool isPlaying() const override;
            void setPlaying( bool playing ) override;

            bool isPaused() const override;
            void setPaused( bool paused ) override;

            bool isRunning() const override;
            void setRunning( bool running ) override;

            bool getQuit() const override;
            void setQuit( bool quit ) override;

            bool hasTasks() const override;

            bool isPauseMenuActive() const override;
            void setPauseMenuActive( bool pauseMenuActive ) override;

            String getCachePath() const override;
            void setCachePath( const String &cachePath ) override;

            String getProjectPath() const override;
            void setProjectPath( const String &projectPath ) override;

            String getProjectLibraryName() const override;
            void setProjectLibraryName( const String &projectLibraryName ) override;

            String getMediaPath() const override;
            void setMediaPath( const String &mediaPath ) override;

            String getSettingsCachePath() const override;
            void setSettingsCachePath( const String &cachePath ) override;

            String getBuildConfig() const override;

            String getProjectLibraryExtension() const override;

            String getProjectLibraryPath() const override;

            /** @copydoc IApplicationManager::getStateTask */
            Thread::Task getStateTask() const override;

            /** @copydoc IApplicationManager::getApplicationTask */
            Thread::Task getApplicationTask() const override;

            bool getEnableRenderer() const override;
            void setEnableRenderer( bool enable ) override;

            SmartPtr<IFSM> getFSM() const override;
            void setFSM( SmartPtr<IFSM> fsm ) override;

            SmartPtr<Properties> getEditorSettings() const override;

            void setEditorSettings( SmartPtr<Properties> properties ) override;

            SmartPtr<Properties> getPlayerSettings() const override;

            void setPlayerSettings( SmartPtr<Properties> properties ) override;

            SmartPtr<ILogManager> getLogManager() const override;
            void setLogManager( SmartPtr<ILogManager> logManager ) override;

            SmartPtr<IFactoryManager> getFactoryManager() const override;
            void setFactoryManager( SmartPtr<IFactoryManager> factoryManager ) override;

            SmartPtr<IProcessManager> getProcessManager() const override;
            void setProcessManager( SmartPtr<IProcessManager> processManager ) override;

            SmartPtr<IApplicationClient> getApplication() const override;
            void setApplication( SmartPtr<IApplicationClient> val ) override;

            SmartPtr<IFileSystem> getFileSystem() const override;
            void setFileSystem( SmartPtr<IFileSystem> fileSystem ) override;

            SmartPtr<ITimer> getTimer() const override;
            void setTimer( SmartPtr<ITimer> timer ) override;

            SmartPtr<IFSMManager> getFsmManager() const override;
            void setFsmManager( SmartPtr<IFSMManager> fsmManager ) override;

            SmartPtr<IFSMManager> getFsmManagerByTask( Thread::Task task ) const;
            void setFsmManagerByTask( Thread::Task task, SmartPtr<IFSMManager> fsmManager );

            SmartPtr<procedural::IProceduralEngine> getProceduralEngine() const override;
            void setProceduralEngine( SmartPtr<procedural::IProceduralEngine> val ) override;

            SmartPtr<procedural::IProceduralManager> getProceduralManager() const override;
            void setProceduralManager( SmartPtr<procedural::IProceduralManager> val ) override;

            SmartPtr<physics::IPhysicsManager2D> getPhysicsManager2D() const override;
            void setPhysicsManager2D( SmartPtr<physics::IPhysicsManager2D> physicsManager ) override;

            SmartPtr<physics::IPhysicsManager> getPhysicsManager() const override;
            void setPhysicsManager( SmartPtr<physics::IPhysicsManager> physicsManager ) override;

            SmartPtr<render::IGraphicsSystem> getGraphicsSystem() const override;
            void setGraphicsSystem( SmartPtr<render::IGraphicsSystem> graphicsSystem ) override;

            SmartPtr<IVideoManager> getVideoManager() const;

            void setVideoManager( SmartPtr<IVideoManager> videoManager );

            SmartPtr<ITaskManager> getTaskManager() const override;
            void setTaskManager( SmartPtr<ITaskManager> taskManager ) override;

            SmartPtr<IProfiler> getProfiler() const override;
            void setProfiler( SmartPtr<IProfiler> profiler ) override;

            SmartPtr<ISystemManager> getSystemSettings() const override;
            void setSystemSettings( SmartPtr<ISystemManager> val ) override;

            SmartPtr<IJobQueue> getJobQueue() const override;
            void setJobQueue( SmartPtr<IJobQueue> val ) override;

            SmartPtr<render::IParticleManager> getParticleManager() const override;
            void setParticleManager( SmartPtr<render::IParticleManager> particleManager ) override;

            SmartPtr<IScriptManager> getScriptManager() const override;
            void setScriptManager( SmartPtr<IScriptManager> physicsManager ) override;

            SmartPtr<IInput> getInput() const override;
            void setInput( SmartPtr<IInput> input ) override;

            SmartPtr<IInputDeviceManager> getInputDeviceManager() const override;
            void setInputDeviceManager( SmartPtr<IInputDeviceManager> inputManager ) override;

            SmartPtr<IThreadPool> getThreadPool() const override;
            void setThreadPool( SmartPtr<IThreadPool> threadPool ) override;

            SmartPtr<IConsole> getConsole() const override;
            void setConsole( SmartPtr<IConsole> console ) override;

            SmartPtr<scene::ICameraManager> getCameraManager() const override;
            void setCameraManager( SmartPtr<scene::ICameraManager> cameraManager ) override;

            SmartPtr<IStateManager> getStateManager() const override;
            void setStateManager( SmartPtr<IStateManager> stateManager ) override;

            SmartPtr<ICommandManager> getCommandManager() const override;
            void setCommandManager( SmartPtr<ICommandManager> commandManager ) override;

            SmartPtr<IResourceManager> getResourceManager() const override;
            void setResourceManager( SmartPtr<IResourceManager> resourceManager ) override;

            SmartPtr<scene::IPrefabManager> getPrefabManager() const override;
            void setPrefabManager( SmartPtr<scene::IPrefabManager> prefabManager ) override;

            SmartPtr<IMeshLoader> getMeshLoader() const override;
            void setMeshLoader( SmartPtr<IMeshLoader> meshLoader ) override;

            SmartPtr<IResourceDatabase> getResourceDatabase() const override;
            void setResourceDatabase( SmartPtr<IResourceDatabase> val ) override;

            SmartPtr<scene::ISceneManager> getSceneManager() const override;
            void setSceneManager( SmartPtr<scene::ISceneManager> sceneManager ) override;

            SmartPtr<ISelectionManager> getSelectionManager() const override;
            void setSelectionManager( SmartPtr<ISelectionManager> selectionManager ) override;

            SmartPtr<ISoundManager> getSoundManager() const override;
            void setSoundManager( SmartPtr<ISoundManager> soundManager ) override;

            SmartPtr<physics::IPhysicsScene3> getPhysicsScene() const override;
            void setPhysicsScene( SmartPtr<physics::IPhysicsScene3> physicsScene ) override;

            SmartPtr<physics::IPhysicsScene3> getObjectsScene() const override;
            void setObjectsScene( SmartPtr<physics::IPhysicsScene3> objectsScene ) override;

            SmartPtr<physics::IPhysicsScene3> getRaycastScene() const override;
            void setRaycastScene( SmartPtr<physics::IPhysicsScene3> raycastScene ) override;

            SmartPtr<physics::IPhysicsScene3> getControlsScene() const override;
            void setControlsScene( SmartPtr<physics::IPhysicsScene3> controlsScene ) override;

            SmartPtr<IPluginInterface> getPluginInterface() const override;
            void setPluginInterface( SmartPtr<IPluginInterface> pluginInterface ) override;

            SmartPtr<scene::IScene> getScene() const override;

            SmartPtr<ui::IUIManager> getUI() const override;
            void setUI( SmartPtr<ui::IUIManager> val ) override;

            SmartPtr<ui::IUIManager> getRenderUI() const override;
            void setRenderUI( SmartPtr<ui::IUIManager> val ) override;

            SmartPtr<IDatabaseManager> getDatabase() const override;
            void setDatabase( SmartPtr<IDatabaseManager> val ) override;

            SmartPtr<Properties> getProperties() const;
            void setProperties( SmartPtr<Properties> val );

            SmartPtr<IVehicleManager> getVehicleManager() const override;
            void setVehicleManager( SmartPtr<IVehicleManager> vehicleManager ) override;

            SmartPtr<IMeshManager> getMeshManager() const override;
            void setMeshManager( SmartPtr<IMeshManager> val ) override;

            /** @copydoc IApplicationManager::getLoadProgress */
            s32 getLoadProgress() const override;

            /** @copydoc IApplicationManager::setLoadProgress */
            void setLoadProgress( s32 loadProgress ) override;

            /** @copydoc IApplicationManager::addLoadProgress */
            void addLoadProgress( s32 loadProgress ) override;

            /** @copydoc IApplicationManager::getActors */
            Array<SmartPtr<scene::IActor>> getActors() const override;

            /** @copydoc IApplicationManager::addPlugin */
            void addPlugin( SmartPtr<ISharedObject> plugin ) override;

            /** @copydoc IApplicationManager::removePlugin */
            void removePlugin( SmartPtr<ISharedObject> plugin ) override;

            /** @copydoc IApplicationManager::getStateObject */
            SmartPtr<IStateContext> getStateObject() const override;

            /** @copydoc IApplicationManager::setStateObject */
            void setStateObject( SmartPtr<IStateContext> stateObject ) override;

            /** @copydoc IApplicationManager::getWindow */
            SmartPtr<render::IWindow> getWindow() const override;

            /** @copydoc IApplicationManager::setWindow */
            void setWindow( SmartPtr<render::IWindow> window ) override;

            /** @copydoc IApplicationManager::getSceneRenderWindow */
            SmartPtr<ui::IUIWindow> getSceneRenderWindow() const;

            /** @copydoc IApplicationManager::setSceneRenderWindow */
            void setSceneRenderWindow( SmartPtr<ui::IUIWindow> sceneRenderWindow );

            /** @copydoc IApplicationManager::getComponentByType */
            SmartPtr<scene::IComponent> getComponentByType( u32 typeId ) const;

            /** @copydoc IApplicationManager::triggerEvent */
            Parameter triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                    const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                    SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

            FB_CLASS_REGISTER_DECL;



        protected:
            SharedPtr<Array<SmartPtr<IFSMManager>>> getFSMManagersPtr() const;
            void setFSMManagersPtr( SharedPtr<Array<SmartPtr<IFSMManager>>> fsmManagers );

            String m_settingsCachePath;
            String m_cachePath;
            String m_projectPath;
            String m_projectLibraryName;
            String m_mediaPath;

            atomic_s32 m_loadProgress;
            atomic_bool m_enableRenderer = false;

            AtomicSmartPtr<IStateContext> m_stateObject;

            AtomicSmartPtr<Properties> m_editorSettings;
            AtomicSmartPtr<Properties> m_playerSettings;

            AtomicSmartPtr<IFSMManager> m_fsmManager;
            AtomicSharedPtr<Array<SmartPtr<IFSMManager>>> m_fsmManagers;

            AtomicSmartPtr<IFactoryManager> m_factoryManager;

            AtomicSmartPtr<ILogManager> m_logManager;

            AtomicSmartPtr<IProcessManager> m_processManager;

            AtomicSmartPtr<IMeshManager> m_meshManager;

            AtomicSmartPtr<Properties> m_properties;
            AtomicSmartPtr<ui::IUIManager> m_ui;

            AtomicSmartPtr<IVehicleManager> m_vehicleManager;

            AtomicSmartPtr<IResourceDatabase> m_resourceDatabase;

            AtomicSmartPtr<IPluginInterface> m_pluginInterface;
            AtomicSmartPtr<ISoundManager> m_soundManager;
            AtomicSmartPtr<ISelectionManager> m_selectionManager;
            AtomicSmartPtr<scene::ISceneManager> m_sceneManager;
            AtomicSmartPtr<scene::IPrefabManager> m_prefabManager;

            AtomicSmartPtr<IMeshLoader> m_meshLoader;

            AtomicSmartPtr<IResourceManager> m_resourceManager;
            AtomicSmartPtr<ICommandManager> m_commandManager;
            AtomicSmartPtr<IProfiler> m_profiler;
            AtomicSmartPtr<ISystemManager> m_systemManager;
            AtomicSmartPtr<IScriptManager> m_scriptManager;

            AtomicSmartPtr<IInput> m_input;
            AtomicSmartPtr<IInputDeviceManager> m_inputManager;
            AtomicSmartPtr<IConsole> m_console;

            AtomicSmartPtr<ui::IUIManager> m_uiManager;
            AtomicSmartPtr<ui::IUIManager> m_renderUiManager;

            AtomicSmartPtr<IThreadPool> m_threadPool;
            AtomicSmartPtr<render::IParticleManager> m_particleManager;
            AtomicSmartPtr<scene::ICameraManager> m_cameraManager;
            AtomicSmartPtr<IStateManager> m_stateManager;
            AtomicSmartPtr<IJobQueue> m_jobQueue;
            AtomicSmartPtr<ITaskManager> m_taskManager;
            AtomicSmartPtr<render::IGraphicsSystem> m_graphicsSystem;
            AtomicSmartPtr<IVideoManager> m_videoManager;
            AtomicSmartPtr<IFileSystem> m_fileSystem;
            AtomicSmartPtr<IApplicationClient> m_application;

            AtomicSmartPtr<ITimer> m_timer;

            AtomicSmartPtr<procedural::IProceduralEngine> m_proceduralEngine;
            AtomicSmartPtr<procedural::IProceduralManager> m_proceduralManager;

            AtomicSmartPtr<physics::IPhysicsManager2D> m_physicsManager2;
            AtomicSmartPtr<physics::IPhysicsManager> m_physicsManager3;
            AtomicSmartPtr<IFSM> m_fsm;

            AtomicSmartPtr<physics::IPhysicsScene3> m_physicsScene;
            AtomicSmartPtr<physics::IPhysicsScene3> m_controlsScene;
            AtomicSmartPtr<physics::IPhysicsScene3> m_softbodyScene;
            AtomicSmartPtr<physics::IPhysicsScene3> m_particleScene;
            AtomicSmartPtr<physics::IPhysicsScene3> m_raycastScene;
            AtomicSmartPtr<physics::IPhysicsScene3> m_objectsScene;

            mutable AtomicSmartPtr<IDatabaseManager> m_database;

            AtomicSmartPtr<render::IWindow> m_window;
            AtomicSmartPtr<ui::IUIWindow> m_sceneRenderWindow;

            Array<SmartPtr<ISharedObject>> m_plugins;

            atomic_bool m_isPauseMenuActive = false;
            atomic_bool m_isRunning = true;
            atomic_bool m_isPlaying = false;
            atomic_bool m_isPaused = false;
            atomic_bool m_isEditor = false;
            atomic_bool m_isEditorCamera = false;

            /** To know if the application is in the process of shutting down. */
            atomic_bool m_quit = false;

            mutable RecursiveMutex m_mutex;
        };

    }  // namespace core
}  // namespace fb

#endif  // CApplicationManager_h__
