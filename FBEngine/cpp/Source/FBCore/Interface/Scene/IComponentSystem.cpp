#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IComponentSystem.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, IComponentSystem, ISharedObject );
}  // namespace fb::scene
