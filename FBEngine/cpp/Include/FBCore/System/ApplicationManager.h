#ifndef __ApplicationManager_h__
#define __ApplicationManager_h__

#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/IApplication.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Interface/System/ITimer.h>
#include <FBCore/Interface/FSM/IFSMManager.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Core/Parameter.h>

namespace fb
{
    namespace core
    {

        /** Implementation of the IApplicationManager interface. */
        class ApplicationManager : public IApplicationManager
        {
        public:
            /** Constructor. */
            ApplicationManager();

            /** Destructor. */
            ~ApplicationManager() override;

            /** @copydoc IApplicationManager::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IApplicationManager::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IApplicationManager::isEditor */
            bool isEditor() const override;

            /** @copydoc IApplicationManager::setEditor */
            void setEditor( bool editor ) override;

            /** @copydoc IApplicationManager::isEditorCamera */
            bool isEditorCamera() const override;

            /** @copydoc IApplicationManager::setEditorCamera */
            void setEditorCamera( bool editor ) override;

            /** @copydoc IApplicationManager::isPlaying */
            bool isPlaying() const override;

            /** @copydoc IApplicationManager::setPlaying */
            void setPlaying( bool playing ) override;

            /** @copydoc IApplicationManager::isPaused */
            bool isPaused() const override;

            /** @copydoc IApplicationManager::setPaused */
            void setPaused( bool paused ) override;

            /** @copydoc IApplicationManager::isRunning */
            bool isRunning() const override;

            /** @copydoc IApplicationManager::setRunning */
            void setRunning( bool running ) override;

            /** @copydoc IApplicationManager::getQuit */
            bool getQuit() const override;

            /** @copydoc IApplicationManager::setQuit */
            void setQuit( bool quit ) override;

            /** @copydoc IApplicationManager::hasTasks */
            bool hasTasks() const override;

            /** @copydoc IApplicationManager::isPauseMenuActive */
            bool isPauseMenuActive() const override;

            /** @copydoc IApplicationManager::setPauseMenuActive */
            void setPauseMenuActive( bool pauseMenuActive ) override;

            /** @copydoc IApplicationManager::getCachePath */
            String getCachePath() const override;

            /** @copydoc IApplicationManager::setCachePath */
            void setCachePath( const String &cachePath ) override;

            /** @copydoc IApplicationManager::getProjectPath */
            String getProjectPath() const override;

            /** @copydoc IApplicationManager::setProjectPath */
            void setProjectPath( const String &projectPath ) override;

            /** @copydoc IApplicationManager::getProjectLibraryName */
            String getProjectLibraryName() const override;

            /** @copydoc IApplicationManager::setProjectLibraryName */
            void setProjectLibraryName( const String &projectLibraryName ) override;

            /** @copydoc IApplicationManager::getMediaPath */
            String getMediaPath() const override;

            /** @copydoc IApplicationManager::setMediaPath */
            void setMediaPath( const String &mediaPath ) override;

            /** @copydoc IApplicationManager::getRenderMediaPath */
            String getRenderMediaPath() const override;

            /** @copydoc IApplicationManager::setRenderMediaPath */
            void setRenderMediaPath( const String &renderMediaPath ) override;

            /** @copydoc IApplicationManager::getSettingsPath */
            String getSettingsPath() const override;

            /** @copydoc IApplicationManager::setSettingsPath */
            void setSettingsPath( const String &cachePath ) override;

            /** @copydoc IApplicationManager::getBuildConfig */
            String getBuildConfig() const override;

            /** @copydoc IApplicationManager::getProjectLibraryExtension */
            String getProjectLibraryExtension() const override;

            /** @copydoc IApplicationManager::getProjectLibraryPath */
            String getProjectLibraryPath() const override;

            /** @copydoc IApplicationManager::getStateTask */
            Thread::Task getStateTask() const override;

            /** @copydoc IApplicationManager::getApplicationTask */
            Thread::Task getApplicationTask() const override;

            /** @copydoc IApplicationManager::getEnableRenderer */
            bool getEnableRenderer() const override;

            /** @copydoc IApplicationManager::setEnableRenderer */
            void setEnableRenderer( bool enable ) override;

            /** @copydoc IApplicationManager::getFSM */
            SmartPtr<IFSM> getFSM() const override;

