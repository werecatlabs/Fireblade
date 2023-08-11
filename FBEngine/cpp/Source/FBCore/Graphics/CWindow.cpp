#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/CWindow.h>
#include <FBCore/FBCore.h>

namespace fb
{
    namespace render
    {
        CWindow::CWindow()
        {
        }

        CWindow::~CWindow()
        {
        }

        void CWindow::load( SmartPtr<ISharedObject> data )
        {
        }

        void CWindow::unload( SmartPtr<ISharedObject> data )
        {
        }

        void CWindow::setPriority( u8 priority )
        {
        }

        u8 CWindow::getPriority() const
        {
            return 0;
        }

        bool CWindow::isActive() const
        {
            return false;
        }

        void CWindow::setActive( bool state )
        {
        }

        void CWindow::setAutoUpdated( bool autoupdate )
        {
        }

        bool CWindow::isAutoUpdated( void ) const
        {
            return false;
        }

        void CWindow::copyContentsToMemory( void *buffer, u32 size,
                                            FrameBuffer bufferId /*= FrameBuffer::Auto */ )
        {
        }

        void CWindow::getMetrics( u32 &width, u32 &height, u32 &colourDepth )
        {
        }

        u32 CWindow::getWidth() const
        {
            return 0;
        }

        u32 CWindow::getHeight() const
        {
            return 0;
        }

        u32 CWindow::getColourDepth() const
        {
            return 0;
        }

        SmartPtr<IViewport> CWindow::addViewport(
            hash32 id, SmartPtr<ICamera> camera, s32 ZOrder /*= 0*/, f32 left /*= 0.0f*/,
            f32 top /*= 0.0f*/, f32 width /*= 1.0f*/, f32 height /*= 1.0f */ )
        {
            return SmartPtr<IViewport>();
        }

        u32 CWindow::getNumViewports() const
        {
            return 0;
        }

        SmartPtr<IViewport> CWindow::getViewport( u32 index )
        {
            return SmartPtr<IViewport>();
        }

        SmartPtr<IViewport> CWindow::getViewportById( hash32 id )
        {
            return SmartPtr<IViewport>();
        }

        void CWindow::removeViewport( SmartPtr<IViewport> vp )
        {
        }

        void CWindow::removeAllViewports()
        {
        }

        void CWindow::getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS, f32 &worstFPS ) const
        {
        }

        IRenderTarget::RenderTargetStats CWindow::getRenderTargetStats() const
        {
            return IRenderTarget::RenderTargetStats();
        }
    } // namespace render
}     // namespace fb
