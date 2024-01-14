#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IOverlay.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, IOverlay, ISharedObject );

    const hash_type IOverlay::STATE_MESSAGE_ATTACH_OBJECT = StringUtil::getHash( "attachObject" );
    const hash_type IOverlay::STATE_MESSAGE_DETACH_OBJECT = StringUtil::getHash( "detachObject" );
    const hash_type IOverlay::STATE_MESSAGE_DETACH_ALL_OBJECTS =
        StringUtil::getHash( "detachAllObject" );

}  // namespace fb::render
