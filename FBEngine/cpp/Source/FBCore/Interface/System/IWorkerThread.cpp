#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/System/IWorkerThread.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IWorkerThread, ISharedObject );
}  // end namespace fb
