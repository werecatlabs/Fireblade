#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Rtti/RttiClassDefinition.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, IEventListener, ISharedObject );
}  // end namespace fb
