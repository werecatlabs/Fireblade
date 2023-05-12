#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IActorProxy.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, IActorProxy, IActor );

    }  // namespace scene
}  // namespace fb