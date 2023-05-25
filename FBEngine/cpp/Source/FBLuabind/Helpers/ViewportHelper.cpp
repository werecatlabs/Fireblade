#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Helpers/ViewportHelper.h"
#include <FBCore/FBCore.h>

namespace fb
{

    Vector2I ViewportHelper::_getActualSize( IViewport *vp )
    {
        return Vector2I::zero();// Vector2I( vp->getActualWidth(), vp->getActualHeight() );
    }

    void ViewportHelper::_setClearEveryFrame( IViewport *vp, bool clear )
    {
        vp->setClearEveryFrame( clear );
    }

    void ViewportHelper::_setClearEveryFrameFlags( IViewport *vp, bool clear, u32 buffers )
    {
        vp->setClearEveryFrame( clear, buffers );
    }

    SmartPtr<render::IViewport> ViewportHelper::_addViewportMinArgs( IRenderTarget *rt, hash32 id,
                                                                     SmartPtr<render::ICamera> camera )
    {
        return rt->addViewport( id, camera );
    }

    SmartPtr<render::IViewport> ViewportHelper::_addViewportZorderArgs( IRenderTarget *rt, hash32 id,
                                                                        SmartPtr<render::ICamera> camera,
                                                                        s32 zorder )
    {
        return rt->addViewport( id, camera, zorder );
    }

    SmartPtr<render::IViewport> ViewportHelper::_addViewportAllArgs( IRenderTarget *rt, hash32 id,
                                                                     SmartPtr<render::ICamera> camera,
                                                                     s32 ZOrder, f32 left, f32 top,
                                                                     f32 width, f32 height )
    {
        return rt->addViewport( id, camera, ZOrder, left, top, width, height );
    }

    void ViewportHelper::setVisibilityMask( IViewport *vp, lua_Integer mask )
    {
        u32 uiMask = *reinterpret_cast<u32 *>( &mask );
        vp->setVisibilityMask( uiMask );
    }

    lua_Integer ViewportHelper::getVisibilityMask( IViewport *vp )
    {
        u32 mask = vp->getVisibilityMask();
        return *reinterpret_cast<lua_Integer *>( &mask );
    }

}  // namespace fb
