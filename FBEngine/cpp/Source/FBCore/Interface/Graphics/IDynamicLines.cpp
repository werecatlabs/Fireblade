#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IDynamicLines.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IDynamicLines, IGraphicsObject );
    }  // end namespace render
}  // end namespace fb