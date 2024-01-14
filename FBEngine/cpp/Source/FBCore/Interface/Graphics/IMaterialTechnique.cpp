#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IMaterialTechnique.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::render
{
    FB_CLASS_REGISTER_DERIVED( fb::render, IMaterialTechnique, IMaterialNode );
}  // namespace fb::render
