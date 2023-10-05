#ifndef _FB_ApplicationClient_H_
#define _FB_ApplicationClient_H_

#include <FBCore/Interface/IApplication.h>
#include <FBCore/Interface/FSM/IFSM.h>
#include <FBCore/Interface/Resource/IResource.h>

namespace fb
{
    namespace core
    {

        /** Base class which manages the standard startup of a game application.
        Create defaults components. Designed to be subclassed for specific case if required. */
        class Application : public IApplication
        {
        public:
            /// constructor
            Application();

            /// destructor
            ~Application() override;

            /** Initialise the game application. */
            void load( SmartPtr<ISharedObject> data ) override;

            /** Shutdown the game application. */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** Starts the game loop.
            @remarks
            Contains a default implementation. This should be overridden to
            do something significant.
            */
            void run() override;

            /** Called to update the application. */
            void update() override;

            SmartPtr<IFSM> getFSM() const override;
            void setFSM( SmartPtr<IFSM> fsm ) override;

            size_t getActiveThreads() const;
            void setActiveThreads( size_t activeThreads );

	        SmartPtr<scene::IActor> createDefaultSky( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultCamera( bool addToScene = true );

            void createRigidStaticMesh();

            void createRigidDynamicMesh();

            SmartPtr<scene::IActor> createDefaultCubemap( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultCube( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultCubeMesh( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultGround( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultTerrain( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultConstraint();

            SmartPtr<scene::IActor> createDirectionalLight( bool addToScene = true );

            SmartPtr<scene::IActor> createPointLight( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultPlane( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultVehicle( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultCar( bool addToScene = true );

            SmartPtr<scene::IActor> createDefaultTruck( bool addToScene = true );

            SmartPtr<scene::IActor> createVehicleFromDatabase( s32 id );

            SmartPtr<scene::IActor> createDefaultParticleSystem( bool addToScene = true );

            SmartPtr<IData> loadVehicleDataFromDatabase( s32 id );

            SmartPtr<render::IMaterial> createDefaultMaterialUI();

            SmartPtr<render::IMaterial> createDefaultMaterial();

            void createDefaultMaterials();

            void createRigidStaticMesh( SmartPtr<scene::IActor> actor, bool recursive );
            void createRigidDynamicMesh( SmartPtr<scene::IActor> actor, bool recursive );

            IResource::ResourceType getResourceType( const String &resourceTypeName );

            String getResourceTypeByName( IResource::ResourceType resourceType );

            SmartPtr<Properties> importScene( const String &filePath );

        protected:
            virtual void createLogManager();
            virtual void createFactoryManager();
            virtual void createStateManager();
            virtual void createTimer();

            virtual void createPrefabManager();

            virtual void createProfiler();

            virtual void createThreadPool();
            virtual void createTaskManager();

            virtual void createTasks();

            virtual void createJobQueue();

            virtual void createFsmManager();
            virtual void createFsm();

            virtual void createOutputManager();

            virtual void createComponentsContainer();

            virtual void createPlatformManager();

            virtual void createFileSystem();

            //
            virtual void createUI();

            virtual void createRenderUI();

            //
            virtual void createSceneManager();

            //
            virtual void createScene();

            /** */
            virtual bool createGraphicsSystem();

	        virtual void createDefaultFont();
            virtual void createRenderWindow();

            virtual void createGraphicsSceneManager();

            //
            virtual void loadGraphicsResources();

            virtual void setupRenderpipeline();

            /** */
            virtual void createPhysics();

            /** */
            virtual void createInputSystem();

            /** Called to create graphics system camera. */
            virtual void createCamera();

            /** */
            virtual void createViewports();

            /** */
            virtual bool createScriptManager();

            /** */
            virtual bool createEntitySystem();

            /** */
            virtual bool createSoundManager();

            /** */
            virtual bool createCameraCtrlManager();

            virtual void createPluginManager();
            virtual void createPlugins();

            // Optional to override this
            virtual void destroyScene();

            void loadScripts();

            virtual IFSM::ReturnType handleEvent( u32 state, IFSM::Event eventType );

            size_t m_activeThreads = 4;

            SmartPtr<IFSM> m_fsm;

            SmartPtr<render::IWindow> m_window;
            SmartPtr<render::IGraphicsScene> m_sceneMgr;
            SmartPtr<render::ICamera> m_camera;
            SmartPtr<render::ISceneNode> m_cameraSceneNode;
            SmartPtr<render::IViewport> m_viewport;

            SmartPtr<IFrameStatistics> m_frameStatistics;
        };
    }  // namespace application
}  // namespace fb

#endif
