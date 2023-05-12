#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/ITransform.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, ITransform, ISharedObject );
    }  // namespace scene
}  // namespace fb
