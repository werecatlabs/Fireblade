#include <FBAssimp/FBAssimpPCH.h>
#include <FBAssimp/FBAssimp.h>
#include <FBAssimp/MeshLoader.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{

    SmartPtr<IMeshLoader> FBAssimp::createAssimpLoader()
    {
        return fb::make_ptr<MeshLoader>();
    }

}  // end namespace fb
