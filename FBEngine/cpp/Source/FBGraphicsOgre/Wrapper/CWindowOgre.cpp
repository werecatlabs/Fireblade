#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include <FBGraphicsOgre/Wrapper/CWindowOgre.h>
#include <FBGraphicsOgre/Wrapper/CViewportOgre.h>
#include <FBGraphicsOgre/Ogre/OgreUtil.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <OgreOverlayManager.h>

#if defined FB_PLATFORM_WIN32
#    include <windows.h>
#    include <FBGraphicsOgre/Window/Windows/WindowWin32.h>
#elif defined FB_PLATFORM_APPLE
#    include <FBGraphicsOgre/Window/Apple/WindowMacOS.h>
#endif

namespace fb
{
    namespace render
    {

        FB_CLASS_REGISTER_DERIVED( fb, CWindowOgre, IWindow );

        CWindowOgre::CWindowOgre() : m_window( nullptr )
        {
        }

        CWindowOgre::~CWindowOgre()
        {
            unload( nullptr );
        }

        void CWindowOgre::load( SmartPtr<ISharedObject> data )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                setSize( Vector2I( 1280, 720 ) );

                auto applicationManager = core::IApplicationManager::instance();
                auto graphicsSystem = applicationManager->getGraphicsSystem();

#if _DEBUG
                auto task = Thread::getCurrentTask();
                auto renderTask = graphicsSystem->getRenderTask();
                FB_ASSERT( task == renderTask );
#endif

                // Confirm Ogre::Root created
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

                //params["contentScalingFactor"] = StringUtil::toString( 1.0f );
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

                auto name = getTitle();
                auto size = getSize();
                auto fullScreen = isFullScreen();

                auto w = static_cast<u32>( size.X() );
                auto h = static_cast<u32>( size.Y() );

                if( auto renderer = root->getRenderSystem() )
                {
                    auto desc = renderer->getRenderWindowDescription();
                    desc.name = name;
                    desc.useFullScreen = fullScreen;
                    desc.width = w;
                    desc.height = h;

#if defined FB_PLATFORM_APPLE
                    desc.width /= 4;
                    desc.height /= 2;
#endif

                    desc.miscParams = params;
                    m_window = root->createRenderWindow( desc );
                }

                auto windowListener = new WindowListener;
                windowListener->setOwner( this );
                m_windowListener = windowListener;

                Ogre::WindowEventUtilities::addWindowEventListener( m_window, windowListener );

                setRenderTarget( m_window );

