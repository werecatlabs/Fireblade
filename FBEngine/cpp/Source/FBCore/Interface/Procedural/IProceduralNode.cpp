#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Procedural/IProceduralNode.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    namespace procedural
    {
        FB_CLASS_REGISTER_DERIVED( fb, IProceduralNode, IProceduralObject );

    }  // namespace procedural
}  // namespace fb