            /** @copydoc IApplicationManager::setFSM */
            void setFSM( SmartPtr<IFSM> fsm ) override;

            /** @copydoc IApplicationManager::getEditorSettings */
            SmartPtr<Properties> getEditorSettings() const override;

            /** @copydoc IApplicationManager::setEditorSettings */
            void setEditorSettings( SmartPtr<Properties> properties ) override;

            /** @copydoc IApplicationManager::getPlayerSettings */
            SmartPtr<Properties> getPlayerSettings() const override;

            /** @copydoc IApplicationManager::setPlayerSettings */
            void setPlayerSettings( SmartPtr<Properties> properties ) override;

            /** @copydoc IApplicationManager::getLogManager */
            SmartPtr<ILogManager> getLogManager() const override;

            /** @copydoc IApplicationManager::setLogManager */
            void setLogManager( SmartPtr<ILogManager> logManager ) override;

            /** @copydoc IApplicationManager::getFactoryManager */
            SmartPtr<IFactoryManager> &getFactoryManager() override;

            /** @copydoc IApplicationManager::getFactoryManager */
            const SmartPtr<IFactoryManager> &getFactoryManager() const override;

            /** @copydoc IApplicationManager::setFactoryManager */
            void setFactoryManager( SmartPtr<IFactoryManager> factoryManager ) override;

            /** @copydoc IApplicationManager::getProcessManager */
            SmartPtr<IProcessManager> getProcessManager() const override;

            /** @copydoc IApplicationManager::setProcessManager */
            void setProcessManager( SmartPtr<IProcessManager> processManager ) override;

            /** @copydoc IApplicationManager::getApplication */
            SmartPtr<IApplication> getApplication() const override;

            /** @copydoc IApplicationManager::setApplication */
            void setApplication( SmartPtr<IApplication> application ) override;

            /** @copydoc IApplicationManager::getFileSystem */
            SmartPtr<IFileSystem> &getFileSystem() override;

            /** @copydoc IApplicationManager::getFileSystem */
            const SmartPtr<IFileSystem> &getFileSystem() const override;

            /** @copydoc IApplicationManager::setFileSystem */
            void setFileSystem( SmartPtr<IFileSystem> fileSystem ) override;

            /** @copydoc IApplicationManager::getTimer */
            SmartPtr<ITimer> &getTimer() override;

            /** @copydoc IApplicationManager::getTimer */
            const SmartPtr<ITimer> &getTimer() const override;

            /** @copydoc IApplicationManager::setTimer */
            void setTimer( SmartPtr<ITimer> timer ) override;

            SmartPtr<IFSMManager> &getFsmManager() override;
            const SmartPtr<IFSMManager> &getFsmManager() const override;
            void setFsmManager( SmartPtr<IFSMManager> fsmManager ) override;

            SmartPtr<IFSMManager> getFsmManagerByTask( Thread::Task task ) const override;
            void setFsmManagerByTask( Thread::Task task, SmartPtr<IFSMManager> fsmManager ) override;

            SmartPtr<procedural::IProceduralEngine> getProceduralEngine() const override;
            void setProceduralEngine( SmartPtr<procedural::IProceduralEngine> val ) override;

            SmartPtr<procedural::IProceduralManager> getProceduralManager() const override;
            void setProceduralManager( SmartPtr<procedural::IProceduralManager> val ) override;

            SmartPtr<physics::IPhysicsManager2D> getPhysicsManager2D() const override;
            void setPhysicsManager2D( SmartPtr<physics::IPhysicsManager2D> physicsManager ) override;

            SmartPtr<physics::IPhysicsManager> getPhysicsManager() const override;
            void setPhysicsManager( SmartPtr<physics::IPhysicsManager> physicsManager ) override;

            SmartPtr<render::IGraphicsSystem> &getGraphicsSystem() override;
            const SmartPtr<render::IGraphicsSystem> &getGraphicsSystem() const override;
            void setGraphicsSystem( SmartPtr<render::IGraphicsSystem> graphicsSystem ) override;

            SmartPtr<IVideoManager> getVideoManager() const override;

            void setVideoManager( SmartPtr<IVideoManager> videoManager ) override;

