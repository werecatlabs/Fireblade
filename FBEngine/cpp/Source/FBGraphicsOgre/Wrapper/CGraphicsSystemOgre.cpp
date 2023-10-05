#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSystemOgre.h>
#include <FBGraphicsOgre/Ogre/CellSceneManager.h>
#include <FBGraphicsOgre/Ogre/BasicSceneManager.h>
#include <FBGraphicsOgre/Core/CompositorManager.h>
#include <FBGraphicsOgre/Wrapper/CMeshManager.h>
#include <FBGraphicsOgre/Wrapper/CDebug.h>
#include <FBGraphicsOgre/Wrapper/CInstanceManager.h>
#include <FBGraphicsOgre/Wrapper/CDeferredShadingSystem.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSettings.h>
#include <FBGraphicsOgre/Wrapper/CGraphicsSceneOgre.h>
#include <FBGraphicsOgre/Wrapper/COverlayManagerOgre.h>
#include <FBGraphicsOgre/Wrapper/CResourceGroupManager.h>
#include <FBGraphicsOgre/Wrapper/CMaterialManagerOgre.h>
#include <FBGraphicsOgre/Wrapper/CTextureManagerOgre.h>
#include <FBGraphicsOgre/Wrapper/CWindowOgre.h>
#include <FBGraphicsOgre/Wrapper/CFontManager.h>
#include <FBGraphicsOgre/Ogre/ResourceGroupHelper.h>
#include <FBGraphicsOgre/Ogre/ResourceLoadingListener.h>
#include <FBGraphicsOgre/FBOgreTypes.h>
#include <FBGraphicsOgre/Ogre/MaterialListener.h>
#include <FBCore/FBCore.h>

#include <OgreBitesConfigDialog.h>
#include <OgreWindowEventUtilities.h>
#include <OgreOverlayManager.h>
#include <OgreOverlaySystem.h>
#include <OgreFontManager.h>
#include <OgreBuildSettings.h>
#include <OgreShaderGenerator.h>
#include <Ogre.h>
#include <OgreSGTechniqueResolverListener.h>

//#include <OgitorsRoot.h>
//#include "OgitorsGlobals.h"

#include "FBGraphics/MeshConverter.h"

#include <FBGraphicsOgre/Terrain/TerrainComponent/OgreTerrain.h>

//#if FB_OGRE_USE_PARTICLE_UNIVERSE
//#    include <ParticleUniversePlugin.h>
//#endif

#ifdef OGRE_STATIC_LIB
#    ifdef FB_PLATFORM_WIN32
#        if FB_BUILD_RENDERER_DX9
#            include <OgreD3D9Plugin.h>
#            include <OgreD3D9RenderSystem.h>
#            include <OgreD3D9Driver.h>
#            include <OgreD3D9DriverList.h>
#            include <OgreD3D9VideoModeList.h>
#            include <OgreD3D9VideoMode.h>
#        endif

#        if FB_BUILD_RENDERER_DX11
#            include <OgreD3D11Plugin.h>
#            include <OgreD3D11RenderSystem.h>
#        endif

#        if FB_BUILD_RENDERER_OPENGL
#            include <OgreGLPlugin.h>
#        endif

#        if FB_BUILD_RENDERER_GL3PLUS
#            include <OgreGL3PlusPlugin.h>
#        endif

#        if FB_BUILD_RENDERER_METAL
#            include <OgreMetalPlugin.h>
#        endif
#    elif defined FB_PLATFORM_APPLE
#        if FB_BUILD_RENDERER_OPENGL
#            include <OgreGLPlugin.h>
#        endif

#        if FB_BUILD_RENDERER_GL3PLUS
#            include <OgreGL3PlusPlugin.h>
#        endif

#        if FB_BUILD_RENDERER_METAL
#            include <OgreMetalPlugin.h>
#        endif
#    endif
#endif

#if FB_CODEC_FREEIMAGE
#    include <OgreFreeImageCodec.h>
#elif FB_CODEC_STBI
#    include <OgreSTBICodec.h>
#endif

#if defined FB_PLATFORM_APPLE
#    include <FBGraphicsOgre/Window/Apple/macOS/macUtil.h>
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_FREEBSD
#    include <xcb/xcb.h>
#    include <X11/Xlib.h>
extern void GLXProc( Ogre::Window *win, const XEvent &event );
extern void XcbProc( xcb_connection_t *xcbConnection, xcb_generic_event_t *event );

typedef std::map<xcb_window_t, Ogre::Window *> XcbWindowMap;
static XcbWindowMap gXcbWindowToOgre;
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#    include "OSX/macUtils.h"
#endif

#ifdef None
#    undef None /* universal null resource or null atom */
#endif

namespace fb
{
    namespace render
    {
        // supported render system plugin names in Ogre
        const String CGraphicsSystemOgre::rsDX12Name = String( "Direct3D12 Rendering Subsystem" );
        const String CGraphicsSystemOgre::rsDX11Name = String( "Direct3D11 Rendering Subsystem" );
        const String CGraphicsSystemOgre::rsGL3Name = String( "OpenGL 3+ Rendering Subsystem" );
        const String CGraphicsSystemOgre::rsDX9Name = String( "Direct3D9 Rendering Subsystem" );
        const String CGraphicsSystemOgre::rsGLName = String( "OpenGL Rendering Subsystem" );
        const String CGraphicsSystemOgre::rsMetalName = String( "Metal Rendering Subsystem" );
        const String CGraphicsSystemOgre::rsVulkanName = String( "Vulkan Rendering Subsystem" );
        const String CGraphicsSystemOgre::rsNoneName = String( "None" );

