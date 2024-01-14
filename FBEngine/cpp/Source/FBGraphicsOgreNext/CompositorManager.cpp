#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/CompositorManager.h>
#include <FBGraphicsOgreNext/Compositor.h>
#include <FBGraphicsOgreNext/Wrapper/CGraphicsSystemOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CSceneManagerOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CCameraOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CWindowOgreNext.h>
#include <FBGraphicsOgreNext/Terra/Terra.h>
#include <FBGraphicsOgreNext/Terra/TerraShadowMapper.h>
#include <FBCore/FBCore.h>
#include <FBGraphicsOgreNext/DepthOfFieldEffect.h>
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <OgreTextureGpuManager.h>
#include <OgreWindow.h>

#include "CompositorPassProvider.h"

namespace fb::render
{

    FB_CLASS_REGISTER_DERIVED( fb, CompositorManager, ISharedObject );

    CompositorManager::CompositorManager()
    {
        if( auto handle = getHandle() )
        {
            handle->setName( "CompositorManager" );
        }

        // createEffects();
        // createDOFCompositor();
    }

    CompositorManager::~CompositorManager()
    {
        unload( nullptr );
    }

    void CompositorManager::setupRenderer( SmartPtr<IGraphicsScene> pISceneManager,
                                           SmartPtr<IWindow> pIWindow, SmartPtr<ICamera> pICamera,
                                           String workspaceName, bool val )
    {
        //FB_ASSERT( m_compositorManager );
        //m_compositorManager->setSceneManager( sceneManager );
        //m_compositorManager->setWindow( window );
        //m_compositorManager->setCamera( camera );
        //m_compositorManager->setWorkspaceName( workspaceName );
        //m_compositorManager->setEnabled( val );
        //loadObject( m_compositorManager, true );

        auto applicationManager = core::ApplicationManager::instance();
        auto graphicsSystem =
            fb::static_pointer_cast<CGraphicsSystemOgreNext>( applicationManager->getGraphicsSystem() );
        auto pCompositorManager =
            fb::static_pointer_cast<CompositorManager>( graphicsSystem->getCompositorManager() );

        auto pSceneManager = fb::dynamic_pointer_cast<CSceneManagerOgreNext>( pISceneManager );
        auto ogreCamera = fb::dynamic_pointer_cast<CCameraOgreNext>( pICamera );
        auto ogreWindow = fb::dynamic_pointer_cast<CWindowOgreNext>( pIWindow );

        auto sceneManager = pSceneManager->getSceneManager();
        auto renderWindow = ogreWindow->getWindow();
        auto camera = ogreCamera->getCamera();

        using namespace Ogre;

        // Setup a basic compositor with a blue clear colour
        auto root = Root::getSingletonPtr();
        auto compositorManager = root->getCompositorManager2();

        const ColourValue backgroundColour( 0.2f, 0.4f, 0.6f );
        compositorManager->createBasicWorkspaceDef( workspaceName, backgroundColour, IdString() );
        compositorManager->addWorkspace( sceneManager, renderWindow->getTexture(), camera, workspaceName,
                                         true );
    }

