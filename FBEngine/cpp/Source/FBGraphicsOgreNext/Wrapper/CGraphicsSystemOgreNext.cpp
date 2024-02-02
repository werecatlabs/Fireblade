#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsSystemOgreNext.h>
#include <FBGraphicsOgreNext/CompositorManager.h>
#include <FBGraphicsOgreNext/Compositor.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/COverlayManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CResourceGroupManager.h>
#include <FBGraphicsOgreNext/Wrapper/CMaterialManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CWindowOgreNext.h>
#include <FBGraphicsOgreNext/ResourceGroupHelper.h>
#include <FBGraphicsOgreNext/ResourceLoadingListener.h>
#include <FBGraphicsOgreNext/FBGraphicsOgreNextTypes.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CWindowOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsSettings.h>
#include <FBGraphicsOgreNext/Wrapper/CDebugOgreNext.h>
#include <FBGraphicsOgreNext/MaterialListener.h>
#include <FBCore/FBCore.h>
#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreItem.h>
#include <OgreHlmsUnlit.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsManager.h>
#include <OgreArchiveManager.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreTextureGpuManager.h>
#include <OgreWindowEventUtilities.h>
#include <OgreWindow.h>
#include <OgreFileSystemLayer.h>
#include <OgreHlmsDiskCache.h>
#include <OgreGpuProgramManager.h>
#include <OgreLogManager.h>
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <OgreAbiUtils.h>

#ifdef OGRE_STATIC_LIB
#    if FB_BUILD_RENDERER_GL3PLUS
#        include <OgreGL3PlusPlugin.h>
#    endif
#    ifdef FB_BUILD_RENDERSYSTEM_GLES2
#        include "OgreGLES2Plugin.h"
#    endif
#    if FB_BUILD_RENDERER_DX11
#        include <OgreD3D11Plugin.h>
#    endif
#    if FB_BUILD_RENDERER_METAL
#        include <OgreMetalPlugin.h>
#    endif
#endif

#if FB_OGRE_USE_PARTICLE_UNIVERSE
#    include <ParticleUniversePlugin.h>
#endif

#include <OgreHlmsBufferManager.h>
#include <OgreMemoryAllocatorConfig.h>
#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFrameStats.h>
#include <Terra/Terra.h>
#include <Terra/TerraShadowMapper.h>
#include <Terra/Hlms/OgreHlmsTerra.h>
#include <OgreWindowEventUtilities.h>

#include "UIManager.h"
#include "FBGraphics/MeshConverter.h"
#include "Wrapper/CFontManagerOgreNext.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_FREEBSD
#    include <xcb/xcb.h>
#    include <X11/Xlib.h>
extern void GLXProc( Ogre::Window *win, const XEvent &event );
extern void XcbProc( xcb_connection_t *xcbConnection, xcb_generic_event_t *event );

typedef Ogre::map<xcb_window_t, Ogre::Window *>::type XcbWindowMap;
static XcbWindowMap gXcbWindowToOgre;
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
//#    include "OSX/macUtils.h"
#    include <FBGraphicsOgreNext/Window/Apple/macOS/macUtil.h>
#endif

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, CGraphicsSystemOgreNext, GraphicsSystem );

    class RenderSystemListener : public Ogre::RenderSystem::Listener
    {
    public:
        virtual void eventOccurred( const String &eventName,
                                    const NameValuePairList *parameters = nullptr )
        {
        }
    };

    CGraphicsSystemOgreNext::CGraphicsSystemOgreNext()
    {
        auto handle = getHandle();
        handle->setName( "CGraphicsSystemOgreNext" );
    }

    CGraphicsSystemOgreNext::~CGraphicsSystemOgreNext()
    {
        unload( nullptr );

        m_loadQueue.clear();
        m_unloadQueue.clear();
    }

    void CGraphicsSystemOgreNext::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            setLoadingState( LoadingState::Loading );

            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );
            FB_ASSERT( applicationManager->isValid() );

            auto factoryManager = applicationManager->getFactoryManager();

            m_eventListener = fb::make_ptr<GraphicsSystemEventListener>();
            applicationManager->addObjectListener( m_eventListener );

            auto mediaPath = applicationManager->getMediaPath();
            applicationManager->setRenderMediaPath( mediaPath + "/OgreNext" );

#ifdef FB_PLATFORM_WIN32
            Ogre::String resourcePath = "";
            Ogre::String pluginsPath;
            // only use plugins.cfg if not static
#    ifndef OGRE_STATIC_LIB
            pluginsPath = mResourcePath + "plugins.cfg";
#    endif

#    if 0
                Ogre::AbiCookie libCookie = Ogre::generateAbiCookie();
                m_root = new Ogre::Root( &libCookie, pluginsPath, mResourcePath + "ogre.cfg",
                                         mResourcePath + "Ogre.log" );
