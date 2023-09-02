#ifndef _CWindow_H
#define _CWindow_H

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBGraphicsOgre/Wrapper/CRenderTargetOgre.h>
#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Interface/System/IStateListener.h>

#include <FBCore/Core/Array.h>
#include <OgreWindowEventUtilities.h>

namespace fb
{
    namespace render
    {
        /**
         * Implementation of the IWindow interface using the Ogre graphics engine.
         */
        class CWindowOgre : public CRenderTargetOgre<IWindow>
        {
        public:
            CWindowOgre();
            ~CWindowOgre() override;

            /** @copydoc ISharedObject::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc ISharedObject::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            void handleEvent( SmartPtr<IWindowEvent> event );

            void update() override;

            void initialise( Ogre::RenderWindow *window );

            /** @copydoc IWindow::getTitle */
            String getTitle() const override;

            /** @copydoc IWindow::setTitle */
            void setTitle( const String &title ) override;

            /** @copydoc IWindow::setFullscreen */
            void setFullscreen( bool fullScreen, unsigned int width, unsigned int height ) override;

            /** @copydoc IWindow::destroy */
            void destroy() override;

            /** @copydoc IWindow::resize */
            void resize( u32 width, u32 height ) override;

            /** @copydoc IWindow::windowMovedOrResized */
            void windowMovedOrResized() override;

            /** @copydoc IWindow::reposition */
            void reposition( s32 left, s32 top ) override;

            /** @copydoc IWindow::maximize */
            void maximize() override;

            /** @copydoc IWindow::isVisible */
            bool isVisible() const override;

            /** @copydoc IWindow::setVisible */
            void setVisible( bool visible ) override;

            /** @copydoc IWindow::isClosed */
            bool isClosed() const override;

            /** @copydoc IWindow::isPrimary */
            bool isPrimary() const override;

            /** @copydoc IWindow::isFullScreen */
            bool isFullScreen() const override;

            /** @copydoc IWindow::setFullscreen */
            void setFullscreen( bool fullscreen );

            /** @copydoc IWindow::getMetrics */
            void getMetrics( u32 &width, u32 &height, u32 &colourDepth, s32 &left, s32 &top ) override;

            /** @copydoc IWindow::suggestPixelFormat */
            u8 suggestPixelFormat() const override;

            /** @copydoc IWindow::isDeactivatedOnFocusChange */
            bool isDeactivatedOnFocusChange() const override;

            /** @copydoc IWindow::setDeactivateOnFocusChange */
            void setDeactivateOnFocusChange( bool deactivate ) override;

            /** @copydoc IWindow::getCustomAttribute */
            void getCustomAttribute( const String &name, void *pData ) override;

            /** @copydoc IRenderTarget::getObject */
            void _getObject( void **ppObject ) const override;

            /** @copydoc IRenderTarget::getViewports */
            Array<SmartPtr<IViewport>> getViewports() const override;

            /** Gets the underlying Ogre window. */
            Ogre::RenderWindow *getWindow() const;

            /** Sets the underlying Ogre window. */
            void setWindow( Ogre::RenderWindow *window );

            /** @copydoc IWindow::getSize */
            Vector2I getSize() const override;

            /** @copydoc IWindow::setSize */
            void setSize( const Vector2I &size ) override;

            /**
             * Sets the color depth of the window.
             *
             * @param val The color depth to set.
             */
            void setColourDepth( u32 val ) override;

            /**
             * Gets the window handle as a string.
             *
             * @return A string containing the window handle.
             */
            String getWindowHandleAsString() const override;

            /**
             * Sets the window handle from a string.
             *
             * @param handle The string containing the window handle to set.
             */
            void setWindowHandleAsString( const String &handle ) override;

            /**
             * Gets the window handle as a void pointer.
             *
             * @param pData The void pointer to hold the window handle.
             */
            void getWindowHandle( void *pData ) override;

            /**
             * Gets the device handle as a void pointer.
             *
             * @param pData The void pointer to hold the device handle.
             */
            void getDeviceHandle( void *pData ) override;

            /**
             * Adds a window listener to the window.
             *
             * @param listener The window listener to add.
             */
            void addListener( SmartPtr<IWindowListener> listener ) override;

            /**
             * Removes a window listener from the window.
             *
             * @param listener The window listener to remove.
             */
            void removeListener( SmartPtr<IWindowListener> listener ) override;

            /**
             * Gets all the window listeners attached to the window.
             *
             * @return An array of smart pointers to the window listeners.
             */
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

                CWindowOgre *getOwner() const;
                void setOwner( CWindowOgre *owner );

            protected:
                CWindowOgre *m_owner = nullptr;
            };

            class WindowListener : public Ogre::WindowEventListener
            {
            public:
                WindowListener() = default;
                ~WindowListener() override = default;

                /** @copydoc Ogre::WindowEventListener::windowMoved */
                void windowMoved( Ogre::RenderWindow *rw );

                /** @copydoc Ogre::WindowEventListener::windowResized */
                void windowResized( Ogre::RenderWindow *rw );

                /** @copydoc Ogre::WindowEventListener::windowClosing */
                bool windowClosing( Ogre::RenderWindow *rw ) override;

                /** @copydoc Ogre::WindowEventListener::windowClosed */
                void windowClosed( Ogre::RenderWindow *rw );

                /** @copydoc Ogre::WindowEventListener::windowFocusChange */
                void windowFocusChange( Ogre::RenderWindow *rw );

                CWindowOgre *getOwner() const;

                void setOwner( CWindowOgre *owner );

            protected:
                CWindowOgre *m_owner = nullptr;
            };

            Array<SmartPtr<IWindowListener>> m_listeners;

            Ogre::RenderWindow *m_window = nullptr;
            Ogre::WindowEventListener *m_windowListener = nullptr;

            WindowWin32 *m_windowWin32 = nullptr;
            WindowMacOS *m_osWindow = nullptr;

            Array<SmartPtr<IViewport>> m_viewports;

            u32 m_colourDepth = 0;
            bool m_isFullscreen = false;

            String m_title;
            String m_windowHandle;
        };
    }  // end namespace render
}  // end namespace fb

#endif
