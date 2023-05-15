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
        FB_CLASS_REGISTER_DERIVED( fb, CViewportOgre, CSharedObject<IViewport> );

        u32 CViewportOgre::m_zOrderExt = 0;

        CViewportOgre::CViewportOgre()
        {
            try
            {
                auto state = fb::make_ptr<ViewportState>();

                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                auto stateManager = applicationManager->getStateManager();
                auto threadPool = applicationManager->getThreadPool();
                auto factoryManager = applicationManager->getFactoryManager();

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
#if _DEBUG
                auto task = Thread::getCurrentTask();
                auto renderTask =
                    core::IApplicationManager::instance()->getGraphicsSystem()->getRenderTask();
                FB_ASSERT( task == renderTask );
#endif

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
                m_id = 0;

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

            if( m_viewport )
            {
                rt->fireViewportPreUpdate( m_viewport );

                const auto actualWidth = m_viewport->getActualWidth();
                const auto actualHeight = m_viewport->getActualHeight();

                if( actualWidth > 0 && actualHeight > 0 )
                {
                    if( m_viewport->isAutoUpdated() )
                    {
                        auto mask = static_cast<u32>( 0 );

                        mask = BitUtil::setFlagValue( mask, IGraphicsObject::UiFlag, getUiEnabled() );
                        mask = BitUtil::setFlagValue( mask, IGraphicsObject::SceneFlag,
                                                      getRenderSceneEnabled() );

                        auto skyEnabled = getSkiesEnabled();
                        m_viewport->setSkiesEnabled( skyEnabled );
                        m_viewport->setVisibilityMask( mask );
                        //rt->_updateViewport( m_viewport );

                        Ogre::Camera *camera = nullptr;

                        if( auto pCamera = getCamera() )
                        {
                            pCamera->_getObject( (void **)&camera );
                        }

                        //if( camera )
                        //{
                        //    camera->_renderScene( m_viewport );
                        //}

                        m_viewport->update();
                    }
                }

                rt->fireViewportPostUpdate( m_viewport );
            }
        }

        void CViewportOgre::setId( u32 id )
        {
            m_id = id;
        }

        u32 CViewportOgre::getId() const
        {
            return m_id;
        }

        void CViewportOgre::setCamera( SmartPtr<ICamera> camera )
        {
            if( auto currentCamera = getCamera() )
            {
                currentCamera->setViewport( nullptr );
            }

            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setCamera( camera );
                }
            }

            if( camera )
            {
                camera->setViewport( this );
            }
        }

        SmartPtr<ICamera> CViewportOgre::getCamera() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getCamera();
                }
            }

            return nullptr;
        }

        s32 CViewportOgre::getZOrder() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getZOrder();
                }
            }

            return 0;
        }

        f32 CViewportOgre::getLeft() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getLeft();
        }

        f32 CViewportOgre::getTop() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getTop();
        }

        f32 CViewportOgre::getWidth() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getWidth();
        }

        f32 CViewportOgre::getHeight() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getHeight();
        }

        s32 CViewportOgre::getActualLeft() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getActualLeft();
        }

        s32 CViewportOgre::getActualTop() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getActualTop();
        }

        s32 CViewportOgre::getActualWidth() const
        {
            if( m_viewport )
            {
                return m_viewport->getActualWidth();
            }

            return 0;
        }

        s32 CViewportOgre::getActualHeight() const
        {
            if( m_viewport )
            {
                return m_viewport->getActualHeight();
            }

            return 0;
        }

        void CViewportOgre::setDimensions( f32 left, f32 top, f32 width, f32 height )
        {
            if( m_viewport )
            {
                m_viewport->setDimensions( left, top, width, height );
            }
        }

        void CViewportOgre::setBackgroundColour( const ColourF &colour )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setBackgroundColour( colour );
                }
            }
        }

        ColourF CViewportOgre::getBackgroundColour() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getBackgroundColour();
                }
            }

            return ColourF::White;
        }

        void CViewportOgre::setClearEveryFrame( bool clear, u32 buffers )
        {
            if( m_viewport )
            {
                m_viewport->setClearEveryFrame( clear, buffers );
            }
        }

        bool CViewportOgre::getClearEveryFrame() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getClearEveryFrame();
        }

        u32 CViewportOgre::getClearBuffers() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getClearBuffers();
        }

        void CViewportOgre::setMaterialScheme( const String &schemeName )
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            m_viewport->setMaterialScheme( schemeName.c_str() );
        }

        String CViewportOgre::getMaterialScheme() const
        {
            return StringUtil::EmptyString;
        }

        void CViewportOgre::getActualDimensions( s32 &left, s32 &top, s32 &width, s32 &height ) const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getActualDimensions( left, top, width, height );
        }

        bool CViewportOgre::_isUpdated() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->_isUpdated();
        }

        void CViewportOgre::_clearUpdatedFlag()
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->_clearUpdatedFlag();
        }

        u32 CViewportOgre::_getNumRenderedFaces() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->_getNumRenderedFaces();
        }

        u32 CViewportOgre::_getNumRenderedBatches() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->_getNumRenderedBatches();
        }

        void CViewportOgre::setRenderSceneEnabled( bool enabled )
        {
            m_renderSceneEnabled = enabled;
        }

        bool CViewportOgre::getRenderSceneEnabled() const
        {
            return m_renderSceneEnabled;
        }

        void CViewportOgre::setOverlaysEnabled( bool enabled )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setOverlaysEnabled( enabled );
                }
            }
        }

        bool CViewportOgre::getOverlaysEnabled() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getOverlaysEnabled();
                }
            }

            return false;
        }

        void CViewportOgre::setSkiesEnabled( bool enabled )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setSkiesEnabled( enabled );
                }
            }
        }

        bool CViewportOgre::getSkiesEnabled() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getSkiesEnabled();
                }
            }

            return false;
        }

        void CViewportOgre::setShadowsEnabled( bool enabled )
        {
            if( m_viewport )
            {
                m_viewport->setShadowsEnabled( enabled );
            }
        }

        bool CViewportOgre::getShadowsEnabled() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->getShadowsEnabled();
        }

        void CViewportOgre::setVisibilityMask( u32 mask )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setVisibilityMask( mask );
                }
            }
        }

        u32 CViewportOgre::getVisibilityMask() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->getVisibilityMask();
                }
            }

            return 0;
        }

        void CViewportOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_viewport;
        }

        void CViewportOgre::setPriority( u8 priority )
        {
            auto target = m_viewport->getTarget();
            target->setPriority( priority );
        }

        u8 CViewportOgre::getPriority() const
        {
            auto target = m_viewport->getTarget();
            return target->getPriority();
        }

        bool CViewportOgre::isActive() const
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    return state->isActive();
                }
            }

            return false;
        }

        void CViewportOgre::setActive( bool active )
        {
            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setActive( active );
                }
            }
        }

        void CViewportOgre::setAutoUpdated( bool autoupdate )
        {
            if( m_viewport )
            {
                ISharedObject::ScopedLock lock(
                    core::IApplicationManager::instance()->getGraphicsSystem() );
                m_viewport->setAutoUpdated( autoupdate );
            }
        }

        bool CViewportOgre::isAutoUpdated() const
        {
            ISharedObject::ScopedLock lock( core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_viewport->isAutoUpdated();
        }

        void CViewportOgre::setZOrder( s32 zorder )
        {
            //FB_ASSERT(getRenderTarget() && getRenderTarget()->hasViewportWithZOrder( zorder ) == false);

            if( auto stateContext = getStateObject() )
            {
                if( auto state = stateContext->getStateByType<ViewportState>() )
                {
                    state->setZOrder( zorder );
                }
            }
        }

        Vector2F CViewportOgre::getPosition() const
        {
            return Vector2F( m_viewport->getLeft(), m_viewport->getTop() );
        }

        void CViewportOgre::setPosition( const Vector2F &position )
        {
        }

        Vector2F CViewportOgre::getSize() const
        {
            return Vector2F( m_viewport->getWidth(), m_viewport->getHeight() );
        }

        void CViewportOgre::setSize( const Vector2F &size )
        {
        }

        SmartPtr<ITexture> CViewportOgre::getBackgroundTexture() const
        {
            return m_texture;
        }

        void CViewportOgre::setBackgroundTexture( SmartPtr<ITexture> texture )
        {
            m_texture = texture;
        }

        String CViewportOgre::getBackgroundTextureName() const
        {
            return m_backgroundTextureName;
        }

        void CViewportOgre::setBackgroundTextureName( const String &textureName )
        {
            m_backgroundTextureName = textureName;
        }

        SmartPtr<IRenderTarget> CViewportOgre::getRenderTarget() const
        {
            return m_renderTarget.lock();
        }

        void CViewportOgre::setRenderTarget( SmartPtr<IRenderTarget> renderTarget )
        {
            if( m_renderTarget )
            {
                removeViewportFromRT();
            }

            m_renderTarget = renderTarget;
        }

        SmartPtr<IWindow> CViewportOgre::getWindow() const
        {
            return m_window.lock();
        }

        void CViewportOgre::setWindow( SmartPtr<IWindow> window )
        {
            m_window = window;
        }

        void CViewportOgre::setUiEnabled( bool enabled )
        {
            m_uiEnabled = enabled;
        }

        bool CViewportOgre::getUiEnabled() const
        {
            return m_uiEnabled;
        }

        void CViewportOgre::ViewportStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

                ISharedObject::ScopedLock lock( graphicsSystem );

                auto owner = getOwner();
                const auto &loadingState = owner->getLoadingState();
                if( loadingState == LoadingState::Loaded )
                {
                    Ogre::Viewport *vp = nullptr;
                    owner->_getObject( (void **)&vp );

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
                                            if( vpZOrder != -1 )
                                            {
                                                if( vp )
                                                {
                                                    vp->setCamera( nullptr );
                                                }

                                                rt->removeViewport( vpZOrder );
                                                vp = nullptr;
                                                owner->setViewport( nullptr );
                                            }

                                            //FB_ASSERT( rt->hasViewportWithZOrder( zorder ) == false );

                                            if( rt->hasViewportWithZOrder( zorder ) == true )
                                            {
                                                rt->removeViewport( zorder );
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
            return m_viewport;
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

                if( m_viewport )
                {
                    auto zorder = m_viewport->getZOrder();

                    if( rt )
                    {
                        rt->removeViewport( zorder );
                    }
                }
            }
        }

    }  // end namespace render
}  // end namespace fb
