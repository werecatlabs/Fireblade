#ifndef MeshLoader_h__
#define MeshLoader_h__

#include <FBGraphicsOgre/FBGraphicsOgrePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>
#include <OgreMesh.h>

namespace fb
{
    namespace render
    {
        
        class MeshLoader : public ISharedObject
        {
        public:
            MeshLoader();
            ~MeshLoader() override;

             static void loadFBMesh( Ogre::MeshPtr mesh, SmartPtr<IMesh> fbMesh );
        };
    } // end namespace render
}     // end namespace fb

#endif  // MeshLoader_h__
