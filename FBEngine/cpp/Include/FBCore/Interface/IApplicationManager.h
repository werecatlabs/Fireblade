#ifndef IApplicationManager_h__
#define IApplicationManager_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/Map.h>
#include <FBCore/Core/Parameter.h>
#include <FBCore/Interface/System/IEvent.h>
#include <FBCore/Thread/ThreadTypes.h>

namespace fb
{
    namespace core
    {

        /** Interface for a class to manage an application.
            Author: Zane Desir
            Version: 1.0
        */
        class IApplicationManager : public ISharedObject
        {
        public:
            /** Virtual destructor. */
            ~IApplicationManager() override = default;

            /** Gets whether or not the renderer is enabled.
                @return True if the renderer is enabled, false otherwise.
            */
            virtual bool getEnableRenderer() const = 0;

            /** Sets whether or not the renderer is enabled.
                @param enable True to enable the renderer, false to disable it.
            */
            virtual void setEnableRenderer( bool enable ) = 0;

            /** Gets whether or not the pause menu is active.
                @return True if the pause menu is active, false otherwise.
            */
            virtual bool isPauseMenuActive() const = 0;

            /** Sets whether or not the pause menu is active.
                @param pauseMenuActive True to activate the pause menu, false to deactivate it.
            */
            virtual void setPauseMenuActive( bool pauseMenuActive ) = 0;

            /** Gets whether or not the application is running in editor mode.
                @return True if running in editor mode, false otherwise.
            */
            virtual bool isEditor() const = 0;

            /** Sets whether or not the application is running in editor mode.
                @param editor True to run in editor mode, false to run in game mode.
            */
            virtual void setEditor( bool editor ) = 0;

            /** Gets whether or not the application is running in editor camera mode.
                @return True if running in editor mode, false otherwise.
            */
            virtual bool isEditorCamera() const = 0;

            /** Sets whether or not the application is running in editor camera mode.
                @param editor True to run in editor mode, false to run in game mode.
            */
            virtual void setEditorCamera( bool editor ) = 0;

            /** Gets whether or not the application is playing.
                @return True if the application is playing, false otherwise.
            */
            virtual bool isPlaying() const = 0;

            /** Sets whether or not the application is playing.
                @param playing True to start playing the application, false to stop playing.
            */
            virtual void setPlaying( bool playing ) = 0;

            /** Gets whether or not the application is paused.
                @return True if the application is paused, false otherwise.
            */
            virtual bool isPaused() const = 0;

            /** Sets whether or not the application is paused.
                @param paused True to pause the application, false to resume it.
            */
            virtual void setPaused( bool paused ) = 0;

            /** Gets a boolean to know if the application is running.
            @return True if the application is running, false otherwise.
            */
            virtual bool isRunning() const = 0;

            /** Sets a boolean to know if the application is running.
            @param running True to run the application, false to stop it.
            */
            virtual void setRunning( bool running ) = 0;

            /** Gets a boolean to know if the application is quitting.
            @return True if the application is quitting, false otherwise.
            */
            virtual bool getQuit() const = 0;

            /** Sets a boolean to know if the application is quitting.
            @param quit True to quit the application, false to keep it running.
            */
            virtual void setQuit( bool quit ) = 0;

            /** Gets a boolean to know if there are any tasks running.
            @return True if there are tasks running, false otherwise.
            */
            virtual bool hasTasks() const = 0;

            /** Gets the path of the cache folder.
            @return The path of the cache folder.
            */
            virtual String getCachePath() const = 0;

            /** Sets the path of the cache folder.
            @param cachePath The path of the cache folder.
            */
            virtual void setCachePath( const String &cachePath ) = 0;

            /** Gets the path of the settings cache folder. */
            virtual String getSettingsPath() const = 0;

            /** Sets the path of the settings cache folder. */
            virtual void setSettingsPath( const String &cachePath ) = 0;

            /** Gets the path of the project. */
            virtual String getProjectPath() const = 0;

            /** Sets the path of the project. */
            virtual void setProjectPath( const String &projectPath ) = 0;

            /** Gets the name of the project library. */
            virtual String getProjectLibraryName() const = 0;

            /** Sets the name of the project library. */
            virtual void setProjectLibraryName( const String &projectLibraryName ) = 0;

