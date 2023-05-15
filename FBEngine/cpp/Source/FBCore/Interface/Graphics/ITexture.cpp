#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ITexture.h>
#include <FBCore/Rtti/RttiClassDefinition.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, ITexture, IResource );

        const hash_type ITexture::STATE_MESSAGE_TEXTURE_SIZE = StringUtil::getHash( "textureSize" );


    }  // end namespace render
}  // end namespace fb
