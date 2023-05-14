#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/IApplicationClient.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    namespace core
    {
        FB_CLASS_REGISTER_DERIVED( fb::core, IApplicationClient, ISharedObject );
    }  // namespace core
}  // end namespace fb