            /** Gets the path of the media folder. */
            virtual String getMediaPath() const = 0;

            /** Sets the path of the media folder. */
            virtual void setMediaPath( const String &mediaPath ) = 0;

            /** Gets the path of the render media folder. */
            virtual String getRenderMediaPath() const = 0;

            /** Sets the path of the render media folder. */
            virtual void setRenderMediaPath( const String &renderMediaPath ) = 0;

            /** Gets the build configuration. */
            virtual String getBuildConfig() const = 0;

            /** Gets the extension of the project library. */
            virtual String getProjectLibraryExtension() const = 0;

            /** Gets the path of the project library. */
            virtual String getProjectLibraryPath() const = 0;

            /** The task used to update the application.
            This is the expected task id that will be used to update the task.
            @return The task id expected to update the task.
            */
            virtual Thread::Task getStateTask() const = 0;

            /** The current task being used to update the application.
            This is determined at runtime.
            @return The current task id used to update the task.
            */
            virtual Thread::Task getApplicationTask() const = 0;

            /**
             * Gets the current Finite State Machine (FSM) instance of the application.
             * @return A shared pointer to the FSM instance.
             */
            virtual SmartPtr<IFSM> getFSM() const = 0;

            /**
             * Sets the Finite State Machine (FSM) instance of the application.
             * @param fsm The new FSM instance to be set.
             */
            virtual void setFSM( SmartPtr<IFSM> fsm ) = 0;

            /**
             * Gets the editor settings for the application.
             * @return A shared pointer to the editor settings.
             */
            virtual SmartPtr<Properties> getEditorSettings() const = 0;

            /**
             * Sets the editor settings for the application.
             * @param properties A shared pointer to the new editor settings to be set.
             */
            virtual void setEditorSettings( SmartPtr<Properties> properties ) = 0;

            /**
             * Gets the player settings for the application.
             * @return A shared pointer to the player settings.
             */
            virtual SmartPtr<Properties> getPlayerSettings() const = 0;

            /**
             * Sets the player settings for the application.
             * @param properties A shared pointer to the new player settings to be set.
             */
            virtual void setPlayerSettings( SmartPtr<Properties> properties ) = 0;

            /**
             * Gets the log manager instance for the application.
             * @return A shared pointer to the log manager instance.
             */
            virtual SmartPtr<ILogManager> getLogManager() const = 0;

            /**
             * Sets the log manager instance for the application.
             * @param logManager A shared pointer to the new log manager instance to be set.
             */
            virtual void setLogManager( SmartPtr<ILogManager> logManager ) = 0;

            /**
             * Gets the factory manager instance for the application.
             * @return A shared pointer to the factory manager instance.
             */
            virtual SmartPtr<IFactoryManager> &getFactoryManager() = 0;

            /**
             * Gets the factory manager instance for the application.
             * @return A shared pointer to the factory manager instance.
             */
            virtual const SmartPtr<IFactoryManager> &getFactoryManager() const = 0;

            /**
             * Sets the factory manager instance for the application.
             * @param factoryManager A shared pointer to the new factory manager instance to be set.
             */
            virtual void setFactoryManager( SmartPtr<IFactoryManager> factoryManager ) = 0;

            /**
             * Gets the process manager instance for the application.
             * @return A shared pointer to the process manager instance.
             */
            virtual SmartPtr<IProcessManager> getProcessManager() const = 0;

            /**
             * Sets the process manager instance for the application.
             * @param processManager A shared pointer to the new process manager instance to be set.
             */
            virtual void setProcessManager( SmartPtr<IProcessManager> processManager ) = 0;

            /**
             * Gets a pointer to the particle manager for the application.
             * @return A SmartPtr to the particle manager.
             */
            virtual SmartPtr<render::IParticleManager> getParticleManager() const = 0;

            /**
             * Sets the particle manager for the application.
             * @param particleManager A SmartPtr to the particle manager.
             */
            virtual void setParticleManager( SmartPtr<render::IParticleManager> particleManager ) = 0;

            /**
             * Gets a pointer to the profiler for the application.
             * @return A SmartPtr to the profiler.
             */
            virtual SmartPtr<IProfiler> getProfiler() const = 0;

