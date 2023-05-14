#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/IPrototype.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace core
    {
        FB_CLASS_REGISTER_DERIVED( fb::core, IPrototype, ISharedObject );
    }  // namespace core
}  // end namespace fb