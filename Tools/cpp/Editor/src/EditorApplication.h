#ifndef __Application_h__
#define __Application_h__

#include <GameEditorPrerequisites.h>
#include <FBCore/Application.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Core/FSMListener.h>

namespace fb
{
    namespace editor
    {

        /**
         * @brief Represents the main application for the editor.
         *
         * The EditorApplication class is derived from the core::Application class and
         * serves as the entry point and manager for the editor application.
         */
        class EditorApplication : public core::Application
        {
        public:
            /**
             * @brief Enumerates the possible states of the editor application.
             */
            enum class State
            {
                Loading,  /**< The application is in the loading state. */
                Editor,   /**< The application is in the editor state. */
                Shutdown, /**< The application is in the shutdown state. */
                Count     /**< The total number of states. */
            };

            /**
             * @brief Default constructor for the EditorApplication class.
             */
            EditorApplication();

            /**
             * @brief Destructor for the EditorApplication class.
             */
            ~EditorApplication() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void run() override;
            void iterate();

            void createLogManager() override;
            void createFactoryManager() override;

            void update() override;

            bool inputEvent( SmartPtr<IInputEvent> event );

            IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

            size_t getWindowHandle() const;

            /**
             * @brief Sets the window handle for the application.
             *
             * @param val The window handle to set.
             */
            void setWindowHandle( size_t val );

            /**
             * @brief Enters play mode in the editor application.
             */
            void enterPlayMode();

            /**
             * @brief Stops play mode in the editor application.
             */
            void stopPlayMode();

            SmartPtr<render::ICamera> getUiCamera() const;
            void setUiCamera( SmartPtr<render::ICamera> camera );

            SmartPtr<render::ISceneNode> getUiCameraSceneNode() const;
            void setUiCameraSceneNode( SmartPtr<render::ISceneNode> sceneNode );

            SmartPtr<render::ICamera> getCamera() const;
            void setCamera( SmartPtr<render::ICamera> camera );

            SmartPtr<render::ISceneNode> getCameraSceneNode() const;
            void setCameraSceneNode( SmartPtr<render::ISceneNode> sceneNode );

            SmartPtr<IStateContext> getStateObject() const;
            void setStateObject( SmartPtr<IStateContext> stateObject );

            SmartPtr<IStateListener> getStateListener() const;
            void setStateListener( SmartPtr<IStateListener> stateListener );

            SmartPtr<render::IViewport> getRttViewport() const;

            void setRttViewport( SmartPtr<render::IViewport> rttViewport );

        private:
            class ApplicationInputListener : public IEventListener
            {
            public:
                ApplicationInputListener( EditorApplication *app );
                ~ApplicationInputListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                bool inputEvent( SmartPtr<IInputEvent> event );
                bool updateEvent( const SmartPtr<IInputEvent> &event );

                void setPriority( s32 priority );
                s32 getPriority() const;

            protected:
                RawPtr<EditorApplication> m_application;
            };

            class ApplicationStateListener : public IStateListener
            {
            public:
                ApplicationStateListener() = default;
                ~ApplicationStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                EditorApplication *getOwner() const;
                void setOwner( EditorApplication *owner );

            protected:
                EditorApplication *m_owner = nullptr;
            };

            class ApplicationFSMListener : public FSMListener
            {
            public:
                ApplicationFSMListener() = default;
                ~ApplicationFSMListener() override = default;

                IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

                EditorApplication *getOwner() const;

                void setOwner( EditorApplication *owner );

            protected:
                EditorApplication *m_owner = nullptr;
            };

            class EventListener : public IEventListener
            {
            public:
                EventListener() = default;
                ~EventListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;
            };

            void loadDebug( SmartPtr<ISharedObject> data );

            void createTimer() override;

            void createFsmManager() override;
            void createFsm() override;

            void createTaskManager() override;

	        void createProfiler();
            void createThreadPool() override;

            void createStateManager() override;

            void createSceneManager() override;
            void createFileSystem() override;

            void createUI() override;
            bool createGraphicsSystem() override;

            void createRenderWindow() override;
            void createCamera() override;
            void createViewports() override;
            void createRenderInitJob();

            void loadGraphicsResources() override;
            void createGraphicsSceneManager() override;
            void createTasks() override;
            void createPhysics() override;

            bool createScriptManager() override;

            void createInputSystem() override;

            size_t m_windowHandle = 0;

            SmartPtr<render::ICamera> m_uiCamera;
            SmartPtr<render::ISceneNode> m_uiCameraSceneNode;

            SmartPtr<render::IViewport> m_uiViewport;
            SmartPtr<render::IViewport> m_rttViewport;

            SmartPtr<render::ITexture> m_renderTarget;

            SmartPtr<scene::CameraController> m_fpsCamera;
            SmartPtr<scene::SphericalCameraController> m_sphericalCamera;

            SmartPtr<scene::IActor> m_sphericalCameraActor;

            RawPtr<ApplicationFrame> m_frame = nullptr;
            SmartPtr<EditorManager> m_editorManager;

            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;

            SmartPtr<ui::IUIApplication> m_application;
            SmartPtr<ui::IUIRenderWindow> m_renderWindow;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // App_h__
