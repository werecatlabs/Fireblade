#ifndef FBPhysxCooker_h__
#define FBPhysxCooker_h__

#include <FBPhysx/FBPhysxPrerequisites.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Base/Map.h>
#include "foundation/PxVec3.h"
#include "PxPhysics.h"
#include "common/PxCoreUtilityTypes.h"
#include "FBPhysx/FBPhysxStream.h"
#include <FBMesh/FBMeshPrerequisites.h>

namespace fb
{
    namespace physics
    {
        using namespace physx;

        class PhysxCooker
        {
        private:
            String mOgreResourceGroup;

        public:
            class Params
            {
            public:
                Vector3F mScale;
                std::map<String, PxMaterial *> mMaterialBindings;
                bool mAddBackfaces;

                Params();
                ~Params();

                Params &scale( const Vector3F &scale );
                Params &materials( std::map<String, PxMaterial *> &bindings );
                Params &backfaces( bool addBackfaces );
            };

            class AddedMaterials
            {
            public:
                PxMaterial **materials;
                PxU32 materialCount;

                AddedMaterials();
                ~AddedMaterials();
            };

            struct MeshInfo
            {
                // vertex buffer
                Array<Vector3F> vertices;

                // index buffer
                Array<int> indices;

                // assigns a material to each triangle.
                Array<String> materials;
            };

            void getMeshInfo( SmartPtr<IMesh> mesh, Params params, MeshInfo &outInfo );
            void mergeVertices( MeshInfo &outInfo, float fMergeDist = 1e-3f );
            void insetMesh( MeshInfo &outInfo, float fAmount );

            PhysxCooker( void );
            ~PhysxCooker( void );

            /*
            setOgreResourceGroup
            Tells OgrePhysX in which ogre resource group it is supposed to look for resources.
            By default, this is set to "General".
            */
            void setOgreResourceGroup( String group );

            /*
            hasPxMesh
            Checks whether the Ogre Resource System has a resource PxsFile.
            */
            bool hasPxMesh( String PxsFile );
            /*
            loadPxMeshFromFile
            Loads a PhysX triangle mesh from a Pxs file. Throws an exception if the file is not found in
            the Ogre resource system.
            */
            PxTriangleMesh *loadPxTriangleMeshFromFile( String PxsFile );

            /*
            cookPxTriangleMesh
            cooks an PhysX triangle mesh from an Ogre mesh.
            out_addedMaterials can be passed to obtain information about the used materials (for per
            triangle materials).
            @see PxShape::setMaterials
            */
            void cookPxTriangleMesh( SmartPtr<IMesh> mesh, PxOutputStream &outputStream, Params params,
                                     AddedMaterials *out_addedMaterials = nullptr );
            void cookPxTriangleMeshToFile( SmartPtr<IMesh> mesh, String PxsOutputFile, Params params,
                                           AddedMaterials *out_addedMaterials = nullptr );

            void cookPxConvexMesh( SmartPtr<IMesh> mesh, PxOutputStream &outputStream, Params params );

            void cookPxCCDSkeleton( SmartPtr<IMesh> mesh, PxOutputStream &outputStream, Params params );

            /*
            createPxTriangleMesh
            Cooks an Px mesh from an ogre mesh and returns it, does not save to file.
            */
            PxTriangleMesh *createPxTriangleMesh( SmartPtr<IMesh> mesh, Params params,
                                                  AddedMaterials *out_addedMaterials = nullptr );

            PxConvexMesh *createPxConvexMesh( SmartPtr<IMesh> mesh, Params params );

            // Singleton
            static PhysxCooker &getSingleton();
        };
    } // end namespace physics
}     // end namespace fb

#endif  // FBPhysxCooker_h__
