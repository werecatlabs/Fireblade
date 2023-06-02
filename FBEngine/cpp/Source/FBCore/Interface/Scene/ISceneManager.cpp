#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Scene/ISceneManager.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Core/StringUtil.h>

namespace fb
{
    namespace scene
    {
        FB_CLASS_REGISTER_DERIVED( fb::scene, ISceneManager, ISharedObject );

        const hash_type ISceneManager::sceneLoadedHash = StringUtil::getHash( "sceneLoaded" );

    }  // namespace scene
}  // namespace fb
