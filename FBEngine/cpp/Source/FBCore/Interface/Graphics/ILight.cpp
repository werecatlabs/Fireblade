#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ILight.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, ILight, IGraphicsObject );

        const hash_type ILight::VISIBILITY_MASK_HASH = StringUtil::getHash( "visibilityMask" );
        const hash_type ILight::LIGHT_TYPE_HASH = StringUtil::getHash( "lightType" );
        const hash_type ILight::DIFFUSE_COLOUR_HASH = StringUtil::getHash( "diffuseColour" );
        const hash_type ILight::SPECULAR_COLOUR_HASH = StringUtil::getHash( "specularColour" );

    }  // end namespace render
}  // end namespace fb
