#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IMaterialTechnique.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IMaterialTechnique, IMaterialNode );
    }  // end namespace render
}  // end namespace fb