            SmartPtr<ITaskManager> getTaskManager() const override;
            void setTaskManager( SmartPtr<ITaskManager> taskManager ) override;

            SmartPtr<IProfiler> getProfiler() const override;
            void setProfiler( SmartPtr<IProfiler> profiler ) override;

            SmartPtr<ISystemSettings> getSystemSettings() const override;
            void setSystemSettings( SmartPtr<ISystemSettings> val ) override;

            SmartPtr<IJobQueue> &getJobQueue() override;
            const SmartPtr<IJobQueue> &getJobQueue() const override;
            void setJobQueue( SmartPtr<IJobQueue> jobQueue ) override;

            SmartPtr<render::IParticleManager> getParticleManager() const override;
            void setParticleManager( SmartPtr<render::IParticleManager> particleManager ) override;

            SmartPtr<IScriptManager> getScriptManager() const override;
            void setScriptManager( SmartPtr<IScriptManager> physicsManager ) override;

            SmartPtr<IInput> getInput() const override;
            void setInput( SmartPtr<IInput> input ) override;

            SmartPtr<IInputDeviceManager> getInputDeviceManager() const override;
            void setInputDeviceManager( SmartPtr<IInputDeviceManager> inputManager ) override;

            SmartPtr<IThreadPool> &getThreadPool() override;
            const SmartPtr<IThreadPool> &getThreadPool() const override;
            void setThreadPool( SmartPtr<IThreadPool> threadPool ) override;

            SmartPtr<IConsole> getConsole() const override;
            void setConsole( SmartPtr<IConsole> console ) override;

            SmartPtr<scene::ICameraManager> getCameraManager() const override;
            void setCameraManager( SmartPtr<scene::ICameraManager> cameraManager ) override;

            SmartPtr<IStateManager> &getStateManager() override;
            const SmartPtr<IStateManager> &getStateManager() const override;
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

            SmartPtr<IPluginManager> getPluginManager() const override;

            void setPluginManager( SmartPtr<IPluginManager> pluginManager ) override;

            /** @copydoc IApplicationManager::addPlugin */
            void addPlugin( SmartPtr<ISharedObject> plugin ) override;

            /** @copydoc IApplicationManager::removePlugin */
            void removePlugin( SmartPtr<ISharedObject> plugin ) override;

            /** @copydoc IApplicationManager::getStateContext */
            SmartPtr<IStateContext> getStateContext() const override;

            /** @copydoc IApplicationManager::setStateContext */
            void setStateContext( SmartPtr<IStateContext> stateContext ) override;

            /** @copydoc IApplicationManager::getWindow */
            SmartPtr<render::IWindow> getWindow() const override;

            /** @copydoc IApplicationManager::setWindow */
            void setWindow( SmartPtr<render::IWindow> window ) override;

            /** @copydoc IApplicationManager::getSceneRenderWindow */
            SmartPtr<ui::IUIWindow> getSceneRenderWindow() const override;

            /** @copydoc IApplicationManager::setSceneRenderWindow */
            void setSceneRenderWindow( SmartPtr<ui::IUIWindow> sceneRenderWindow ) override;

            /** @copydoc IApplicationManager::getComponentByType */
            SmartPtr<scene::IComponent> getComponentByType( u32 typeId ) const override;

            /** @copydoc IApplicationManager::triggerEvent */
            Parameter triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                    const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                    SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

            RawPtr<TypeManager> getTypeManager() const override;

            void setTypeManager( RawPtr<TypeManager> typeManager ) override;

            Array<SmartPtr<ISharedObject>> getChildObjects() const;

            /** Allocate memory. */
            void *allocateMemory( size_t size );

            /** Frees memory. */
            virtual void freeMemory( void *ptr );

            /** Gets the instance of the application manager. */
            static SmartPtr<ApplicationManager> &instance();

            /** Sets the instance of the application manager. */
            static void setInstance( SmartPtr<ApplicationManager> instance );

            FB_CLASS_REGISTER_DECL;

        protected:
            SharedPtr<Array<SmartPtr<IFSMManager>>> getFSMManagersPtr() const;
            void setFSMManagersPtr( SharedPtr<Array<SmartPtr<IFSMManager>>> fsmManagers );

