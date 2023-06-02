#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/ISystem.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, ISystem, ISharedObject );
    }  // namespace scene
}  // namespace fb