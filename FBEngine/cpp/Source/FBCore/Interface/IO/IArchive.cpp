#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/IO/IArchive.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IArchive, ISharedObject );
}  // end namespace fb