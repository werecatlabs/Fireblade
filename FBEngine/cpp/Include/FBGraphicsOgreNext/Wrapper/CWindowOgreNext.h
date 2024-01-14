#ifndef __CWindowOgreNext_H
#define __CWindowOgreNext_H

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBGraphicsOgreNext/Wrapper/CRenderTargetOgreNext.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/System/IStateListener.h>
#include <FBCore/Core/ConcurrentQueue.h>
#include <OgreWindowEventUtilities.h>

namespace fb
{
    namespace render
    {

        class CWindowOgreNext : public CRenderTargetOgreNext<IWindow>
        {
        public:
            CWindowOgreNext();
            ~CWindowOgreNext() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;

            void initialise( Ogre::Window *window );

            //
            // IRenderTarget functions
            //
            void update() override;
            void swapBuffers() override;

            void setPriority( u8 priority ) override;
            u8 getPriority() const override;

            bool isActive() const override;
            void setActive( bool state ) override;

            void setAutoUpdated( bool autoupdate ) override;
            bool isAutoUpdated( void ) const override;

            void copyContentsToMemory( void *buffer, u32 size,
                                       FrameBuffer bufferId = FrameBuffer::Auto ) override;

            void getMetrics( u32 &width, u32 &height, u32 &colourDepth );
            u32 getWidth() const;
            u32 getHeight() const;
            u32 getColourDepth() const override;

            SmartPtr<IViewport> addViewport( hash32 id, SmartPtr<ICamera> camera, s32 ZOrder = 0,
                                             f32 left = 0.0f, f32 top = 0.0f, f32 width = 1.0f,
                                             f32 height = 1.0f ) override;

            u32 getNumViewports() const override;
            SmartPtr<IViewport> getViewport( u32 index ) override;
            SmartPtr<IViewport> getViewportById( hash32 id ) override;
            void removeViewport( SmartPtr<IViewport> vp ) override;
            void removeAllViewports() override;
            void getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS, f32 &worstFPS ) const;
            RenderTargetStats getRenderTargetStats() const override;

            //
            // IWindow functions
            //
            void handleEvent( SmartPtr<IWindowEvent> event ) override;

            /** @copydoc IWindow::getTitle */
            String getTitle() const override;

            /** @copydoc IWindow::setTitle */
            void setTitle( const String &title ) override;

            void setFullscreen( bool fullScreen, unsigned int width, unsigned int height ) override;

            void destroy() override;

            void resize( u32 width, u32 height ) override;

            void windowMovedOrResized() override;

            void reposition( s32 left, s32 top ) override;

            void maximize() override;

            bool isVisible() const override;

            void setVisible( bool visible ) override;

            bool isClosed() const override;

            bool isPrimary() const override;

            bool isFullScreen() const override;
            void setFullscreen( bool fullscreen );

            void getMetrics( unsigned int &width, unsigned int &height, unsigned int &colourDepth,
                             int &left, int &top ) override;

            u8 suggestPixelFormat() const override;

            bool isDeactivatedOnFocusChange() const override;

            void setDeactivateOnFocusChange( bool deactivate ) override;

            void getCustomAttribute( const String &name, void *pData ) override;

            void _getObject( void **ppObject ) const override;

            Array<SmartPtr<IViewport>> getViewports() const override;

            Ogre::Window *getWindow() const;
            void setWindow( Ogre::Window *val );

            Vector2I getSize() const;
            void setSize( const Vector2I &size );

            void setColourDepth( u32 val ) override;

            String getWindowHandleAsString() const override;
            void setWindowHandleAsString( const String &handle ) override;

            void getWindowHandle( void *pData ) override;
            void getDeviceHandle( void *pData ) override;

            void addListener( SmartPtr<IWindowListener> listener ) override;

            void removeListener( SmartPtr<IWindowListener> listener ) override;

            Array<SmartPtr<IWindowListener>> getListeners() const override;

            FB_CLASS_REGISTER_DECL;

        protected:
            class WindowStateListener : public IStateListener
            {
            public:
                WindowStateListener() = default;
                ~WindowStateListener() override;

                void handleStateChanged( const SmartPtr<IStateMessage> &message ) override;
                void handleStateChanged( SmartPtr<IState> &state ) override;
                void handleQuery( SmartPtr<IStateQuery> &query ) override;

                CWindowOgreNext *getOwner() const;
                void setOwner( CWindowOgreNext *owner );

            protected:
                CWindowOgreNext *m_owner = nullptr;
            };

            class WindowListener : public Ogre::WindowEventListener
            {
            public:
                WindowListener() = default;
                ~WindowListener() override = default;

                bool windowClosing( Ogre::Window *rw ) override;

                CWindowOgreNext *getOwner() const;

                void setOwner( CWindowOgreNext *owner );

            protected:
                CWindowOgreNext *m_owner = nullptr;
            };

            void handleQueuedEvent( SmartPtr<IWindowEvent> event );

            ConcurrentQueue<SmartPtr<IWindowEvent>> m_eventQueue;

            Array<SmartPtr<IWindowListener>> m_listeners;

            SmartPtr<RenderWindowState> m_state;

            WindowListener *m_windowEventListener = nullptr;
            Ogre::Window *m_window = nullptr;

            WindowWin32 *m_windowWin32 = nullptr;
            WindowMacOS *m_osWindow = nullptr;

            Vector2I m_size;
            u32 m_colourDepth = 0;
            bool m_isFullscreen = false;

            String m_label;
            String m_windowHandle;
        };
    }  // end namespace render
}  // end namespace fb

#endif
