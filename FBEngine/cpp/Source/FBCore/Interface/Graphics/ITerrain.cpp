#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/ITerrain.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, ITerrain, ISharedObject );
    }  // end namespace render
}  // end namespace fb