            /**
             * Sets the profiler for the application.
             * @param profiler A SmartPtr to the profiler.
             */
            virtual void setProfiler( SmartPtr<IProfiler> profiler ) = 0;

            /**
             * Gets a pointer to the system settings for the application.
             * @return A SmartPtr to the system settings.
             */
            virtual SmartPtr<ISystemSettings> getSystemSettings() const = 0;

            /**
             * Sets the system settings for the application.
             * @param systemManager A SmartPtr to the system settings.
             */
            virtual void setSystemSettings( SmartPtr<ISystemSettings> systemManager ) = 0;

            /**
             * Gets a pointer to the application client.
             * @return A SmartPtr to the application client.
             */
            virtual SmartPtr<IApplication> getApplication() const = 0;

            /**
             * Sets the application client.
             * @param application A SmartPtr to the application client.
             */
            virtual void setApplication( SmartPtr<IApplication> application ) = 0;

            /**
             * Gets a pointer to the job queue for the application.
             * @return A SmartPtr to the job queue.
             */
            virtual SmartPtr<IJobQueue> &getJobQueue() = 0;

            /**
             * Gets a pointer to the job queue for the application.
             * @return A SmartPtr to the job queue.
             */
            virtual const SmartPtr<IJobQueue> &getJobQueue() const = 0;

            /**
             * Sets the job queue for the application.
             * @param jobQueue A SmartPtr to the job queue.
             */
            virtual void setJobQueue( SmartPtr<IJobQueue> jobQueue ) = 0;

            /**
             * Gets a pointer to the graphics system for the application.
             * @return A SmartPtr to the graphics system.
             */
            virtual SmartPtr<render::IGraphicsSystem> &getGraphicsSystem() = 0;

            /**
             * Gets a pointer to the graphics system for the application.
             * @return A SmartPtr to the graphics system.
             */
            virtual const SmartPtr<render::IGraphicsSystem> &getGraphicsSystem() const = 0;

            /**
             * Sets the graphics system for the application.
             * @param graphicsSystem A SmartPtr to the graphics system.
             */
            virtual void setGraphicsSystem( SmartPtr<render::IGraphicsSystem> graphicsSystem ) = 0;

            virtual SmartPtr<IVideoManager> getVideoManager() const = 0;

            virtual void setVideoManager( SmartPtr<IVideoManager> videoManager ) = 0;

            /**
             * Gets a pointer to the task manager for the application.
             * @return A SmartPtr to the task manager.
             */
            virtual SmartPtr<ITaskManager> getTaskManager() const = 0;

            /**
             * Sets the task manager for the application.
             * @param taskManager A SmartPtr to the task manager.
             */
            virtual void setTaskManager( SmartPtr<ITaskManager> taskManager ) = 0;

            /**
             * Gets a pointer to the file system for the application.
             * @return A SmartPtr to the file system.
             */
            virtual SmartPtr<IFileSystem> &getFileSystem() = 0;

            /**
             * Gets a pointer to the file system for the application.
             * @return A SmartPtr to the file system.
             */
            virtual const SmartPtr<IFileSystem> &getFileSystem() const = 0;

            /**
             * Sets the file system for the application.
             * @param fileSystem A SmartPtr to the file system.
             */
            virtual void setFileSystem( SmartPtr<IFileSystem> fileSystem ) = 0;

            /**
             * Gets the timer used by the application.
             @return The timer used by the application.
             */
            virtual SmartPtr<ITimer> &getTimer() = 0;

            /**
             * Gets the timer used by the application.
             @return The timer used by the application.
             */
            virtual const SmartPtr<ITimer> &getTimer() const = 0;

            /**
             * Sets the timer used by the application.
             @param timer The timer used by the application.
             */
            virtual void setTimer( SmartPtr<ITimer> timer ) = 0;

            /**
             * Gets the finite state machine manager.
             */
            virtual SmartPtr<IFSMManager> &getFsmManager() = 0;

            /**
             * Gets the finite state machine manager.
             */
            virtual const SmartPtr<IFSMManager> &getFsmManager() const = 0;

            /**
             * Sets the finite state machine manager.
             */
            virtual void setFsmManager( SmartPtr<IFSMManager> fsmManager ) = 0;

