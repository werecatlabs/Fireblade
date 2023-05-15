#ifndef MeshLoader_h__
#define MeshLoader_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Base/Array.h>
#include <FBCore/Base/StringTypes.h>
#include <OgreMesh.h>

#if FB_USE_ASSET_IMPORT
#    include "assimp/importer.hpp"
#    include <assimp/scene.h>
#    include <assimp/postprocess.h>
#endif

namespace fb
{
    namespace render
    {
        
        class MeshLoader : public CSharedObject<ISharedObject>
        {
        public:
            MeshLoader();
            ~MeshLoader() override;

             static void loadFBMesh( Ogre::MeshPtr mesh, SmartPtr<IMesh> fbMesh );
        };
    } // end namespace render
}     // end namespace fb

#endif  // MeshLoader_h__
