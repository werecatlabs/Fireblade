#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IMaterial.h>
#include <FBCore/Interface/Graphics/IMaterialNode.h>
#include <FBCore/Interface/Graphics/IMaterialPass.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IMaterial, IResource );

        const hash_type IMaterial::SET_TEXTURE_HASH = StringUtil::getHash( "setTexture" );
        const hash_type IMaterial::FRAGMENT_FLOAT_HASH = StringUtil::getHash( "fragmentFloat" );
        const hash_type IMaterial::FRAGMENT_VECTOR2F_HASH = StringUtil::getHash( "fragmentVector2f" );
        const hash_type IMaterial::FRAGMENT_VECTOR3F_HASH = StringUtil::getHash( "fragmentVector3f" );
        const hash_type IMaterial::FRAGMENT_VECTOR4F_HASH = StringUtil::getHash( "fragmentVector4f" );
        const hash_type IMaterial::FRAGMENT_COLOUR_HASH = StringUtil::getHash( "fragmentColour" );
        const hash_type IMaterial::LIGHTING_ENABLED_HASH = StringUtil::getHash( "lightingEnabled" );
    }  // end namespace render
}  // end namespace fb