            /**
             * Gets the finite state machine manager.
             */
            virtual SmartPtr<IFSMManager> getFsmManagerByTask( Thread::Task task ) const = 0;

            /**
             * Sets the finite state machine manager.
             */
            virtual void setFsmManagerByTask( Thread::Task task, SmartPtr<IFSMManager> fsmManager ) = 0;

            /**
             * Gets the procedural engine used by the application.
             */
            virtual SmartPtr<procedural::IProceduralEngine> getProceduralEngine() const = 0;

            /**
             * Sets the procedural engine used by the application.
             */
            virtual void setProceduralEngine( SmartPtr<procedural::IProceduralEngine> val ) = 0;

            /**
             * Gets the procedural manager used by the application.
             */
            virtual SmartPtr<procedural::IProceduralManager> getProceduralManager() const = 0;

            /**
             * Sets the procedural manager used by the application.
             */
            virtual void setProceduralManager(
                SmartPtr<procedural::IProceduralManager> proceduralManager ) = 0;

            /**
             * Gets the 2D physics manager used by the application.
             */
            virtual SmartPtr<physics::IPhysicsManager2D> getPhysicsManager2D() const = 0;

            /**
             * Sets the 2D physics manager used by the application.
             */
            virtual void setPhysicsManager2D( SmartPtr<physics::IPhysicsManager2D> physicsManager ) = 0;

            /**
             * Gets the physics manager used by the application.
             */
            virtual SmartPtr<physics::IPhysicsManager> getPhysicsManager() const = 0;

            /**
             * Sets the physics manager used by the application.
             */
            virtual void setPhysicsManager( SmartPtr<physics::IPhysicsManager> physicsManager ) = 0;

            /**
             * Get the physics scene for the application.
             * @return The physics scene for the application.
             */
            virtual SmartPtr<physics::IPhysicsScene3> getPhysicsScene() const = 0;

            /**
             * Set the physics scene for the application.
             * @param physicsScene The new physics scene for the application.
             */
            virtual void setPhysicsScene( SmartPtr<physics::IPhysicsScene3> physicsScene ) = 0;

            /**
             * Get the objects scene for the application.
             * @return The objects scene for the application.
             */
            virtual SmartPtr<physics::IPhysicsScene3> getObjectsScene() const = 0;

            /**
             * Set the objects scene for the application.
             * @param objectsScene The new objects scene for the application.
             */
            virtual void setObjectsScene( SmartPtr<physics::IPhysicsScene3> objectsScene ) = 0;

            /**
             * Get the raycast scene for the application.
             * @return The raycast scene for the application.
             */
            virtual SmartPtr<physics::IPhysicsScene3> getRaycastScene() const = 0;

            /**
             * Set the raycast scene for the application.
             * @param raycastScene The new raycast scene for the application.
             */
            virtual void setRaycastScene( SmartPtr<physics::IPhysicsScene3> raycastScene ) = 0;

            /**
             * Get the controls scene for the application.
             * @return The controls scene for the application.
             */
            virtual SmartPtr<physics::IPhysicsScene3> getControlsScene() const = 0;

            /**
             * Set the controls scene for the application.
             * @param controlsScene The new controls scene for the application.
             */
            virtual void setControlsScene( SmartPtr<physics::IPhysicsScene3> controlsScene ) = 0;

            /**
             * Get the script manager for the application.
             * @return The script manager for the application.
             */
            virtual SmartPtr<IScriptManager> getScriptManager() const = 0;

            /**
             * Set the script manager for the application.
             * @param scriptManager The new script manager for the application.
             */
            virtual void setScriptManager( SmartPtr<IScriptManager> scriptManager ) = 0;

            /**
             * Get the input for the application.
             * @return The input for the application.
             */
            virtual SmartPtr<IInput> getInput() const = 0;

            /**
             * Set the input for the application.
             * @param input The new input for the application.
             */
            virtual void setInput( SmartPtr<IInput> input ) = 0;

            /**
             * Get the input device manager for the application.
             * @return The input device manager for the application.
             */
            virtual SmartPtr<IInputDeviceManager> getInputDeviceManager() const = 0;

            /**
             * Set the input device manager for the application.
             * @param inputManager The new input device manager for the application.
             */
            virtual void setInputDeviceManager( SmartPtr<IInputDeviceManager> inputManager ) = 0;

