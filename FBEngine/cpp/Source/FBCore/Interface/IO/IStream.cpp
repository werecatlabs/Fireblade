#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IStream, ISharedObject );
}  // end namespace fb