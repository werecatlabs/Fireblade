#ifndef _MeshCoverter_H
#define _MeshCoverter_H

#include <FBGraphics/FBGraphicsPrerequisites.h>
#include <FBCore/Interface/Graphics/IMeshConverter.h>
#include <FBCore/Interface/Mesh/IMesh.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{

    //--------------------------------------------
    class MeshConverter : public SharedObject<render::IMeshConverter>
    {
    public:
        MeshConverter();
        ~MeshConverter();

        bool ExportSelected( const char *fileName );

        void writeMesh( const SmartPtr<scene::IActor> actor );
        void writeOgreMesh( const SmartPtr<scene::IActor> actor );

        void covertIrrMeshToOgreMesh();

        static void convertFBMeshToFColladaMesh( SmartPtr<IMesh> mesh, FCDGeometryMesh *outMesh );

        static Ogre::Mesh *convertFBMeshToOgreMesh( const String &newMeshName, SmartPtr<IMesh> mesh );
        static Ogre::Mesh *convertIrrToOgreMesh( irr::scene::IAnimatedMesh *mesh );
        static Ogre::Mesh *convertIrrToOgreMesh( irr::scene::IMesh *mesh );
        static irr::scene::IAnimatedMesh *convertOgreToIrrMesh( Ogre::Mesh *mesh );
        static SmartPtr<IMesh> convertOgreMeshToFBMesh( Ogre::Mesh *mesh );

        /** Returns a boolean indicating whether or not the mesh is valid. */
        static bool isMeshValid( Ogre::Mesh *mesh );
    };

}  // end namespace fb

#endif
