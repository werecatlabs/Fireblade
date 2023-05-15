#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IMaterialNode.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb::render, IMaterialNode, ISharedObject );
    }  // end namespace render
}  // end namespace fb