            /**
             * Get the thread pool for the application.
             * @return The thread pool for the application.
             */
            virtual SmartPtr<IThreadPool> &getThreadPool() = 0;

            /**
             * Get the thread pool for the application.
             * @return The thread pool for the application.
             */
            virtual const SmartPtr<IThreadPool> &getThreadPool() const = 0;

            /**
             * Set the thread pool for the application.
             * @param threadPool The new thread pool for the application.
             */
            virtual void setThreadPool( SmartPtr<IThreadPool> threadPool ) = 0;

            /**
             * @brief Gets the console.
             * @return A SmartPtr to the console.
             */
            virtual SmartPtr<IConsole> getConsole() const = 0;

            /**
             * @brief Sets the console.
             * @param console A SmartPtr to the console to be set.
             */
            virtual void setConsole( SmartPtr<IConsole> console ) = 0;

            /**
             * @brief Gets the camera manager.
             * @return A SmartPtr to the camera manager.
             */
            virtual SmartPtr<scene::ICameraManager> getCameraManager() const = 0;

            /**
             * @brief Sets the camera manager.
             * @param cameraManager A SmartPtr to the camera manager to be set.
             */
            virtual void setCameraManager( SmartPtr<scene::ICameraManager> cameraManager ) = 0;

            /**
             * @brief Gets the state manager.
             * @return A SmartPtr to the state manager.
             */
            virtual SmartPtr<IStateManager> &getStateManager() = 0;
            virtual const SmartPtr<IStateManager> &getStateManager() const = 0;

            /**
             * @brief Sets the state manager.
             * @param stateManager A SmartPtr to the state manager to be set.
             */
            virtual void setStateManager( SmartPtr<IStateManager> stateManager ) = 0;

            /**
             * @brief Gets the command manager.
             * @return A SmartPtr to the command manager.
             */
            virtual SmartPtr<ICommandManager> getCommandManager() const = 0;

            /**
             * @brief Sets the command manager.
             * @param commandManager A SmartPtr to the command manager to be set.
             */
            virtual void setCommandManager( SmartPtr<ICommandManager> commandManager ) = 0;

            /**
             * @brief Gets the resource manager.
             * @return A SmartPtr to the resource manager.
             */
            virtual SmartPtr<IResourceManager> getResourceManager() const = 0;

            /**
             * @brief Sets the resource manager.
             * @param resourceManager A SmartPtr to the resource manager to be set.
             */
            virtual void setResourceManager( SmartPtr<IResourceManager> resourceManager ) = 0;

            /**
             * @brief Gets the prefab manager.
             * @return A SmartPtr to the prefab manager.
             */
            virtual SmartPtr<scene::IPrefabManager> getPrefabManager() const = 0;

            /**
             * @brief Sets the prefab manager.
             * @param prefabManager A SmartPtr to the prefab manager to be set.
             */
            virtual void setPrefabManager( SmartPtr<scene::IPrefabManager> prefabManager ) = 0;

            /**
             * @brief Gets the mesh loader.
             * @return A SmartPtr to the mesh loader.
             */
            virtual SmartPtr<IMeshLoader> getMeshLoader() const = 0;

            /**
             * @brief Sets the mesh loader.
             * @param meshLoader A SmartPtr to the mesh loader to be set.
             */
            virtual void setMeshLoader( SmartPtr<IMeshLoader> meshLoader ) = 0;

            /**
             * Returns the resource database.
             *
             * @return The resource database.
             */
            virtual SmartPtr<IResourceDatabase> getResourceDatabase() const = 0;

            /**
             * Sets the resource database.
             *
             * @param resourceDatabase The resource database.
             */
            virtual void setResourceDatabase( SmartPtr<IResourceDatabase> resourceDatabase ) = 0;

            /**
             * Returns the scene manager.
             *
             * @return The scene manager.
             */
            virtual SmartPtr<scene::ISceneManager> getSceneManager() const = 0;

            /**
             * Sets the scene manager.
             *
             * @param sceneManager The scene manager.
             */
            virtual void setSceneManager( SmartPtr<scene::ISceneManager> sceneManager ) = 0;

            /**
             * Returns the scene.
             *
             * @return The scene.
             */
            virtual SmartPtr<scene::IScene> getScene() const = 0;