        CGraphicsSystemOgre::CGraphicsSystemOgre()
        {
            m_resourceGroupManager = fb::make_ptr<CResourceGroupManager>();
        }

        CGraphicsSystemOgre::~CGraphicsSystemOgre()
        {
            unload( nullptr );
        }

        bool CGraphicsSystemOgre::initialiseRTShaderSystem()
        {
            using namespace Ogre;

#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
            if( RTShader::ShaderGenerator::initialize() )
            {
                m_shaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();

                auto renderApi = getRenderApi();
                switch( renderApi )
                {
                case IGraphicsSystem::RenderApi::None:
                {
                    m_shaderGenerator->setTargetLanguage( "none" );
                }
                break;
                case IGraphicsSystem::RenderApi::DX9:
                case IGraphicsSystem::RenderApi::DX11:
                case IGraphicsSystem::RenderApi::DX12:
                {
                    m_shaderGenerator->setTargetLanguage( "hlsl" );
                }
                break;
                case IGraphicsSystem::RenderApi::GL:
                case IGraphicsSystem::RenderApi::GL3Plus:
                case IGraphicsSystem::RenderApi::Vulkan:
                {
                    m_shaderGenerator->setTargetLanguage( "glsl" );
                }
                break;
                case IGraphicsSystem::RenderApi::Metal:
                {
                    m_shaderGenerator->setTargetLanguage( "metal" );
                }
                break;
                default:
                {
                    m_shaderGenerator->setTargetLanguage( "none" );
                }
                break;
                }

                // Create and register the material manager listener if it doesn't exist yet.
                if( !m_materialMgrListener )
                {
                    m_materialMgrListener =
                        new OgreBites::SGTechniqueResolverListener( m_shaderGenerator );
                    MaterialManager::getSingleton().addListener( m_materialMgrListener );
                }

                return true;
            }
#endif
            return false;
        }

        void CGraphicsSystemOgre::handleStateChanged( const SmartPtr<IStateMessage> &message )
        {
        }

        void CGraphicsSystemOgre::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void CGraphicsSystemOgre::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        void CGraphicsSystemOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                using namespace Ogre;

                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );
                FB_ASSERT( applicationManager->isValid() );

                CGraphicsSystem::load( data );

                FB_ASSERT( applicationManager->isValid() );

#ifdef FB_PLATFORM_WIN32
                Ogre::String mResourcePath = "";
                Ogre::String pluginsPath;
                // only use plugins.cfg if not static
#    ifndef OGRE_STATIC_LIB
                pluginsPath = mResourcePatplasm 23h + "plugins.cfg";
#    endif

                m_root = new Root( pluginsPath, mResourcePath + "ogre.cfg", mResourcePath + "Ogre.log" );

#    ifdef OGRE_STATIC_LIB
#        if FB_BUILD_RENDERER_DX11
                m_d3d11Plugin = new D3D11Plugin;
                m_root->installPlugin( m_d3d11Plugin );
#        endif

#        if FB_BUILD_RENDERER_DX9
                m_d3d9Plugin = new D3D9Plugin;
                m_root->installPlugin( m_d3d9Plugin );
#        endif

#    endif

#    if FB_BUILD_RENDERER_OPENGL
                m_glPlugin = new GLPlugin;
                m_root->installPlugin( m_glPlugin );
#    endif

#    if FB_BUILD_RENDERER_GL3PLUS
                m_gl3Plugin = new GL3PlusPlugin;
                m_root->installPlugin( m_gl3Plugin );
#    endif
#elif defined( FB_PLATFORM_APPLE )
                Ogre::String mResourcePath = "";
                Ogre::String pluginsPath;
                // only use plugins.cfg if not static
#    ifndef OGRE_STATIC_LIB
                pluginsPath = mResourcePath + "plugins.cfg";
#    endif

                m_root = new Ogre::Root( pluginsPath, mResourcePath + "ogre.cfg",
                                         mResourcePath + "Ogre.log" );

#    if FB_BUILD_RENDERER_METAL
                m_metalPlugin = new Ogre::MetalPlugin;
                m_root->installPlugin( m_metalPlugin );
#    endif

#    if FB_BUILD_RENDERER_OPENGL
                m_glPlugin = new GLPlugin;
                m_root->installPlugin( m_glPlugin );
#    endif

#    if FB_BUILD_RENDERER_GL3PLUS
                m_gl3Plugin = new GL3PlusPlugin;
                m_root->installPlugin( m_gl3Plugin );
#    endif
#elif defined( FB_PLATFORM_IOS )
                Ogre::String pluginsPath;
                // only use plugins.cfg if not static
#    ifndef OGRE_STATIC_LIB
                pluginsPath = m_ResourcePath + "plugins.cfg";
#    endif

                m_root = new Ogre::Root( pluginsPath, Ogre::String( "/ogre.cfg" ),
                                         Ogre::String( "Ogre.log" ) );

#    ifdef OGRE_STATIC_LIB
                m_staticPluginLoader = new Ogre::StaticPluginLoader;
                m_staticPluginLoader->load();
#    endif
#endif

                m_overlaySystem = new OverlaySystem;

                FB_ASSERT( applicationManager->isValid() );

                m_overlayMgr = fb::make_ptr<COverlayManagerOgre>();
                m_compositorManager = fb::make_ptr<CompositorManager>();
                m_resourceGroupManager = fb::make_ptr<CResourceGroupManager>();

