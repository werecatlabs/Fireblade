#ifndef _CGraphicsSystem_H
#define _CGraphicsSystem_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBGraphics/Wrapper/CGraphicsSystem.h>
#include <FBCore/Interface/Graphics/IGraphicsSystem.h>
#include <FBCore/Memory/CSharedObject.h>
#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>
#include <OgreRenderQueueListener.h>
#include <OgreRTShaderSystem.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/ConcurrentArray.h>
#include <FBCore/Base/ConcurrentQueue.h>
#include <FBCore/Base/HashMap.h>
#include <FBCore/Base/Set.h>
#include <OgreBuildSettings.h>
#include <OgreComponents.h>
#include <FBCore/State/States/BaseStateT.h>

#define OGRE_USE_GLES2

#ifdef OGRE_STATIC_LIB
#    define OGRE_STATIC_GL
#    if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//#    define OGRE_STATIC_Direct3D9
// dx10 will only work on vista, so be careful about statically linking
#        if OGRE_USE_D3D10
#            define OGRE_STATIC_Direct3D10
#        endif
#    endif
#    define OGRE_STATIC_CgProgramManager
#    ifdef OGRE_USE_PCZ
#        define OGRE_STATIC_PCZSceneManager
#        define OGRE_STATIC_OctreeZone
#    endif
#    if OGRE_VERSION >= 0x10800
#        if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#            define OGRE_IS_IOS 1
#        endif
#    else
#        if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#            define OGRE_IS_IOS 1
#        endif
#    endif
#    ifdef OGRE_IS_IOS
#        undef OGRE_STATIC_CgProgramManager
#        undef OGRE_STATIC_GL
//#    define OGRE_STATIC_GLES 1
#        ifdef OGRE_USE_GLES2
#            define OGRE_STATIC_GLES2 1
#            define INCLUDE_RTSHADER_SYSTEM
#            undef OGRE_STATIC_GLES
#        endif
#        ifdef __OBJC__
#            import <UIKit/UIKit.h>
#        endif
#    endif
#endif

namespace fb
{
    namespace render
    {

        /** Implements the graphics system interface to use the ogre rendering engine. */
        class CGraphicsSystemOgre : public CGraphicsSystem
        {
        public:
            static const String rsDX12Name;
            static const String rsDX11Name;
            static const String rsGL3Name;
            static const String rsDX9Name;
            static const String rsGLName;
            static const String rsMetalName;
            static const String rsVulkanName;
            static const String rsNoneName;

            CGraphicsSystemOgre();
            ~CGraphicsSystemOgre() override;

            /** @copydoc  */
            virtual void handleStateChanged( const SmartPtr<IStateMessage> &message );

            /** @copydoc  */
            virtual void handleStateChanged( SmartPtr<IState> &state );

            /** @copydoc  */
            virtual void handleQuery( SmartPtr<IStateQuery> &query );

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

            void setGraphicsScene( SmartPtr<IGraphicsScene> smgr );

            /** @copydoc IGraphicsSystem::getSceneManager */
            SmartPtr<IGraphicsScene> getGraphicsScene( const String &name ) const override;

            /** @copydoc IGraphicsSystem::getSceneManagerById */
            SmartPtr<IGraphicsScene> getGraphicsSceneById( hash32 id ) const override;

            /** @copydoc IGraphicsSystem::getOverlayManager */
            SmartPtr<IOverlayManager> getOverlayManager() const override;

            /** @copydoc IGraphicsSystem::getCompositorManager */
            SmartPtr<CompositorManager> getCompositorManager() const;

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

            SmartPtr<IMeshManager> getMeshManager() const;

            void setMeshManager( SmartPtr<IMeshManager> meshManager );

            /** @copydoc IGraphicsSystem::configure */
            SmartPtr<IWindow> getDefaultWindow() const override;

            /** @copydoc IGraphicsSystem::setDefaultWindow */
            void setDefaultWindow( SmartPtr<IWindow> val ) override;

            /** @copydoc IGraphicsSystem::getWindows */
            Array<SmartPtr<IWindow>> getWindows() const override;

            /** @copydoc IGraphicsSystem::configure */
            void createDebugTextOverlay( void );

            /** @copydoc IGraphicsSystem::generateDebugText */
            void generateDebugText( f32 timeSinceLast, Ogre::String &outText );

            /** @copydoc IGraphicsSystem::messagePump */
            void messagePump() override;

            /** @copydoc IGraphicsSystem::getSpriteRenderer */
            SmartPtr<ISpriteRenderer> getSpriteRenderer() const override;

            /** @copydoc IGraphicsSystem::getFontManager */
            SmartPtr<IFontManager> getFontManager() const override;

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

            /** @copydoc IGraphicsSystem::unloadObject */
            SmartPtr<IGraphicsSettings> createConfiguration() override;

            /** @copydoc IGraphicsSystem::getDebug */
            SmartPtr<IDebug> getDebug() const override;

            /** @copydoc IGraphicsSystem::setDebug */
            void setDebug( SmartPtr<IDebug> debug ) override;

            /** @copydoc IGraphicsSystem::getRenderApi */
            RenderApi getRenderApi() const;

            /** @copydoc IGraphicsSystem::setRenderApi */
            void setRenderApi( RenderApi renderApi );

