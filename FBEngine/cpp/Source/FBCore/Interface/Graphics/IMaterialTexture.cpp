#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IMaterialTexture.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, IMaterialTexture, IMaterialNode );

    const String IMaterialTexture::texturePathStr = String( "texture" );
    const String IMaterialTexture::scaleStr = String( "scale" );
    const String IMaterialTexture::tintStr = String( "tint" );
    const String IMaterialTexture::textureTypeStr = String( "textureType" );

    /** Virtual destructor. */
    IMaterialTexture::~IMaterialTexture() = default;

}  // namespace fb::render
