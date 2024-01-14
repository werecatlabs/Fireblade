#ifndef _CGraphicsSystemOgreNext_H
#define _CGraphicsSystemOgreNext_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Graphics/GraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/ConcurrentArray.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <FBCore/Core/HashMap.h>
#include <FBCore/Core/Set.h>

namespace fb
{
    namespace render
    {
       
        /** Graphics system implementation to wrap ogre next functionality. */
        class CGraphicsSystemOgreNext : public GraphicsSystem
        {
        public:
            CGraphicsSystemOgreNext();
            CGraphicsSystemOgreNext( const CGraphicsSystemOgreNext &other ) = delete;
            ~CGraphicsSystemOgreNext() override;

            /** @copydoc IGraphicsSystem::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsSystem::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IGraphicsSystem::configure */
            bool configure( SmartPtr<IGraphicsSettings> config ) override;

            /** @copydoc IGraphicsSystem::addSceneManager */
            SmartPtr<IGraphicsScene> addGraphicsScene( const String &type, const String &name ) override;

            /** @copydoc IGraphicsSystem::getSceneManager */
            SmartPtr<IGraphicsScene> getGraphicsScene() const override;

            /** @copydoc IGraphicsSystem::getSceneManager */
            SmartPtr<IGraphicsScene> getGraphicsScene( const String &name ) const override;

            /** @copydoc IGraphicsSystem::getSceneManagerById */
            SmartPtr<IGraphicsScene> getGraphicsSceneById( hash32 id ) const override;

            /** @copydoc IGraphicsSystem::getOverlayManager */
            SmartPtr<IOverlayManager> getOverlayManager() const override;

            /** @copydoc IGraphicsSystem::getCompositorManager */
            SmartPtr<CompositorManager> getCompositorManager() const;

            void setCompositorManager( SmartPtr<CompositorManager> compositorManager);

            /** @copydoc IGraphicsSystem::getResourceGroupManager */
            SmartPtr<IResourceGroupManager> getResourceGroupManager() const override;

            /** @copydoc IGraphicsSystem::getMaterialManager */
            SmartPtr<IMaterialManager> getMaterialManager() const override;

            /** @copydoc IGraphicsSystem::getTextureManager */
            SmartPtr<ITextureManager> getTextureManager() const override;

            /** @copydoc IGraphicsSystem::getInstanceManager */
            SmartPtr<IInstanceManager> getInstanceManager() const override;

            /** @copydoc IGraphicsSystem::createRenderWindow */
            SmartPtr<IWindow> createRenderWindow( const String &name, u32 width, u32 height,
                                                  bool fullScreen,
                                                  const SmartPtr<Properties> &properties ) override;

            /** @copydoc IGraphicsSystem::configure */
            SmartPtr<IWindow> getRenderWindow(
                const String &name = StringUtil::EmptyString ) const override;

            static s32 getLoadPriority( SmartPtr<ISharedObject> obj );

            /** @copydoc IGraphicsSystem::configure */
            void update() override;

            /** Gets the deferred shading system. */
            SmartPtr<IDeferredShadingSystem> &getDeferredShadingSystem();

            /** Gets the deferred shading system. */
            const SmartPtr<IDeferredShadingSystem> &getDeferredShadingSystem() const;

            /** Sets the deferred shading system. */
            void setDeferredShadingSystem( SmartPtr<IDeferredShadingSystem> deferredShadingSystem );

            /** @copydoc IGraphicsSystem::configure */
            bool isDeferredShadingSystemEnabled() const;

            /** @copydoc IGraphicsSystem::configure */
            void addDeferredShadingSystem( bool enabled, SmartPtr<IViewport> vp );

            /** @copydoc IGraphicsSystem::configure */
            SmartPtr<IDeferredShadingSystem> addDeferredShadingSystem( SmartPtr<IViewport> vp ) override;

            /** @copydoc IGraphicsSystem::configure */
            void removeDeferredShadingSystem( SmartPtr<IViewport> vp ) override;

            /** @copydoc IGraphicsSystem::configure */
            Array<SmartPtr<IDeferredShadingSystem>> getDeferredShadingSystems() const override;

