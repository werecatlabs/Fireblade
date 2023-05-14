#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Graphics/IDebugLine.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>
#include <FBCore/Memory/TypeManager.h>

namespace fb
{
    namespace render
    {
        FB_CLASS_REGISTER_DERIVED( fb, IDebugLine, ISharedObject );
    }  // end namespace render
}  // end namespace fb
