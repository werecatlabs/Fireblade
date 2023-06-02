#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/IScene.h>
#include <FBCore/System/RttiClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, IScene, ISharedObject );
    }  // namespace scene
}  // namespace fb
