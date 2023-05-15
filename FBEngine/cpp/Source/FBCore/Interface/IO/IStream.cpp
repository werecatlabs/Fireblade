#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/IO/IStream.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IStream, ISharedObject );
}  // end namespace fb