#ifndef MeshLoader_h__
#define MeshLoader_h__

#include <FBGraphicsOgreNext/FBGraphicsOgreNextPrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>
#include <FBMesh/FBMeshPrerequisites.h>
#include <FBCore/FBCorePrerequisites.h>
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

        class MeshLoader : public ISharedObject
        {
        public:
            MeshLoader();
            ~MeshLoader();

            static void createV2Mesh( Ogre::Mesh *mesh, SmartPtr<IMesh> fbmesh );

            static void loadFBMesh( Ogre::MeshPtr mesh, const String &meshPath );
            static Ogre::Mesh *loadFBMesh( SmartPtr<IStream> stream );
            static void loadFBMesh( Ogre::MeshPtr mesh, SmartPtr<IMesh> fbMesh );

            static Ogre::v1::MeshPtr convertFBMeshToOgreMesh( const String &newMeshName,
                                                              SmartPtr<IMesh> mesh );

            static Ogre::IndexBufferPacked *importFromV1( SmartPtr<IIndexBuffer> indexData );
            static void importBuffersFromV1( Ogre::MeshPtr newMesh, Ogre::SubMesh *pSubMesh,
                                             SmartPtr<IMesh> mesh, SmartPtr<ISubMesh> subMesh,
                                             bool halfPos, bool halfTexCoords, bool qTangents,
                                             bool halfPose, size_t vaoPassIdx );
            static void importPosesFromV1( SmartPtr<ISubMesh> subMesh,
                                           Ogre::VertexBufferPacked *vertexBuffer, bool halfPrecision );
            static void importFromV1( Ogre::MeshPtr newMesh, Ogre::SubMesh *newSubMesh,
                                      SmartPtr<IMesh> mesh, SmartPtr<ISubMesh> subMesh, bool halfPos,
                                      bool halfTexCoords, bool qTangents, bool halfPose );
            static void importV1( IMesh *mesh, bool halfPos, bool halfTexCoords, bool qTangents,
                                  bool halfPose );

            void loadMesh( const String &meshName );
            SmartPtr<IMesh> loadEngineMesh( const String &meshName );

            SmartPtr<IMesh> load( const String &meshName, SmartPtr<ISceneNode> fbParent,
                                  SmartPtr<render::IGraphicsScene> smgr );

#if FB_USE_ASSET_IMPORT
            SmartPtr<ISceneNode> loadNode( aiNode *node, SmartPtr<ISceneNode> fbNode,
                                           SmartPtr<render::IGraphicsScene> smgr );
            void loadDataFromNode( const aiScene *mScene, const aiNode *pNode, const String &mDir );
#endif

            bool getUseSingleMesh() const;
            void setUseSingleMesh( bool val );

            bool getQuietMode() const;
            void setQuietMode( bool val );

            SmartPtr<IMeshManager> getMeshMgr() const;
            void setMeshMgr( SmartPtr<IMeshManager> val );

        protected:
#if FB_USE_ASSET_IMPORT
            bool createSubMesh( const String &name, int index, const aiNode *pNode, const aiMesh *mesh,
                                const aiMaterial *mat, SmartPtr<IMesh> mMesh, const String &mDir );
            
            typedef std::map<String, const aiNode *> BoneNodeMap;
            BoneNodeMap mBoneNodesByName;

            typedef std::map<String, const aiBone *> BoneMap;
            BoneMap mBonesByName;

            typedef std::map<String, aiMatrix4x4> NodeTransformMap;
            NodeTransformMap mNodeDerivedTransformByName;
#endif

            Array<SmartPtr<IMesh>> mMeshes;
            bool m_useSingleMesh;
            bool mQuietMode;

            SmartPtr<IMeshManager> m_meshMgr;
        };

    }  // end namespace render
}  // end namespace fb

#endif  // MeshLoader_h__
