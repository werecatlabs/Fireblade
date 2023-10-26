#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/Window/Windows/WindowWin32.h>
#include <FBCore/FBCore.h>
#include <Ogre.h>
#include <OgreWindow.h>

namespace fb
{
    namespace render
    {
        LRESULT CALLBACK _WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
        {
            if( uMsg == WM_CREATE )
            {  // Store pointer to Win32Window in user data area
                auto pLParam = (LPCREATESTRUCT)lParam;
                auto pCreateParams = (LONG_PTR)( pLParam->lpCreateParams );
                SetWindowLongPtr( hWnd, GWLP_USERDATA, pCreateParams );
                return 0;
            }

            auto win = (WindowWin32 *)GetWindowLongPtr( hWnd, GWLP_USERDATA );
            if( !win )
            {
                return DefWindowProc( hWnd, uMsg, wParam, lParam );
            }

            auto renderWindow = win->getRenderWindow();
            if( renderWindow )
            {
                auto e = fb::make_ptr<fb::WindowMessageData>();
                e->setMessage( uMsg );
                e->setWParam( wParam );
                e->setLParam( lParam );
                renderWindow->handleEvent( e );
            }

            return DefWindowProc( hWnd, uMsg, wParam, lParam );
        }

        WindowWin32::WindowWin32()
        {
        }

        WindowWin32::~WindowWin32()
        {
            unload( nullptr );
        }

        void WindowWin32::load( SmartPtr<ISharedObject> data )
        {
            unsigned int width = 1280;
            unsigned int height = 720;
            int left = -1;  // Defaults to screen center
            int top = -1;   // Defaults to screen center
            WNDPROC windowProc = _WndProc;
            String title = "Test";
            bool hidden = false;
            String border;
            bool outerSize = false;
            bool hwGamma = false;
            bool enableDoubleClick = false;
            int monitorIndex = -1;
            HMONITOR hMonitor = nullptr;
            HINSTANCE hInst = nullptr;
            bool fullScreen = false;

            // WNDCLASS dummyClass;
            // memset( &dummyClass, 0, sizeof( WNDCLASS ) );
            // dummyClass.style = CS_OWNDC;
            // dummyClass.hInstance = hinst;
            // dummyClass.lpfnWndProc = dummyWndProc;
            // dummyClass.lpszClassName = dummyText;
            // RegisterClass( &dummyClass );

            // HWND hwnd = CreateWindow( dummyText, dummyText, WS_POPUP | WS_CLIPCHILDREN, 0, 0, 32, 32,
            // 0,
            //                           0, hinst, 0 );

            auto mIsExternal = false;
            if( !mIsExternal )
            {
                DWORD dwStyleEx = 0;
                MONITORINFOEX monitorInfoEx;
                RECT rc;

                // If we didn't specified the adapter index, or if it didn't find it
                if( hMonitor == nullptr )
                {
                    POINT windowAnchorPoint;

                    // Fill in anchor point.
                    windowAnchorPoint.x = left;
                    windowAnchorPoint.y = top;

                    // Get the nearest monitor to this window.
                    hMonitor = MonitorFromPoint( windowAnchorPoint, MONITOR_DEFAULTTOPRIMARY );
                }

                // Get the target monitor info
                memset( &monitorInfoEx, 0, sizeof( MONITORINFOEX ) );
                monitorInfoEx.cbSize = sizeof( MONITORINFOEX );
                GetMonitorInfo( hMonitor, &monitorInfoEx );

                size_t devNameLen = strlen( monitorInfoEx.szDevice );
                mDeviceName = new char[devNameLen + 1];

                strcpy( mDeviceName, monitorInfoEx.szDevice );

                // Update window style flags.
                mFullscreenWinStyle = ( hidden ? 0 : WS_VISIBLE ) | WS_CLIPCHILDREN | WS_POPUP;
                mWindowedWinStyle = ( hidden ? 0 : WS_VISIBLE ) | WS_CLIPCHILDREN;

                if( border == "none" )
                    mWindowedWinStyle |= WS_POPUP;
                else if( border == "fixed" )
                    mWindowedWinStyle |=
                        WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
                else
                    mWindowedWinStyle |= WS_OVERLAPPEDWINDOW;

                // No specified top left -> Center the window in the middle of the monitor
                if( left == -1 || top == -1 )
                {
                    auto screenw = monitorInfoEx.rcWork.right - monitorInfoEx.rcWork.left;
                    auto screenh = monitorInfoEx.rcWork.bottom - monitorInfoEx.rcWork.top;

                    unsigned int winWidth, winHeight;
                    // adjustWindow( width, height, &winWidth, &winHeight );

                    // clamp window dimensions to screen size
                    int outerw = ( winWidth < screenw ) ? winWidth : screenw;
                    int outerh = ( winHeight < screenh ) ? winHeight : screenh;

                    if( left == -1 )
                        left = monitorInfoEx.rcWork.left + ( screenw - outerw ) / 2;
                    else if( monitorIndex != -1 )
                        left += monitorInfoEx.rcWork.left;

                    if( top == -1 )
                        top = monitorInfoEx.rcWork.top + ( screenh - outerh ) / 2;
                    else if( monitorIndex != -1 )
                        top += monitorInfoEx.rcWork.top;
                }
                else if( monitorIndex != -1 )
                {
                    left += monitorInfoEx.rcWork.left;
                    top += monitorInfoEx.rcWork.top;
                }

                mWidth = width;
                mHeight = height;
                mTop = top;
                mLeft = left;

                if( mIsFullScreen )
                {
                    dwStyleEx |= WS_EX_TOPMOST;
                    mTop = monitorInfoEx.rcMonitor.top;
                    mLeft = monitorInfoEx.rcMonitor.left;
                }
                else
                {
                    int screenw = GetSystemMetrics( SM_CXSCREEN );
                    int screenh = GetSystemMetrics( SM_CYSCREEN );

                    if( !outerSize )
                    {
                        // Calculate window dimensions required
                        // to get the requested client area
                        SetRect( &rc, 0, 0, mWidth, mHeight );
                        AdjustWindowRect( &rc, getWindowStyle( fullScreen ), false );
                        mWidth = rc.right - rc.left;
                        mHeight = rc.bottom - rc.top;

                        // Clamp window rect to the nearest display monitor.
                        if( mLeft < monitorInfoEx.rcWork.left )
                            mLeft = monitorInfoEx.rcWork.left;

                        if( mTop < monitorInfoEx.rcWork.top )
                            mTop = monitorInfoEx.rcWork.top;

                        if( static_cast<int>( mWidth ) > monitorInfoEx.rcWork.right - mLeft )
                            mWidth = monitorInfoEx.rcWork.right - mLeft;

                        if( static_cast<int>( mHeight ) > monitorInfoEx.rcWork.bottom - mTop )
                            mHeight = monitorInfoEx.rcWork.bottom - mTop;
                    }
                }

                UINT classStyle = CS_OWNDC;
                if( enableDoubleClick )
                    classStyle |= CS_DBLCLKS;

                // register class and create window
                WNDCLASS wc = { classStyle,
                                windowProc,
                                0,
                                0,
                                hInst,
                                LoadIcon( nullptr, IDI_APPLICATION ),
                                LoadCursor( nullptr, IDC_ARROW ),
                                static_cast<HBRUSH>( GetStockObject( BLACK_BRUSH ) ),
                                nullptr,
                                "OgreGLWindow" };
                RegisterClass( &wc );

                if( mIsFullScreen )
                {
                    switchMode( mWidth, mHeight, mDisplayFrequency );
                }

                // Pass pointer to self as WM_CREATE parameter
                mHWnd = CreateWindowEx( dwStyleEx, "OgreGLWindow", title.c_str(),
                                        getWindowStyle( fullScreen ), mLeft, mTop, mWidth, mHeight,
                                        nullptr, nullptr, hInst, this );

                // LogManager::getSingleton().stream()
                //     << "Created Win32Window '" << mName << "' : " << mWidth << "x" << mHeight << ", "
                //     << mColourDepth << "bpp";
            }
        }