                FB_ASSERT( applicationManager->isValid() );

                m_materialManager = fb::make_ptr<CMaterialManagerOgre>();

                FB_ASSERT( applicationManager->isValid() );

                m_textureManager = fb::make_ptr<CTextureManagerOgre>();
                m_instanceManager = fb::make_ptr<CInstanceManager>();

                FB_ASSERT( applicationManager->isValid() );

                m_meshManager = fb::make_ptr<CMeshManager>();

                FB_ASSERT( applicationManager->isValid() );
                m_fontManager = fb::make_ptr<CFontManager>();

                FB_ASSERT( applicationManager->isValid() );

                m_resourceGroupHelper = new ResourceGroupHelper;

                m_resourceLoadingListener = new ResourceLoadingListener;

                auto meshConverter = fb::make_ptr<MeshConverter>();
                setMeshConverter( meshConverter );

                FB_ASSERT( applicationManager->isValid() );

                ResourceGroupManager *ogreResGrpMgr = ResourceGroupManager::getSingletonPtr();
                ogreResGrpMgr->setLoadingListener( m_resourceLoadingListener );

                FB_ASSERT( applicationManager->isValid() );

                m_frameListener = new AppFrameListener( this );
                m_root->addFrameListener( m_frameListener );

                FB_ASSERT( applicationManager->isValid() );

                //#if FB_OGRE_USE_PARTICLE_UNIVERSE
                //                m_particlePlugin = new ParticleUniverse::ParticleUniversePlugin;
                //#endif

                m_cellSceneManagerFactory = new CellSceneManagerFactory;
                m_root->addSceneManagerFactory( m_cellSceneManagerFactory );

                m_basicSceneManagerFactory = new BasicSceneManagerFactory;
                m_root->addSceneManagerFactory( m_basicSceneManagerFactory );

#if FB_CODEC_FREEIMAGE
                FreeImageCodec::startup();
#elif FB_CODEC_STBI
                STBIImageCodec::startup();
#endif

                FB_ASSERT( applicationManager->isValid() );

                auto debug = fb::make_ptr<CDebug>();
                setDebug( debug );

                mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

                Ogre::StringVector disabledPluginPaths;
                //m_ogitorsRoot = OGRE_NEW Ogitors::OgitorsRoot(&disabledPluginPaths);

