#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Procedural/IProceduralObject.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::procedural
{
    FB_CLASS_REGISTER_DERIVED( fb, IProceduralObject, ISharedObject );
}  // namespace fb::procedural
