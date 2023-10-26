#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Wrapper/CWindowOgreNext.h>
#include <FBGraphicsOgreNext/Wrapper/CViewportOgreNext.h>
#include <FBGraphicsOgreNext/OgreUtil.h>
#include <FBCore/FBCore.h>
#include <OgreWindow.h>
#include <Ogre.h>

#if defined FB_PLATFORM_WIN32
#    include <FBGraphicsOgreNext/Window/Windows/WindowWin32.h>
#elif defined FB_PLATFORM_APPLE
#    include <FBGraphicsOgreNext/Window/Apple/WindowMacOS.h>
#endif

#ifdef FB_PLATFORM_WIN32
#    include <windows.h>
#endif

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED( fb, CWindowOgreNext, IWindow );

        CWindowOgreNext::CWindowOgreNext() : m_window( nullptr )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto stateManager = applicationManager->getStateManager();
                FB_ASSERT( stateManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto stateObject = stateManager->addStateObject();

                auto stateListener = factoryManager->make_ptr<WindowStateListener>();
                stateListener->setOwner( this );
                m_stateListener = stateListener;
                stateObject->addStateListener( stateListener );

                auto state = factoryManager->make_ptr<RenderWindowState>();

                stateObject->setState( state );
                stateObject->setOwner( this );
                setStateObject( stateObject );

                m_state = state;

                auto renderTask =
                    applicationManager->hasTasks() ? Thread::Task::Render : Thread::Task::Primary;
                m_state->setTaskId( renderTask );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        CWindowOgreNext::~CWindowOgreNext()
        {
            unload( nullptr );
        }

        void CWindowOgreNext::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                auto root = Ogre::Root::getSingletonPtr();

                FB_ASSERT( root );
                FB_ASSERT( root->isInitialised() );

                Ogre::NameValuePairList params;

                auto handle = getWindowHandleAsString();

#if defined FB_PLATFORM_WIN32
#    if 1
                if( !StringUtil::isNullOrEmpty( handle ) )
                {
                    params["externalWindowHandle"] = handle;
                }
                else
                {
                    m_windowWin32 = new WindowWin32;
                    m_windowWin32->setRenderWindow( this );
                    m_windowWin32->load( nullptr );

                    size_t windowHandle = 0;
                    m_windowWin32->getWindowHandle( &windowHandle );

                    params["externalWindowHandle"] = StringUtil::toString( windowHandle );
                }
#    else
                if( !StringUtil::isNullOrEmpty( handle ) )
                {
                    params["externalWindowHandle"] = OgreUtil::toString( handle );
                }
#    endif
#elif defined FB_PLATFORM_APPLE
#    if 1
                if( !StringUtil::isNullOrEmpty( m_windowHandle ) )
                {
                    params["externalWindowHandle"] = OgreUtil::toString( handle );
                }
                else
                {
                    m_osWindow = new WindowMacOS;

                    m_osWindow->setWindow( this );
                    m_osWindow->load( nullptr );

                    size_t windowHandle = 0;
                    m_osWindow->getWindowHandle( &windowHandle );

                    params["externalWindowHandle"] = StringUtil::toString( windowHandle );
                }

                params["contentScalingFactor"] = StringUtil::toString( 1.0f );
#    else
                if( !StringUtil::isNullOrEmpty( m_windowHandle ) )
                {
                    params["externalWindowHandle"] = OgreUtil::toString( handle );
                }
#    endif
#elif defined FB_PLATFORM_LINUX
                if( !StringUtil::isNullOrEmpty( m_windowHandle ) )
                {
                    params["externalWindowHandle"] = OgreUtil::toString( handle );
                }
#endif

                m_window =
                    root->createRenderWindow( m_label, static_cast<u32>( m_size.X() ),
                                              static_cast<u32>( m_size.Y() ), m_isFullscreen, &params );

                m_windowEventListener = new WindowListener();
                m_windowEventListener->setOwner( this );
                Ogre::WindowEventUtilities::addWindowEventListener( m_window, m_windowEventListener );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CWindowOgreNext::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                const auto &loadingState = getLoadingState();
                if( loadingState != LoadingState::Unloaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto applicationManager = core::IApplicationManager::instance();
                    FB_ASSERT( applicationManager );

                    auto stateManager = applicationManager->getStateManager();
                    FB_ASSERT( stateManager );

                    if( m_state )
                    {
                        m_state->unload( data );
                        m_state = nullptr;
                    }

                    //if( m_stateListener )
                    //{
                    //    if( m_stateObject )
                    //    {
                    //        m_stateObject->removeStateListener( m_stateListener );
                    //    }

                    //    m_stateListener->unload( data );
                    //    m_stateListener = nullptr;
                    //}

                    //if( m_stateObject )
                    //{
                    //    stateManager->removeStateObject( m_stateObject );

                    //    m_stateObject->unload( data );
                    //    m_stateObject = nullptr;
                    //}

                    if( m_windowEventListener )
                    {
                        Ogre::WindowEventUtilities::removeWindowEventListener( m_window,
                                                                               m_windowEventListener );
                        delete m_windowEventListener;
                        m_windowEventListener = nullptr;
                    }

                    m_window = nullptr;

                    if( m_state )
                    {
                        m_state->setOwner( nullptr );
                        m_state->unload( nullptr );
                        m_state = nullptr;
                    }

                    CRenderTargetOgreNext<IWindow>::unload( data );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CWindowOgreNext::initialise( Ogre::Window *window )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                m_window = window;

                m_windowEventListener = new WindowListener();
                m_windowEventListener->setOwner( this );
                Ogre::WindowEventUtilities::addWindowEventListener( m_window, m_windowEventListener );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        //
        // IRenderTarget functions
        //

        void CWindowOgreNext::update()
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            ISharedObject::ScopedLock lock( graphicsSystem );

            if( !m_eventQueue.empty() )
            {
                SmartPtr<IWindowEvent> event;
                while( m_eventQueue.try_pop( event ) )
                {
                    auto listeners = getListeners();
                    for( auto listener : listeners )
                    {
                        listener->handleEvent( event );
                    }

                    handleQueuedEvent( event );
                }
            }
        }

        void CWindowOgreNext::getMetrics( u32 &width, u32 &height, u32 &colourDepth )
        {
            // m_window->getMetrics(width, height, colourDepth);
        }

        u32 CWindowOgreNext::getWidth() const
        {
            if( m_window )
            {
                return m_window->getWidth();
            }

            return 0;
        }

        u32 CWindowOgreNext::getHeight() const
        {
            if( m_window )
            {
                return m_window->getHeight();
            }

            return 0;
        }

        u32 CWindowOgreNext::getColourDepth() const
        {
            return 0;  //
        }

        SmartPtr<IViewport> CWindowOgreNext::addViewport( hash32 id, SmartPtr<ICamera> camera,
                                                          s32 ZOrder, f32 left, f32 top, f32 width,
                                                          f32 height )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                auto factoryManager = applicationManager->getFactoryManager();

                Ogre::Camera *ogreCamera = nullptr;
                if( camera )
                {
                    camera->_getObject( (void **)&ogreCamera );
                }

                auto viewport = factoryManager->make_ptr<CViewportOgreNext>();
                m_viewports[id] = viewport;

                viewport->setCamera( camera );

                return viewport;
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }

            return nullptr;
        }

        u32 CWindowOgreNext::getNumViewports() const
        {
            auto size = m_viewports.size();
            return static_cast<u32>( size );
        }

        SmartPtr<IViewport> CWindowOgreNext::getViewport( u32 index )
        {
            if( index < 0 || index >= m_viewports.size() )
            {
                FB_LOG_ERROR( "CWindow::getViewport - index out of bounds." );
                return nullptr;
            }

            auto it = m_viewports.begin();
            std::advance( it, index );
            return it->second;
        }

        SmartPtr<IViewport> CWindowOgreNext::getViewportById( hash32 id )
        {
            auto it = m_viewports.find( id );
            if( it != m_viewports.end() )
            {
                return it->second;
            }

            FB_LOG( "CWindow::getViewportById - could not find viewport." );
            return nullptr;
        }

        void CWindowOgreNext::removeViewport( SmartPtr<IViewport> vp )
        {
            // m_window->removeViewport(ZOrder);
        }

        void CWindowOgreNext::removeAllViewports()
        {
            //
            // m_window->removeAllViewports();
        }

        void CWindowOgreNext::getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS,
                                             f32 &worstFPS ) const
        {
            //
            // m_window->getStatistics(lastFPS, avgFPS, bestFPS, worstFPS);
        }

        IRenderTarget::RenderTargetStats CWindowOgreNext::getRenderTargetStats() const
        {
            RenderTargetStats renderTargetStats;

            // const Ogre::RenderTarget::FrameStats& frameStats = m_window->getStatistics();

            // renderTargetStats.lastFPS = frameStats.lastFPS;
            // renderTargetStats.avgFPS = frameStats.avgFPS;
            // renderTargetStats.bestFPS = frameStats.bestFPS;
            // renderTargetStats.worstFPS = frameStats.worstFPS;
            // renderTargetStats.bestFrameTime = frameStats.bestFrameTime;
            // renderTargetStats.worstFrameTime = frameStats.worstFrameTime;
            // renderTargetStats.triangleCount = frameStats.triangleCount;
            // renderTargetStats.batchCount = frameStats.batchCount;

            return renderTargetStats;
        }

        void CWindowOgreNext::handleEvent( SmartPtr<IWindowEvent> event )
        {
            m_eventQueue.push( event );
        }

        String CWindowOgreNext::getTitle() const
        {
            return m_window->getTitle();
        }

        //
        // IWindow functions
        //

        void CWindowOgreNext::setTitle( const String &title )
        {
            m_window->setTitle( title );
        }

        void CWindowOgreNext::setFullscreen( bool fullScreen, unsigned int width, unsigned int height )
        {
            if( m_window )
            {
                m_window->requestFullscreenSwitch( fullScreen, false, 0, width, height, 1, 60 );
            }
        }

        void CWindowOgreNext::destroy()
        {
            if( m_window )
            {
                m_window->destroy();
            }
        }

        void CWindowOgreNext::resize( unsigned int width, unsigned int height )
        {
            try
            {
                if( isThreadSafe() )
                {
                    if( m_window )
                    {
                        m_window->requestResolution( width, height );
                    }
                }
                else
                {
                    auto applicationManager = core::IApplicationManager::instance();
                    auto factoryManager = applicationManager->getFactoryManager();

                    auto message = factoryManager->make_ptr<StateMessageVector2I>();
                    message->setType( RESIZE_HASH );
                    message->setValue( Vector2I( width, height ) );

                    if( auto stateContext = getStateObject() )
                    {
                        stateContext->addMessage( Thread::Task::Render, message );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CWindowOgreNext::windowMovedOrResized()
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto materialManager = graphicsSystem->getMaterialManager();
                FB_ASSERT( materialManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    if( m_window )
                    {
                        m_window->windowMovedOrResized();
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessage>();
                    message->setType( MOVED_OR_RESIZED_HASH );

                    if( auto stateContext = getStateObject() )
                    {
                        stateContext->addMessage( stateTask, message );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CWindowOgreNext::maximize()
        {
#ifdef FB_PLATFORM_WIN32
            HWND hwnd;

            m_window->getCustomAttribute( "WINDOW", &hwnd );
            ShowWindow( hwnd, SW_SHOWMAXIMIZED );
#endif
        }

        void CWindowOgreNext::reposition( int left, int top )
        {
            try
            {
                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                auto graphicsSystem = applicationManager->getGraphicsSystem();
                FB_ASSERT( graphicsSystem );

                auto materialManager = graphicsSystem->getMaterialManager();
                FB_ASSERT( materialManager );

                auto factoryManager = applicationManager->getFactoryManager();
                FB_ASSERT( factoryManager );

                auto renderTask = graphicsSystem->getRenderTask();
                auto stateTask = graphicsSystem->getStateTask();
                auto task = Thread::getCurrentTask();

                const auto &loadingState = getLoadingState();
                if( loadingState == LoadingState::Loaded && task == renderTask )
                {
                    if( m_window )
                    {
                        m_window->reposition( left, top );
                    }
                }
                else
                {
                    auto message = factoryManager->make_ptr<StateMessageVector2I>();
                    message->setType( REPOSITION_HASH );
                    message->setValue( Vector2I( left, top ) );

                    if( auto stateContext = getStateObject() )
                    {
                        stateContext->addMessage( stateTask, message );
                    }
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        bool CWindowOgreNext::isVisible() const
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto state = stateObject->getLatestOutputStateByType<RenderWindowState>() )
                {
                    return state->isVisible();
                }
            }

            return false;
        }

        void CWindowOgreNext::setVisible( bool visible )
        {
            if( auto stateObject = getStateObject() )
            {
                if( auto state = stateObject->getLatestOutputStateByType<RenderWindowState>() )
                {
                    state->setVisible( visible );
                }
            }
        }

        bool CWindowOgreNext::isClosed() const
        {
            if( m_window )
            {
                return m_window->isClosed();
            }

            return false;
        }

        bool CWindowOgreNext::isPrimary() const
        {
            if( m_window )
            {
                return m_window->isPrimary();
            }

            return false;
        }

        bool CWindowOgreNext::isFullScreen() const
        {
            return m_isFullscreen;
        }

        void CWindowOgreNext::setFullscreen( bool fullscreen )
        {
            m_isFullscreen = fullscreen;
        }

        void CWindowOgreNext::getMetrics( unsigned int &width, unsigned int &height,
                                          unsigned int &colourDepth, int &left, int &top )
        {
            //
            // return m_window->getMetrics(width, height, colourDepth, left, top);
        }

        u8 CWindowOgreNext::suggestPixelFormat() const
        {
            //
            // return m_window->suggestPixelFormat();

            return 0;
        }

        bool CWindowOgreNext::isDeactivatedOnFocusChange() const
        {
            //
            // return m_window->isDeactivatedOnFocusChange();

            return false;
        }

        void CWindowOgreNext::setDeactivateOnFocusChange( bool deactivate )
        {
            //
            // m_window->setDeactivateOnFocusChange(deactivate);
        }

        void CWindowOgreNext::getCustomAttribute( const String &name, void *pData )
        {
            m_window->getCustomAttribute( name.c_str(), pData );
        }

        void CWindowOgreNext::getWindowHandle( void *pData )
        {
#if defined FB_PLATFORM_WIN32
            getCustomAttribute( "WINDOW", pData );
#elif defined FB_PLATFORM_APPLE
#    if defined FB_BUILD_RENDERER_METAL
            if(m_osWindow)
            {
                m_osWindow->getWindowHandle( pData );
                return;
            }

            getCustomAttribute( "WINDOW", (void *)pData );
            if( !pData )
            {
                getCustomAttribute( "MetalDevice", (void *)&pData );
            }
#    else
            getCustomAttribute( "WINDOW", pData );
            if( windowHnd == 0 )
            {
                getCustomAttribute( "RENDERDOC", pData );
            }
#    endif
#else
            getCustomAttribute( "WINDOW", pData );
#endif
        }

        void CWindowOgreNext::getDeviceHandle( void *pData )
        {
        }

        void CWindowOgreNext::addListener( SmartPtr<IWindowListener> listener )
        {
            m_listeners.push_back( listener );
        }

        void CWindowOgreNext::removeListener( SmartPtr<IWindowListener> listener )
        {
            auto it = std::find( m_listeners.begin(), m_listeners.end(), listener );
            if( it != m_listeners.end() )
            {
                m_listeners.erase( it );
            }
        }

        Array<SmartPtr<IWindowListener>> CWindowOgreNext::getListeners() const
        {
            return m_listeners;
        }

        void CWindowOgreNext::handleQueuedEvent( SmartPtr<IWindowEvent> event )
        {
            auto windowEvent = fb::dynamic_pointer_cast<WindowMessageData>( event );
            auto uMsg = windowEvent->getMessage();
            auto renderWindow = this;

#ifdef FB_PLATFORM_WIN32
            switch( uMsg )
            {
            case WM_CLOSE:
            {
                bool close = true;

                if( renderWindow )
                {
                    auto listeners = renderWindow->getListeners();
                    for( auto listener : listeners )
                    {
                        auto retValue = listener->handleEvent(
                            IEvent::Type::Window, IWindowListener::windowClosingHash, Array<Parameter>(),
                            nullptr, nullptr, nullptr );

                        if( !retValue.getBool() )
                        {
                            close = false;
                        }
                    }
                }

                if( !close )
                {
                    return;
                }

                auto applicationManager = core::IApplicationManager::instance();
                FB_ASSERT( applicationManager );

                applicationManager->setQuit( true );
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
                windowMovedOrResized();

                auto listeners = renderWindow->getListeners();
                for( auto listener : listeners )
                {
                    //listener->windowMoved( win );

                    auto retValue =
                        listener->handleEvent( IEvent::Type::Window, IWindowListener::windowMovedHash,
                                               Array<Parameter>(), nullptr, nullptr, nullptr );
                }
            }
            break;
            case WM_DISPLAYCHANGE:
            {
                windowMovedOrResized();

                auto listeners = renderWindow->getListeners();
                for( auto listener : listeners )
                {
                    //listener->windowResized( win );

                    auto retValue =
                        listener->handleEvent( IEvent::Type::Window, IWindowListener::windowResizedHash,
                                               Array<Parameter>(), nullptr, nullptr, nullptr );
                }
            }
            break;
            case WM_SIZE:
            {
                // log->logMessage("WM_SIZE");
                windowMovedOrResized();

                auto listeners = renderWindow->getListeners();
                for( auto listener : listeners )
                {
                    //listener->windowResized( win );

                    auto retValue =
                        listener->handleEvent( IEvent::Type::Window, IWindowListener::windowResizedHash,
                                               Array<Parameter>(), nullptr, nullptr, nullptr );
                }
            }
            break;

            default:
            {
            }
            };
#endif
        }

        void CWindowOgreNext::_getObject( void **ppObject ) const
        {
            *ppObject = m_window;
        }

        void CWindowOgreNext::setPriority( u8 priority )
        {
            // m_window->setPriority(priority);
        }

        u8 CWindowOgreNext::getPriority() const
        {
            // return m_window->getPriority();
            return 0;
        }

        bool CWindowOgreNext::isActive() const
        {
            // return m_window->isActive();
            return false;
        }

        void CWindowOgreNext::setActive( bool state )
        {
            // m_window->setActive( state );
        }

        void CWindowOgreNext::setAutoUpdated( bool autoupdate )
        {
            // m_window->setAutoUpdated(autoupdate);
        }

        bool CWindowOgreNext::isAutoUpdated( void ) const
        {
            return false;  // m_window->isAutoUpdated();
        }

        void CWindowOgreNext::copyContentsToMemory( void *buffer, u32 size, FrameBuffer bufferId )
        {
        }

        void CWindowOgreNext::swapBuffers()
        {
            // if(Thread::getCurrentTask() != Thread::Task::TASK_ID_RENDER)
            //{
            //
            //	m_window->swapBuffers();
            // }
            // else
            //{
            //	m_window->swapBuffers();
            // }
        }

        Array<SmartPtr<IViewport>> CWindowOgreNext::getViewports() const
        {
            Array<SmartPtr<IViewport>> viewports;
            viewports.reserve( m_viewports.size() );

            auto it = m_viewports.begin();
            for( ; it != m_viewports.end(); ++it )
            {
                viewports.push_back( it->second );
            }

            return viewports;
        }

        Ogre::Window *CWindowOgreNext::getWindow() const
        {
            return m_window;
        }

        void CWindowOgreNext::setWindow( Ogre::Window *val )
        {
            m_window = val;
        }

        Vector2I CWindowOgreNext::getSize() const
        {
            if( m_window )
            {
                auto width = m_window->getWidth();
                auto height = m_window->getHeight();

                return Vector2I( static_cast<f32>( width ), static_cast<f32>( height ) );
            }

            return Vector2I::zero();
        }

        void CWindowOgreNext::setSize( const Vector2I &size )
        {
            m_size = size;
        }

        void CWindowOgreNext::setColourDepth( u32 val )
        {
        }

        String CWindowOgreNext::getWindowHandleAsString() const
        {
            return m_windowHandle;
        }

        void CWindowOgreNext::setWindowHandleAsString( const String &handle )
        {
            m_windowHandle = handle;
        }

        CWindowOgreNext::WindowStateListener::~WindowStateListener()
        {
        }

        void CWindowOgreNext::WindowStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
            auto renderWindowState = fb::static_pointer_cast<RenderWindowState>( state );

            if( auto owner = getOwner() )
            {
                auto visible = renderWindowState->isVisible();

                if( auto window = owner->getWindow() )
                {
                    window->_setVisible( visible );
                }
            }

            state->setDirty( false );
        }

        void CWindowOgreNext::WindowStateListener::handleStateChanged(
            const SmartPtr<IStateMessage> &message )
        {
            if( message->isExactly<StateMessage>() )
            {
                FB_ASSERT( fb::dynamic_pointer_cast<StateMessage>( message ) );
                auto stateMessage = fb::static_pointer_cast<StateMessage>( message );
                auto type = stateMessage->getType();

                if( type == MOVED_OR_RESIZED_HASH )
                {
                    m_owner->windowMovedOrResized();
                }
            }
            else if( message->isExactly<StateMessageVector2I>() )
            {
                FB_ASSERT( fb::dynamic_pointer_cast<StateMessageVector2I>( message ) );
                auto stateMessage = fb::static_pointer_cast<StateMessageVector2I>( message );
                auto type = stateMessage->getType();
                auto value = stateMessage->getValue();

                if( type == RESIZE_HASH )
                {
                    m_owner->resize( static_cast<u32>( value.X() ), static_cast<u32>( value.Y() ) );
                }
            }
        }

        void CWindowOgreNext::WindowStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CWindowOgreNext *CWindowOgreNext::WindowStateListener::getOwner() const
        {
            return m_owner;
        }

        void CWindowOgreNext::WindowStateListener::setOwner( CWindowOgreNext *owner )
        {
            m_owner = owner;
        }

        bool CWindowOgreNext::WindowListener::windowClosing( Ogre::Window *rw )
        {
            (void)rw;

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            applicationManager->setQuit( true );

            return true;
        }

        CWindowOgreNext *CWindowOgreNext::WindowListener::getOwner() const
        {
            return m_owner;
        }

        void CWindowOgreNext::WindowListener::setOwner( CWindowOgreNext *owner )
        {
            m_owner = owner;
        }

    }  // end namespace render
}  // end namespace fb
