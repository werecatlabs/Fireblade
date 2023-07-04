#ifndef __Application_h__
#define __Application_h__

#include <GameEditorPrerequisites.h>
#include <FBApplication/CApplicationClient.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Core/FSMListener.h>

namespace fb
{
    namespace editor
    {

        /** The editor application. */
        class Application : public application::CApplicationClient
        {
        public:
            enum class State
            {
                Loading,
                Editor,
                Shutdown,

                Count
            };

            Application();
            ~Application() override;

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
            void setWindowHandle( size_t val );

            // todo
            void enterPlayMode();
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
            class ApplicationInputListener : public SharedObject<IEventListener>
            {
            public:
                ApplicationInputListener( Application *app );
                ~ApplicationInputListener() override;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;

                bool inputEvent( SmartPtr<IInputEvent> event );
                bool updateEvent( const SmartPtr<IInputEvent> &event );

                void setPriority( s32 priority );
                s32 getPriority() const;

            protected:
                RawPtr<Application> m_application;
            };

            class ApplicationStateListener : public SharedObject<IStateListener>
            {
            public:
                ApplicationStateListener() = default;
                ~ApplicationStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                Application *getOwner() const;
                void setOwner( Application *owner );

            protected:
                Application *m_owner = nullptr;
            };

            class ApplicationFSMListener : public FSMListener
            {
            public:
                ApplicationFSMListener() = default;
                ~ApplicationFSMListener() override = default;

                IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType ) override;

                Application *getOwner() const;

                void setOwner( Application *owner );

            protected:
                Application *m_owner = nullptr;
            };

            class EventListener : public SharedObject<IEventListener>
            {
            public:
                EventListener() = default;
                ~EventListener() override = default;

                Parameter handleEvent( IEvent::Type eventType, hash_type eventValue,
                                       const Array<Parameter> &arguments, SmartPtr<ISharedObject> sender,
                                       SmartPtr<ISharedObject> object, SmartPtr<IEvent> event ) override;
            };

            void createTimer() override;

            void createFsmManager() override;
            void createFsm() override;

            void createTaskManager() override;

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

            SmartPtr<render::ICamera> m_camera;
            SmartPtr<render::ISceneNode> m_cameraSceneNode;

            SmartPtr<render::IViewport> m_viewport;
            SmartPtr<render::IViewport> m_uiViewport;
            SmartPtr<render::IViewport> m_rttViewport;

            SmartPtr<render::ITexture> m_renderTarget;

            SmartPtr<scene::CameraController> m_fpsCamera;
            SmartPtr<scene::SphericalCameraController> m_sphericalCamera;

            SmartPtr<scene::IActor> m_sphericalCameraActor;

            RawPtr<ApplicationFrame> m_frame = nullptr;
            SmartPtr<EditorManager> m_editorManager;

            atomic_bool m_isRunning = true;

            SmartPtr<IStateContext> m_stateObject;
            SmartPtr<IStateListener> m_stateListener;

            SmartPtr<ui::IUIApplication> m_application;
            SmartPtr<ui::IUIRenderWindow> m_renderWindow;
        };
    }  // end namespace editor
}  // end namespace fb

#endif  // App_h__
