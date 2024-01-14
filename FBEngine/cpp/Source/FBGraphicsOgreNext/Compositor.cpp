#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Compositor.h>
#include <FBGraphicsOgreNext/CompositorManager.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsSystemOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CTextureOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CWindowOgreNext.h>
#include <FBGraphicsOgreNext/Terra/Terra.h>
#include <FBGraphicsOgreNext/Terra/TerraShadowMapper.h>
#include <FBCore/FBCore.h>
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <OgreTextureGpuManager.h>
#include <OgreRoot.h>
#include <OgreWindow.h>

#include "Compositor/OgreCompositorManager2.h"
#include "Compositor/OgreCompositorNode.h"
#include "Compositor/OgreCompositorNodeDef.h"
#include "Compositor/OgreCompositorWorkspace.h"
#include "Compositor/OgreCompositorWorkspaceDef.h"
#include "Compositor/Pass/OgreCompositorPassDef.h"
#include "Compositor/Pass/PassQuad/OgreCompositorPassQuadDef.h"
#include "Compositor/Pass/PassQuad/OgreCompositorPassQuadDef.h"
#include "Compositor/Pass/PassScene/OgreCompositorPassSceneDef.h"
#include "OgreDepthBuffer.h"
#include "Terra/Hlms/OgreHlmsTerra.h"
#include "Terra/TerraWorkspaceListener.h"

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, Compositor, SharedGraphicsObject<ISharedObject> );

    u32 Compositor::m_nameExt = 0;

    Compositor::Compositor()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "Compositor" );
        }

        auto applicationManager = core::ApplicationManager::instance();
        FB_ASSERT( applicationManager );

        auto stateManager = applicationManager->getStateManager();
        auto graphicsSystem = applicationManager->getGraphicsSystem();
        auto factoryManager = applicationManager->getFactoryManager();

        FB_ASSERT( stateManager );
        FB_ASSERT( graphicsSystem );
        FB_ASSERT( factoryManager );

        auto stateContext = stateManager->addStateObject();
        FB_ASSERT( stateContext );

        setStateContext( stateContext );

        auto stateListener = factoryManager->make_ptr<StateListener>();
        stateListener->setOwner( this );
        setStateListener( stateListener );
        stateContext->addStateListener( stateListener );

        auto state = factoryManager->make_ptr<CompositorState>();
        state->setTaskId( Thread::Task::Render );
        stateContext->addState( state );
        stateContext->setOwner( this );
    }

    Compositor::~Compositor()
    {
        unload( nullptr );
        destroyStateContext();
    }

    void Compositor::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            if( !isLoaded() )
            {
                setLoadingState( LoadingState::Loading );

                auto applicationManager = core::ApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                ISharedObject::ScopedLock lock( graphicsSystem );

                // m_compositorWorkspace = setupTestCompositor();
                m_compositorWorkspace = setupBasicCompositor();
                //m_compositorWorkspace = setupTerrainCompositor();

                auto enabled = isEnabled();
                setupEnabled( enabled );

                setLoadingState( LoadingState::Loaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Compositor::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setLoadingState( LoadingState::Unloading );

            //m_compositorWorkspace = setupTestCompositor();
            m_compositorWorkspace = setupBasicCompositor();
            //m_compositorWorkspace = setupTerrainCompositor();

            setLoadingState( LoadingState::Loaded );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Compositor::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
            setSceneManager( nullptr );
            setCamera( nullptr );
            setWindow( nullptr );

            destroyStateContext();

            if( isLoaded() )
            {
                setLoadingState( LoadingState::Unloading );

                auto applicationManager = core::ApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                ISharedObject::ScopedLock lock( graphicsSystem );

                auto root = Ogre::Root::getSingletonPtr();
                auto pCompositorManager = root->getCompositorManager2();

                if( m_compositorWorkspace )
                {
                    pCompositorManager->removeWorkspace( m_compositorWorkspace );
                    m_compositorWorkspace = nullptr;
                }

                SharedGraphicsObject<ISharedObject>::unload( data );
                setLoadingState( LoadingState::Unloaded );
            }
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void Compositor::setEnabled( bool enabled )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<CompositorState>() )
            {
                state->setEnabled( enabled );
            }
        }
    }

    void Compositor::stopCompositor()
    {
        FB_ASSERT( Thread::getCurrentTask() == Thread::Task::Render );

        if( m_compositorWorkspace )
        {
            m_compositorWorkspace->removeListener( mTerraWorkspaceListener );
        }

        delete mTerraWorkspaceListener;
        mTerraWorkspaceListener = nullptr;
    }

    auto Compositor::setupEnabled( bool enabled ) -> bool
    {
        try
        {
            //FB_ASSERT( Thread::getCurrentTask() == Thread::Task::Render );

            auto applicationManager = core::ApplicationManager::instance();

            auto pGraphicsSystem = applicationManager->getGraphicsSystem();
            auto graphicsSystem = fb::static_pointer_cast<CGraphicsSystemOgreNext>( pGraphicsSystem );

            ISharedObject::ScopedLock lock( graphicsSystem );

            auto root = Ogre::Root::getSingletonPtr();
            auto *pOgreCompositorManager = root->getCompositorManager2();

            if( enabled )
            {
                if( !m_compositorWorkspace )
                {
                    //if( StringUtil::isNullOrEmpty( m_workspaceName ) )
                    //{
                    //    setupTestCompositor();
                    //    //setupTerrainCompositor();
                    //}

                    auto compositorManager = graphicsSystem->getCompositorManager();

                    auto pCamera = getCamera();

                    auto viewport = pCamera->getViewport();

                    auto pSceneManager =
                        fb::dynamic_pointer_cast<CSceneManagerOgreNext>( getSceneManager() );
                    auto ogreCamera = fb::dynamic_pointer_cast<CCameraOgreNext>( pCamera );
                    auto ogreWindow = fb::dynamic_pointer_cast<CWindowOgreNext>( getWindow() );

                    auto sceneManager = pSceneManager->getSceneManager();
                    auto renderWindow = ogreWindow->getWindow();
                    auto camera = ogreCamera->getCamera();

                    SmartPtr<CTextureOgreNext> targetTexture;
                    if( camera )
                    {
                        targetTexture = ogreCamera->getEditorTexture();
                    }

                    if( !targetTexture )
                    {
                        targetTexture = ogreCamera->getTargetTexture();
                    }

                    auto workspaceName = getWorkspaceName();
                    if( StringUtil::isNullOrEmpty( workspaceName ) )
                    {
                        workspaceName = StringUtil::getUUID();
                    }

                    if( targetTexture )
                    {
                        if( targetTexture->isLoaded() )
                        {
                            auto texture = targetTexture->getTexture();
                            if( texture )
                            {
                                m_compositorWorkspace = pOgreCompositorManager->addWorkspace(
                                    sceneManager, texture, camera, workspaceName, true );
                                FB_ASSERT( m_compositorWorkspace );
                            }
                            else
                            {
                                FB_LOG_ERROR( "Compositor::setEnabled: Texture null." );

                                auto w = pOgreCompositorManager->addWorkspace(
                                    sceneManager, renderWindow->getTexture(), camera, workspaceName,
                                    true );
                                m_compositorWorkspace = w;
                                FB_ASSERT( m_compositorWorkspace );
                            }

                            return true;
                        }
                    }
                    else
                    {
                        auto w = pOgreCompositorManager->addWorkspace(
                            sceneManager, renderWindow->getTexture(), camera, workspaceName, true );
                        m_compositorWorkspace = w;
                        FB_ASSERT( m_compositorWorkspace );

                        return true;
                    }
                }
                else
                {
                    //m_scenePassDef->mVisibilityMask = Ogre::VisibilityFlags::RESERVED_VISIBILITY_FLAGS;
                    m_compositorWorkspace->setEnabled( true );
                    return true;
                }
            }
            else
            {
                if( m_compositorWorkspace )
                {
                    //m_scenePassDef->mVisibilityMask = 0;
                    m_compositorWorkspace->setEnabled( false );
                    //pOgreCompositorManager->removeWorkspace( m_compositorWorkspace );

                    //m_compositorWorkspace = nullptr;
                }

                if( m_compositorWorkspace )
                {
                    pOgreCompositorManager->removeWorkspace( m_compositorWorkspace );
                    m_compositorWorkspace = nullptr;
                }

                return true;
            }

            return false;
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }

        return false;
    }

    auto Compositor::isEnabled() const -> bool
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<CompositorState>() )
            {
                return state->isEnabled();
            }
        }

        return false;
    }

    auto Compositor::getCompositorWorkspace() const -> Ogre::CompositorWorkspace *
    {
        return m_compositorWorkspace;
    }

    void Compositor::setCompositorWorkspace( Ogre::CompositorWorkspace *compositorWorkspace )
    {
        m_compositorWorkspace = compositorWorkspace;
    }

    auto Compositor::getSceneManager() const -> SmartPtr<IGraphicsScene>
    {
        auto p = m_sceneManager.load();
        return p.lock();
    }

    void Compositor::setSceneManager( SmartPtr<IGraphicsScene> sceneManager )
    {
        m_sceneManager = sceneManager;
    }

    auto Compositor::getWindow() const -> SmartPtr<IWindow>
    {
        auto p = m_window.load();
        return p.lock();
    }

    void Compositor::setWindow( SmartPtr<IWindow> window )
    {
        m_window = window;
    }

    auto Compositor::getCamera() const -> SmartPtr<ICamera>
    {
        auto p = m_camera.load();
        return p.lock();
    }

    void Compositor::setCamera( SmartPtr<ICamera> camera )
    {
        m_camera = camera;
    }

    auto Compositor::getWorkspaceName() const -> String
    {
        return m_workspaceName;
    }

    void Compositor::setWorkspaceName( const String &workspaceName )
    {
        m_workspaceName = workspaceName;
    }

    auto Compositor::setupTestCompositor() -> Ogre::CompositorWorkspace *
    {
        FB_ASSERT( Thread::getCurrentTask() == Thread::Task::Render );

        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem =
            fb::static_pointer_cast<CGraphicsSystemOgreNext>( applicationManager->getGraphicsSystem() );
        auto pCompositorManager =
            fb::static_pointer_cast<CompositorManager>( graphicsSystem->getCompositorManager() );

        auto pCamera = getCamera();

        auto pSceneManager = fb::dynamic_pointer_cast<CSceneManagerOgreNext>( getSceneManager() );
        auto ogreCamera = fb::dynamic_pointer_cast<CCameraOgreNext>( pCamera );
        auto ogreWindow = fb::dynamic_pointer_cast<CWindowOgreNext>( getWindow() );

        auto sceneManager = pSceneManager->getSceneManager();
        auto renderWindow = ogreWindow->getWindow();
        auto camera = ogreCamera->getCamera();

        using namespace Ogre;

        // Setup a basic compositor with a blue clear colour
        auto root = Root::getSingletonPtr();
        auto compositorManager = root->getCompositorManager2();

        auto workspaceName = getWorkspaceName();
        const ColourValue backgroundColour( 0.2f, 0.4f, 0.6f );
        //compositorManager->createBasicWorkspaceDef( workspaceName, backgroundColour, IdString("MainShadowNode") );
        return compositorManager->addWorkspace( sceneManager, renderWindow->getTexture(), camera,
                                                "PbsMaterialsWorkspace", true );
    }

    auto Compositor::getProperties() const -> fb::SmartPtr<fb::Properties>
    {
        auto properties = SharedGraphicsObject<ISharedObject>::getProperties();

        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<CompositorState>() )
            {
                properties->setProperty( "isEnabled", state->isEnabled() );
            }
        }

        return properties;
    }

    void Compositor::setProperties( SmartPtr<Properties> properties )
    {
        if( auto stateContext = getStateContext() )
        {
            if( auto state = stateContext->getStateByType<CompositorState>() )
            {
                bool enabled = false;
                properties->getPropertyValue( "isEnabled", enabled );

                setEnabled( enabled );
            }
        }
    }

    auto Compositor::getChildObjects() const -> fb::Array<fb::SmartPtr<fb::ISharedObject>>
    {
        auto objects = SharedGraphicsObject<ISharedObject>::getChildObjects();
        //objects.push_back( getCamera() );
        objects.emplace_back( getWindow() );
        objects.emplace_back( getSceneManager() );
        return objects;
    }

    auto Compositor::setupBasicCompositor() -> Ogre::CompositorWorkspace *
    {
        //FB_ASSERT(Thread::getCurrentTask() == Thread::Task::Render);

#if 0
			using namespace Ogre;
			auto root = Ogre::Root::getSingletonPtr();
			auto compositorManager = root->getCompositorManager2();

			auto applicationManager = core::ApplicationManager::instance();
			auto graphicsSystem = fb::static_pointer_cast<CGraphicsSystemOgreNext>(applicationManager->getGraphicsSystem());
			auto pCompositorManager = fb::static_pointer_cast<CCompositorManager>(graphicsSystem->getCompositorManager());

			auto pSceneManager = fb::dynamic_pointer_cast<CSceneManagerOgre>(pCompositorManager->getSceneManager());
			auto ogreCamera = fb::dynamic_pointer_cast<CCameraOgre>(pCompositorManager->getCamera());
			auto ogreWindow = fb::dynamic_pointer_cast<CWindowOgre>(pCompositorManager->getWindow());

			auto sceneManager = pSceneManager->getSceneManager();
			auto renderWindow = ogreWindow->getWindow();
			auto camera = ogreCamera->getCamera();

			if (!compositorManager->hasNodeDefinition("Motion Blur"))
			{
				/// Motion blur effect
				CompositorNodeDef* motionBlurDef = compositorManager->addNodeDefinition("Motion Blur");

				// Input channels
				motionBlurDef->addTextureSourceName("rt_input", 0, TextureDefinitionBase::TEXTURE_INPUT);
				motionBlurDef->addTextureSourceName("rt_output", 1, TextureDefinitionBase::TEXTURE_INPUT);

				motionBlurDef->mCustomIdentifier = "Ogre/Postprocess";

				// Local textures
				motionBlurDef->setNumLocalTextureDefinitions(1);
				{
					TextureDefinitionBase::TextureDefinition* texDef =
						motionBlurDef->addTextureDefinition("sum");
					texDef->width = 0;
					texDef->height = 0;
					texDef->format = Ogre::PFG_RGBA8_UNORM_SRGB;
					texDef->textureFlags &= (uint32)~TextureFlags::DiscardableContent;

					RenderTargetViewDef* rtv = motionBlurDef->addRenderTextureView("sum");
					RenderTargetViewEntry attachment;
					attachment.textureName = "sum";
					rtv->colourAttachments.push_back(attachment);
					rtv->depthBufferId = Ogre::DepthBuffer::POOL_NO_DEPTH;
				}

				motionBlurDef->setNumTargetPass(3);

				/// Initialisation pass for sum texture
				{
					CompositorTargetDef* targetDef = motionBlurDef->addTargetPass("sum");
					{
						CompositorPassQuadDef* passQuad;
						passQuad = static_cast<CompositorPassQuadDef*>(targetDef->addPass(PASS_QUAD));
						passQuad->mNumInitialPasses = 1;
						passQuad->mMaterialName = "Ogre/Copy/4xFP32";
						passQuad->addQuadTextureSource(0, "rt_input");
					}
				}
				/// Do the motion blur
				{
					CompositorTargetDef* targetDef = motionBlurDef->addTargetPass("rt_output");

					{
						CompositorPassQuadDef* passQuad;
						passQuad = static_cast<CompositorPassQuadDef*>(targetDef->addPass(PASS_QUAD));
						passQuad->mMaterialName = "Postprocess/Combine";
						passQuad->addQuadTextureSource(0, "rt_input");
						passQuad->addQuadTextureSource(1, "sum");
					}
				}
				/// Copy back sum texture for the next frame
				{
					CompositorTargetDef* targetDef = motionBlurDef->addTargetPass("sum");

					{
						CompositorPassQuadDef* passQuad;
						passQuad = static_cast<CompositorPassQuadDef*>(targetDef->addPass(PASS_QUAD));
						passQuad->mMaterialName = "Ogre/Copy/4xFP32";
						passQuad->addQuadTextureSource(0, "rt_output");
					}
				}

				// Output channels
				motionBlurDef->setNumOutputChannels(2);
				motionBlurDef->mapOutputChannel(0, "rt_output");
				motionBlurDef->mapOutputChannel(1, "rt_input");
			}

			auto w = compositorManager->addWorkspace(sceneManager, renderWindow->getTexture(), camera, "Motion Blur", true);
			return w;

#elif 1
        using namespace Ogre;

        auto applicationManager = core::ApplicationManager::instance();

        auto pGraphicsSystem = applicationManager->getGraphicsSystem();
        auto graphicsSystem = fb::static_pointer_cast<CGraphicsSystemOgreNext>( pGraphicsSystem );

        auto compositorManager = graphicsSystem->getCompositorManager();

        auto pCamera = getCamera();

        auto viewport = pCamera->getViewport();

        auto renderScene = true;
        auto showOverlays = true;
        auto showUI = true;
        auto bgColour = ColourF::Blue;
        auto mask = Ogre::VisibilityFlags::RESERVED_VISIBILITY_FLAGS;

        if( viewport )
        {
            renderScene = viewport->getEnableSceneRender();
            showOverlays = viewport->getOverlaysEnabled();
            showUI = viewport->getEnableUI();
            bgColour = viewport->getBackgroundColour();
            mask = viewport->getVisibilityMask();
        }

        auto pSceneManager = fb::dynamic_pointer_cast<CSceneManagerOgreNext>( getSceneManager() );
        auto ogreCamera = fb::dynamic_pointer_cast<CCameraOgreNext>( pCamera );
        auto ogreWindow = fb::dynamic_pointer_cast<CWindowOgreNext>( getWindow() );

        auto sceneManager = pSceneManager->getSceneManager();
        auto renderWindow = ogreWindow->getWindow();
        auto camera = ogreCamera->getCamera();

        //// Setup a basic compositor with a blue clear colour
        auto root = Root::getSingletonPtr();
        auto *pOgreCompositorManager = root->getCompositorManager2();
        auto workspaceName = getWorkspaceName();

        const ColourValue backgroundColour( bgColour.r, bgColour.g, bgColour.b );

        auto shadowNodeName = IdString( "MainShadowNode" );

#    if 0
			pOgreCompositorManager->createBasicWorkspaceDef(workspaceName, backgroundColour, shadowNodeName);
#    else
        CompositorNodeDef *nodeDef = pOgreCompositorManager->addNodeDefinition(
            "AutoGen " + IdString( workspaceName + "/Node" ).getReleaseText() );

        SmartPtr<CTextureOgreNext> targetTexture;
        if( camera )
        {
            targetTexture = ogreCamera->getEditorTexture();
        }

        // Input texture
        nodeDef->addTextureSourceName( "WindowRT", 0, TextureDefinitionBase::TEXTURE_INPUT );

        nodeDef->setNumTargetPass( 1 );
        {
            CompositorTargetDef *targetDef = nodeDef->addTargetPass( "WindowRT" );
            targetDef->setNumPasses( 2 );

            {
                {
                    auto passScene =
                        static_cast<CompositorPassSceneDef *>( targetDef->addPass( PASS_SCENE ) );
                    passScene->mShadowNode = shadowNodeName;
                    passScene->setAllClearColours( backgroundColour );
                    passScene->setAllLoadActions( LoadAction::Clear );
                    passScene->mStoreActionDepth = StoreAction::DontCare;
                    passScene->mStoreActionStencil = StoreAction::DontCare;
                    passScene->mIncludeOverlays = showOverlays;

                    if( !renderScene )
                    {
                        passScene->mVisibilityMask = 0;
                    }
                    //else
                    //{
                    //    passScene->mVisibilityMask = mask;
                    //}

                    m_scenePassDef = passScene;
                }

                if( renderScene )
                {
                    auto passProvider = pOgreCompositorManager->getCompositorPassProvider();
                    if( passProvider )
                    {
                        auto uiNodeName = String( "scene_gui" );
                        auto passScene = targetDef->addPass( PASS_CUSTOM, IdString( uiNodeName ) );
                    }
                }

                if( showUI )
                {
                    auto passProvider = pOgreCompositorManager->getCompositorPassProvider();
                    if( passProvider )
                    {
                        auto uiNodeName = String( "app_gui" );
                        auto passScene = targetDef->addPass( PASS_CUSTOM, IdString( uiNodeName ) );
                    }
                }
            }
        }

        CompositorWorkspaceDef *workDef =
            pOgreCompositorManager->addWorkspaceDefinition( workspaceName );
        workDef->connectExternal( 0, nodeDef->getName(), 0 );
#    endif

        return nullptr;

        //setupEnabled( true );
        //return m_compositorWorkspace;

#elif 0
        using namespace Ogre;

        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem =
            fb::static_pointer_cast<CGraphicsSystemOgreNext>( applicationManager->getGraphicsSystem() );
        auto compositorManager =
            fb::static_pointer_cast<CompositorManager>( graphicsSystem->getCompositorManager() );

        auto pSceneManager = fb::dynamic_pointer_cast<CSceneManagerOgreNext>( getSceneManager() );
        auto ogreCamera = fb::dynamic_pointer_cast<CCameraOgreNext>( getCamera() );
        auto ogreWindow = fb::dynamic_pointer_cast<CWindowOgreNext>( getWindow() );

        auto sceneManager = pSceneManager->getSceneManager();
        auto renderWindow = ogreWindow->getWindow();
        auto camera = ogreCamera->getCamera();

        //// Setup a basic compositor with a blue clear colour
        auto root = Ogre::Root::getSingletonPtr();
        Ogre::CompositorManager2 *pCompositorManager = root->getCompositorManager2();
        const Ogre::ColourValue backgroundColour( 0.2f, 0.4f, 0.6f );

        auto workspaceName = "ui";
        auto workspaceDefinition = pCompositorManager->addWorkspaceDefinition( workspaceName );

        auto uiNodeName = String( "colibri_gui" );
        CompositorNodeDef *uiDef = pCompositorManager->addNodeDefinition( uiNodeName );
        uiDef->addTextureSourceName( "rt_output", 0, TextureDefinitionBase::TEXTURE_INPUT );

        uiDef->setNumTargetPass( 1 );

        auto uiTargetDef = uiDef->addTargetPass( "rt_output" );
        uiTargetDef->setNumPasses( 1 );

        CompositorPassDef *renderPass =
            uiTargetDef->addPass( Ogre::CompositorPassType::PASS_SCENE, IdString( "render_scene" ) );
        renderPass->setAllClearColours( backgroundColour );

        uiDef->setNumOutputChannels( 1 );
        uiDef->mapOutputChannel( 0, "rt_output" );

        SmartPtr<CTextureOgreNext> targetTexture;
        if( camera )
        {
            targetTexture = ogreCamera->getTargetTexture();
        }

        if( targetTexture )
        {
            auto texture = targetTexture->getTexture();
            return pCompositorManager->addWorkspace( sceneManager, texture, camera, workspaceName,
                                                     true );
        }
        else
        {
            auto finalComposition = pCompositorManager->getNodeDefinition( "FinalComposition" );
            if( finalComposition )
            {
                // workspaceDef->connect("colibri_gui", "FinalComposition");
                // auto uiDef = finalComposition->addTargetPass("colibri_gui");
            }

            // auto workDef = pCompositorManager->addWorkspaceDefinition(workspaceName);
            // if (workDef)
            {
                // workspaceDefinition->connectExternal(0, "FinalComposition", 0);
                // workspaceDef->connect()
                // workspaceDefinition->connectExternal(0, uiNodeName, 0);
                workspaceDefinition->connectExternal( 0, uiNodeName, 0 );
            }

            auto w = pCompositorManager->addWorkspace( sceneManager, renderWindow->getTexture(), camera,
                                                       workspaceName, true );
            w->reconnectAllNodes();
            return w;
        }
#endif
    }

    auto Compositor::setupTerrainCompositor() -> Ogre::CompositorWorkspace *
    {
        FB_ASSERT( Thread::getCurrentTask() == Thread::Task::Render );

        using namespace Ogre;

        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem =
            fb::static_pointer_cast<CGraphicsSystemOgreNext>( applicationManager->getGraphicsSystem() );

        auto compositorManager =
            fb::static_pointer_cast<CompositorManager>( graphicsSystem->getCompositorManager() );
        auto pSceneManager = fb::dynamic_pointer_cast<CSceneManagerOgreNext>( getSceneManager() );
        auto ogreCamera = fb::dynamic_pointer_cast<CCameraOgreNext>( getCamera() );
        auto ogreWindow = fb::dynamic_pointer_cast<CWindowOgreNext>( getWindow() );

        // The first time this function gets called Terra is not initialized. This is a very possible
        // scenario i.e. load a level without Terrain, but we still need a workspace to render.
        //
        // Thus we pass a PF_NULL texture to the workspace as a dud that barely consumes any
        // memory (it consumes no GPU memory btw) by specifying PF_NULL. Alternatively you
        // could use a different workspace (either defined in script or programmatically) that
        // doesn't require specifying a second external texture. But using a dud is just simpler.
        //
        // The second time we get called, Terra will be initialized and we can pass the
        // proper external texture filled with the UAV so Ogre can place the right
        // barriers.
        //
        // Note: We *could* delay the creation of the workspace in this sample until Terra
        // is initialized; instead of creating the workspace unnecessarily twice.
        // However we're doing this on purpose to show how to deal with perfectly valid &
        // very common scenarios.
        using namespace Ogre;

        Root *root = Root::getSingletonPtr();

        SceneManager *sceneManager = pSceneManager->getSceneManager();

        Window *renderWindow = ogreWindow->getWindow();
        Camera *camera = ogreCamera->getCamera();
        CompositorManager2 *pCompositorManager = root->getCompositorManager2();

        CompositorWorkspace *oldWorkspace = getCompositorWorkspace();
        if( oldWorkspace )
        {
            TextureGpu *terraShadowTex = oldWorkspace->getExternalRenderTargets()[1];
            pCompositorManager->removeWorkspace( oldWorkspace );
            if( terraShadowTex->getPixelFormat() == PFG_NULL )
            {
                TextureGpuManager *textureManager = root->getRenderSystem()->getTextureGpuManager();
                textureManager->destroyTexture( terraShadowTex );
            }
        }

        CompositorChannelVec externalChannels( 2 );
        // Render window
        externalChannels[0] = renderWindow->getTexture();

        // Terra's Shadow texture
        // ResourceLayoutMap initialLayouts;
        // ResourceAccessMap initialUavAccess;
        if( graphicsSystem->getTerra() )
        {
            // Terra is initialized
            const auto shadowMapper = graphicsSystem->getTerra()->getShadowMapper();
            shadowMapper->fillUavDataForCompositorChannel( &externalChannels[1] );
        }
        else
        {
            // The texture is not available. Create a dummy dud.
            TextureGpuManager *textureManager = root->getRenderSystem()->getTextureGpuManager();
            TextureGpu *nullTex = textureManager->createOrRetrieveTexture(
                "DummyNull", GpuPageOutStrategy::Discard, TextureFlags::ManualTexture,
                TextureTypes::Type2D );
            nullTex->setResolution( 1u, 1u );
            nullTex->setPixelFormat( PFG_R10G10B10A2_UNORM );
            nullTex->scheduleTransitionTo( GpuResidency::Resident );
            externalChannels[1] = nullTex;
        }

        return pCompositorManager->addWorkspace( sceneManager, externalChannels, camera,
                                                 "Tutorial_TerrainWorkspace", true, -1, nullptr );
    }

    Compositor::StateListener::StateListener() = default;

    Compositor::StateListener::~StateListener()
    {
        unload( nullptr );
    }

    void Compositor::StateListener::unload( SmartPtr<ISharedObject> data )
    {
        m_owner = nullptr;
    }

    void Compositor::StateListener::handleStateChanged( const SmartPtr<IStateMessage> &message )
    {
    }

    void Compositor::StateListener::handleStateChanged( SmartPtr<IState> &state )
    {
        auto postProcessingState = fb::static_pointer_cast<CompositorState>( state );
        if( auto owner = getOwner() )
        {
            if( owner->isLoaded() )
            {
                auto enabled = postProcessingState->isEnabled();
                auto ret = owner->setupEnabled( enabled );

                if( ret )
                {
                    state->setDirty( false );
                }
            }
        }
    }

    void Compositor::StateListener::handleQuery( SmartPtr<IStateQuery> &query )
    {
    }

    auto Compositor::StateListener::getOwner() const -> SmartPtr<Compositor>
    {
        auto p = m_owner.load();
        return p.lock();
    }

    void Compositor::StateListener::setOwner( SmartPtr<Compositor> owner )
    {
        m_owner = owner;
    }
}  // namespace fb::render
