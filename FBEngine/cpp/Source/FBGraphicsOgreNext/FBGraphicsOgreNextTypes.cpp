#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/FBGraphicsOgreNextTypes.h>
#include <FBCore/Core/StringUtil.h>  //ß

namespace fb
{

    const hash32 GraphicsOgreNextTypes::FRAME_EVENT_PRE_RENDER = StringUtil::getHash( "pre_render" );
    const hash32 GraphicsOgreNextTypes::FRAME_EVENT_RENDER_QUEUED =
        StringUtil::getHash( "render_queued" );
    const hash32 GraphicsOgreNextTypes::FRAME_EVENT_POST_RENDER = StringUtil::getHash( "post_render" );

    const hash32 GraphicsOgreNextTypes::VISIBILITY_MASK_HASH = StringUtil::getHash( "visibilityMask" );
    const hash32 GraphicsOgreNextTypes::STATE_MESSAGE_CAST_SHADOWS =
        StringUtil::getHash( "cast_shadows" );
    const hash32 GraphicsOgreNextTypes::STATE_MESSAGE_RENDER_QUEUE =
        StringUtil::getHash( "renderQueue" );

    const hash32 GraphicsOgreNextTypes::STATE_MESSAGE_ATTENUATION = StringUtil::getHash( "attenuation" );
    const hash32 GraphicsOgreNextTypes::STATE_MESSAGE_DIRECTION = StringUtil::getHash( "direction" );

}  // namespace fb
