#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Video/IVideoTexture.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IVideoTexture, render::ITexture );
}  // end namespace fb
