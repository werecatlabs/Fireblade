#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/System/IJob.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IJob, ISharedObject );
}  // end namespace fb
