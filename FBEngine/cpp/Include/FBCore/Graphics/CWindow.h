#ifndef CWindow_h__
#define CWindow_h__

#include <FBCore/Interface/Graphics/IWindow.h>
#include <FBCore/Graphics/CSharedGraphicsObject.h>

namespace fb
{
    namespace render
    {

        class CWindow : public CSharedGraphicsObject<IWindow>
        {
        public:
            CWindow();
            ~CWindow() override;

            void load( SmartPtr<ISharedObject> data ) override;
            void unload( SmartPtr<ISharedObject> data ) override;
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
        };

    }  // namespace render
}  // namespace fb

#endif  // CWindow_h__