                //m_window->resize( w, h );
                //windowMovedOrResized();

#if defined FB_PLATFORM_APPLE
                if (m_osWindow)
                {
                    m_osWindow->updateTrackingSize();
                }
#endif

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CWindowOgre::unload( SmartPtr<ISharedObject> data )
        {
            try
            {
                if( const auto &loadingState = getLoadingState(); loadingState == LoadingState::Loaded )
                {
                    setLoadingState( LoadingState::Unloading );

                    auto root = Ogre::Root::getSingletonPtr();

                    if( m_windowListener )
                    {
                        Ogre::WindowEventUtilities::removeWindowEventListener( m_window,
                                                                               m_windowListener );
                        m_windowListener = nullptr;
                    }

                    if( m_window )
                    {
                        root->destroyRenderTarget( m_window );
                        m_window = nullptr;
                        m_renderTarget = nullptr;
                    }

#if defined FB_PLATFORM_WIN32
                    if( m_windowWin32 )
                    {
                        delete m_windowWin32;
                        m_windowWin32 = nullptr;
                    }
#elif defined FB_PLATFORM_APPLE
                    if( m_osWindow )
                    {
                        delete m_osWindow;
                        m_osWindow = nullptr;
                    }
#endif

                    CRenderTargetOgre<IWindow>::unload( nullptr );

                    setLoadingState( LoadingState::Unloaded );
                }
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        void CWindowOgre::update()
        {
            if( m_window )
            {
                auto &oMgr = Ogre::OverlayManager::getSingleton();
                auto width = oMgr.getViewportWidth();
                auto height = oMgr.getViewportHeight();

#if defined FB_PLATFORM_APPLE
                if( m_osWindow && m_window )
                {
                    auto requestedSize = Vector2I( getSize().x, getSize().y );
                    auto osWindowSize = Vector2I( m_osWindow->getWidth(), m_osWindow->getHeight() );
                    auto windowSize = Vector2I( m_window->getWidth(), m_window->getHeight() );

                    //m_osWindow->setFrameSize(w, h);

                    if( requestedSize != osWindowSize )
                    {
                        //m_window->resize( width, height );
                        //windowMovedOrResized();

                        auto w = static_cast<u32>( requestedSize.X() );
                        auto h = static_cast<u32>( requestedSize.Y() );

                        //m_osWindow->setSize( w, h );
                        //m_osWindow->setFrameSize(w, h);
                        //m_window->windowMovedOrResized();
                    }

                    if( requestedSize != windowSize )
                    {
                        //m_window->resize( width, height );
                        //windowMovedOrResized();

                        auto w = static_cast<u32>( requestedSize.X() );
                        auto h = static_cast<u32>( requestedSize.Y() );

                        //m_osWindow->setSize( w, h );

                        //m_window->resize(w/2, h/2);
                        //m_window->resize(w, h);
                        //m_window->windowMovedOrResized();
                    }
                    //m_osWindow->setSize( requestedSize.X(), requestedSize.Y());
                    //m_osWindow->setFrameSize(windowSize.X(), windowSize.Y());
                }
#endif

                auto w = m_window->getWidth();
                auto h = m_window->getHeight();

                if( w != width || h != height )
                {
                    //m_window->resize( width, height );
                    //windowMovedOrResized();
                }

                if( width <= 0 && height <= 0 )
                {
                    auto size = m_window->getNumViewports();
                    for( size_t i = 0; i < size; ++i )
                    {
                        auto vp = m_window->getViewport( (u16)i );
                        vp->_updateDimensions();
                    }
                }

                CRenderTargetOgre<IWindow>::update();
            }
        }

        void CWindowOgre::initialise( Ogre::RenderWindow *window )
        {
            try
            {
                setLoadingState( LoadingState::Loading );

                m_window = window;
                setRenderTarget( m_window );

                setLoadingState( LoadingState::Loaded );
            }
            catch( std::exception &e )
            {
                FB_LOG_EXCEPTION( e );
            }
        }

        String CWindowOgre::getTitle() const
        {
            return m_title;
        }

        void CWindowOgre::setTitle( const String &title )
        {
            m_title = title;
        }

        void CWindowOgre::setFullscreen( bool fullScreen, unsigned int width, unsigned int height )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            m_isFullscreen = fullScreen;
            m_window->setFullscreen( fullScreen, width, height );
        }

        void CWindowOgre::setFullscreen( bool fullscreen )
        {
            m_isFullscreen = fullscreen;
        }

        void CWindowOgre::destroy()
        {
            if( m_window )
            {
                m_window->destroy();
            }
        }

        void CWindowOgre::resize( u32 width, u32 height )
        {
            if( m_window )
            {
                m_window->resize( width, height );
            }
        }

        void CWindowOgre::windowMovedOrResized()
        {
            if( m_window )
            {
                m_window->windowMovedOrResized();
            }
        }

        void CWindowOgre::reposition( s32 left, s32 top )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            m_window->reposition( left, top );
        }

        void CWindowOgre::maximize()
        {
            if( !m_window )
            {
                return;
            }

#ifdef FB_PLATFORM_WIN32
            HWND hwnd;

            m_window->getCustomAttribute( "WINDOW", &hwnd );
            ShowWindow( hwnd, SW_SHOWMAXIMIZED );
#elif defined FB_PLATFORM_APPLE
            if( m_osWindow && m_window )
            {
                m_osWindow->maximize();
            }
#endif

            m_window->windowMovedOrResized();

            auto numViewports = m_window->getNumViewports();
            for( u16 i = 0; i < numViewports; i++ )
            {
                auto vp = m_window->getViewport( i );
                vp->_updateDimensions();
            }
        }

        bool CWindowOgre::isVisible() const
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_window->isVisible();
        }

        void CWindowOgre::setVisible( bool visible )
        {
            if( m_window )
            {
                m_window->setVisible( visible );
            }
        }

        bool CWindowOgre::isClosed() const
        {
            if( m_window )
            {
                return m_window->isClosed();
            }

            return false;
        }

        bool CWindowOgre::isPrimary() const
        {
            if( m_window )
            {
                return m_window->isPrimary();
            }

            return false;
        }

        bool CWindowOgre::isFullScreen() const
        {
            return m_isFullscreen;
        }

