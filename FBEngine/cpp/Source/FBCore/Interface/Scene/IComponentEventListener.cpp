#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IComponentEventListener.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb::scene
{
    FB_CLASS_REGISTER_DERIVED( fb::scene, IComponentEventListener, ISharedObject );
}  // namespace fb::scene