                FB_ASSERT( applicationManager->isValid() );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CGraphicsSystemOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                if( isLoaded() )
                {
                    setLoadingState( LoadingState::Unloading );

                    FB_ASSERT( isValid() );

                    if( auto textureManager = getTextureManager() )
                    {
                        textureManager->unload( nullptr );
                        m_textureManager = nullptr;
                    }

                    for( auto window : m_windows )
                    {
                        window->unload( nullptr );
                    }

                    m_windows.clear();

                    if( auto defaultWindow = getDefaultWindow() )
                    {
                        defaultWindow->unload( nullptr );
                        setDefaultWindow( nullptr );
                    }

                    while( !m_loadQueue.empty() )
                    {
                        SmartPtr<ISharedObject> obj;
                        if( m_loadQueue.try_pop( obj ) )
                        {
                            obj = nullptr;
                        }
                    }

                    Array<SmartPtr<ISharedObject>> objects;
                    objects.reserve( 1024 );

                    while( !m_unloadQueue.empty() )
                    {
                        SmartPtr<ISharedObject> obj;
                        if( m_unloadQueue.try_pop( obj ) )
                        {
                            objects.push_back( obj );
                        }
                    }

                    for( auto &obj : objects )
                    {
                        obj->unload( nullptr );
                    }

                    objects.clear();

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

                    if( m_fontManager )
                    {
                        m_fontManager->unload( nullptr );
                        m_fontManager = nullptr;
                    }

                    if( auto compositorManager = getCompositorManager() )
                    {
                        compositorManager->unload( nullptr );
                        m_compositorManager = nullptr;
                    }

                    if( auto p = getScenesPtr() )
                    {
                        auto &scenes = *p;
                        for( auto sceneManager : scenes )
                        {
                            sceneManager->unload( nullptr );
                        }

                        setScenesPtr( nullptr );
                    }

                    if( auto defaultSceneManager = getGraphicsScene() )
                    {
                        defaultSceneManager->unload( nullptr );
                        setGraphicsScene( nullptr );
                    }

                    if( auto materialManager = getMaterialManager() )
                    {
                        materialManager->unload( nullptr );
                        m_materialManager = nullptr;
                    }

                    if( auto resourceGroupManager = getResourceGroupManager() )
                    {
                        resourceGroupManager->unload( nullptr );
                        m_resourceGroupManager = nullptr;
                    }

                    if( auto meshManager = getMeshManager() )
                    {
                        meshManager->unload( nullptr );
                        setMeshManager( nullptr );
                    }

                    // m_particlePlugin->uninstall();
                    // m_particlePlugin->shutdown();
                    // FB_SAFE_DELETE(m_particlePlugin);

                    //#ifdef FB_PLATFORM_WIN32
                    //		if (m_d3d9Plugin)
                    //		{
                    //			m_d3d9Plugin->uninstall();
                    //			m_d3d9Plugin->shutdown();
                    //			FB_SAFE_DELETE(m_d3d9Plugin);
                    //		}
                    //
                    // if( m_d3d11Plugin )
                    //{
                    //    m_d3d11Plugin->uninstall();
                    //    m_d3d11Plugin->shutdown();
                    //    FB_SAFE_DELETE( m_d3d11Plugin );
                    //}
                    //#endif
                    //
                    //		if (m_glPlugin)
                    //		{
                    //			m_glPlugin->uninstall();
                    //			m_glPlugin->shutdown();
                    //			FB_SAFE_DELETE(m_glPlugin);
                    //		}

                    FB_SAFE_DELETE( mTerrainGlobals );

                    if( m_cellSceneManagerFactory )
                    {
                        m_root->removeSceneManagerFactory( m_cellSceneManagerFactory );
                        FB_SAFE_DELETE( m_cellSceneManagerFactory );
                    }

                    if( m_basicSceneManagerFactory )
                    {
                        m_root->removeSceneManagerFactory( m_basicSceneManagerFactory );
                        FB_SAFE_DELETE( m_basicSceneManagerFactory );
                    }

#if FB_CODEC_FREEIMAGE
                    Ogre::FreeImageCodec::shutdown();
#elif FB_CODEC_STBI
                    Ogre::STBIImageCodec::shutdown();
#endif

                    if( m_materialMgrListener )
                    {
                        Ogre::MaterialManager::getSingleton().removeListener( m_materialMgrListener );
                        delete m_materialMgrListener;
                        m_materialMgrListener = nullptr;
                    }

                    auto shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
                    if( shaderGenerator )
                    {
                        shaderGenerator->destroy();
                    }

                    FB_SAFE_DELETE( m_root );
                    FB_SAFE_DELETE( m_resourceLoadingListener );
                    FB_SAFE_DELETE( m_frameListener );

#if FB_BUILD_RENDERER_DX11
                    if( m_d3d11Plugin )
                    {
                        FB_SAFE_DELETE( m_d3d11Plugin );
                    }
#endif

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IGraphicsSettings> CGraphicsSystemOgre::createConfiguration()
        {
            return fb::make_ptr<CGraphicsSettings>();
        }

        bool CGraphicsSystemOgre::isUpdating() const
        {
            return m_isUpdating;
        }

        void CGraphicsSystemOgre::setUpdating( bool val )
        {
            m_isUpdating = val;
        }

        IGraphicsSystem::RenderApi CGraphicsSystemOgre::getRenderApi( String renderPluginName )
        {
            if( renderPluginName == rsDX11Name )
            {
                return IGraphicsSystem::RenderApi::DX11;
            }
            if( renderPluginName == rsGL3Name )
            {
                return IGraphicsSystem::RenderApi::GL3Plus;
            }
            if( renderPluginName == rsGLName )
            {
                return IGraphicsSystem::RenderApi::GL;
            }
            if( renderPluginName == rsMetalName )
            {
                return IGraphicsSystem::RenderApi::Metal;
            }

            return IGraphicsSystem::RenderApi::None;
        }

        String CGraphicsSystemOgre::getRenderPluginName( IGraphicsSystem::RenderApi renderApi )
        {
            switch( renderApi )
            {
            case IGraphicsSystem::RenderApi::None:
                return rsNoneName;
            case IGraphicsSystem::RenderApi::DX9:
                return rsDX9Name;
            case IGraphicsSystem::RenderApi::DX11:
                return rsDX11Name;
            case IGraphicsSystem::RenderApi::DX12:
                return rsDX12Name;
            case IGraphicsSystem::RenderApi::GL:
                return rsGLName;
            case IGraphicsSystem::RenderApi::GL3Plus:
                return rsGL3Name;
            case IGraphicsSystem::RenderApi::Vulkan:
                return rsVulkanName;
            case IGraphicsSystem::RenderApi::Metal:
                return rsMetalName;
            default:
                return rsNoneName;
            }

            return rsNoneName;
        }

        bool CGraphicsSystemOgre::chooseRenderSystem()
        {
            try
            {
                Ogre::RenderSystem *selectedRenderSystem = nullptr;

                auto renderApi = getRenderApi();
                if( renderApi == IGraphicsSystem::RenderApi::None )
                {
                    auto renderers = m_root->getAvailableRenderers();
                    if( !renderers.empty() )
                    {
                        selectedRenderSystem = renderers.front();
                    }

                    auto selectedRenderPluginName = selectedRenderSystem->getName();
                    auto selectedRenderApi = getRenderApi( selectedRenderPluginName );
                    setRenderApi( selectedRenderApi );
                }
                else
                {
                    switch( renderApi )
                    {
                    case IGraphicsSystem::RenderApi::None:
                    {
                        auto renderers = m_root->getAvailableRenderers();
                        if( !renderers.empty() )
                        {
                            selectedRenderSystem = renderers.front();
                        }
                    }
                    break;
                    default:
                    {
                        auto rsName = getRenderPluginName( renderApi );
                        selectedRenderSystem = m_root->getRenderSystemByName( rsGLName );
                    }
                    break;
                    }
                }

                if( selectedRenderSystem )
                {
                    m_root->setRenderSystem( selectedRenderSystem );
                }
                else
                {
                    // no supported render system found
                    const String msg( "No supported render system found. Program will now exit." );
                    FB_LOG_ERROR( msg );
                    return false;
                }

                if( selectedRenderSystem )
                {
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
                            FB_LOG_ERROR( msg );
                            return false;
                        }
                    }
                }

                return true;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return false;
        }

