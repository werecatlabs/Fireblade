#include <FBCore/FBCorePCH.h>
#include <FBCore/Graphics/Window.h>
#include <FBCore/Interface/Graphics/ICamera.h>
#include <FBCore/Interface/Graphics/IViewport.h>

namespace fb::render
{

    Window::Window() = default;

    Window::~Window() = default;

    void Window::load( SmartPtr<ISharedObject> data )
    {
    }

    void Window::unload( SmartPtr<ISharedObject> data )
    {
    }

    void Window::setPriority( u8 priority )
    {
    }

    auto Window::getPriority() const -> u8
    {
        return 0;
    }

    auto Window::isActive() const -> bool
    {
        return false;
    }

    void Window::setActive( bool state )
    {
    }

    void Window::setAutoUpdated( bool autoupdate )
    {
    }

    auto Window::isAutoUpdated() const -> bool
    {
        return false;
    }

    void Window::copyContentsToMemory( void *buffer, u32 size,
                                       FrameBuffer bufferId /*= FrameBuffer::Auto */ )
    {
    }

    void Window::getMetrics( u32 &width, u32 &height, u32 &colourDepth )
    {
    }

    auto Window::getWidth() const -> u32
    {
        return 0;
    }

    auto Window::getHeight() const -> u32
    {
        return 0;
    }

    auto Window::getColourDepth() const -> u32
    {
        return 0;
    }

    auto Window::addViewport( hash32 id, SmartPtr<ICamera> camera, s32 ZOrder /*= 0*/,
                              f32 left /*= 0.0f*/, f32 top /*= 0.0f*/, f32 width /*= 1.0f*/,
                              f32 height /*= 1.0f */ ) -> SmartPtr<IViewport>
    {
        return {};
    }

    auto Window::getNumViewports() const -> u32
    {
        return 0;
    }

    auto Window::getViewport( u32 index ) -> SmartPtr<IViewport>
    {
        return {};
    }

    auto Window::getViewportById( hash32 id ) -> SmartPtr<IViewport>
    {
        return {};
    }

    void Window::removeViewport( SmartPtr<IViewport> vp )
    {
    }

    void Window::removeAllViewports()
    {
    }

    void Window::getStatistics( f32 &lastFPS, f32 &avgFPS, f32 &bestFPS, f32 &worstFPS ) const
    {
    }

    auto Window::getRenderTargetStats() const -> IRenderTarget::RenderTargetStats
    {
        return {};
    }

}  // namespace fb::render
