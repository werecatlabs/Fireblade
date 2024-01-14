#include <FBAssimp/FBAssimpPCH.h>
#include <FBAssimp/FBAssimp.h>
#include <FBAssimp/MeshLoader.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{

    auto FBAssimp::createAssimpLoader() -> SmartPtr<IMeshLoader>
    {
        return fb::make_ptr<MeshLoader>();
    }

}  // end namespace fb
