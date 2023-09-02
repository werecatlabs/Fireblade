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
        enum LoaderParams
        {
            LP_GENERATE_SINGLE_MESH = 1 << 0,

            // See the two possible methods for material gneration
            LP_GENERATE_MATERIALS_AS_CODE = 1 << 1,

            // 3ds max exports the animation over a longer time frame than the animation actually plays for
            // this is a fix for that
            LP_CUT_ANIMATION_WHERE_NO_FURTHER_CHANGE = 1 << 2,

            // when 3ds max exports as DAE it gets some of the transforms wrong, get around this by using
            // this option and a prior run with of the model exported as ASE
            LP_USE_LAST_RUN_NODE_DERIVED_TRANSFORMS = 1 << 3,

            // Quiet mode - don't output anything
            LP_QUIET_MODE = 1 << 4
        };

        MeshLoader();
        ~MeshLoader() override;

        Array<render::IMaterial> createMaterials( const String &meshPath );

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

        bool getQuietMode() const override;
        void setQuietMode( bool quietMode ) override;

        SmartPtr<IMeshResource> getMeshResource() const;
        void setMeshResource( SmartPtr<IMeshResource> meshResource );

        String getMeshPath() const;

        SmartPtr<fb::scene::IActor> getRootActor() const;

        void setRootActor( fb::SmartPtr<fb::scene::IActor> rootActor );

        FB_CLASS_REGISTER_DECL;

    protected:
        void createActor( SmartPtr<ITransformNode> parentNode, SmartPtr<scene::IActor> parentActor );

#if FB_USE_FBXSDK
        //FbxScene* pScene;
        //FbxManager* pSdkManager;
#elif FB_USE_ASSET_IMPORT
        void createMaterials( Array<render::IMaterial> &materials, const aiScene *mScene,
                              const aiNode *pNode, const String &mDir );

        void loadDataFromNode( const aiScene *mScene, const aiNode *pNode, const String &mDir );

        void loadDataFromActor( SmartPtr<scene::IActor> actor, const aiScene *mScene,
                                const aiNode *pNode, const String &mDir );

        void loadToMesh( SmartPtr<IMesh> mesh, const aiScene *mScene, const aiNode *pNode,
                         const String &mDir );

        void computeNodesDerivedTransform( const aiScene *mScene, const aiNode *pNode,
                                           aiMatrix4x4 accTransform );

        bool createSubMesh( const String &name, int index, const aiNode *pNode, const aiMesh *mesh,
                            const aiMaterial *mat, SmartPtr<IMesh> mMesh, const String &mDir );

        SmartPtr<render::IMaterial> createMaterial( unsigned int mMaterialIndex, const aiMaterial *mat,
                                                    const String &mDir );

        SmartPtr<render::IMaterial> createMaterialByScript( unsigned int mMaterialIndex,
                                                            const aiMaterial *mat );

        void createMaterial( SmartPtr<scene::Material> pMat, s32 index, const aiMaterial *mat,
                             const String &mDir );

        using BoneNodeMap = std::map<String, const aiNode *>;
        BoneNodeMap mBoneNodesByName;

        using BoneMap = std::map<String, const aiBone *>;
        BoneMap mBonesByName;

        using NodeTransformMap = std::map<String, aiMatrix4x4>;
        NodeTransformMap mNodeDerivedTransformByName;
#endif

        SmartPtr<scene::IActor> m_rootActor;

        Array<SmartPtr<IMesh>> mMeshes;
        bool m_useSingleMesh;
        bool m_quietMode;

        SmartPtr<IMeshResource> m_meshResource;

        int mLoaderParams = 0;
        Vector3<f32> m_meshScale = Vector3<f32>::unit();

        u64 m_fileHash;
        String m_meshPath;
    };
}  // end namespace fb

#endif  // MeshLoader_h__