            /** @copydoc IGraphicsSystem::configure */
            void restoreConfig();

            /** @copydoc IGraphicsSystem::configure */
            void saveConfig();

            /** @copydoc IGraphicsSystem::configure */
            SmartPtr<IWindow> getDefaultWindow() const override;

            /** @copydoc IGraphicsSystem::setDefaultWindow */
            void setDefaultWindow( SmartPtr<IWindow> val ) override;

            /** @copydoc IGraphicsSystem::v */
            Array<SmartPtr<IWindow>> getWindows() const override;

            /** @copydoc IGraphicsSystem::configure */
            void createDebugTextOverlay( void );

            /** @copydoc IGraphicsSystem::generateDebugText */
            void generateDebugText( f32 timeSinceLast, Ogre::String &outText );

            /** @copydoc IGraphicsSystem::messagePump */
            void messagePump() override;

            /** @copydoc IGraphicsSystem::getSpriteRenderer */
            SmartPtr<ISpriteRenderer> getSpriteRenderer() const override;

            /** @copydoc IGraphicsSystem::setupRenderer */
            void setupRenderer( SmartPtr<IGraphicsScene> sceneManager, SmartPtr<IWindow> window,
                                SmartPtr<ICamera> camera, String workspaceName, bool val ) override;

            /** @copydoc IGraphicsSystem::getStateTask */
            Thread::Task getStateTask() const override;

            /** @copydoc IGraphicsSystem::getRenderTask */
            Thread::Task getRenderTask() const override;

            /** @copydoc IGraphicsSystem::loadObject */
            void loadObject( SmartPtr<ISharedObject> graphicsObject, bool forceQueue = false ) override;

            /** @copydoc IGraphicsSystem::unloadObject */
            void unloadObject( SmartPtr<ISharedObject> graphicsObject,
                               bool forceQueue = false ) override;

            SmartPtr<IGraphicsSettings> createConfiguration() override;

            SmartPtr<IDebug>& getDebug() override;
            const SmartPtr<IDebug>& getDebug() const override;
            void setDebug( SmartPtr<IDebug> debug ) override;

            RenderApi getRenderApi() const ;
            void setRenderApi( RenderApi renderApi ) ;

            SmartPtr<IMeshConverter> getMeshConverter() const;

            void setMeshConverter( SmartPtr<IMeshConverter> meshConverter );

            bool isValid() const override;

            bool getUseRTSS() const;
            void setUseRTSS( bool val );

            Ogre::CompositorWorkspace *getCompositorWorkspace() const;
            void setCompositorWorkspace( Ogre::CompositorWorkspace *val );

            Ogre::Terra *getTerra() const;
            void setTerra( Ogre::Terra *val );

            Ogre::v1::OverlaySystem *getOverlaySystem() const;
            void setOverlaySystem( Ogre::v1::OverlaySystem *val );

            SmartPtr<IFontManager> getFontManager() const override;

            void setFontManager(SmartPtr<IFontManager> fontManager);

            void lock() override;

            void unlock() override;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            //! Internal winProc (RenderWindow's use this when creating the Win32 Window)
            static LRESULT CALLBACK _WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE && !defined __OBJC__ && !defined( __LP64__ )
            //! Internal UPP Window Handler (RenderWindow's use this when creating the OS X Carbon Window
            static OSStatus _CarbonWindowHandler( EventHandlerCallRef nextHandler, EventRef event,
                                                  void *wnd );
#endif

            FB_CLASS_REGISTER_DECL;

        protected:
            class AppFrameListener : public Ogre::FrameListener
            {
            public:
                AppFrameListener( CGraphicsSystemOgreNext *graphicsSystem );
                ~AppFrameListener() override;

                bool frameEnded( const Ogre::FrameEvent &evt ) override;
                bool frameStarted( const Ogre::FrameEvent &evt ) override;
                bool frameRenderingQueued( const Ogre::FrameEvent &evt ) override;

            protected:
                CGraphicsSystemOgreNext *m_graphicsSystem;
                float m_time;
            };

