#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/System/IFactoryManager.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IFactoryManager, ISharedObject );
}  // end namespace fb