        void CGraphicsSystemOgre::setRenderSystemDefaults( Ogre::RenderSystem *renderSystem )
        {
            try
            {
                if( !renderSystem )
                {
                    FB_LOG_ERROR( "No render system" );
                    return;
                }

                int width = 800;   // GetSystemMetrics(SM_CXSCREEN);
                int height = 800;  // GetSystemMetrics(SM_CYSCREEN);

                // choose suitable resolution
                Vector2I resolution( 1280, 720 );  // hack
                renderSystem->setConfigOption(
                    "Video Mode",
                    Ogre::StringConverter::toString( resolution.X() ) + Ogre::String( " x " ) +
                        Ogre::StringConverter::toString( resolution.Y() ) + " @ 32-bit colour" );

                renderSystem->setConfigOption( Ogre::String( "Full Screen" ), Ogre::String( "No" ) );

                // set anti alias defaults
                // renderSystem->setConfigOption(String("Anti aliasing"), String("None"));*/

#if FB_BUILD_RENDERER_DX9
                // renderSystem->setConfigOption( String( "Allow DirectX9Ex" ), String( "Yes" ) );
#endif
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool CGraphicsSystemOgre::configure( SmartPtr<IGraphicsSettings> config )
        {
            try
            {
                FB_ASSERT( isValid() );
                FB_ASSERT( m_root );
                FB_ASSERT( isLoaded() );

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );
                FB_ASSERT( applicationManager->isValid() );

                auto showDialog = false;
                auto applyDefaults = true;
                auto createWindow = true;

                if( config )
                {
                    showDialog = config->getShowDialog();
                    applyDefaults = true;
                    createWindow = config->getCreateWindow();
                }

                if( m_root->restoreConfig() )
                {
                    if( const auto renderSystem = m_root->getRenderSystem() )
                    {
                        const auto errStr = renderSystem->validateConfigOptions();
                        if( !StringUtil::isNullOrEmpty( errStr ) )
                        {
                            if( applyDefaults )
                            {
                                setRenderSystemDefaults( renderSystem );
                            }

                            if( !m_root->showConfigDialog( OgreBites::getNativeConfigDialog() ) )
                            {
                                return false;
                            }
                        }
                    }
                    else
                    {
                        chooseRenderSystem();

                        if( applyDefaults )
                        {
                            setRenderSystemDefaults( renderSystem );
                        }

                        if( !m_root->showConfigDialog( OgreBites::getNativeConfigDialog() ) )
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    chooseRenderSystem();

                    if( applyDefaults )
                    {
                        const auto renderSystem = m_root->getRenderSystem();
                        setRenderSystemDefaults( renderSystem );
                    }

                    if( showDialog && !m_root->showConfigDialog( OgreBites::getNativeConfigDialog() ) )
                    {
                        return false;
                    }
                }

#if 0  // defined FB_PLATFORM_WIN32
                auto ogreWindow = m_root->initialise( createWindow );
                if( ogreWindow )
                {
                    auto window = fb::make_ptr<CWindowOgre>();

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
                    auto window = fb::make_ptr<CWindowOgre>();

                    auto windowHandle = window->getHandle();
                    windowHandle->setName( "DefaultWindow" );

                    // window->initialise( ogreWindow );
                    window->load( nullptr );

                    setDefaultWindow( window );
                    m_windows.push_back( window );
                }
#endif

                if( createWindow )
                {
                    if( getUseRTSS() )
                    {
                        initialiseRTShaderSystem();
                    }
                }

                if( auto debug = getDebug() )
                {
                    debug->load( nullptr );
                }

                return true;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return false;
        }

        SmartPtr<IGraphicsScene> CGraphicsSystemOgre::addGraphicsScene( const String &type,
                                                                        const String &name )
        {
            FB_ASSERT( !StringUtil::isNullOrEmpty( type ) );
            FB_ASSERT( !StringUtil::isNullOrEmpty( name ) );

            auto sceneManager = fb::make_ptr<CGraphicsSceneOgre>();
            sceneManager->setType( type );
            sceneManager->setName( name );

            auto defaultSceneManager = getGraphicsScene();
            if( !defaultSceneManager )
            {
                setGraphicsScene( sceneManager );
            }

            addScenePtr( sceneManager );
            loadObject( sceneManager );

            return sceneManager;
        }

        SmartPtr<IOverlayManager> CGraphicsSystemOgre::getOverlayManager() const
        {
            return m_overlayMgr;
        }

        SmartPtr<CompositorManager> CGraphicsSystemOgre::getCompositorManager() const
        {
            return m_compositorManager;
        }

        SmartPtr<IResourceGroupManager> CGraphicsSystemOgre::getResourceGroupManager() const
        {
            return m_resourceGroupManager;
        }

        SmartPtr<IWindow> CGraphicsSystemOgre::createRenderWindow(
            const String &name, u32 width, u32 height, bool fullScreen,
            const SmartPtr<Properties> &properties )
        {
            auto window = fb::make_ptr<CWindowOgre>();

            auto windowHandle = window->getHandle();
            windowHandle->setName( name );

            window->setTitle( name );
            window->setSize( Vector2I( width, height ) );
            window->setFullscreen( fullScreen );
            m_windows.push_back( window );

            String handle;
            if( properties->getPropertyValue( "WindowHandle", handle ) )
            {
                window->setWindowHandleAsString( handle );
            }

            auto defaultWindow = getDefaultWindow();
            if( defaultWindow == nullptr )
            {
                m_defaultWindow = window;
            }

            window->load( nullptr );
            // loadObject( window );

            if( m_windows.size() == 1 )
            {
                if( getUseRTSS() )
                {
                    initialiseRTShaderSystem();
                }
            }

            return window;
        }

        SmartPtr<IWindow> CGraphicsSystemOgre::getRenderWindow( const String &name ) const
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

        s32 CGraphicsSystemOgre::getLoadPriority( SmartPtr<ISharedObject> obj )
        {
            if( obj->isDerived<IWindow>() )
            {
                return 100000;
            }
            if( obj->isDerived<IResourceGroupManager>() )
            {
                return 95000;
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
            else if( obj->isDerived<IOverlayElement>() )
            {
                return 1000;
            }
            else if( obj->isDerived<ui::IUIManager>() )
            {
                return 1000;
            }

            return 0;
        }

        void CGraphicsSystemOgre::update()
        {
            try
            {
                if( isLoaded() )
                {
                    ScopedLock lock( this );

                    setUpdating( true );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto timer = applicationManager->getTimer();
                    FB_ASSERT( timer );

                    auto task = Thread::getCurrentTask();
                    auto t = timer->getTime();
                    auto dt = timer->getDeltaTime();

                    auto hasUnloadedSceneManagers = false;
                    if( auto p = getScenesPtr() )
                    {
                        auto &scenes = *p;

                        for( auto scene : scenes )
                        {
                            if( scene )
                            {
                                if( scene->isLoaded() )
                                {
                                    scene->update();
                                }
                                else
                                {
                                    //hasUnloadedSceneManagers = true;
                                }
                            }
                        }
                    }

                    if( !m_unloadQueue.empty() )
                    {
                        SmartPtr<ISharedObject> obj;
                        while( m_unloadQueue.try_pop( obj ) )
                        {
                            obj->unload( nullptr );
                        }
                    }

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
                            const auto &graphicsObjectLoadingState = loadObj->getLoadingState();
                            if( !( graphicsObjectLoadingState == LoadingState::Loading ||
                                   graphicsObjectLoadingState == LoadingState::Loaded ) )
                            {
                                if( loadObj->isDerived<IGraphicsObject>() )
                                {
                                    if( !hasUnloadedSceneManagers )
                                    {
                                        loadObj->load( nullptr );
                                    }
                                }
                                else
                                {
                                    loadObj->load( nullptr );
                                }
                            }
                        }
                    }

#if 1
                    auto resourceGroupManager = getResourceGroupManager();
                    const auto &resourceManagerLoaded = resourceGroupManager->getLoadingState();
                    if( resourceManagerLoaded == LoadingState::Loaded )
                    {
                        FB_PROFILE_START( "RenderOneFrame" );

                        Ogre::FrameEvent evt;
                        evt.timeSinceLastFrame = evt.timeSinceLastEvent = static_cast<Ogre::Real>( dt );

                        if( !m_root->_fireFrameStarted( evt ) )
                        {
                            applicationManager->setRunning( false );
                        }

                        for( auto window : m_windows )
                        {
                            if( window )
                            {
                                window->preUpdate();
                            }
                        }

                        FB_PROFILE_START( "updateAllRenderTargets" );

                        //                     if( m_deferredShadingSystem )
                        //{
                        //    m_deferredShadingSystem->update();
                        //}

                        for( auto window : m_windows )
                        {
                            if( window )
                            {
                                window->update();
                            }
                        }

                        auto textureManager = getTextureManager();
                        if( textureManager )
                        {
                            textureManager->update();
                        }

                        FB_PROFILE_END( "updateAllRenderTargets" );

                        FB_PROFILE_START( "fireFrameRenderingQueued" );

                        if( textureManager )
                        {
                            textureManager->postUpdate();
                        }

                        for( auto window : m_windows )
                        {
                            if( window )
                            {
                                window->postUpdate();
                            }
                        }

                        if( !m_root->_fireFrameRenderingQueued( evt ) )
                        {
                            applicationManager->setRunning( false );
                        }

                        FB_PROFILE_END( "fireFrameRenderingQueued" );

                        if( !m_root->_fireFrameEnded( evt ) )
                        {
                            applicationManager->setRunning( false );
                        }

                        FB_PROFILE_END( "RenderOneFrame" );
                    }
#else

                    auto fDT = static_cast<f32>( dt );
                    m_root->renderOneFrame( fDT );
#endif

                    if( auto renderUI = applicationManager->getRenderUI() )
                    {
                        renderUI->update();
                    }

                    if( auto debug = getDebug() )
                    {
                        debug->preUpdate();
                        debug->update();
                        debug->postUpdate();
                    }

                    setUpdating( false );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        SmartPtr<IMaterialManager> CGraphicsSystemOgre::getMaterialManager() const
        {
            return m_materialManager;
        }

        SmartPtr<ITextureManager> CGraphicsSystemOgre::getTextureManager() const
        {
            return m_textureManager;
        }

        SmartPtr<IInstanceManager> CGraphicsSystemOgre::getInstanceManager() const
        {
            return m_instanceManager;
        }

        bool CGraphicsSystemOgre::isDeferredShadingSystemEnabled() const
        {
            return false;
        }

        void CGraphicsSystemOgre::addDeferredShadingSystem( bool enabled, SmartPtr<IViewport> vp )
        {
        }

        SmartPtr<IDeferredShadingSystem> CGraphicsSystemOgre::addDeferredShadingSystem(
            SmartPtr<IViewport> vp )
        {
            SmartPtr<CDeferredShadingSystem> deferredShadingSystem( new CDeferredShadingSystem( vp ) );

            m_deferredShadingSystems.push_back( deferredShadingSystem );
            return deferredShadingSystem;
        }

        SmartPtr<IMeshManager> CGraphicsSystemOgre::getMeshManager() const
        {
            return m_meshManager;
        }

        void CGraphicsSystemOgre::setMeshManager( SmartPtr<IMeshManager> meshManager )
        {
            m_meshManager = meshManager;
        }

        SmartPtr<IWindow> CGraphicsSystemOgre::getDefaultWindow() const
        {
            return m_defaultWindow;
        }

        void CGraphicsSystemOgre::setDefaultWindow( SmartPtr<IWindow> defaultWindow )
        {
            m_defaultWindow = defaultWindow;
        }

        void CGraphicsSystemOgre::messagePump()
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MSG msg;
            while( PeekMessage( &msg, nullptr, 0U, 0U, PM_REMOVE ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_FREEBSD
#    ifndef OGRE_CONFIG_UNIX_NO_X11
            // GLX Message Pump
            auto windows = getWindows();

            xcb_connection_t *xcbConnection = 0;

            if( !windows.empty() )
                windows.front()->getCustomAttribute( "xcb_connection_t", &xcbConnection );

            if( !xcbConnection )
            {
                // Uses the older Xlib
                auto win = windows.begin();
                auto end = windows.end();

                Display *xDisplay = 0;  // same for all windows

                for( ; win != end; win++ )
                {
                    XID xid;
                    XEvent event;

                    if( !xDisplay )
                        ( *win )->getCustomAttribute( "XDISPLAY", &xDisplay );

                    ( *win )->getCustomAttribute( "WINDOW", &xid );

                    while( XCheckWindowEvent(
                        xDisplay, xid, StructureNotifyMask | VisibilityChangeMask | FocusChangeMask,
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

        void CGraphicsSystemOgre::removeDeferredShadingSystem( SmartPtr<IViewport> vp )
        {
        }

        Array<SmartPtr<IDeferredShadingSystem>> CGraphicsSystemOgre::getDeferredShadingSystems() const
        {
            return m_deferredShadingSystems;
        }

        SmartPtr<ISpriteRenderer> CGraphicsSystemOgre::getSpriteRenderer() const
        {
            static SmartPtr<ISpriteRenderer> ptr;
            return ptr;
        }

        SmartPtr<IFontManager> CGraphicsSystemOgre::getFontManager() const
        {
            return m_fontManager;
        }

        void CGraphicsSystemOgre::setupRenderer( SmartPtr<IGraphicsScene> sceneManager,
                                                 SmartPtr<IWindow> window, SmartPtr<ICamera> camera,
                                                 String workspaceName, bool val )
        {
            try
            {
                auto textureManager = Ogre::TextureManager::getSingletonPtr();
                if( textureManager )
                {
                    textureManager->setDefaultNumMipmaps( 4 );
                }

                // m_particlePlugin->install();

                auto materialManager = Ogre::MaterialManager::getSingletonPtr();
                if( materialManager )
                {
                    auto materialListener = new MaterialListener();
                    materialManager->addListener( materialListener );
                }

                if( getUseRTSS() )
                {
                    auto shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
                    if( shaderGenerator )
                    {
                        auto pSceneManager = shaderGenerator->getActiveSceneManager();
                        if( !pSceneManager )
                        {
                            sceneManager->_getObject( (void **)&pSceneManager );
                            shaderGenerator->addSceneManager( pSceneManager );
                        }

                        shaderGenerator->validateScheme(
                            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME );
                    }
                }

                // auto task = Thread::getCurrentTask();
                // auto renderTask = getRenderTask();

                // if( task == renderTask )
                //{
                //     m_resourceGroupManager->load( nullptr );
                // }
                // else
                //{
                //     m_loadQueue.push( m_resourceGroupManager );
                // }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool CGraphicsSystemOgre::isValid() const
        {
            const auto &loadingState = getLoadingState();

            switch( loadingState )
            {
            case LoadingState::Allocated:
            {
                return true;
            }
            break;
            case LoadingState::Loaded:
            {
                return m_root != nullptr;
            }
            break;
            case LoadingState::Unloaded:
            {
                return m_root == nullptr;
            }
            break;
            }

            return true;
        }

        IGraphicsSystem::RenderApi CGraphicsSystemOgre::getRenderApi() const
        {
            return m_renderApi;
        }

        void CGraphicsSystemOgre::setRenderApi( RenderApi renderApi )
        {
            m_renderApi = renderApi;
        }

        SmartPtr<IMeshConverter> CGraphicsSystemOgre::getMeshConverter() const
        {
            return m_meshConverter;
        }

        void CGraphicsSystemOgre::setMeshConverter( SmartPtr<IMeshConverter> meshConverter )
        {
            m_meshConverter = meshConverter;
        }

        Thread::Task CGraphicsSystemOgre::getStateTask() const
        {
            return Thread::Task::Render;
        }

        Thread::Task CGraphicsSystemOgre::getRenderTask() const
        {
            return Thread::Task::Render;
        }

        bool CGraphicsSystemOgre::getUseRTSS() const
        {
            return m_useRTSS;
        }

        void CGraphicsSystemOgre::setUseRTSS( bool useRTSS )
        {
            m_useRTSS = useRTSS;
        }

        SmartPtr<IDebug> CGraphicsSystemOgre::getDebug() const
        {
            return m_debug;
        }

        void CGraphicsSystemOgre::setDebug( SmartPtr<IDebug> debug )
        {
            m_debug = debug;
        }

        void CGraphicsSystemOgre::loadObject( SmartPtr<ISharedObject> graphicsObject, bool forceQueue )
        {
#if 0
            ScopeLock lock( this );

            if( !graphicsObject->isLoaded() )
            {
                graphicsObject->load( nullptr );
            }
#else
            FB_ASSERT( graphicsObject );

            const auto &loadingState = getLoadingState();
            if( loadingState == LoadingState::Loaded )
            {
                if( forceQueue || !Thread::getTaskFlag( Thread::Render_Flag ) )
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
                    if( !graphicsObject->isLoaded() )
                    {
                        graphicsObject->load( nullptr );
                    }
                }
            }
            else
            {
                const auto &graphicsObjectLoadingState = graphicsObject->getLoadingState();
                if( !( graphicsObjectLoadingState == LoadingState::Loading ||
                       graphicsObjectLoadingState == LoadingState::Loaded ) )
                {
                    m_loadQueue.push( graphicsObject );
                }
            }
#endif
        }

        void CGraphicsSystemOgre::unloadObject( SmartPtr<ISharedObject> graphicsObject, bool forceQueue )
        {
#if 0
            ScopeLock lock( this );

            if( graphicsObject->isLoaded() )
            {
                graphicsObject->unload( nullptr );
            }
#else
            FB_ASSERT( graphicsObject );

            auto applicationManager = core::IApplicationManager::instance();
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
                    auto renderTask = getRenderTask();
                    auto currentTask = Thread::getCurrentTask();
                    if( forceQueue || currentTask != renderTask )
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

        Array<SmartPtr<IWindow>> CGraphicsSystemOgre::getWindows() const
        {
            return m_windows;
        }

        bool CGraphicsSystemOgre::AppFrameListener::frameStarted( const Ogre::FrameEvent &evt )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            //auto event = factoryManager->make_ptr<CFrameEvent>();
            //event->setTime( m_time );
            //event->setTimeInterval( evt.timeSinceLastFrame );
            //event->setType( FRAME_EVENT_PRE_RENDER );

            //auto listeners = m_graphicsSystem->getFrameListeners();
            //for( u32 i = 0; i < listeners.size(); ++i )
            //{
            //    SmartPtr<IFrameListener> listener = listeners[i];
            //    listener->frameEvent( event );
            //}

            return true;
        }

        bool CGraphicsSystemOgre::AppFrameListener::frameRenderingQueued( const Ogre::FrameEvent &evt )
        {
            m_time += evt.timeSinceLastFrame;

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            //auto event = factoryManager->make_ptr<CFrameEvent>();
            //event->setTime( m_time );
            //event->setTimeInterval( evt.timeSinceLastFrame );
            //event->setType( FRAME_EVENT_RENDER_QUEUED );

            //auto listeners = m_graphicsSystem->getFrameListeners();
            //for( u32 i = 0; i < listeners.size(); ++i )
            //{
            //    SmartPtr<IFrameListener> listener = listeners[i];
            //    listener->frameEvent( event );
            //}

            return true;
        }

        bool CGraphicsSystemOgre::AppFrameListener::frameEnded( const Ogre::FrameEvent &evt )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            //auto event = factoryManager->make_ptr<CFrameEvent>();
            //event->setTime( m_time );
            //event->setTimeInterval( evt.timeSinceLastFrame );
            //event->setType( FRAME_EVENT_POST_RENDER );

            //auto listeners = m_graphicsSystem->getFrameListeners();
            //for( u32 i = 0; i < listeners.size(); ++i )
            //{
            //    SmartPtr<IFrameListener> listener = listeners[i];
            //    listener->frameEvent( event );
            //}

            return true;
        }

        CGraphicsSystemOgre::AppFrameListener::~AppFrameListener()
        {
        }

        CGraphicsSystemOgre::AppFrameListener::AppFrameListener( CGraphicsSystemOgre *graphicsSystem ) :
            m_graphicsSystem( graphicsSystem ),
            m_time( 0.0f )
        {
        }

        CGraphicsSystemOgre::RenderQueueListener::RenderQueueListener(
            CGraphicsSystemOgre *graphicsSystem ) :
            m_graphicsSystem( graphicsSystem )
        {
        }

        CGraphicsSystemOgre::RenderQueueListener::~RenderQueueListener()
        {
            m_graphicsSystem = nullptr;
        }

        void CGraphicsSystemOgre::RenderQueueListener::renderQueueStarted(
            Ogre::uint8 queueGroupId, const Ogre::String &invocation, bool &skipThisInvocation )
        {
            if( queueGroupId == Ogre::RENDER_QUEUE_OVERLAY )
            {
                Ogre::Viewport *vp = Ogre::Root::getSingletonPtr()->getRenderSystem()->_getViewport();
                if( vp != nullptr )
                {
                    Ogre::SceneManager *sceneMgr = vp->getCamera()->getSceneManager();
                    if( vp->getOverlaysEnabled() )
                    {
                        Ogre::OverlayManager::getSingleton()._queueOverlaysForRendering(
                            vp->getCamera(), sceneMgr->getRenderQueue(), vp );
                    }
                }
            }
        }

        bool CGraphicsSystemOgre::WindowEventListener::windowClosing( Ogre::RenderWindow *rw )
        {
            auto applicationManager = core::IApplicationManager::instance();
            applicationManager->setRunning( false );
            return false;
        }

        CGraphicsSystemOgre::WindowEventListener::~WindowEventListener()
        {
        }

        CGraphicsSystemOgre::WindowEventListener::WindowEventListener()
        {
        }

    }  // end namespace render
}  // end namespace fb
