#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IVideoTexture.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, IVideoTexture, render::ITexture );
}  // end namespace fb
