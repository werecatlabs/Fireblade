#ifndef WindowWin32_h__
#define WindowWin32_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>

namespace fb
{
    namespace render
    {

        class WindowWin32
        {
        public:
            WindowWin32();
            ~WindowWin32();

            void load( SmartPtr<ISharedObject> data );
            void unload( SmartPtr<ISharedObject> data );

            void getWindowHandle( void *pData );

            DWORD getWindowStyle( bool fullScreen ) const;

	        void switchMode( u32 width, u32 height, u32 frequency );

            SmartPtr<IWindow> getRenderWindow() const;
            void setRenderWindow( SmartPtr<IWindow> window );

            void windowMovedOrResized();

        protected:
            /// The name of this target.
            String mName;

            /// The priority of the render target.
            u8 mPriority = 0;

            bool mIsFullScreen = false;

            u32 mTop = -1;
            u32 mLeft = -1;
            u32 mWidth = 1280;
            u32 mHeight = 720;

            HWND mHWnd;  // Win32 Window handle
            HDC mHDC;
            HGLRC mGlrc;
            char *mDeviceName = nullptr;
            bool mOwnsGLContext = false;
            bool mSizing = false;
            int mDisplayFrequency = 0; // fullscreen only, to restore display
            u32 mColourDepth = 0;
            DWORD mWindowedWinStyle = 0;   // Windowed mode window style flags.
            DWORD mFullscreenWinStyle = 0;  // Fullscreen mode window style flags.
        
            SmartPtr<render::IWindow> m_renderWindow;
        };

    }  // namespace render
}  // namespace fb

#endif  // WindowWin32_h__
