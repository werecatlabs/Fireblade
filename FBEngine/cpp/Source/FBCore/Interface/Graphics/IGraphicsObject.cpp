#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IGraphicsObject.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::render
{
    const hash_type IGraphicsObject::STATE_MESSAGE_RENDER_QUEUE = StringUtil::getHash( "renderQueue" );
    const hash_type IGraphicsObject::STATE_MESSAGE_DIRECTION = StringUtil::getHash( "direction" );

    const u32 IGraphicsObject::AllProperties = ( 1 << 0 ); /* 0x01*/

    const u32 IGraphicsObject::OverlayFlag = 1 << 1;
    const u32 IGraphicsObject::UiFlag = 1 << 2;
    const u32 IGraphicsObject::SceneFlag = 1 << 3;

    FB_CLASS_REGISTER_DERIVED( fb, IGraphicsObject, ISharedObject );
}  // namespace fb::render
