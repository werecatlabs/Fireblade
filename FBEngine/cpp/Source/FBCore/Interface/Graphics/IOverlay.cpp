#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IOverlay.h>
#include <FBCore/Rtti/RttiClassDefinition.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IOverlay, ISharedObject );

        const hash_type IOverlay::STATE_MESSAGE_ATTACH_OBJECT = StringUtil::getHash( "attachObject" );
        const hash_type IOverlay::STATE_MESSAGE_DETACH_OBJECT = StringUtil::getHash( "detachObject" );
        const hash_type IOverlay::STATE_MESSAGE_DETACH_ALL_OBJECTS =
            StringUtil::getHash( "detachAllObject" );

    }  // end namespace render
}  // end namespace fb