        void WindowWin32::unload( SmartPtr<ISharedObject> data )
        {
            if( mHWnd )
            {
                DestroyWindow( mHWnd );
                mHWnd = nullptr;
            }
        }

        void WindowWin32::getWindowHandle( void *pData )
        {
            *static_cast<void **>( pData ) = mHWnd;
        }

        DWORD WindowWin32::getWindowStyle( bool fullScreen ) const
        {
            if( fullScreen )
            {
                return mFullscreenWinStyle;
            }

            return mWindowedWinStyle;
        }

        void WindowWin32::switchMode( u32 width, u32 height, u32 frequency )
        {
            DEVMODE displayDeviceMode = {};

            displayDeviceMode.dmSize = sizeof( DEVMODE );
            displayDeviceMode.dmBitsPerPel = mColourDepth;
            displayDeviceMode.dmPelsWidth = width;
            displayDeviceMode.dmPelsHeight = height;
            displayDeviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            if( frequency )
            {
                displayDeviceMode.dmDisplayFrequency = frequency;
                displayDeviceMode.dmFields |= DM_DISPLAYFREQUENCY;
                if( ChangeDisplaySettingsEx( mDeviceName, &displayDeviceMode, nullptr,
                                             CDS_FULLSCREEN | CDS_TEST,
                                             nullptr ) != DISP_CHANGE_SUCCESSFUL )
                {
                    FB_LOG( "ChangeDisplaySettings with user display frequency failed" );
                    displayDeviceMode.dmFields ^= DM_DISPLAYFREQUENCY;
                }
            }

            if( ChangeDisplaySettingsEx( mDeviceName, &displayDeviceMode, nullptr, CDS_FULLSCREEN,
                                         nullptr ) != DISP_CHANGE_SUCCESSFUL )
            {
                FB_LOG_ERROR( "ChangeDisplaySettings failed" );
            }
        }

        SmartPtr<IWindow> WindowWin32::getRenderWindow() const
        {
            return m_renderWindow;
        }

        void WindowWin32::setRenderWindow( SmartPtr<IWindow> window )
        {
            m_renderWindow = window;
        }

        void WindowWin32::windowMovedOrResized()
        {
            if( auto renderWindow = getRenderWindow() )
            {
                renderWindow->windowMovedOrResized();
            }
        }

    }  // namespace render
}  // namespace fb
