#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/ISceneManagerListener.h>
#include <FBCore/Rtti/ReflectionClassDefinition.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, ISceneManagerListener, ISharedObject );
    }  // namespace scene
}  // namespace fb