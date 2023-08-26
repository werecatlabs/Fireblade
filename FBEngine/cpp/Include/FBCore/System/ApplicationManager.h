#ifndef __ApplicationManager_h__
#define __ApplicationManager_h__

#include <FBCore/Interface/IApplicationManager.h>

namespace fb
{
    namespace core
    {

        class ApplicationManager : public IApplicationManager
        {
        public:
            ApplicationManager();
            ~ApplicationManager() override;

            /** @copydoc IObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            bool isEditor() const override;
            void setEditor( bool editor ) override;

            /** @copydoc IApplicationManager::isEditorCamera */
            bool isEditorCamera() const;

            /** @copydoc IApplicationManager::setEditorCamera */
            void setEditorCamera( bool editorCamera );

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

            SmartPtr<IFSMManager> getFsmManagerByTask( Thread::Task task ) const;
            void setFsmManagerByTask( Thread::Task task, SmartPtr<IFSMManager> fsmManager );

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
            void setTaskManager( SmartPtr<ITaskManager> val ) override;

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
            void setResourceDatabase( SmartPtr<IResourceDatabase> resourceDatabase ) override;

            SmartPtr<scene::ISceneManager> getSceneManager() const override;
            void setSceneManager( SmartPtr<scene::ISceneManager> sceneManager ) override;

            SmartPtr<ISelectionManager> getSelectionManager() const override;
            void setSelectionManager( SmartPtr<ISelectionManager> selectionManager ) override;

            SmartPtr<ISoundManager> getSoundManager() const override;
            void setSoundManager( SmartPtr<ISoundManager> soundManager ) override;

            SmartPtr<physics::IPhysicsScene3> getPhysicsScene() const override;
            void setPhysicsScene( SmartPtr<physics::IPhysicsScene3> val ) override;

            SmartPtr<physics::IPhysicsScene3> getObjectsScene() const override;
            void setObjectsScene( SmartPtr<physics::IPhysicsScene3> val ) override;

            SmartPtr<physics::IPhysicsScene3> getRaycastScene() const override;
            void setRaycastScene( SmartPtr<physics::IPhysicsScene3> val ) override;

            SmartPtr<physics::IPhysicsScene3> getControlsScene() const override;
            void setControlsScene( SmartPtr<physics::IPhysicsScene3> val ) override;

            SmartPtr<IPluginInterface> getPluginInterface() const override;
            void setPluginInterface( SmartPtr<IPluginInterface> val ) override;

            SmartPtr<scene::IScene> getScene() const override;
            void setScene( SmartPtr<scene::IScene> scene );

            SmartPtr<ui::IUIManager> getUI() const override;
            void setUI( SmartPtr<ui::IUIManager> val ) override;

            SmartPtr<ui::IUIManager> getRenderUI() const override;
            void setRenderUI( SmartPtr<ui::IUIManager> val ) override;

            SmartPtr<IDatabaseManager> getDatabase() const override;
            void setDatabase( SmartPtr<IDatabaseManager> val ) override;

            SmartPtr<Properties> &getProperties();
            const SmartPtr<Properties> &getProperties() const;
            void setProperties( SmartPtr<Properties> val );

            SmartPtr<IVehicleManager> getVehicleManager() const override;
            void setVehicleManager( SmartPtr<IVehicleManager> val ) override;

            SmartPtr<IMeshManager> getMeshManager() const override;
            void setMeshManager( SmartPtr<IMeshManager> val ) override;

            /** @copydoc IApplicationManager::getLoadProgress */
            s32 getLoadProgress() const override;

            /** @copydoc IApplicationManager::setLoadProgress */
            void setLoadProgress( s32 loadProgress ) override;

            /** @copydoc IApplicationManager::addLoadProgress */
            void addLoadProgress( s32 loadProgress ) override;

            void addPlugin( SmartPtr<ISharedObject> plugin ) override;

            void removePlugin( SmartPtr<ISharedObject> plugin ) override;

            SmartPtr<IStateContext> getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> stateObject );

            SmartPtr<render::IWindow> getWindow() const;
            void setWindow( SmartPtr<render::IWindow> window );

            SmartPtr<ui::IUIWindow> getSceneRenderWindow() const;

            void setSceneRenderWindow( SmartPtr<ui::IUIWindow> sceneRenderWindow );

            Parameter triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                    const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                    SmartPtr<ISharedObject> object, SmartPtr<IEvent> event );

