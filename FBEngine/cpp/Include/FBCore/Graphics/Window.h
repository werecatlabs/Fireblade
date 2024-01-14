#ifndef __Window_h__
#define __Window_h__

#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Graphics/SharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        /** Implementation of IWindow interface. Useful as a base class.
         */
        class Window : public SharedGraphicsObject<IWindow>
        {
        public:
            /** Default constructor.
             */
            Window();

            /** Destructor.
             */
            ~Window() override;

            /** @copydoc IWindow::load */
            void load( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IWindow::unload */
            void unload( SmartPtr<ISharedObject> data ) override;

            /** @copydoc IWindow::setPriority */
            void setPriority( u8 priority ) override;

            /** @copydoc IWindow::getPriority */
            u8 getPriority() const override;

            /** @copydoc IWindow::isActive */
            bool isActive() const override;

            /** @copydoc IWindow::setActive */
            void setActive( bool state ) override;

            /** @copydoc IWindow::setAutoUpdated */
            void setAutoUpdated( bool autoupdate ) override;

            /** @copydoc IWindow::isAutoUpdated */
            bool isAutoUpdated( void ) const override;

            /** @copydoc IWindow::copyContentsToMemory */
            void copyContentsToMemory( void *buffer, u32 size,
                                       FrameBuffer bufferId = FrameBuffer::Auto ) override;
            void getMetrics( u32 &width, u32 &height, u32 &colourDepth );
            u32 getWidth() const;
            u32 getHeight() const;
            u32 getColourDepth() const override;

            /** @copydoc IWindow::addViewport */
            SmartPtr<IViewport> addViewport( hash32 id, SmartPtr<ICamera> camera, s32 ZOrder = 0,
                                             f32 left = 0.0f, f32 top = 0.0f, f32 width = 1.0f,
                                             f32 height = 1.0f ) override;

            /** @copydoc IWindow::getNumViewports */
            u32 getNumViewports() const override;

            /** @copydoc IWindow::getViewport */
            SmartPtr<IViewport> getViewport( u32 index ) override;

            /** @copydoc IWindow::getViewportById */
            SmartPtr<IViewport> getViewportById( hash32 id ) override;

            /** @copydoc IWindow::removeViewport */
            void removeViewport( SmartPtr<IViewport> vp ) override;

            /** @copydoc IWindow::removeAllViewports */
            void removeAllViewports() override;

            void getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS, f32 &worstFPS ) const;

            /** @copydoc IWindow::getRenderTargetStats */
            RenderTargetStats getRenderTargetStats() const override;
        };

    }  // namespace render
}  // namespace fb

#endif  // CWindow_h__
