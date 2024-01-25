#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/System/IProfiler.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IProfiler, ISharedObject );
}  // end namespace fb