            SmartPtr<IAsyncOperation> triggerEventAsync( IEvent::Type eventType, hash_type eventValue,
                                                         const Array<Parameter> &arguments,
                                                         SmartPtr<ISharedObject> sender,
                                                         SmartPtr<IEvent> event );

            bool isValid() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            SharedPtr<Array<SmartPtr<IFSMManager>>> getFSMManagersPtr() const;

            String m_settingsCachePath;
            String m_cachePath;
            String m_projectPath;
            String m_projectLibraryName;
            String m_mediaPath;

            atomic_s32 m_loadProgress;
            atomic_bool m_enableRenderer = false;

            SmartPtr<IStateContext> m_stateObject;

            SmartPtr<IFSMManager> m_fsmManager;
            SharedPtr<Array<SmartPtr<IFSMManager>>> m_fsmManagers;

            SmartPtr<IFactoryManager> m_factoryManager;

            SmartPtr<ILogManager> m_logManager;

            SmartPtr<IProcessManager> m_processManager;

            SmartPtr<IMeshManager> m_meshManager;

            SmartPtr<scene::IScene> m_scene;
            SmartPtr<Properties> m_properties;
            SmartPtr<scene::IActor> m_pilot;
            SmartPtr<ui::IUIManager> m_ui;

            SmartPtr<IVehicleManager> m_vehicleManager;

            SmartPtr<IResourceDatabase> m_resourceDatabase;

            SmartPtr<IPluginInterface> m_pluginInterface;
            SmartPtr<ISoundManager> m_soundManager;
            SmartPtr<ISelectionManager> m_selectionManager;
            SmartPtr<scene::ISceneManager> m_sceneManager;
            SmartPtr<scene::IPrefabManager> m_prefabManager;

            SmartPtr<IMeshLoader> m_meshLoader;

            SmartPtr<IResourceManager> m_resourceManager;
            SmartPtr<ICommandManager> m_commandManager;
            SmartPtr<IProfiler> m_profiler;
            SmartPtr<ISystemManager> m_systemManager;
            SmartPtr<IScriptManager> m_scriptManager;
            SmartPtr<IInputDeviceManager> m_inputManager;
            SmartPtr<IConsole> m_console;

            SmartPtr<ui::IUIManager> m_uiManager;
            SmartPtr<ui::IUIManager> m_renderUiManager;

            SmartPtr<IThreadPool> m_threadPool;
            SmartPtr<render::IParticleManager> m_particleManager;
            SmartPtr<scene::ICameraManager> m_cameraManager;
            SmartPtr<IStateManager> m_stateManager;
            SmartPtr<IJobQueue> m_jobQueue;
            SmartPtr<ITaskManager> m_taskManager;
            SmartPtr<render::IGraphicsSystem> m_graphicsSystem;
            SmartPtr<IVideoManager> m_videoManager;
            SmartPtr<IFileSystem> m_fileSystem;
            SmartPtr<IApplicationClient> m_application;

            SmartPtr<ITimer> m_timer;

            SmartPtr<procedural::IProceduralEngine> m_proceduralEngine;
            SmartPtr<procedural::IProceduralManager> m_proceduralManager;

            SmartPtr<physics::IPhysicsManager2D> m_physicsManager2;
            SmartPtr<physics::IPhysicsManager> m_physicsManager3;
            SmartPtr<IFSM> m_fsm;

            SmartPtr<physics::IPhysicsScene3> m_physicsScene;
            SmartPtr<physics::IPhysicsScene3> m_controlsScene;
            SmartPtr<physics::IPhysicsScene3> m_softbodyScene;
            SmartPtr<physics::IPhysicsScene3> m_particleScene;
            SmartPtr<physics::IPhysicsScene3> m_raycastScene;
            SmartPtr<physics::IPhysicsScene3> m_objectsScene;

            mutable SmartPtr<IDatabaseManager> m_database;

            SmartPtr<ui::IUIWindow> m_sceneRenderWindow;

            atomic_bool m_isPauseMenuActive = false;
            atomic_bool m_isRunning = true;
            bool m_isPlaying = false;
            bool m_isPaused = false;
            bool m_isEditor = false;
            bool m_isEditorCamera = false;

            ///
            atomic_bool m_quit = false;
        };

    }  // namespace core
}  // namespace fb

#endif  // CApplicationManager_h__