            class WindowEventListener : public Ogre::WindowEventListener
            {
            public:
                WindowEventListener();
                ~WindowEventListener() override;

                bool windowClosing( Ogre::RenderWindow *rw );
            };

            bool isUpdating() const;
            void setUpdating( bool val );

            bool chooseRenderSystem();

            /** */
            void setRenderSystemDefaults( Ogre::RenderSystem *renderSystem );

            SmartPtr<IWindow> initialise( bool autoCreateWindow, const String &windowTitle,
                                          const String &customCapabilitiesConfig );

            void installPlugins( Ogre::Root *root );

            
            SmartPtr<IFontManager> m_fontManager;

            RenderApi m_renderApi = RenderApi::None;

            ConcurrentQueue<SmartPtr<ISharedObject>> m_loadQueue;
            ConcurrentQueue<SmartPtr<ISharedObject>> m_unloadQueue;

            ConcurrentArray<SmartPtr<ISharedObject>> m_graphicsObjects;

            SmartPtr<IGraphicsScene> m_sceneManager;

            SmartPtr<IDebug> m_debug;

            ///
            AtomicSmartPtr<IOverlayManager> m_overlayMgr;

            ///
            AtomicSmartPtr<CompositorManager> m_compositorManager;

            ///
            AtomicSmartPtr<IResourceGroupManager> m_resGrpMgr;

            ///
            AtomicSmartPtr<IMaterialManager> m_materialManager;

            ///
            AtomicSmartPtr<ITextureManager> m_textureManager;

            ///
            AtomicSmartPtr<IInstanceManager> m_instanceManager;

            AtomicSmartPtr<IMeshConverter> m_meshConverter;

            ///
            SmartPtr<IMeshManager> m_meshManager;

            AtomicSmartPtr<IWindow> m_defaultWindow;

            ///
            Ogre::Root *m_root = nullptr;

            Ogre::v1::TextAreaOverlayElement *mDebugText = nullptr;
            Ogre::v1::TextAreaOverlayElement *mDebugTextShadow = nullptr;

            AppFrameListener *m_frameListener = nullptr;

            ResourceGroupHelper *m_resourceGroupHelper = nullptr;

            ///
            ResourceLoadingListener *m_resourceLoadingListener = nullptr;

            ///
            MaterialListener *m_materialListener = nullptr;

            Ogre::Plugin *m_particlePlugin = nullptr;

            Ogre::StaticPluginLoader *m_staticPluginLoader = nullptr;

            Ogre::v1::OverlaySystem *m_overlaySystem = nullptr;

#ifdef OGRE_STATIC_LIB
            unsigned int mDummy = 0;
#    if FB_BUILD_RENDERER_GL3PLUS
            Ogre::GL3PlusPlugin *mGL3PlusPlugin = nullptr;
#    endif
#    if FB_BUILD_RENDERER_GLES2
            Ogre::GLES2Plugin *mGLES2Plugin = nullptr;
#    endif
#    if FB_BUILD_RENDERER_DX11
            Ogre::D3D11Plugin *mD3D11PlusPlugin = nullptr;
#    endif
#    if FB_BUILD_RENDERER_METAL
            Ogre::MetalPlugin *mMetalPlugin = nullptr;
#    endif
#endif

            Ogre::Terra *mTerra = nullptr;

            ///
            /// Factories
            ///
            CellSceneManagerFactory *m_cellSceneManagerFactory = nullptr;

            ///
            Array<SmartPtr<IWindow>> m_windows;

            ///
            Array<SmartPtr<IDeferredShadingSystem>> m_deferredShadingSystems;
            
            ///
            Array<SmartPtr<IGraphicsScene>> m_sceneManagers;

            Array<Ogre::Window *> _msWindows;

            Ogre::String mPluginsFolder;
            Ogre::String mWriteAccessFolder;
            Ogre::String mResourcePath;

            bool mAlwaysAskForConfig = false;
            bool mUseHlmsDiskCache = false;
            bool mUseMicrocodeCache = false;

            atomic_bool m_isUpdating = false;
        };
    }  // end namespace render
}  // end namespace fb

#endif
