#include <FBCore/FBCorePCH.h>
#include "FBCore/Interface/Graphics/IVideoMaterial.h"
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, IVideoMaterial, ISharedObject );
}  // end namespace fb