            SmartPtr<IMeshConverter> getMeshConverter() const override;

            void setMeshConverter( SmartPtr<IMeshConverter> meshConverter ) override;

            /** @copydoc IGraphicsSystem::isValid */
            bool isValid() const override;

            bool getUseRTSS() const;
            void setUseRTSS( bool useRTSS );

        protected:
            class AppFrameListener : public Ogre::FrameListener
            {
            public:
                AppFrameListener( CGraphicsSystemOgre *graphicsSystem );
                ~AppFrameListener() override;

                bool frameEnded( const Ogre::FrameEvent &evt ) override;
                bool frameStarted( const Ogre::FrameEvent &evt ) override;
                bool frameRenderingQueued( const Ogre::FrameEvent &evt ) override;

            protected:
                CGraphicsSystemOgre *m_graphicsSystem;
                float m_time;
            };

            class RenderQueueListener : public Ogre::RenderQueueListener
            {
            public:
                RenderQueueListener( CGraphicsSystemOgre *graphicsSystem );
                ~RenderQueueListener() override;

                void renderQueueStarted( Ogre::uint8 queueGroupId, const Ogre::String &invocation,
                                         bool &skipThisInvocation ) override;

                CGraphicsSystemOgre *m_graphicsSystem = nullptr;
            };

            class WindowEventListener : public Ogre::WindowEventListener
            {
            public:
                WindowEventListener();
                ~WindowEventListener() override;

                bool windowClosing( Ogre::RenderWindow *rw ) override;
            };

            bool isUpdating() const;
            void setUpdating( bool val );

            RenderApi getRenderApi( String renderPluginName );
            String getRenderPluginName( RenderApi renderApi );

            bool chooseRenderSystem();

            /** */
            void setRenderSystemDefaults( Ogre::RenderSystem *renderSystem );

            bool initialiseRTShaderSystem();

            SmartPtr<IFontManager> m_fontManager;

            RenderApi m_renderApi = RenderApi::None;

            ConcurrentQueue<SmartPtr<ISharedObject>> m_loadQueue;
            ConcurrentQueue<SmartPtr<ISharedObject>> m_unloadQueue;

            ConcurrentArray<SmartPtr<ISharedObject>> m_graphicsObjects;

            AtomicSmartPtr<IGraphicsScene> m_defaultSceneManager;

            AtomicSmartPtr<IDebug> m_debug;

            ///
            AtomicSmartPtr<IOverlayManager> m_overlayMgr;

            ///
            AtomicSmartPtr<CompositorManager> m_compositorManager;

            ///
            AtomicSmartPtr<IResourceGroupManager> m_resourceGroupManager;

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

            Ogre::RenderQueueListener *m_rqListener = nullptr;

            AppFrameListener *m_frameListener;

            ResourceGroupHelper *m_resourceGroupHelper = nullptr;

            ///
            ResourceLoadingListener *m_resourceLoadingListener = nullptr;

            ///
            MaterialListener *m_materialListener = nullptr;

            Ogre::Plugin *m_particlePlugin = nullptr;

            Ogre::StaticPluginLoader *m_staticPluginLoader = nullptr;

            Ogre::OverlaySystem *m_overlaySystem = nullptr;

#ifdef OGRE_STATIC_LIB
#    if FB_BUILD_RENDERER_OPENGL
            Ogre::GLPlugin *m_glPlugin = nullptr;
#    endif
#    if FB_BUILD_RENDERER_GL3PLUS
            Ogre::GL3PlusPlugin *m_gl3Plugin = nullptr;
#    endif
#    if defined OGRE_BUILD_RENDERSYSTEM_GLES2
            Ogre::GLES2Plugin *mGLES2Plugin = nullptr;
#    endif
#    if FB_BUILD_RENDERER_DX11
            Ogre::D3D11Plugin *m_d3d11Plugin = nullptr;
#    endif
#    if FB_BUILD_RENDERER_DX9
            Ogre::D3D9Plugin *m_d3d9Plugin = nullptr;
#    endif
#    if FB_BUILD_RENDERER_METAL
            Ogre::MetalPlugin *m_metalPlugin = nullptr;
#    endif
#endif

            ///
            /// Factories
            ///
            CellSceneManagerFactory *m_cellSceneManagerFactory = nullptr;
            BasicSceneManagerFactory *m_basicSceneManagerFactory = nullptr;

            Ogre::TerrainGlobalOptions *mTerrainGlobals = nullptr;

            Ogitors::OgitorsRoot *m_ogitorsRoot = nullptr;

            ///
            Array<SmartPtr<IDeferredShadingSystem>> m_deferredShadingSystems;

            ///
            Array<SmartPtr<IGraphicsScene>> m_sceneManagers;

            bool m_useRTSS = false;
            atomic_bool m_isUpdating = false;

#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
            Ogre::RTShader::ShaderGenerator *m_shaderGenerator =
                nullptr;  // The Shader generator instance.
            OgreBites::SGTechniqueResolverListener *m_materialMgrListener =
                nullptr;  // Shader generator material manager listener.
#endif                    // INCLUDE_RTSHADER_SYSTEM
        };
    }  // end namespace render
}  // end namespace fb

#endif
