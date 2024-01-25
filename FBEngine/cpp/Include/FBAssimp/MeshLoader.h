#ifndef MeshLoader_h__
#define MeshLoader_h__

#include <FBAssimp/FBAssimpPrerequisites.h>
#include <FBCore/Interface/Mesh/IMeshLoader.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Math/Vector3.h>

#if FB_USE_FBXSDK
//#include <fbxsdk.h>
#elif FB_USE_ASSET_IMPORT
#    include <assimp/importer.hpp>
#    include <assimp/scene.h>
#    include <assimp/postprocess.h>
#endif

namespace fb
{
    class MeshLoader : public IMeshLoader
    {
    public:
        MeshLoader();
        ~MeshLoader() override;

        Array<render::IMaterial> createMaterials( const String &meshPath ) override;

        /** @copydoc IMeshLoader::loadActor */
        SmartPtr<scene::IActor> loadActor( SmartPtr<IMeshResource> resource ) override;

        /** @copydoc IMeshLoader::loadActor */
        SmartPtr<scene::IActor> loadActor( const String &meshName ) override;

        /** @copydoc IMeshLoader::loadMesh */
        SmartPtr<IMesh> loadMesh( SmartPtr<IMeshResource> resource ) override;

        /** @copydoc IMeshLoader::loadMesh */
        SmartPtr<IMesh> loadMesh( const String &meshName ) override;

        bool getUseSingleMesh() const override;
        void setUseSingleMesh( bool useSingleMesh ) override;

        bool getQuietMode() const;
        void setQuietMode( bool quietMode );

        SmartPtr<IMeshResource> getMeshResource() const;
        void setMeshResource( SmartPtr<IMeshResource> meshResource );

        String getMeshPath() const;

        SmartPtr<scene::IActor> getRootActor() const;

        void setRootActor( SmartPtr<scene::IActor> rootActor );

        bool getImportMesh() const;

        void setImportMesh( bool importMesh );

        FB_CLASS_REGISTER_DECL;

    protected:
#if FB_USE_ASSET_IMPORT
        void createMaterials( Array<render::IMaterial> &materials, const aiScene *mScene,
                              const aiNode *pNode, const String &mDir );

        void loadDataFromActor( SmartPtr<scene::IActor> actor, const aiScene *mScene,
                                const aiNode *pNode, const String &mDir );

        void loadToMesh( SmartPtr<IMesh> mesh, const aiScene *mScene, const aiNode *pNode,
                         const String &mDir );

        void computeNodesDerivedTransform( const aiScene *mScene, const aiNode *pNode,
                                           aiMatrix4x4 accTransform );

        bool createSubMesh( const String &name, int index, const aiNode *pNode, const aiMesh *mesh,
                            const aiMaterial *mat, SmartPtr<IMesh> mMesh, const String &mDir );

        void loadAnimations( const aiScene *scene, SmartPtr<scene::IActor> actor );

        void createMaterial( SmartPtr<scene::Material> pMat, s32 index, const aiMaterial *mat,
                             const String &mDir );

        using BoneNodeMap = std::map<String, const aiNode *>;
        BoneNodeMap m_boneNodesByName;

        using BoneMap = std::map<String, const aiBone *>;
        BoneMap m_bonesByName;

        using NodeTransformMap = std::map<String, aiMatrix4x4>;
        NodeTransformMap m_nodeDerivedTransformByName;
#endif

        SmartPtr<scene::IActor> m_rootActor;

        SmartPtr<scene::MeshResourceDirector> m_director;

        Array<SmartPtr<IMesh>> m_meshes;
        bool m_useSingleMesh = false;
        bool m_quietMode = false;
        bool m_importMesh = false;

        SmartPtr<IMeshResource> m_meshResource;

        Vector3<f32> m_meshScale = Vector3<f32>::unit();

        u64 m_fileHash = 0;
        String m_meshPath;
    };
}  // end namespace fb

#endif  // MeshLoader_h__
