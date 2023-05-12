#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IMaterialTexture.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IMaterialTexture, IMaterialNode );
    }  // end namespace render
}  // end namespace fb