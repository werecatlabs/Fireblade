#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/Physics/IPhysicsManager.h>

#include <FBCore/System/RttiClassDefinition.h>

namespace fb::physics
{
    FB_CLASS_REGISTER_DERIVED( fb, IPhysicsManager, ISharedObject );
}  // namespace fb::physics