            SmartPtr<scene::IComponent> getActorComponent( SmartPtr<scene::IActor> actor,
                                                           u32 typeId ) const;

            String m_settingsCachePath;
            String m_cachePath;
            String m_projectPath;
            String m_projectLibraryName;
            String m_mediaPath;
            String m_renderMediaPath;

            atomic_s32 m_loadProgress;
            atomic_bool m_enableRenderer = false;

            SmartPtr<IStateContext> m_stateContext;

            SmartPtr<Properties> m_editorSettings;
            SmartPtr<Properties> m_playerSettings;

            SmartPtr<IFSMManager> m_fsmManager;
            AtomicSharedPtr<Array<SmartPtr<IFSMManager>>> m_fsmManagers;

            SmartPtr<IFactoryManager> m_factoryManager;

            SmartPtr<ILogManager> m_logManager;

            SmartPtr<IProcessManager> m_processManager;

            SmartPtr<IMeshManager> m_meshManager;

            SmartPtr<Properties> m_properties;
            SmartPtr<ui::IUIManager> m_ui;

            SmartPtr<IVehicleManager> m_vehicleManager;

            SmartPtr<IResourceDatabase> m_resourceDatabase;

            SmartPtr<ISoundManager> m_soundManager;
            SmartPtr<ISelectionManager> m_selectionManager;
            SmartPtr<scene::ISceneManager> m_sceneManager;
            SmartPtr<scene::IPrefabManager> m_prefabManager;

            SmartPtr<IMeshLoader> m_meshLoader;

            SmartPtr<IResourceManager> m_resourceManager;
            SmartPtr<ICommandManager> m_commandManager;
            SmartPtr<IProfiler> m_profiler;
            SmartPtr<ISystemSettings> m_systemManager;
            SmartPtr<IScriptManager> m_scriptManager;

            SmartPtr<IInput> m_input;
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
            SmartPtr<IApplication> m_application;

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

            SmartPtr<render::IWindow> m_window;
            SmartPtr<ui::IUIWindow> m_sceneRenderWindow;

            SmartPtr<IPluginManager> m_pluginManager;

            AtomicRawPtr<TypeManager> m_typeManager;

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

            static SmartPtr<ApplicationManager> m_instance;
        };

        inline SmartPtr<render::IGraphicsSystem> &ApplicationManager::getGraphicsSystem()
        {
            return m_graphicsSystem;
        }

        inline const SmartPtr<render::IGraphicsSystem> &ApplicationManager::getGraphicsSystem() const
        {
            return m_graphicsSystem;
        }

        inline SmartPtr<ITimer> &ApplicationManager::getTimer()
        {
            return m_timer;
        }

        inline const SmartPtr<ITimer> &ApplicationManager::getTimer() const
        {
            return m_timer;
        }

        inline SmartPtr<IFSMManager> &ApplicationManager::getFsmManager()
        {
            return m_fsmManager;
        }

        inline const SmartPtr<IFSMManager> &ApplicationManager::getFsmManager() const
        {
            return m_fsmManager;
        }

        inline SmartPtr<IThreadPool> &ApplicationManager::getThreadPool()
        {
            return m_threadPool;
        }

        inline const SmartPtr<IThreadPool> &ApplicationManager::getThreadPool() const
        {
            return m_threadPool;
        }

        inline SmartPtr<IFactoryManager> &ApplicationManager::getFactoryManager()
        {
            return m_factoryManager;
        }

        inline const SmartPtr<IFactoryManager> &ApplicationManager::getFactoryManager() const
        {
            return m_factoryManager;
        }

        inline SmartPtr<IFileSystem> &ApplicationManager::getFileSystem()
        {
            return m_fileSystem;
        }

        inline const SmartPtr<IFileSystem> &ApplicationManager::getFileSystem() const
        {
            return m_fileSystem;
        }

        inline SmartPtr<IJobQueue> &ApplicationManager::getJobQueue()
        {
            return m_jobQueue;
        }

        inline const SmartPtr<IJobQueue> &ApplicationManager::getJobQueue() const
        {
            return m_jobQueue;
        }

        inline SmartPtr<ApplicationManager> &ApplicationManager::instance()
        {
            return m_instance;
        }
    }  // namespace core
}  // namespace fb

#endif  // CApplicationManager_h__
