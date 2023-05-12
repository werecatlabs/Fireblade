#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Procedural/IProceduralObject.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace procedural
    {
        FB_CLASS_REGISTER_DERIVED( fb, IProceduralObject, ISharedObject );
    }  // namespace procedural
}  // namespace fb