        void CWindowOgre::getMetrics( unsigned int &width, unsigned int &height,
                                      unsigned int &colourDepth, int &left, int &top )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_window->getMetrics( width, height, left, top );
        }

        u8 CWindowOgre::suggestPixelFormat() const
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_window->suggestPixelFormat();
        }

        bool CWindowOgre::isDeactivatedOnFocusChange() const
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            return m_window->isDeactivatedOnFocusChange();
        }

        void CWindowOgre::setDeactivateOnFocusChange( bool deactivate )
        {
            ISharedObject::ScopedLock lock(core::IApplicationManager::instance()->getGraphicsSystem() );
            m_window->setDeactivateOnFocusChange( deactivate );
        }

        void CWindowOgre::getCustomAttribute( const String &name, void *pData )
        {
            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            auto graphicsSystem = applicationManager->getGraphicsSystem();
            FB_ASSERT( graphicsSystem );

            auto task = Thread::getCurrentTask();
            auto renderTask = graphicsSystem->getRenderTask();

            if( task == renderTask )
            {
                if( m_window )
                {
                    m_window->getCustomAttribute( name, pData );
                }
            }
        }

        void CWindowOgre::getWindowHandle( void *pData )
        {
#if defined FB_PLATFORM_WIN32
            getCustomAttribute( "WINDOW", pData );
#elif defined FB_PLATFORM_APPLE
#    if defined FB_BUILD_RENDERER_METAL
            getCustomAttribute( "WINDOW", (void *)pData );
            if( !pData )
            {
                // getCustomAttribute("MetalDevice", &windowHnd);
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

        void CWindowOgre::getDeviceHandle( void *pData )
        {
        }

        void CWindowOgre::addListener( SmartPtr<IWindowListener> listener )
        {
            m_listeners.push_back( listener );
        }

        //--------------------------------------------
        void CWindowOgre::removeListener( SmartPtr<IWindowListener> listener )
        {
            auto it = std::find( m_listeners.begin(), m_listeners.end(), listener );
            if( it != m_listeners.end() )
            {
                m_listeners.erase( it );
            }
        }

        //--------------------------------------------
        Array<SmartPtr<IWindowListener>> CWindowOgre::getListeners() const
        {
            return m_listeners;
        }

        void CWindowOgre::_getObject( void **ppObject ) const
        {
            *ppObject = m_window;
        }

        Array<SmartPtr<IViewport>> CWindowOgre::getViewports() const
        {
            // Array<SmartPtr<IViewport>> viewports;
            // viewports.reserve( m_viewports.size() );

            // auto it = m_viewports.begin();
            // for(; it != m_viewports.end(); ++it)
            //{
            //     viewports.push_back( it->second );
            // }

            return m_viewports;
        }

        Ogre::RenderWindow *CWindowOgre::getWindow() const
        {
            return m_window;
        }

        void CWindowOgre::setWindow( Ogre::RenderWindow *window )
        {
            m_window = window;
        }

        Vector2I CWindowOgre::getSize() const
        {
#if defined _DEBUG
#    if defined FB_PLATFORM_APPLE
            /*if( m_osWindow && m_window )
            {
                auto osWindowSize = Vector2I( m_osWindow->getWidth(), m_osWindow->getHeight() );
                auto windowSize = Vector2I( m_window->getWidth(), m_window->getHeight() );
                FB_ASSERT( osWindowSize == windowSize );
            }
             */
#    endif
#endif

#if defined FB_PLATFORM_APPLE
            /*if( m_osWindow )
            {
                auto w = m_osWindow->getWidth();
                auto h = m_osWindow->getHeight();
                return Vector2F( w, h );
            }
            */
#endif
            if( m_window )
            {
                auto w = m_window->getWidth();
                auto h = m_window->getHeight();
                return Vector2I( w, h );
            }

            return m_size;
        }

        void CWindowOgre::setSize( const Vector2I &size )
        {
            m_size = size;
        }

        void CWindowOgre::setColourDepth( u32 val )
        {
        }

        String CWindowOgre::getWindowHandleAsString() const
        {
            return m_windowHandle;
        }

        void CWindowOgre::setWindowHandleAsString( const String &handle )
        {
            m_windowHandle = handle;
        }

        CWindowOgre::WindowStateListener::~WindowStateListener()
        {
        }

        void CWindowOgre::WindowStateListener::handleStateChanged( SmartPtr<IState> &state )
        {
        }

        void CWindowOgre::WindowStateListener::handleStateChanged(
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

        void CWindowOgre::WindowStateListener::handleQuery( SmartPtr<IStateQuery> &query )
        {
        }

        CWindowOgre *CWindowOgre::WindowStateListener::getOwner() const
        {
            return m_owner;
        }

        void CWindowOgre::WindowStateListener::setOwner( CWindowOgre *owner )
        {
            m_owner = owner;
        }

        void CWindowOgre::WindowListener::windowMoved( Ogre::RenderWindow *rw )
        {
            (void)rw;
        }

        void CWindowOgre::WindowListener::windowResized( Ogre::RenderWindow *rw )
        {
            (void)rw;
        }

        bool CWindowOgre::WindowListener::windowClosing( Ogre::RenderWindow *rw )
        {
            (void)rw;

            auto applicationManager = core::IApplicationManager::instance();
            FB_ASSERT( applicationManager );

            applicationManager->setQuit( true );

            return true;
        }

        void CWindowOgre::WindowListener::windowClosed( Ogre::RenderWindow *rw )
        {
            (void)rw;
        }

        void CWindowOgre::WindowListener::windowFocusChange( Ogre::RenderWindow *rw )
        {
            (void)rw;
        }

        CWindowOgre *CWindowOgre::WindowListener::getOwner() const
        {
            return m_owner;
        }

        void CWindowOgre::WindowListener::setOwner( CWindowOgre *owner )
        {
            m_owner = owner;
        }
    }  // namespace render
}  // namespace fb