            /**
             * Returns the selection manager.
             *
             * @return The selection manager.
             */
            virtual SmartPtr<ISelectionManager> getSelectionManager() const = 0;

            /**
             * Sets the selection manager.
             *
             * @param selectionManager The selection manager.
             */
            virtual void setSelectionManager( SmartPtr<ISelectionManager> selectionManager ) = 0;

            /**
             * Returns the sound manager.
             *
             * @return The sound manager.
             */
            virtual SmartPtr<ISoundManager> getSoundManager() const = 0;

            /**
             * Sets the sound manager.
             *
             * @param soundManager The sound manager.
             */
            virtual void setSoundManager( SmartPtr<ISoundManager> soundManager ) = 0;

            /**
             * @brief Returns a SmartPtr to the IUIManager object used by the application.
             *
             * @return A SmartPtr to the IUIManager object used by the application.
             */
            virtual SmartPtr<ui::IUIManager> getUI() const = 0;

            /**
             * @brief Sets the IUIManager object to be used by the application.
             *
             * @param ui A SmartPtr to the IUIManager object to be used by the application.
             */
            virtual void setUI( SmartPtr<ui::IUIManager> ui ) = 0;

            /**
             * @brief Returns a SmartPtr to the IUIManager object used for rendering by the application.
             *
             * @return A SmartPtr to the IUIManager object used for rendering by the application.
             */
            virtual SmartPtr<ui::IUIManager> getRenderUI() const = 0;

            /**
             * @brief Sets the IUIManager object to be used for rendering by the application.
             *
             * @param renderUI A SmartPtr to the IUIManager object to be used for rendering by the application.
             */
            virtual void setRenderUI( SmartPtr<ui::IUIManager> renderUI ) = 0;

            /**
             * @brief Returns a SmartPtr to the IDatabaseManager object used by the application.
             *
             * @return A SmartPtr to the IDatabaseManager object used by the application.
             */
            virtual SmartPtr<IDatabaseManager> getDatabase() const = 0;

            /**
             * @brief Sets the IDatabaseManager object to be used by the application.
             *
             * @param database A SmartPtr to the IDatabaseManager object to be used by the application.
             */
            virtual void setDatabase( SmartPtr<IDatabaseManager> database ) = 0;

            /**
             * @brief Returns a SmartPtr to the IMeshManager object used by the application.
             *
             * @return A SmartPtr to the IMeshManager object used by the application.
             */
            virtual SmartPtr<IMeshManager> getMeshManager() const = 0;

            /**
             * @brief Sets the IMeshManager object to be used by the application.
             *
             * @param meshManager A SmartPtr to the IMeshManager object to be used by the application.
             */
            virtual void setMeshManager( SmartPtr<IMeshManager> meshManager ) = 0;

            /**
             * @brief Returns a SmartPtr to the IVehicleManager object used by the application.
             *
             * @return A SmartPtr to the IVehicleManager object used by the application.
             */
            virtual SmartPtr<IVehicleManager> getVehicleManager() const = 0;

            /**
             * @brief Sets the IVehicleManager object to be used by the application.
             *
             * @param vehicleManager A SmartPtr to the IVehicleManager object to be used by the application.
             */
            virtual void setVehicleManager( SmartPtr<IVehicleManager> vehicleManager ) = 0;

            /**
             * Gets the progress of loading.
             * @return The progress of loading.
             */
            virtual s32 getLoadProgress() const = 0;

            /**
             * Sets the progress of loading.
             * @param loadProgress The progress of loading.
             */
            virtual void setLoadProgress( s32 loadProgress ) = 0;

            /**
             * Adds to the progress of loading.
             * @param loadProgress The amount of progress to add.
             */
            virtual void addLoadProgress( s32 loadProgress ) = 0;

            /**
             * Gets an array of loaded actors.
             */
            virtual Array<SmartPtr<scene::IActor>> getActors() const = 0;

            /** Gets the plugin manager. */
            virtual SmartPtr<IPluginManager> getPluginManager() const = 0;

            /** Sets the plugin manager. */
            virtual void setPluginManager( SmartPtr<IPluginManager> pluginManager ) = 0;

