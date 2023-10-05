#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CViewportOgre.h>
#include <FBGraphicsOgre/Wrapper/CCameraOgre.h>
#include <FBCore/FBCore.h>
#include <OgreViewport.h>
#include <Ogre.h>
#include <OgreRTShaderSystem.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, CViewportOgre, CViewport );

        u32 CViewportOgre::m_zOrderExt = 0;

        CViewportOgre::CViewportOgre()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                auto stateManager = applicationManager->getStateManager();
                auto threadPool = applicationManager->getThreadPool();
                auto factoryManager = applicationManager->getFactoryManager();

                auto state = factoryManager->make_ptr<ViewportState>();

                auto stateTask = graphicsSystem->getStateTask();
                state->setTaskId( stateTask );

                auto stateObject = stateManager->addStateObject();

                auto viewportStateListener = factoryManager->make_ptr<ViewportStateListener>();
                viewportStateListener->setOwner( this );
                setStateListener( viewportStateListener );

                stateObject->setState( state );
                stateObject->addStateListener( viewportStateListener );
                setStateObject( stateObject );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        CViewportOgre::~CViewportOgre()
        {
            unload( nullptr );
        }

        void CViewportOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                FB_ASSERT( Thread::getTaskFlag( Thread::Render_Flag ) );

                setLoadingState( LoadingState::Loading );

                //auto camera = getCamera();
                //Ogre::Camera *ogreCamera = nullptr;
                //if( camera )
                //{
                //    camera->_getObject( (void **)&ogreCamera );
                //}

                //auto ZOrder = getZOrder();
                //auto left = 0.0f;
                //auto top = 0.0f;
                //auto width = 1.0f;
                //auto height = 1.0f;

                //auto renderTarget = getRenderTarget();
                //if( renderTarget )
                //{
                //    //if (renderTarget->isDerived<IWindow>())
                //    {
                //        Ogre::RenderTarget *rt = nullptr;
                //        renderTarget->_getObject( (void **)&rt );

                //        if( rt )
                //        {
                //            //if( ZOrder == -1 )
                //            {
                //                ZOrder = renderTarget->getNumViewports() - 1;
                //                if( ZOrder < 0 )
                //                {
                //                    ZOrder = 0;
                //                }

                //                ZOrder = mZOrder++;
                //            }

                //            FB_ASSERT( rt->getViewportByZOrder( ZOrder ) != nullptr );
                //            auto vp = rt->addViewport( ogreCamera, ZOrder, left, top, width, height );
                //            m_viewport = vp;
                //        }
                //    }
                //}
                //else
                //{
                //    auto window = getWindow();
                //    Ogre::RenderWindow *ogreWindow = nullptr;
                //    if( window )
                //    {
                //        window->_getObject( (void **)&ogreWindow );
                //    }

                //    if( ogreWindow )
                //    {
                //        //if (ZOrder == -1)
                //        {
                //            ZOrder = renderTarget->getNumViewports() - 1;
                //            if( ZOrder < 0 )
                //            {
                //                ZOrder = 0;
                //            }

                //            ZOrder = mZOrder++;
                //        }

                //        auto vp =
                //            ogreWindow->addViewport( ogreCamera, ZOrder, left, top, width, height );
                //        m_viewport = vp;
                //    }
                //}

                //#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
                //                if( m_viewport )
                //                {
                //                    auto schemeName = Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME;
                //                    m_viewport->setMaterialScheme( schemeName );
                //                }
                //#endif
                //
                //                if( m_viewport )
                //                {
                //                    if( auto stateContext = getStateObject() )
                //                    {
                //                        if( auto state = stateContext->getStateByType<ViewportState>() )
                //                        {
                //                            state->setZOrder( m_viewport->getZOrder() );
                //                        }
                //                    }
                //                }

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CViewportOgre::reload( SmartPtr<ISharedObject> data )
        {
            unload( data );
            load( data );
        }

        void CViewportOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Unloading );

                removeViewportFromRT();

                m_viewport = nullptr;
                m_renderTarget = nullptr;

                m_window = nullptr;

                CViewport::unload( data );

                setLoadingState( LoadingState::Unloaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CViewportOgre::update()
        {
            Ogre::RenderTarget *rt = nullptr;

            if( auto renderTarget = getRenderTarget() )
            {
                renderTarget->_getObject( (void **)&rt );
            }

            if( auto vp = getViewport() )
            {
                FB_ASSERT( MathF::isFinite( vp->getWidth() ) );
                FB_ASSERT( MathF::isFinite( vp->getHeight() ) );
                FB_ASSERT( MathF::isFinite( vp->getTop() ) );
                FB_ASSERT( MathF::isFinite( vp->getLeft() ) );

                rt->fireViewportPreUpdate( vp );

                const auto actualWidth = vp->getActualWidth();
                const auto actualHeight = vp->getActualHeight();

                if( actualWidth > 0 && actualHeight > 0 )
                {
                    if( vp->isAutoUpdated() )
                    {
                        auto mask = static_cast<u32>( 0 );

                        mask = BitUtil::setFlagValue( mask, IGraphicsObject::UiFlag, getEnableUI() );
                        mask = BitUtil::setFlagValue( mask, IGraphicsObject::SceneFlag,
                                                      getEnableSceneRender() );

                        auto skyEnabled = getSkiesEnabled();
                        vp->setSkiesEnabled( skyEnabled );
                        vp->setVisibilityMask( mask );
                        //rt->_updateViewport( vp );

                        Ogre::Camera *camera = nullptr;

                        if( auto pCamera = getCamera() )
                        {
                            pCamera->_getObject( (void **)&camera );
                        }

                        //if( camera )
                        //{
                        //    camera->_renderScene( m_viewport );
                        //}

                        vp->update();
                    }
                }

                rt->fireViewportPostUpdate( vp );
            }
        }

        void CViewportOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_viewport.load();
        }

        void CViewportOgre::ViewportStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                ISharedObject::ScopedLock lock( graphicsSystem );

                if( auto owner = getOwner() )
                {
                    if( owner->isLoaded() )
                    {
                        Ogre::Viewport *vp = nullptr;
                        owner->_getObject( (void **)&vp );

                        //if (vp)
                        //{
                        //    vp->setCamera( nullptr );
                        //}

                        if( state->isDerived<ViewportState>() )
                        {
                            auto viewportState = fb::static_pointer_cast<ViewportState>( state );
                            if( viewportState )
                            {
                                auto dirty = false;

                                auto zorder = viewportState->getZOrder();
                                auto backgroundColour = viewportState->getBackgroundColour();
                                auto c = Ogre::ColourValue( backgroundColour.r, backgroundColour.g,
                                                            backgroundColour.b, backgroundColour.a );

                                Ogre::Camera *ogreCamera = nullptr;

                                auto pCamera = viewportState->getCamera();
                                if( pCamera )
                                {
                                    if( pCamera->isLoaded() )
                                    {
                                        pCamera->_getObject( (void **)&ogreCamera );
                                    }
                                    else
                                    {
                                        dirty = true;
                                    }
                                }

                                auto vpZOrder = -1;

                                if( vp )
                                {
                                    vpZOrder = vp->getZOrder();
                                }

                                if( viewportState->isActive() )
                                {
                                    if( zorder != vpZOrder )
                                    {
                                        if( auto renderTarget = owner->getRenderTarget() )
                                        {
                                            Ogre::RenderTarget *rt = nullptr;
                                            renderTarget->_getObject( (void **)&rt );

                                            auto left = 0.0f;
                                            auto top = 0.0f;
                                            auto width = 1.0f;
                                            auto height = 1.0f;

                                            if( rt )
                                            {
                                                FB_ASSERT( rt->hasViewportWithZOrder( zorder ) ==
                                                           false );

                                                if( vpZOrder != -1 )
                                                {
                                                    if( vp )
                                                    {
                                                        vp->setCamera( nullptr );
                                                    }

                                                    auto rttVP = rt->getViewportByZOrder( zorder );
                                                    FB_ASSERT( rttVP == vp );
                                                    rttVP->setCamera( nullptr );

                                                    rt->removeViewport( vpZOrder );
                                                    vp = nullptr;
                                                    owner->setViewport( nullptr );
                                                }

                                                FB_ASSERT( rt->hasViewportWithZOrder( zorder ) ==
                                                           false );

                                                if( rt->hasViewportWithZOrder( zorder ) == true )
                                                {
                                                    auto rttVP = rt->getViewportByZOrder( zorder );
                                                    rttVP->setCamera( nullptr );

                                                    rt->removeViewport( zorder );
                                                    owner->setViewport( nullptr );
                                                }

                                                if( rt->hasViewportWithZOrder( zorder ) == false )
                                                {
                                                    vp = rt->addViewport( ogreCamera, zorder, left, top,
                                                                          width, height );
                                                    owner->setViewport( vp );
                                                }
                                            }
                                        }
                                    }

                                    if( vp )
                                    {
                                        vp->setBackgroundColour( c );

                                        auto overlaysEnabled = viewportState->getOverlaysEnabled();
                                        if( overlaysEnabled != vp->getOverlaysEnabled() )
                                        {
                                            vp->setOverlaysEnabled( overlaysEnabled );
                                        }
                                    }
                                }
                                else
                                {
                                    if( vp )
                                    {
                                        vp->setCamera( nullptr );
                                    }

                                    if( vpZOrder != -1 )
                                    {
                                        if( auto renderTarget = owner->getRenderTarget() )
                                        {
                                            Ogre::RenderTarget *rt = nullptr;
                                            renderTarget->_getObject( (void **)&rt );

                                            if( rt )
                                            {
                                                auto rttVP = rt->getViewportByZOrder( vpZOrder );
                                                rttVP->setCamera( nullptr );

                                                rt->removeViewport( vpZOrder );
                                                owner->setViewport( nullptr );
                                            }
                                        }
                                    }
                                }

                                viewportState->setDirty( dirty );
                            }
                        }
                    }
                }
            }
            catch( std::exception &e )
            {
                auto error = String( e.what() );
                FB_LOG_ERROR( error );
            }
        }

        void CViewportOgre::ViewportStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
        }

        void CViewportOgre::ViewportStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CViewportOgre *CViewportOgre::ViewportStateListener::getOwner() const
        {
            return m_owner;
        }

        void CViewportOgre::ViewportStateListener::setOwner( CViewportOgre *owner )
        {
            m_owner = owner;
        }

        Ogre::Viewport *CViewportOgre::getViewport() const
        {
            return m_viewport.load();
        }

        void CViewportOgre::setViewport( Ogre::Viewport *viewport )
        {
            m_viewport = viewport;
        }

        void CViewportOgre::removeViewportFromRT()
        {
            if( auto renderTarget = getRenderTarget() )
            {
                Ogre::RenderTarget *rt = nullptr;
                renderTarget->_getObject( (void **)&rt );

                if( auto vp = getViewport() )
                {
                    auto zorder = vp->getZOrder();

                    if( rt )
                    {
                        rt->removeViewport( zorder );
                    }
                }
            }
        }

    }  // end namespace render
}  // end namespace fb
