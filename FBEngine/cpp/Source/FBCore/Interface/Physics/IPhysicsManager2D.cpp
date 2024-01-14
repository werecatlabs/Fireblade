#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Interface/Physics/IPhysicsManager2D.h>

#include <FBCore/System/RttiClassDefinition.h>

namespace fb::physics
{
    FB_CLASS_REGISTER_DERIVED( fb, IPhysicsManager2D, ISharedObject );
}  // namespace fb::physics