            /**
             * Adds a plugin to the application manager.
             *
             * @param plugin The plugin to add.
             */
            virtual void addPlugin( SmartPtr<ISharedObject> plugin ) = 0;

            /**
             * Removes a plugin from the application manager.
             *
             * @param plugin The plugin to remove.
             */
            virtual void removePlugin( SmartPtr<ISharedObject> plugin ) = 0;

            /**
             * Gets the state object of the application manager.
             */
            virtual SmartPtr<IStateContext> getStateContext() const = 0;

            /**
             * Sets the state object of the application manager.
             *
             * @param stateContext The new state object.
             */
            virtual void setStateContext( SmartPtr<IStateContext> stateContext ) = 0;

            /**
             * Gets the main window of the application.
             */
            virtual SmartPtr<render::IWindow> getWindow() const = 0;

            /**
             * Sets the main window of the application.
             *
             * @param window The new main window.
             */
            virtual void setWindow( SmartPtr<render::IWindow> window ) = 0;

            /**
             * Gets the scene render window.
             */
            virtual SmartPtr<ui::IUIWindow> getSceneRenderWindow() const = 0;

            /**
             * Sets the scene render window.
             */
            virtual void setSceneRenderWindow( SmartPtr<ui::IUIWindow> sceneRenderWindow ) = 0;

            /** Gets a component by type. */
            virtual SmartPtr<scene::IComponent> getComponentByType( u32 typeId ) const = 0;

            /** Handles a event.
            @param eventType The event type.
            @param eventValue The event value.
            @param sender The object triggering the event. This can be null.
            @param event The event data. This can be null.
            @return Contains a return parameter. */
            virtual Parameter triggerEvent( IEvent::Type eventType, hash_type eventValue,
                                            const Array<Parameter> &arguments,
                                            SmartPtr<ISharedObject> sender,
                                            SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) = 0;

            /** Gets the type manager.
            @return A pointer to the type manager.
            */
            virtual RawPtr<TypeManager> getTypeManager() const = 0;

            /** Sets the type manager.
            @param typeManager A pointer to the type manager.
            */
            virtual void setTypeManager( RawPtr<TypeManager> typeManager ) = 0;

            /**
             * Gets a component by the template type.
             * @tparam T The type of the component.
             * @return A SmartPtr to the component, or a nullptr if the component is not found.
             */
            template <class T>
            SmartPtr<T> getComponent() const;

            /** Allocates memory. */
            virtual void *allocateMemory( size_t size ) = 0;

            /** Frees memory. */
            virtual void freeMemory( void *ptr ) = 0;

            template <class T>
            SmartPtr<T> make_ptr();

            template <class T, class... _Types>
            SmartPtr<T> make_ptr( _Types &&..._Args );

            /**
             * Gets the application manager instance.
             */
            static SmartPtr<IApplicationManager> &instance();

            /**
             * Sets the application manager instance.
             * @param instance A SmartPtr to the instance of the IApplicationManager.
             */
            static void setInstance( SmartPtr<IApplicationManager> instance );

            FB_CLASS_REGISTER_DECL;

        private:
            /**
             * The instance of the application manager, used for storing the singleton instance of this class.
             */
            static SmartPtr<IApplicationManager> m_instance;
        };

        template <class T>
        SmartPtr<T> IApplicationManager::getComponent() const
        {
            auto typeInfo = T::typeInfo();
            auto component = getComponentByType( typeInfo );
            return fb::static_pointer_cast<T>( component );
        }

        template <class T>
        SmartPtr<T> IApplicationManager::make_ptr()
        {
            auto size = sizeof( T );
            auto ptr = allocateMemory( size );

            auto object = new( ptr ) T();
            auto p = SmartPtr<T>( object );
            object->removeReference();
            return p;
        }

        template <class T, class... _Types>
        SmartPtr<T> IApplicationManager::make_ptr( _Types &&..._Args )
        {
            auto size = sizeof( T );
            auto ptr = allocateMemory( size );

            auto object = new( ptr ) T( std::forward<_Types>( _Args )... );
            auto p = SmartPtr<T>( ptr );
            object->removeReference();
            return p;
        }

        inline SmartPtr<IApplicationManager> &IApplicationManager::instance()
        {
            return m_instance;
        }

    }  // namespace core
}  // end namespace fb

#endif  // IApplicationManager_h__
