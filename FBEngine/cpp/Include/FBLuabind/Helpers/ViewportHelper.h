#ifndef ViewportHelper_h__
#define ViewportHelper_h__

namespace fb
{
    using namespace fb::render;

    
    class ViewportHelper
    {
    public:
        static Vector2I _getActualSize( render::IViewport *vp );
        static void _setClearEveryFrame( IViewport *vp, bool clear );
        static void _setClearEveryFrameFlags( IViewport *vp, bool clear, u32 buffers );

        static SmartPtr<render::IViewport> _addViewportMinArgs( IRenderTarget *rt, hash32 id,
                                                                SmartPtr<render::ICamera> camera );
        static SmartPtr<render::IViewport> _addViewportZorderArgs( IRenderTarget *rt, hash32 id,
                                                                   SmartPtr<render::ICamera> camera,
                                                                   s32 zorder );
        static SmartPtr<render::IViewport> _addViewportAllArgs( IRenderTarget *rt, hash32 id,
                                                                SmartPtr<render::ICamera> camera,
                                                                s32 ZOrder, f32 left, f32 top, f32 width,
                                                                f32 height );

        static lua_Integer getVisibilityMask( IViewport *vp );

        static void setVisibilityMask( IViewport *vp, lua_Integer mask );
    };

}  // end namespace fb

#endif  // ViewportHelper_h__
