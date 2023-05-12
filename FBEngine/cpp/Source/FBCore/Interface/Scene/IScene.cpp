#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IScene.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, IScene, ISharedObject );
    }  // namespace scene
}  // namespace fb