    void CompositorManager::load( SmartPtr<ISharedObject> data )
    {
        try
        {
            auto applicationManager = core::ApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto factoryManager = applicationManager->getFactoryManager();
            FB_ASSERT( factoryManager );

            auto root = Ogre::Root::getSingletonPtr();
            auto compoProvider = OGRE_NEW CompositorPassProvider();
            auto compositorManager = root->getCompositorManager2();
            compositorManager->setCompositorPassProvider( compoProvider );

            //setupRenderer( getSceneManager(), getWindow(), getCamera(), "__Demo__", true );

            //auto compositor = factoryManager->make_ptr<Compositor>();
            //compositor->setWindow( window );
            //compositor->setSceneManager( m_creator );
            //compositor->setCamera( this );
            //graphicsSystem->loadObject( compositor, true );
            //setCompositor( compositor );
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CompositorManager::reload( SmartPtr<ISharedObject> data )
    {
        try
        {
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CompositorManager::unload( SmartPtr<ISharedObject> data )
    {
        try
        {
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CompositorManager::registerCompositors( SmartPtr<IViewport> viewport )
    {
        try
        {
            Ogre::Viewport *vp;
            viewport->_getObject( reinterpret_cast<void **>( &vp ) );
            FB_ASSERT_TRUE( !vp );
            if( !vp )
            {
                FB_EXCEPTION( "Viewport not found." );
            }

            // hvListener = new HeatVisionListener();
            // hdrListener = new HDRListener();
            ////gaussianListener = new GaussianListener();

            // mCompositorSelectorViewManager = new ItemSelectorViewManager("CompositorSelectorWin");
            //  tell view manager to notify us when an item changes selection state
            // mCompositorSelectorViewManager->setItemSelectorController(this);
            // iterate through Compositor Managers resources and add name keys ast Item selectors to
            // Compositor selector view manager Ogre::CompositorManager::ResourceMapIterator
            // resourceIterator = 	Ogre::CompositorManager::getSingleton().getResourceIterator();

            //// add all compositor resources to the view container
            // while (resourceIterator.hasMoreElements())
            //{
            //	Ogre::ResourcePtr resource = resourceIterator.getNext();
            //	const Ogre::String& compositorName = resource->getName();
            //	// Don't add base Ogre/Scene compositor to view
            //	if (compositorName == "Ogre/Scene")
            //		continue;

            //	//mCompositorSelectorViewManager->addItemSelector(compositorName);
            //	int addPosition = -1;
            //	if (compositorName.find("HDR") != -1)
            //	{
            //		// HDR must be first in the chain
            //		addPosition = 0;

            //		if(!hdrListener)
            //			hdrListener = new HDRListener();
            //	}

            //	//Ogre::CompositorInstance *instance =
            //Ogre::CompositorManager::getSingleton().addCompositor(vp, compositorName, addPosition);
            //	//Ogre::CompositorManager::getSingleton().setCompositorEnabled(vp, compositorName,
            //false);

            //	// special handling for Heat Vision which uses a listener
            //	if(instance && (compositorName == "Heat Vision"))
            //	{
            //		//instance->addListener(hvListener);
            //	}
            //	else if(instance && (compositorName == "HDR"))
            //	{
            //		instance->addListener(hdrListener);
            //		hdrListener->notifyViewportSize(vp->getActualWidth(), vp->getActualHeight());
            //		hdrListener->notifyCompositor(instance);
            //	}
            //	else if(instance && (compositorName == "Gaussian Blur"))
            //	{
            //		//instance->addListener(gaussianListener);
            //		//gaussianListener->notifyViewportSize(vp->getActualWidth(),
            //vp->getActualHeight());
            //	}
            //}
        }
        catch( std::exception &e )
        {
            FB_LOG_EXCEPTION( e );
        }
    }

    void CompositorManager::registerCompositor( SmartPtr<IViewport> viewport,
                                                const String &compositorName )
    {
        Ogre::Viewport *vp;
        viewport->_getObject( reinterpret_cast<void **>( &vp ) );
        FB_ASSERT_TRUE( !vp );
        if( !vp )
        {
            FB_EXCEPTION( "Viewport not found." );
        }

        // Ogre::CompositorManager* compositorManager = Ogre::CompositorManager::getSingletonPtr();

        // int addPosition = -1;
        // if (compositorName.find("HDR") != -1)
        //{
        //	// HDR must be first in the chain
        //	addPosition = 0;

        //	if(!hdrListener)
        //		hdrListener = new HDRListener();
        //}

        // Ogre::CompositorInstance *instance = compositorManager->addCompositor(vp,
        // compositorName.c_str(), addPosition);

        // if(!instance)
        //	return;

        //// special handling for Heat Vision which uses a listener
        // if(compositorName == "Heat Vision")
        //{
        //	//instance->addListener(hvListener);
        // }
        // else if(compositorName.find("HDR") != -1)
        //{
        //	instance->addListener(hdrListener);
        //	hdrListener->notifyViewportSize(vp->getActualWidth(), vp->getActualHeight());
        //	hdrListener->notifyCompositor(instance);
        // }
        // else if(compositorName == "DoF_Compositor")
        //{
        //	DepthOfFieldEffect* dofEffect = new DepthOfFieldEffect(vp);
        //	instance->addListener(dofEffect);
        //	m_dofEffects[vp] = dofEffect;
        // }
        // else if(compositorName == "Gaussian Blur")
        //{
        //	//instance->addListener(gaussianListener);
        //	//gaussianListener->notifyViewportSize(vp->getActualWidth(), vp->getActualHeight());
        // }
    }

    void CompositorManager::removeCompositor( SmartPtr<IViewport> viewport,
                                              const String &compositorName )
    {
        Ogre::Viewport *vp;
        viewport->_getObject( reinterpret_cast<void **>( &vp ) );
        FB_ASSERT_TRUE( !vp );
        if( !vp )
        {
            FB_EXCEPTION( "Viewport not found." );
        }

        // Ogre::CompositorManager* compositorMgr = Ogre::CompositorManager::getSingletonPtr();
        // compositorMgr->removeCompositor(vp, compositorName.c_str());
    }

    void CompositorManager::setCompositorEnabled( const String &name, SmartPtr<IViewport> viewport,
                                                  bool enabled )
    {
        // Ogre::Viewport *vp;
        // viewport->_getObject((void**)&vp);
        // FB_ASSERT_TRUE(!vp);

        // Ogre::CompositorManager* compositorManager = Ogre::CompositorManager::getSingletonPtr();
        // compositorManager->setCompositorEnabled(vp, name.c_str(), enabled);
    }

    auto CompositorManager::isCompositorEnabled( const String &name, SmartPtr<IViewport> viewport ) const
        -> bool
    {
        // Ogre::Viewport *vp;
        // viewport->_getObject((void**)&vp);
        // FB_ASSERT_TRUE(!vp);

        // Ogre::CompositorManager* compositorManager = Ogre::CompositorManager::getSingletonPtr();
        // Ogre::CompositorChain* compositorChain = compositorManager->getCompositorChain(vp);
        // Ogre::CompositorInstance* compositorInstance =
        // compositorChain->getCompositor(name.c_str()); return compositorInstance->getEnabled();

        return false;
    }

    void CompositorManager::setCompositorProperties( const String &name, SmartPtr<IViewport> viewport,
                                                     const Properties &properties )
    {
        //Ogre::Viewport *vp;
        //viewport->_getObject( (void **)&vp );
        //FB_ASSERT_TRUE( !vp );

        //if(name == ( "DoF_Compositor" ))
        //{
        //    auto it = m_dofEffects.find( vp );
        //    if(it != m_dofEffects.end())
        //    {
        //        DepthOfFieldEffect *dofEffect = it->second;

        //        f32 nearDepth = 0;
        //        f32 focalDepth = 0;
        //        f32 farDepth = 0;

        //        if(!properties.getPropertyValue( "nearDepth", nearDepth ))
        //            FB_LOG_MESSAGE( "CompositorManager",
        //                        "CCompositorManager::setCompositorProperties - nearDepth "
        //                        "property not found" );

        //        if(!properties.getPropertyValue( "focalDepth", focalDepth ))
        //            FB_LOG_MESSAGE( "CompositorManager",
        //                        "CCompositorManager::setCompositorProperties - focalDepth "
        //                        "property not found" );

        //        if(!properties.getPropertyValue( "farDepth", farDepth ))
        //            FB_LOG_MESSAGE( "CompositorManager",
        //                        "CCompositorManager::setCompositorProperties - farDepth "
        //                        "property not found" );

        //        dofEffect->setFocalDepths( nearDepth, focalDepth, farDepth );
        //    }
        //}
    }

    void CompositorManager::getCompositorProperties( const String &name, SmartPtr<IViewport> viewport,
                                                     Properties &properties ) const
    {
    }

    auto CompositorManager::getSceneManager() const -> SmartPtr<IGraphicsScene>
    {
        return m_sceneManager;
    }

    void CompositorManager::setSceneManager( SmartPtr<IGraphicsScene> sceneManager )
    {
        m_sceneManager = sceneManager;
    }

    auto CompositorManager::getWindow() const -> SmartPtr<IWindow>
    {
        return m_window;
    }

    void CompositorManager::setWindow( SmartPtr<IWindow> window )
    {
        m_window = window;
    }

    auto CompositorManager::getCamera() const -> SmartPtr<ICamera>
    {
        return m_camera;
    }

    void CompositorManager::setCamera( SmartPtr<ICamera> camera )
    {
        m_camera = camera;
    }
}  // namespace fb::render
