#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb, ITexture, IResource );

    const hash_type ITexture::STATE_MESSAGE_TEXTURE_SIZE = StringUtil::getHash( "textureSize" );

}  // namespace fb::render