#    else
            m_root =
                new Ogre::Root( pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log" );
#    endif

#elif defined( FB_PLATFORM_APPLE )
            Ogre::String mResourcePath = "";
            Ogre::String pluginsPath;
            // only use plugins.cfg if not static
#    ifndef OGRE_STATIC_LIB
            pluginsPath = mResourcePath + "plugins.cfg";
#    endif

            m_root =
                new Ogre::Root( pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log" );
#elif defined( FB_PLATFORM_IOS )
            Ogre::String pluginsPath;
            // only use plugins.cfg if not static
#    ifndef OGRE_STATIC_LIB
            pluginsPath = m_ResourcePath + "plugins.cfg";
#    endif

            m_root =
                new Ogre::Root( pluginsPath, Ogre::String( "/ogre.cfg" ), Ogre::String( "Ogre.log" ) );
#else
            Ogre::String pluginsPath;
            // only use plugins.cfg if not static
#    ifndef OGRE_STATIC_LIB
            pluginsPath = m_ResourcePath + "plugins.cfg";
#    endif

            m_root =
                new Ogre::Root( pluginsPath, Ogre::String( "/ogre.cfg" ), Ogre::String( "Ogre.log" ) );
#endif

            // m_overlaySystem = new Ogre::OverlaySystem;

            FB_ASSERT( applicationManager->isValid() );

            m_overlayMgr = fb::make_ptr<COverlayManagerOgreNext>();

            FB_ASSERT( applicationManager->isValid() );

            m_resGrpMgr = fb::make_ptr<CResourceGroupManager>();
            FB_ASSERT( applicationManager->isValid() );

            m_materialManager = fb::make_ptr<CMaterialManagerOgreNext>();
            FB_ASSERT( applicationManager->isValid() );

            m_textureManager = fb::make_ptr<CTextureManagerOgreNext>();
            FB_ASSERT( applicationManager->isValid() );

            m_meshManager = fb::make_ptr<MeshManager>();
            m_fontManager = fb::make_ptr<CFontManagerOgreNext>();

            FB_ASSERT( applicationManager->isValid() );

            auto compositorManager = fb::make_ptr<CompositorManager>();

            m_compositorManager = compositorManager;

            // m_resourceGroupHelper = new ResourceGroupHelper;

            FB_ASSERT( applicationManager->isValid() );

            m_resourceLoadingListener = new ResourceLoadingListener;

            auto meshConverter = fb::make_ptr<MeshConverter>();
            setMeshConverter( meshConverter );

            auto ogreResGrpMgr = Ogre::ResourceGroupManager::getSingletonPtr();
            FB_ASSERT( ogreResGrpMgr );

            ogreResGrpMgr->setLoadingListener( m_resourceLoadingListener );

            FB_ASSERT( applicationManager->isValid() );

            m_frameListener = new AppFrameListener( this );
            m_root->addFrameListener( m_frameListener );

            FB_ASSERT( applicationManager->isValid() );

#if FB_OGRE_USE_PARTICLE_UNIVERSE
            m_particlePlugin = new ParticleUniverse::ParticleUniversePlugin;
#endif

            // m_cellSceneManagerFactory = new CellSceneManagerFactory;
            // m_root->addSceneManagerFactory(m_cellSceneManagerFactory);

            // BasicSceneManagerFactory* m_basicSceneManagerFactory = new BasicSceneManagerFactory;
            // m_root->addSceneManagerFactory(m_basicSceneManagerFactory);

            FB_ASSERT( applicationManager->isValid() );

            auto debug = fb::make_ptr<CDebugOgreNext>();
            setDebug( debug );

            auto renderUI = factoryManager->make_object<ui::UIManager>();
            if( renderUI )
            {
                applicationManager->setRenderUI( renderUI );
                loadObject( renderUI );
            }
            else
            {
                FB_LOG( "Failed to create renderUI" );
            }

            FB_ASSERT( applicationManager->isValid() );

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CGraphicsSystemOgreNext::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            FB_ASSERT( isValid() );

            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();

                while( !m_loadQueue.empty() )
                {
                    SmartPtr<ISharedObject> obj;
                    if( m_loadQueue.try_pop( obj ) )
                    {
                        obj = nullptr;
                    }
                }

                while( !m_unloadQueue.empty() )
                {
                    SmartPtr<ISharedObject> obj;
                    if( m_unloadQueue.try_pop( obj ) )
                    {
                        obj->unload( nullptr );
                    }
                }

                m_loadQueue.clear();
                m_unloadQueue.clear();

                FB_SAFE_DELETE( m_resourceGroupHelper );

                if( auto debug = getDebug() )
                {
                    debug->unload( nullptr );
                    setDebug( nullptr );
                }

                if( auto overlayManager = getOverlayManager() )
                {
                    overlayManager->unload( nullptr );
                    m_overlayMgr = nullptr;
                }

                FB_SAFE_DELETE( m_overlaySystem );

                if( auto fontManager = getFontManager() )
                {
                    fontManager->unload( nullptr );
                    setFontManager( nullptr );
                }

                for( auto window : m_windows )
                {
                    window->unload( nullptr );
                }

                m_windows.clear();
                m_defaultWindow = nullptr;

                for( auto sceneManager : m_sceneManagers )
                {
                    sceneManager->clearScene();
                    sceneManager->unload( nullptr );
                }

                m_sceneManagers.clear();

                if( auto compositorManager = getCompositorManager() )
                {
                    compositorManager->unload( nullptr );
                    setCompositorManager( nullptr );
                }

                if( auto materialManager = getMaterialManager() )
                {
                    materialManager->unload( nullptr );
                    m_materialManager = nullptr;
                }

                if( auto resourceGroupManager = getResourceGroupManager() )
                {
                    resourceGroupManager->unload( nullptr );
                    m_resGrpMgr = nullptr;
                }

                // auto renderSystem = m_root->getRenderSystem();
                // auto textureGpuManager = renderSystem->getTextureGpuManager();
                // if (textureGpuManager)
                //{
                //	textureGpuManager->shutdown();
                // }

                auto ogreResourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
                if( ogreResourceGroupManager )
                {
                    ogreResourceGroupManager->shutdownAll();
                }

                // m_root->removeSceneManagerFactory(m_cellSceneManagerFactory);

                //                    if( m_root )
                //                    {
                //#if FB_BUILD_RENDERER_GL3PLUS
                //                        if( mGL3PlusPlugin )
                //                        {
                //                            m_root->uninstallPlugin( mGL3PlusPlugin );
                //                            mGL3PlusPlugin = nullptr;
                //                        }
                //#endif
                //
                //#if FB_BUILD_RENDERER_GLES2
                //                        if( mGLES2Plugin )
                //                        {
                //                            m_root->uninstallPlugin( mGLES2Plugin );
                //                            mGLES2Plugin = nullptr;
                //                        }
                //#endif
                //
                //#if FB_BUILD_RENDERER_DX11
                //                        if( mD3D11PlusPlugin )
                //                        {
                //                            m_root->uninstallPlugin( mD3D11PlusPlugin );
                //                            mD3D11PlusPlugin = nullptr;
                //                        }
                //#endif
                //
                //#if FB_BUILD_RENDERER_METAL
                //                        if( mMetalPlugin )
                //                        {
                //                            m_root->uninstallPlugin( mMetalPlugin );
                //                            mMetalPlugin = nullptr;
                //                        }
                //#endif
                //                    }
                //
                //                    if( m_particlePlugin )
                //                    {
                //                        // m_particlePlugin->uninstall();
                //                        // m_particlePlugin->shutdown();
                //                        //FB_SAFE_DELETE( m_particlePlugin );
                //                    }

                // FB_SAFE_DELETE(m_cellSceneManagerFactory);

                FB_SAFE_DELETE( m_root );
                FB_SAFE_DELETE( m_resourceLoadingListener );
                FB_SAFE_DELETE( m_frameListener );

                if( m_eventListener )
                {
                    applicationManager->removeObjectListener( m_eventListener );
                    m_eventListener = nullptr;
                }

                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CGraphicsSystemOgreNext::isUpdating() const -> bool
    {
        return m_isUpdating;
    }

    void CGraphicsSystemOgreNext::setUpdating( bool val )
    {
        m_isUpdating = val;
    }

    auto CGraphicsSystemOgreNext::chooseRenderSystem() -> bool
    {
        try
        {
            Ogre::RenderSystem *selectedRenderSystem = nullptr;

            // supported render system plugin names in Ogre
            const auto rsDX11Name = String( "Direct3D11 Rendering Subsystem" );
            const auto rsGL3Name = String( "OpenGL 3+ Rendering Subsystem" );
            const auto rsDX9Name = String( "Direct3D9 Rendering Subsystem" );
            const auto rsGLName = String( "OpenGL Rendering Subsystem" );

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            auto rsDX11 = m_root->getRenderSystemByName( rsDX11Name );
            auto rsGL3 = m_root->getRenderSystemByName( rsGL3Name );
            auto rsDX9 = m_root->getRenderSystemByName( rsDX9Name );
            auto rsGL = m_root->getRenderSystemByName( rsGLName );

            if( rsDX11 )
            {
                selectedRenderSystem = rsDX11;
            }
            else if( rsGL3 )
            {
                selectedRenderSystem = rsGL3;
            }
            else if( rsDX9 )
            {
                selectedRenderSystem = rsDX9;
            }
            else if( rsGL )
            {
                selectedRenderSystem = rsGL;
            }
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            auto renderers = m_root->getAvailableRenderers();
            for( auto renderer : renderers )
            {
                auto name = renderer->getName();
                static const auto preferred = String( "Metal" );
                if( name.find( preferred ) != String::npos )
                {
                    selectedRenderSystem = renderer;
                    break;
                }
            }

            if( !selectedRenderSystem )
            {
                if( !renderers.empty() )
                {
                    selectedRenderSystem = renderers[0];
                }
            }
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            auto renderers = m_root->getAvailableRenderers();
            for( auto renderer : renderers )
            {
                auto name = renderer->getName();
                static const auto preferred = String( "Metal" );
                if( name.find( preferred ) != String::npos )
                {
                    selectedRenderSystem = renderer;
                    break;
                }
            }

            if( !selectedRenderSystem )
            {
                if( !renderers.empty() )
                {
                    selectedRenderSystem = renderers[0];
                }
            }
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            auto renderers = m_root->getAvailableRenderers();
            for( auto renderer : renderers )
            {
                auto name = renderer->getName();
                static const auto preferred = String( "GL" );
                if( name.find( preferred ) != String::npos )
                {
                    selectedRenderSystem = renderer;
                    break;
                }
            }

            if( !selectedRenderSystem )
            {
                if( !renderers.empty() )
                {
                    selectedRenderSystem = renderers[0];
                }
            }
#else
#    pragma error "Unsupported platform!"
#endif

            if( selectedRenderSystem )
            {
                m_root->setRenderSystem( selectedRenderSystem );
            }
            else
            {
                // no supported render system found
                const String msg( "No supported render system found. Program will now exit." );
                MessageBoxUtil::show( msg.c_str() );
                return false;
            }

            // Pick first rendering device
            auto &configOptions = selectedRenderSystem->getConfigOptions();
            auto deviceIterator = configOptions.find( "Rendering Device" );
            if( deviceIterator != configOptions.end() )
            {
                const auto &deviceNames = deviceIterator->second.possibleValues;
                if( !deviceNames.empty() )
                {
                    const Ogre::String &deviceName = deviceNames.at( 0 );
                    selectedRenderSystem->setConfigOption( deviceIterator->first, deviceName );
                }
                else
                {
                    const String msg( "No render devices found. Exiting program." );
                    MessageBoxUtil::show( msg.c_str() );
                    return false;
                }
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return true;
    }

    void CGraphicsSystemOgreNext::setRenderSystemDefaults( Ogre::RenderSystem *renderSystem )
    {
        if( !renderSystem )
        {
            MessageBoxUtil::show( "No render system" );
            return;
        }

        // int width = 800;//GetSystemMetrics(SM_CXSCREEN);
        // int height = 800;//GetSystemMetrics(SM_CYSCREEN);

        // choose suitable resolution
        Vector2I resolution( 1280, 720 );  // hack
        renderSystem->setConfigOption(
            "Video Mode", Ogre::StringConverter::toString( resolution.X() ) + Ogre::String( " x " ) +
                              Ogre::StringConverter::toString( resolution.Y() ) + " @ 32-bit colour" );

        renderSystem->setConfigOption( Ogre::String( "Full Screen" ), Ogre::String( "No" ) );

        // set anti alias defaults
        // renderSystem->setConfigOption(String("Anti aliasing"), String("None"));*/
    }

    auto CGraphicsSystemOgreNext::configure( SmartPtr<IGraphicsSettings> config ) -> bool
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( m_root );

        bool showDialog = false;
        bool applyDefaults = true;
        bool createWindow = true;

        if( config )
        {
            showDialog = config->getShowDialog();
            applyDefaults = true;
            createWindow = config->getCreateWindow();
        }

        installPlugins( m_root );

        // Show the configuration dialog and initialise the system
        // You can skip this and use root.restoreConfig() to load configuration
        // settings if you were sure there are valid ones saved in ogre.cfg
        /*
        if (m_root->restoreConfig())
        {
            //validate config options
            auto renderSystem = m_root->getRenderSystem();
            if (renderSystem)
            {
                String errStr = renderSystem->validateConfigOptions();
                if (errStr.size() > 0)
                {
                    setRenderSystemDefaults(renderSystem);

                    //show config dialog
                    if (!m_root->showConfigDialog())
                    {
                        return false;
                    }
                }
            }
            else
            {
                //set defaults
                chooseRenderSystem();
                auto renderSystem = m_root->getRenderSystem();
                setRenderSystemDefaults(renderSystem);

                //show config dialog
                if (!m_root->showConfigDialog())
                {
                    return false;
                }
            }
        }
        else
            */
        {
            // set defaults
            chooseRenderSystem();
            auto renderSystem = m_root->getRenderSystem();
            setRenderSystemDefaults( renderSystem );

            // show config dialog
            if( showDialog && !m_root->showConfigDialog() )
            {
                return false;
            }
        }

#if 0
            auto ogreWindow = m_root->initialise( createWindow );
            if( ogreWindow )
            {
                auto window = fb::make_ptr<CWindowOgreNext>();

                auto windowHandle = window->getHandle();
                windowHandle->setName( "DefaultWindow" );

                window->initialise( ogreWindow );

                setDefaultWindow( window );
                m_windows.push_back( window );
            }
#else
        m_root->initialise( false );

        if( createWindow )
        {
            auto window = fb::make_ptr<CWindowOgreNext>();

            auto windowHandle = window->getHandle();
            windowHandle->setName( "DefaultWindow" );

            // window->initialise( ogreWindow );
            window->load( nullptr );

            setDefaultWindow( window );
            m_windows.emplace_back( window );
        }
#endif

        m_overlaySystem = OGRE_NEW Ogre::v1::OverlaySystem();

        auto debug = getDebug();
        if( debug )
        {
            debug->load( nullptr );
        }

        return true;
    }

    auto CGraphicsSystemOgreNext::initialise( bool autoCreateWindow, const String &windowTitle,
                                              const String &customCapabilitiesConfig )
        -> SmartPtr<IWindow>
    {
        FB_ASSERT( m_root );

        auto window = fb::make_ptr<CWindowOgreNext>();

        auto windowHandle = window->getHandle();
        windowHandle->setName( "DefaultWindow" );

        auto renderSystem = m_root->getRenderSystem();
        FB_ASSERT( renderSystem );

        auto ogreWindow = m_root->initialise( autoCreateWindow );
        window->initialise( ogreWindow );

        return window;
    }

    void CGraphicsSystemOgreNext::installPlugins( Ogre::Root *root )
    {
#ifdef OGRE_STATIC_LIB
#    ifdef OGRE_BUILD_RENDERSYSTEM_METAL
#        if FB_BUILD_RENDERER_METAL
        if( !mMetalPlugin )
        {
            mMetalPlugin = OGRE_NEW Ogre::MetalPlugin();
        }

        root->installPlugin( mMetalPlugin, nullptr );
#        endif
#    endif
#    ifdef OGRE_BUILD_RENDERSYSTEM_GL3PLUS
#        if FB_BUILD_RENDERER_GL3PLUS
        if( !mGL3PlusPlugin )
        {
            mGL3PlusPlugin = OGRE_NEW Ogre::GL3PlusPlugin();
        }

        root->installPlugin( mGL3PlusPlugin, nullptr );
#        endif
#    endif
#    ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
        if( !mGLES2Plugin )
            mGLES2Plugin = OGRE_NEW Ogre::GLES2Plugin();
        root->installPlugin( mGLES2Plugin );
#    endif
#    ifdef OGRE_BUILD_RENDERSYSTEM_D3D11
#        if FB_BUILD_RENDERER_DX11
        if( !mD3D11PlusPlugin )
        {
            mD3D11PlusPlugin = OGRE_NEW Ogre::D3D11Plugin();
        }

        root->installPlugin( mD3D11PlusPlugin, nullptr );
#        endif
#    endif

#endif
    }

    auto CGraphicsSystemOgreNext::getFontManager() const -> SmartPtr<IFontManager>
    {
        return m_fontManager;
    }

    void CGraphicsSystemOgreNext::setFontManager( SmartPtr<IFontManager> fontManager )
    {
        m_fontManager = fontManager;
    }

    void CGraphicsSystemOgreNext::lock()
    {
        m_mutex.lock();
    }

    void CGraphicsSystemOgreNext::unlock()
    {
        m_mutex.unlock();
    }

    auto CGraphicsSystemOgreNext::addGraphicsScene( const String &type, const String &name )
        -> SmartPtr<IGraphicsScene>
    {
        FB_ASSERT( !StringUtil::isNullOrEmpty( type ) );
        FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

        auto sceneManager = fb::make_ptr<CSceneManagerOgreNext>();
        sceneManager->setType( type );
        sceneManager->setName( name );

        auto defaultSceneManager = getGraphicsScene();
        if( !defaultSceneManager )
        {
            m_sceneManager = sceneManager;
        }

        m_sceneManagers.emplace_back( sceneManager );
        loadObject( sceneManager );

        return sceneManager;
    }

    auto CGraphicsSystemOgreNext::getGraphicsScene( const String &name ) const
        -> SmartPtr<IGraphicsScene>
    {
        for( auto sceneManager : m_sceneManagers )
        {
            if( sceneManager->getName() == name )
            {
                return sceneManager;
            }
        }

        return nullptr;
    }

    auto CGraphicsSystemOgreNext::getGraphicsScene() const -> SmartPtr<IGraphicsScene>
    {
        return m_sceneManager;
    }

    auto CGraphicsSystemOgreNext::getGraphicsSceneById( hash32 id ) const -> SmartPtr<IGraphicsScene>
    {
        for( auto sceneManager : m_sceneManagers )
        {
            auto name = sceneManager->getName();
            auto hash = StringUtil::getHash( name );
            if( hash == id )
            {
                return sceneManager;
            }
        }

        return nullptr;
    }

    auto CGraphicsSystemOgreNext::getOverlayManager() const -> SmartPtr<IOverlayManager>
    {
        return m_overlayMgr;
    }

    auto CGraphicsSystemOgreNext::getCompositorManager() const -> SmartPtr<CompositorManager>
    {
        return m_compositorManager;
    }

    void CGraphicsSystemOgreNext::setCompositorManager( SmartPtr<CompositorManager> compositorManager )
    {
        m_compositorManager = compositorManager;
    }

    auto CGraphicsSystemOgreNext::getResourceGroupManager() const -> SmartPtr<IResourceGroupManager>
    {
        return m_resGrpMgr;
    }

    auto CGraphicsSystemOgreNext::createRenderWindow( const String &name, u32 width, u32 height,
                                                      bool fullScreen,
                                                      const SmartPtr<Properties> &properties )
        -> SmartPtr<IWindow>
    {
        auto window = fb::make_ptr<CWindowOgreNext>();

        auto windowHandle = window->getHandle();
        windowHandle->setName( name );

        //window->setLabel( name );
        window->setSize( Vector2I( static_cast<f32>( width ), static_cast<f32>( height ) ) );
        window->setFullscreen( fullScreen );
        m_windows.emplace_back( window );

        String handle;
        if( properties->getPropertyValue( "WindowHandle", handle ) )
        {
            window->setWindowHandleAsString( handle );
        }

        auto defaultWindow = getDefaultWindow();
        if( defaultWindow == nullptr )
        {
            setDefaultWindow( window );
        }

        window->load( nullptr );
        // loadObject( window );

        return window;
    }

    auto CGraphicsSystemOgreNext::getRenderWindow( const String &name ) const -> SmartPtr<IWindow>
    {
        for( auto window : m_windows )
        {
            auto windowHandle = window->getHandle();
            if( windowHandle->getName() == name )
            {
                return window;
            }
        }

        return nullptr;
    }

    auto CGraphicsSystemOgreNext::getLoadPriority( SmartPtr<ISharedObject> obj ) -> s32
    {
        if( obj->isDerived<IWindow>() )
        {
            return 100000;
        }
        if( obj->isDerived<IResourceGroupManager>() )
        {
            return 95000;
        }
        if( obj->isDerived<CompositorManager>() )
        {
            return 94000;
        }
        if( obj->isDerived<Compositor>() )
        {
            return 93000;
        }
        if( obj->isDerived<IGraphicsScene>() )
        {
            return 90000;
        }
        if( obj->isDerived<ISceneNode>() )
        {
            return 85000;
        }
        if( obj->isDerived<ICamera>() )
        {
            return 80000;
        }
        if( obj->isDerived<IMaterial>() )
        {
            return 78000;
        }
        if( obj->isDerived<CompositorManager>() )
        {
            return 75000;
        }
        if( obj->isDerived<IGraphicsMesh>() )
        {
            return 70000;
        }
        if( obj->isDerived<ILight>() )
        {
            return 65000;
        }
        if( obj->isDerived<IOverlayElement>() )
        {
            return 1000;
        }
        if( obj->isDerived<ui::IUIManager>() )
        {
            return 1000;
        }

        return 0;
    }

    void CGraphicsSystemOgreNext::update()
    {
        try
        {
            RecursiveMutex::ScopedLock lock( m_mutex );

            if( isLoaded() )
            {
                setUpdating( true );

                if( auto compositorManager = getCompositorManager() )
                {
                    if( !compositorManager->isLoaded() )
                    {
                        compositorManager->load( nullptr );
                    }
                }

                FB_ASSERT( getResourceGroupManager() );
                FB_ASSERT( getResourceGroupManager() && getResourceGroupManager()->isLoaded() );

                auto applicationManager = core::ApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto timer = applicationManager->getTimer();
                FB_ASSERT( timer );

                auto dt = timer->getDeltaTime();
                FB_ASSERT( dt > 0.0 );
                FB_ASSERT( MathD::isFinite( dt ) );

                auto windows = getWindows();
                for( auto window : windows )
                {
                    window->update();
                }

                for( auto sceneManager : m_sceneManagers )
                {
                    if( sceneManager )
                    {
                        sceneManager->update();
                    }
                }

                if( auto overlayManager = getOverlayManager() )
                {
                    overlayManager->update();
                }

                if( auto ui = applicationManager->getRenderUI() )
                {
                    ui->update();
                }

                // if(m_deferredShadingSystem)
                //	m_deferredShadingSystem->update(task, t, dt);

                if( !m_loadQueue.empty() )
                {
                    auto loadArray = Array<SmartPtr<ISharedObject>>();
                    loadArray.reserve( 10 );

                    SmartPtr<ISharedObject> obj;
                    while( m_loadQueue.try_pop( obj ) )
                    {
                        loadArray.push_back( obj );
                    }

                    std::sort( loadArray.begin(), loadArray.end(),
                               []( const SmartPtr<ISharedObject> &a,
                                   const SmartPtr<ISharedObject> &b ) -> bool {
                                   auto aPriority = getLoadPriority( a );
                                   auto bPriority = getLoadPriority( b );

                                   return aPriority > bPriority;
                               } );

                    for( auto loadObj : loadArray )
                    {
                        if( !loadObj->isLoaded() )
                        {
                            loadObj->load( nullptr );
                        }
                    }
                }

                if( !m_unloadQueue.empty() )
                {
                    auto unloadArray = Array<SmartPtr<ISharedObject>>();
                    unloadArray.reserve( 10 );

                    SmartPtr<ISharedObject> obj;
                    while( m_unloadQueue.try_pop( obj ) )
                    {
                        unloadArray.push_back( obj );
                    }

                    for( auto loadObj : unloadArray )
                    {
                        loadObj->unload( nullptr );
                    }
                }

                // String text = "";
                // generateDebugText(dt, text);

                FB_ASSERT( getResourceGroupManager() );
                FB_ASSERT( getResourceGroupManager() && getResourceGroupManager()->isLoaded() );

                auto fDT = static_cast<f32>( dt );
                m_root->renderOneFrame( fDT );

                if( auto debug = getDebug() )
                {
                    debug->preUpdate();
                    debug->update();
                    debug->postUpdate();
                }

                for( auto sceneManager : m_sceneManagers )
                {
                    if( sceneManager )
                    {
                        sceneManager->postUpdate();
                    }
                }

                setUpdating( false );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    auto CGraphicsSystemOgreNext::getMaterialManager() const -> SmartPtr<IMaterialManager>
    {
        return m_materialManager;
    }

    auto CGraphicsSystemOgreNext::getTextureManager() const -> SmartPtr<ITextureManager>
    {
        return m_textureManager;
    }

    auto CGraphicsSystemOgreNext::getInstanceManager() const -> SmartPtr<IInstanceManager>
    {
        return m_instanceManager;
    }

    auto CGraphicsSystemOgreNext::isDeferredShadingSystemEnabled() const -> bool
    {
        return false;
    }

    void CGraphicsSystemOgreNext::addDeferredShadingSystem( bool enabled, SmartPtr<IViewport> vp )
    {
    }

    auto CGraphicsSystemOgreNext::addDeferredShadingSystem( SmartPtr<IViewport> vp )
        -> SmartPtr<IDeferredShadingSystem>
    {
        // SmartPtr<CDeferredShadingSystem> deferredShadingSystem(new CDeferredShadingSystem(vp));

        // m_deferredShadingSystems.push_back(deferredShadingSystem);
        // return deferredShadingSystem;

        return nullptr;
    }

    auto CGraphicsSystemOgreNext::getDefaultWindow() const -> SmartPtr<IWindow>
    {
        return m_defaultWindow;
    }

    void CGraphicsSystemOgreNext::setDefaultWindow( SmartPtr<IWindow> val )
    {
        m_defaultWindow = val;
    }

    void CGraphicsSystemOgreNext::messagePump()
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        // Windows Message Loop (NULL means check all HWNDs belonging to this context)
        MSG msg;
        while( PeekMessage( &msg, nullptr, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_FREEBSD
#    ifndef OGRE_CONFIG_UNIX_NO_X11
        // GLX Message Pump

        xcb_connection_t *xcbConnection = 0;

        if( !_msWindows.empty() )
            _msWindows.front()->getCustomAttribute( "xcb_connection_t", &xcbConnection );

        if( !xcbConnection )
        {
            // Uses the older Xlib
            auto win = _msWindows.begin();
            auto end = _msWindows.end();

            Display *xDisplay = 0;  // same for all windows

            for( ; win != end; win++ )
            {
                XID xid;
                XEvent event;

                if( !xDisplay )
                    ( *win )->getCustomAttribute( "XDISPLAY", &xDisplay );

                ( *win )->getCustomAttribute( "WINDOW", &xid );

                while( XCheckWindowEvent( xDisplay, xid,
                                          StructureNotifyMask | VisibilityChangeMask | FocusChangeMask,
                                          &event ) )
                {
                    // GLXProc( *win, event );
                }

                // The ClientMessage event does not appear under any Event Mask
                while( XCheckTypedWindowEvent( xDisplay, xid, ClientMessage, &event ) )
                {
                    // GLXProc( *win, event );
                }
            }
        }
        else
        {
            // Uses the newer xcb
            xcb_generic_event_t *nextEvent = 0;

            nextEvent = xcb_poll_for_event( xcbConnection );

            while( nextEvent )
            {
                // XcbProc( xcbConnection, nextEvent );
                free( nextEvent );
                nextEvent = xcb_poll_for_event( xcbConnection );
            }
        }
#    endif
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE && !defined __OBJC__ && !defined __LP64__
        // OSX Message Pump
        EventRef event = NULL;
        EventTargetRef targetWindow;
        targetWindow = GetEventDispatcherTarget();

        // If we are unable to get the target then we no longer care about events.
        if( !targetWindow )
            return;

        // Grab the next event, process it if it is a window event
        while( ReceiveNextEvent( 0, NULL, kEventDurationNoWait, true, &event ) == noErr )
        {
            // Dispatch the event
            SendEventToEventTarget( event, targetWindow );
            ReleaseEvent( event );
        }
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        mac_dispatchOneEvent();
#endif
    }

    void CGraphicsSystemOgreNext::removeDeferredShadingSystem( SmartPtr<IViewport> vp )
    {
    }

    auto CGraphicsSystemOgreNext::getDeferredShadingSystems() const
        -> Array<SmartPtr<IDeferredShadingSystem>>
    {
        return m_deferredShadingSystems;
    }

    auto CGraphicsSystemOgreNext::getSpriteRenderer() const -> SmartPtr<ISpriteRenderer>
    {
        return nullptr;
    }

    void CGraphicsSystemOgreNext::setupRenderer( SmartPtr<IGraphicsScene> sceneManager,
                                                 SmartPtr<IWindow> window, SmartPtr<ICamera> camera,
                                                 String workspaceName, bool val )
    {
        //auto compositorManager = getCompositorManager();
        //FB_ASSERT( compositorManager );

        //compositorManager->setSceneManager( sceneManager );
        //compositorManager->setWindow( window );
        //compositorManager->setCamera( camera );
        ////compositorManager->setWorkspaceName( workspaceName );
        ////compositorManager->setEnabled( val );
        //loadObject( compositorManager );
    }

    auto CGraphicsSystemOgreNext::getTerra() const -> Ogre::Terra *
    {
        return mTerra;
    }

    void CGraphicsSystemOgreNext::setTerra( Ogre::Terra *val )
    {
        mTerra = val;
    }

    auto CGraphicsSystemOgreNext::createConfiguration() -> SmartPtr<IGraphicsSettings>
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto factoryManager = applicationManager->getFactoryManager();
        FB_ASSERT( factoryManager );

        auto configuration = factoryManager->make_ptr<CGraphicsSettings>();
        FB_ASSERT( configuration );

        return configuration;
    }

    auto CGraphicsSystemOgreNext::getDebug() -> SmartPtr<IDebug> &
    {
        return m_debug;
    }

    auto CGraphicsSystemOgreNext::getDebug() const -> const SmartPtr<IDebug> &
    {
        return m_debug;
    }

    void CGraphicsSystemOgreNext::setDebug( SmartPtr<IDebug> debug )
    {
        m_debug = debug;
    }

    auto CGraphicsSystemOgreNext::getRenderApi() const -> IGraphicsSystem::RenderApi
    {
        return m_renderApi;
    }

    void CGraphicsSystemOgreNext::setRenderApi( RenderApi renderApi )
    {
        m_renderApi = renderApi;
    }

    auto CGraphicsSystemOgreNext::getMeshConverter() const -> SmartPtr<IMeshConverter>
    {
        return m_meshConverter;
    }

    void CGraphicsSystemOgreNext::loadObject( SmartPtr<ISharedObject> graphicsObject, bool forceQueue )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto taskManager = applicationManager->getTaskManager();
        auto renderTask = taskManager->getTask( Thread::Task::Render );

        if( forceQueue || renderTask->isExecuting() )
        {
            const auto &graphicsObjectLoadingState = graphicsObject->getLoadingState();
            if( !( graphicsObjectLoadingState == LoadingState::Loading ||
                   graphicsObjectLoadingState == LoadingState::Loaded ) )
            {
                m_loadQueue.push( graphicsObject );
            }
        }
        else
        {
            auto stateTask = getStateTask();
            auto renderTask = getRenderTask();
            auto task = Thread::getCurrentTask();

            if( task != stateTask )
            {
                const auto &graphicsObjectLoadingState = graphicsObject->getLoadingState();
                if( !( graphicsObjectLoadingState == LoadingState::Loading ||
                       graphicsObjectLoadingState == LoadingState::Loaded ) )
                {
                    m_loadQueue.push( graphicsObject );
                }
            }
            else
            {
                RecursiveMutex::ScopedLock lock( m_mutex );

                if( !graphicsObject->isLoaded() )
                {
                    graphicsObject->load( nullptr );
                }
            }
        }
    }

    void CGraphicsSystemOgreNext::unloadObject( SmartPtr<ISharedObject> graphicsObject, bool forceQueue )
    {
#if 0
        if( graphicsObject )
        {
            RecursiveMutex::ScopedLock lock( m_mutex );
            graphicsObject->unload( nullptr ); 
        }
#else
        FB_ASSERT( graphicsObject );

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded )
        {
            if( applicationManager->getQuit() )
            {
                graphicsObject->unload( nullptr );
            }
            else
            {
                auto stateTask = getStateTask();
                if( forceQueue || stateTask != Thread::Task::Render )
                {
                    const auto &loadingState = graphicsObject->getLoadingState();
                    if( loadingState != LoadingState::Unloaded )
                    {
                        m_unloadQueue.push( graphicsObject );
                    }
                }
                else
                {
                    graphicsObject->unload( nullptr );
                }
            }
        }
        else
        {
            graphicsObject->unload( nullptr );
        }
#endif
    }

    auto CGraphicsSystemOgreNext::getOverlaySystem() const -> Ogre::v1::OverlaySystem *
    {
        return m_overlaySystem;
    }

    void CGraphicsSystemOgreNext::setOverlaySystem( Ogre::v1::OverlaySystem *val )
    {
        m_overlaySystem = val;
    }

    void CGraphicsSystemOgreNext::setMeshConverter( SmartPtr<IMeshConverter> meshConverter )
    {
        m_meshConverter = meshConverter;
    }

    auto CGraphicsSystemOgreNext::isValid() const -> bool
    {
        const auto &loadingState = getLoadingState();
        if( loadingState == LoadingState::Loaded )
        {
            return m_root != nullptr;
        }

        return true;
    }

    auto CGraphicsSystemOgreNext::getStateTask() const -> Thread::Task
    {
        return Thread::Task::Render;
    }

    auto CGraphicsSystemOgreNext::getRenderTask() const -> Thread::Task
    {
        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        if( applicationManager->isLoading() )
        {
            return Thread::Task::Primary;
        }

        // auto threadPool = applicationManager->getThreadPool();

        // if (isUpdating())
        //{
        //	return Thread::getCurrentTask();
        // }

        auto taskManager = applicationManager->getTaskManager();
        if( taskManager )
        {
            auto task = taskManager->getTask( Thread::Task::Render );
            if( task )
            {
                if( task->isExecuting() )
                {
                    return Thread::Task::Render;
                }
                if( task->isPrimary() )
                {
                    return Thread::Task::Primary;
                }
            }
        }

        return applicationManager->hasTasks() ? Thread::Task::Render : Thread::Task::Primary;
    }

    auto CGraphicsSystemOgreNext::getWindows() const -> Array<SmartPtr<IWindow>>
    {
        return m_windows;
    }

    auto CGraphicsSystemOgreNext::AppFrameListener::frameStarted( const Ogre::FrameEvent &evt ) -> bool
    {
        //SmartPtr<IFrameEvent> event( new CFrameEvent );
        //event->setTime( m_time );
        //event->setTimeInterval( evt.timeSinceLastFrame );
        //event->setType( GraphicsOgreNextTypes::FRAME_EVENT_PRE_RENDER );

        //Array<SmartPtr<IFrameListener>> listeners = m_graphicsSystem->getFrameListeners();
        //for( u32 i = 0; i < listeners.size(); ++i )
        //{
        //    SmartPtr<IFrameListener> listener = listeners[i];
        //    listener->frameEvent( event );
        //}

        return true;
    }

    auto CGraphicsSystemOgreNext::AppFrameListener::frameRenderingQueued( const Ogre::FrameEvent &evt )
        -> bool
    {
        //m_time += evt.timeSinceLastFrame;

        //SmartPtr<IFrameEvent> event( new CFrameEvent );
        //event->setTime( m_time );
        //event->setTimeInterval( evt.timeSinceLastFrame );
        //event->setType( GraphicsOgreNextTypes::FRAME_EVENT_RENDER_QUEUED );

        //Array<SmartPtr<IFrameListener>> listeners = m_graphicsSystem->getFrameListeners();
        //for( u32 i = 0; i < listeners.size(); ++i )
        //{
        //    SmartPtr<IFrameListener> listener = listeners[i];
        //    listener->frameEvent( event );
        //}

        return true;
    }

    auto CGraphicsSystemOgreNext::AppFrameListener::frameEnded( const Ogre::FrameEvent &evt ) -> bool
    {
        //SmartPtr<IFrameEvent> event( new CFrameEvent );
        //event->setTime( m_time );
        //event->setTimeInterval( evt.timeSinceLastFrame );
        //event->setType( GraphicsOgreNextTypes::FRAME_EVENT_POST_RENDER );

        //Array<SmartPtr<IFrameListener>> listeners = m_graphicsSystem->getFrameListeners();
        //for( u32 i = 0; i < listeners.size(); ++i )
        //{
        //    SmartPtr<IFrameListener> listener = listeners[i];
        //    listener->frameEvent( event );
        //}

        return true;
    }

    CGraphicsSystemOgreNext::AppFrameListener::~AppFrameListener() = default;

    CGraphicsSystemOgreNext::AppFrameListener::AppFrameListener(
        CGraphicsSystemOgreNext *graphicsSystem ) :
        m_graphicsSystem( graphicsSystem ),
        m_time( 0.0f )
    {
    }

    void CGraphicsSystemOgreNext::createDebugTextOverlay()
    {
        // Ogre::v1::OverlayManager& overlayManager = Ogre::v1::OverlayManager::getSingleton();
        // Ogre::v1::Overlay* overlay = overlayManager.create("DebugText");

        // Ogre::v1::OverlayContainer* panel = static_cast<Ogre::v1::OverlayContainer*>(
        //	overlayManager.createOverlayElement("Panel", "DebugPanel"));
        // mDebugText = static_cast<Ogre::v1::TextAreaOverlayElement*>(
        //	overlayManager.createOverlayElement("TextArea", "DebugText"));
        // mDebugText->setFontName("DebugFont");
        // mDebugText->setCharHeight(0.025f);

        // mDebugTextShadow = static_cast<Ogre::v1::TextAreaOverlayElement*>(
        //	overlayManager.createOverlayElement("TextArea", "0DebugTextShadow"));
        // mDebugTextShadow->setFontName("DebugFont");
        // mDebugTextShadow->setCharHeight(0.025f);
        // mDebugTextShadow->setColour(Ogre::ColourValue::Black);
        // mDebugTextShadow->setPosition(0.002f, 0.002f);

        // panel->addChild(mDebugTextShadow);
        // panel->addChild(mDebugText);
        // overlay->add2D(panel);
        // overlay->show();

        // auto overlayManager = Ogre::v1::OverlayManager::getSingletonPtr();
        // auto overlay = overlayManager->create("Test");

        // auto panel = (Ogre::v1::OverlayContainer*)overlayManager->createOverlayElement("Panel",
        // "Panel");
        ////panel->setVisible(true);
        ////panel->setMaterialName("DebugCube");
        // overlay->add2D(panel);

        // auto text =
        // (Ogre::v1::TextAreaOverlayElement*)overlayManager->createOverlayElement("TextArea",
        // "TextArea"); text->setMetricsMode(Ogre::v1::GuiMetricsMode::GMM_PIXELS);
        // text->setHorizontalAlignment(Ogre::v1::GuiHorizontalAlignment::GHA_CENTER);
        // text->setVerticalAlignment(Ogre::v1::GuiVerticalAlignment::GVA_CENTER);
        // text->setFontName("DebugFont");
        // text->setCharHeight(18);
        // text->setCaption("Test");
        // text->setSpaceWidth(9);
        // text->setColour(Ogre::ColourValue::White);
        // panel->addChild(text);

        ////auto textName = String("test");
        ////auto mElement =
        /// Ogre::v1::OverlayManager::getSingleton().createOverlayElementFromTemplate("SdkTrays/Label",
        ///"BorderPanel", textName); /auto mTextArea =
        ///(Ogre::v1::TextAreaOverlayElement*)((Ogre::v1::OverlayContainer*)mElement)->getChild(textName
        ///+ "/LabelCaption"); /mTextArea->setCaption("Test2"); /panel->addChild(mTextArea);

        // overlay->show();
    }

    void CGraphicsSystemOgreNext::generateDebugText( float timeSinceLast, Ogre::String &outText )
    {
        const Ogre::FrameStats *frameStats = m_root->getFrameStats();

        Ogre::String finalText;
        finalText.reserve( 128 );
        finalText = "Frame time:\t";
        finalText += Ogre::StringConverter::toString( timeSinceLast * 1000.0f );
        finalText += " ms\n";
        finalText += "Frame FPS:\t";
        finalText += Ogre::StringConverter::toString( 1.0f / timeSinceLast );
        finalText += "\nAvg time:\t";
        finalText += Ogre::StringConverter::toString( frameStats->getAvgTime() );
        finalText += " ms\n";
        finalText += "Avg FPS:\t";
        finalText += Ogre::StringConverter::toString( 1000.0f / frameStats->getAvgTime() );
        finalText += "\n\nPress F1 to toggle help";

        outText.swap( finalText );

        if( mDebugText )
        {
            mDebugText->setCaption( finalText );
        }

        if( mDebugTextShadow )
        {
            mDebugTextShadow->setCaption( finalText );
        }
    }

    auto CGraphicsSystemOgreNext::WindowEventListener::windowClosing( Ogre::RenderWindow *rw ) -> bool
    {
        auto applicationManager = core::ApplicationManager::instance();
        applicationManager->setRunning( false );
        return false;
    }

    CGraphicsSystemOgreNext::WindowEventListener::~WindowEventListener() = default;

    CGraphicsSystemOgreNext::WindowEventListener::WindowEventListener() = default;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

    LRESULT CALLBACK CGraphicsSystemOgreNext::_WndProc( HWND hWnd, UINT uMsg, WPARAM wParam,
                                                        LPARAM lParam )
    {
        using namespace Ogre;

        auto applicationManager = core::ApplicationManager::instance();
        auto ui = applicationManager->getUI();

        auto data = fb::make_ptr<WindowMessageData>();
        data->setWindowHandle( hWnd );
        data->setMessage( uMsg );
        data->setWParam( wParam );
        data->setLParam( lParam );

        if( ui )
        {
            ui->messagePump( data );
        }

        if( uMsg == WM_CREATE )
        {
            // Store pointer to Win32Window in user data area
            SetWindowLongPtr( hWnd, GWLP_USERDATA,
                              (LONG_PTR)( ( (LPCREATESTRUCT)lParam )->lpCreateParams ) );
            return 0;
        }

        // look up window instance
        // note: it is possible to get a WM_SIZE before WM_CREATE
        auto win = (Window *)GetWindowLongPtr( hWnd, GWLP_USERDATA );
        if( !win )
        {
            return DefWindowProc( hWnd, uMsg, wParam, lParam );
        }

        // LogManager* log = LogManager::getSingletonPtr();
        // Iterator of all listeners registered to this Window
        auto listeners = WindowEventUtilities::_msListeners;

        multimap<Window *, Ogre::WindowEventListener *>::type::iterator index;

        auto start = listeners.lower_bound( win );
        auto end = listeners.upper_bound( win );

        switch( uMsg )
        {
        case WM_ACTIVATE:
        {
            bool active = ( LOWORD( wParam ) != WA_INACTIVE );
            win->setFocused( active );

            for( ; start != end; ++start )
            {
                ( start->second )->windowFocusChange( win );
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint( hWnd, &ps );
            win->_setVisible( !IsRectEmpty( &ps.rcPaint ) );
            EndPaint( hWnd, &ps );
        }
        break;
        case WM_SYSKEYDOWN:
        {
            switch( wParam )
            {
            case VK_CONTROL:
            case VK_SHIFT:
            case VK_MENU:  // ALT
                // return zero to bypass defProc and signal we processed the message
                return 0;
            }
        }
        break;
        case WM_SYSKEYUP:
        {
            switch( wParam )
            {
            case VK_CONTROL:
            case VK_SHIFT:
            case VK_MENU:  // ALT
            case VK_F10:
                // return zero to bypass defProc and signal we processed the message
                return 0;
            }
        }
        break;
        case WM_SYSCHAR:
        {
            // return zero to bypass defProc and signal we processed the message, unless it's an
            // ALT-space
            if( wParam != VK_SPACE )
            {
                return 0;
            }
        }
        break;
        case WM_ENTERSIZEMOVE:
            // log->logMessage("WM_ENTERSIZEMOVE");
            break;
        case WM_EXITSIZEMOVE:
            // log->logMessage("WM_EXITSIZEMOVE");
            break;
        case WM_MOVE:
        {
            // log->logMessage("WM_MOVE");
            win->windowMovedOrResized();

            for( auto listener : listeners )
            {
                ( listener.second )->windowMoved( win );
            }
        }
        break;
        case WM_DISPLAYCHANGE:
        {
            win->windowMovedOrResized();

            for( auto listener : listeners )
            {
                ( listener.second )->windowResized( win );
            }
        }
        break;
        case WM_SIZE:
        {
            // log->logMessage("WM_SIZE");
            win->windowMovedOrResized();

            for( auto listener : listeners )
            {
                ( listener.second )->windowResized( win );
            }
        }
        break;
        case WM_GETMINMAXINFO:
        {
            // Prevent the window from going smaller than some minimu size
            ( (MINMAXINFO *)lParam )->ptMinTrackSize.x = 100;
            ( (MINMAXINFO *)lParam )->ptMinTrackSize.y = 100;
        }
        break;
        case WM_CLOSE:
        {
            // log->logMessage("WM_CLOSE");
            bool close = true;

            for( auto listener : listeners )
            {
                if( !( listener.second )->windowClosing( win ) )
                {
                    close = false;
                }
            }

            if( !close )
            {
                return 0;
            }

            for( index = listeners.lower_bound( win ); index != end; ++index )
            {
                ( index->second )->windowClosed( win );
            }

            win->destroy();
            return 0;
        }
        }

        return DefWindowProc( hWnd, uMsg, wParam, lParam );
    }
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_FREEBSD
    //
    void GLXProc( Ogre::Window *win, const XEvent &event )
    {
        // An iterator for the window listeners
        Ogre::WindowEventUtilities::WindowEventListeners::iterator index,
            start = Ogre::WindowEventUtilities::_msListeners.lower_bound( win ),
            end = Ogre::WindowEventUtilities::_msListeners.upper_bound( win );

        switch( event.type )
        {
        case ClientMessage:
        {
            ::Atom atom;
            win->getCustomAttribute( "ATOM", &atom );
            if( event.xclient.format == 32 && event.xclient.data.l[0] == (long)atom )
            {  // Window closed by window manager
                // Send message first, to allow app chance to unregister things that need done before
                // window is shutdown
                bool close = true;
                for( index = start; index != end; ++index )
                {
                    if( !( index->second )->windowClosing( win ) )
                        close = false;
                }
                if( !close )
                    return;

                for( index = start; index != end; ++index )
                    ( index->second )->windowClosed( win );
                win->destroy();
            }
            break;
        }
        case DestroyNotify:
        {
            if( !win->isClosed() )
            {
                // Window closed without window manager warning.
                for( index = start; index != end; ++index )
                    ( index->second )->windowClosed( win );
                win->destroy();
            }
            break;
        }
        case ConfigureNotify:
        {
            // This could be slightly more efficient if windowMovedOrResized took arguments:
            Ogre::uint32 oldWidth, oldHeight;
            Ogre::int32 oldLeft, oldTop;
            win->getMetrics( oldWidth, oldHeight, oldLeft, oldTop );
            win->windowMovedOrResized();

            Ogre::uint32 newWidth, newHeight;
            Ogre::int32 newLeft, newTop;
            win->getMetrics( newWidth, newHeight, newLeft, newTop );

            if( newLeft != oldLeft || newTop != oldTop )
            {
                for( index = start; index != end; ++index )
                    ( index->second )->windowMoved( win );
            }

            if( newWidth != oldWidth || newHeight != oldHeight )
            {
                for( index = start; index != end; ++index )
                    ( index->second )->windowResized( win );
            }
            break;
        }
        case FocusIn:   // Gained keyboard focus
        case FocusOut:  // Lost keyboard focus
            win->setFocused( event.type == FocusIn );
            for( index = start; index != end; ++index )
                ( index->second )->windowFocusChange( win );
            break;
        case MapNotify:  // Restored
            win->setFocused( true );
            for( index = start; index != end; ++index )
                ( index->second )->windowFocusChange( win );
            break;
        case UnmapNotify:  // Minimised
            win->setFocused( false );
            win->_setVisible( false );
            for( index = start; index != end; ++index )
                ( index->second )->windowFocusChange( win );
            break;
        case VisibilityNotify:
            switch( event.xvisibility.state )
            {
            case VisibilityUnobscured:
                win->setFocused( true );
                win->_setVisible( true );
                break;
            case VisibilityPartiallyObscured:
                win->setFocused( true );
                win->_setVisible( true );
                break;
            case VisibilityFullyObscured:
                win->setFocused( false );
                win->_setVisible( false );
                break;
            }
            for( index = start; index != end; ++index )
                ( index->second )->windowFocusChange( win );
            break;
        default:
            break;
        }  // End switch event.type
    }
    //
    void XcbProc( xcb_connection_t *xcbConnection, xcb_generic_event_t *e )
    {
        XcbWindowMap::const_iterator itWindow;
        Ogre::WindowEventUtilities::WindowEventListeners::iterator index, start, end;

        const Ogre::uint8 responseType = e->response_type & ~0x80;
        switch( responseType )
        {
        case XCB_CLIENT_MESSAGE:
        {
            xcb_client_message_event_t *event = reinterpret_cast<xcb_client_message_event_t *>( e );
            itWindow = gXcbWindowToOgre.find( event->window );
            if( itWindow != gXcbWindowToOgre.end() )
            {
                Ogre::Window *win = itWindow->second;
                if( event->format == 32u )
                {
                    xcb_atom_t wmProtocols;
                    xcb_atom_t wmDeleteWindow;
                    win->getCustomAttribute( "mWmProtocols", &wmProtocols );
                    win->getCustomAttribute( "mWmDeleteWindow", &wmDeleteWindow );

                    if( event->type == wmProtocols && event->data.data32[0] == wmDeleteWindow )
                    {
                        start = Ogre::WindowEventUtilities::_msListeners.lower_bound( win );
                        end = Ogre::WindowEventUtilities::_msListeners.upper_bound( win );

                        // Window closed by window manager
                        // Send message first, to allow app chance to unregister things that need
                        // done before window is shutdown
                        bool close = true;
                        for( index = start; index != end; ++index )
                        {
                            if( !( index->second )->windowClosing( win ) )
                                close = false;
                        }
                        if( !close )
                            return;

                        for( index = start; index != end; ++index )
                            ( index->second )->windowClosed( win );
                        win->destroy();
                    }
                }
            }
        }
        break;
        case XCB_FOCUS_IN:   // Gained keyboard focus
        case XCB_FOCUS_OUT:  // Lost keyboard focus
        {
            xcb_focus_in_event_t *event = reinterpret_cast<xcb_focus_in_event_t *>( e );
            itWindow = gXcbWindowToOgre.find( event->event );
            if( itWindow != gXcbWindowToOgre.end() )
            {
                Ogre::Window *win = itWindow->second;
                win->setFocused( responseType == XCB_FOCUS_IN );

                start = Ogre::WindowEventUtilities::_msListeners.lower_bound( win );
                end = Ogre::WindowEventUtilities::_msListeners.upper_bound( win );
                for( index = start; index != end; ++index )
                    ( index->second )->windowFocusChange( win );
            }
        }
        break;
        case XCB_MAP_NOTIFY:  // Restored
        {
            xcb_map_notify_event_t *event = reinterpret_cast<xcb_map_notify_event_t *>( e );
            itWindow = gXcbWindowToOgre.find( event->window );
            if( itWindow != gXcbWindowToOgre.end() )
            {
                Ogre::Window *win = itWindow->second;
                win->setFocused( true );

                start = Ogre::WindowEventUtilities::_msListeners.lower_bound( win );
                end = Ogre::WindowEventUtilities::_msListeners.upper_bound( win );
                for( index = start; index != end; ++index )
                    ( index->second )->windowFocusChange( win );
            }
        }
        break;
        case XCB_UNMAP_NOTIFY:  // Minimized
        {
            xcb_unmap_notify_event_t *event = reinterpret_cast<xcb_unmap_notify_event_t *>( e );
            itWindow = gXcbWindowToOgre.find( event->window );
            if( itWindow != gXcbWindowToOgre.end() )
            {
                Ogre::Window *win = itWindow->second;
                win->setFocused( false );
                win->_setVisible( false );

                start = Ogre::WindowEventUtilities::_msListeners.lower_bound( win );
                end = Ogre::WindowEventUtilities::_msListeners.upper_bound( win );
                for( index = start; index != end; ++index )
                    ( index->second )->windowFocusChange( win );
            }
        }
        break;
        case XCB_VISIBILITY_NOTIFY:
        {
            xcb_visibility_notify_event_t *event =
                reinterpret_cast<xcb_visibility_notify_event_t *>( e );
            itWindow = gXcbWindowToOgre.find( event->window );
            if( itWindow != gXcbWindowToOgre.end() )
            {
                Ogre::Window *win = itWindow->second;
                xcb_visibility_t visibility = static_cast<xcb_visibility_t>( event->state );
                switch( visibility )
                {
                case XCB_VISIBILITY_UNOBSCURED:
                case XCB_VISIBILITY_PARTIALLY_OBSCURED:
                    win->setFocused( true );
                    win->_setVisible( true );
                    break;
                case XCB_VISIBILITY_FULLY_OBSCURED:
                    win->setFocused( false );
                    win->_setVisible( false );
                    break;
                }

                start = Ogre::WindowEventUtilities::_msListeners.lower_bound( win );
                end = Ogre::WindowEventUtilities::_msListeners.upper_bound( win );
                for( index = start; index != end; ++index )
                    ( index->second )->windowFocusChange( win );
            }
        }
        break;
        case XCB_CONFIGURE_NOTIFY:
        {
            xcb_configure_notify_event_t *event = reinterpret_cast<xcb_configure_notify_event_t *>( e );

            itWindow = gXcbWindowToOgre.find( event->window );
            if( itWindow != gXcbWindowToOgre.end() )
            {
                Ogre::Window *win = itWindow->second;

                // This could be slightly more efficient if windowMovedOrResized took arguments:
                Ogre::uint32 oldWidth, oldHeight;
                Ogre::int32 oldLeft, oldTop;
                win->getMetrics( oldWidth, oldHeight, oldLeft, oldTop );
                win->windowMovedOrResized();

                Ogre::uint32 newWidth, newHeight;
                Ogre::int32 newLeft, newTop;
                win->getMetrics( newWidth, newHeight, newLeft, newTop );

                start = Ogre::WindowEventUtilities::_msListeners.lower_bound( win );
                end = Ogre::WindowEventUtilities::_msListeners.upper_bound( win );

                if( newLeft != oldLeft || newTop != oldTop )
                {
                    for( index = start; index != end; ++index )
                        ( index->second )->windowMoved( win );
                }

                if( newWidth != oldWidth || newHeight != oldHeight )
                {
                    for( index = start; index != end; ++index )
                        ( index->second )->windowResized( win );
                }
            }
        }
        break;
        case XCB_DESTROY_NOTIFY:
        {
            xcb_visibility_notify_event_t *event =
                reinterpret_cast<xcb_visibility_notify_event_t *>( e );
            itWindow = gXcbWindowToOgre.find( event->window );
            if( itWindow != gXcbWindowToOgre.end() )
            {
                Ogre::Window *win = itWindow->second;
                if( !win->isClosed() )
                {
                    start = Ogre::WindowEventUtilities::_msListeners.lower_bound( win );
                    end = Ogre::WindowEventUtilities::_msListeners.upper_bound( win );
                    for( index = start; index != end; ++index )
                        ( index->second )->windowClosed( win );
                    win->destroy();
                }
            }
        }
        break;
        }
    }
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE && !defined __OBJC__ && !defined __LP64__
    //
    namespace Ogre
    {
        OSStatus WindowEventUtilities::_CarbonWindowHandler( EventHandlerCallRef nextHandler,
                                                             EventRef event, void *wnd )
        {
            OSStatus status = noErr;

            // Only events from our window should make it here
            // This ensures that our user data is our WindowRef
            Window *curWindow = (Window *)wnd;
            if( !curWindow )
                return eventNotHandledErr;

            // Iterator of all listeners registered to this Window
            WindowEventListeners::iterator index, start = _msListeners.lower_bound( curWindow ),
                                                  end = _msListeners.upper_bound( curWindow );

            // We only get called if a window event happens
            UInt32 eventKind = GetEventKind( event );

            switch( eventKind )
            {
            case kEventWindowActivated:
                curWindow->setFocused( true );
                for( ; start != end; ++start )
                    ( start->second )->windowFocusChange( curWindow );
                break;
            case kEventWindowDeactivated:
                curWindow->setFocused( false );

                for( ; start != end; ++start )
                    ( start->second )->windowFocusChange( curWindow );

                break;
            case kEventWindowShown:
            case kEventWindowExpanded:
                curWindow->setFocused( true );
                curWindow->setVisible( true );
                for( ; start != end; ++start )
                    ( start->second )->windowFocusChange( curWindow );
                break;
            case kEventWindowHidden:
            case kEventWindowCollapsed:
                curWindow->setFocused( false );
                curWindow->setVisible( false );
                for( ; start != end; ++start )
                    ( start->second )->windowFocusChange( curWindow );
                break;
            case kEventWindowDragCompleted:
                curWindow->windowMovedOrResized();
                for( ; start != end; ++start )
                    ( start->second )->windowMoved( curWindow );
                break;
            case kEventWindowBoundsChanged:
                curWindow->windowMovedOrResized();
                for( ; start != end; ++start )
                    ( start->second )->windowResized( curWindow );
                break;
            case kEventWindowClose:
            {
                bool close = true;
                for( ; start != end; ++start )
                {
                    if( !( start->second )->windowClosing( curWindow ) )
                        close = false;
                }
                if( close )
                    // This will cause event handling to continue on to the standard handler, which
                    // calls DisposeWindow(), which leads to the 'kEventWindowClosed' event
                    status = eventNotHandledErr;
                break;
            }
            case kEventWindowClosed:
                curWindow->destroy();
                for( ; start != end; ++start )
                    ( start->second )->windowClosed( curWindow );
                break;
            default:
                status = eventNotHandledErr;
                break;
            }

            return status;
        }
    }  // namespace Ogre
#endif

    void CGraphicsSystemOgreNext::GraphicsSystemEventListener::setOwner(
        SmartPtr<CGraphicsSystemOgreNext> owner )
    {
        m_owner = owner;
    }

    auto CGraphicsSystemOgreNext::GraphicsSystemEventListener::getOwner() const
        -> SmartPtr<CGraphicsSystemOgreNext>
    {
        return m_owner;
    }

    auto CGraphicsSystemOgreNext::GraphicsSystemEventListener::handleEvent(
        IEvent::Type eventType, hash_type eventValue, const Array<Parameter> &arguments,
        SmartPtr<ISharedObject> sender, SmartPtr<ISharedObject> object, SmartPtr<IEvent> event )
        -> Parameter
    {
        auto task = Thread::getCurrentTask();
        if( task == Thread::Task::Render )
        {
            if( eventValue == IEvent::loadingStateChanged )
            {
                if( arguments[1].getS32() == static_cast<s32>( LoadingState::Loaded ) )
                {
                    if( sender->isDerived<render::CSceneManagerOgreNext>() )
                    {
                        auto sceneManager =
                            fb::static_pointer_cast<render::CSceneManagerOgreNext>( sender );

                        auto applicationManager = core::ApplicationManager::instance();
                        auto ui = applicationManager->getRenderUI();
                        if( ui )
                        {
                            auto renderUI = fb::static_pointer_cast<ui::UIManager>( ui );
                            renderUI->setGraphicsScene( sceneManager );
                        }
                    }
                    else if( sender->isDerived<render::CSceneManagerOgreNext>() )
                    {
                    }
                }
            }
        }

        return {};
    }
}  // namespace fb::render
