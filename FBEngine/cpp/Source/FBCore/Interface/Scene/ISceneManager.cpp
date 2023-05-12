#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/Reflection/ReflectionClassDefinition.h>
#include <FBCore/Base/StringUtil.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, ISceneManager, ISharedObject );

        const hash_type ISceneManager::sceneLoadedHash = StringUtil::getHash( "sceneLoaded" );

    }  // namespace scene
}  // namespace fb
