#ifndef FBXConverter_h__
#define FBXConverter_h__



#include <FBMesh/FBX/FBXOptions.h>
#include "FBMesh/FBX/FBXMesh.h"
//#include "FBMesh/FBX/FBXScene.h"
#include "FBXMeshScene.h"
#include "FBXMaterialSet.h"
#include <FBCore/Math/Transform3.h>



namespace fb
{


	class FBXConverter
	{
	public:
		// public methods
		FBXConverter();
		~FBXConverter();

		SmartPtr<IStateNode> loadScene(const String& fileName);

		void createStateNode(FbxNode* pNode, SmartPtr<IStateNode> parent, SmartPtr<IStateSceneManager> sceneManager);
		
		bool exportScene(const char* fbxfilename, const char* outputmeshfilename, const char* logfilename, const char* clipName, const char* frame0name, bool copyUniqueTextures, int bindframe);

		bool AddFBXAnimationToExisting(const char* fbxfilename, const char* skeletonfilename, const char* meshfilename, const char* logfilename, const char* clipName, int start, int stop, float rate);

		bool DeleteOgreAnimation(const char* skeletonfilename, const char* meshfilename, const char* logfilename, const char* animname);

		bool printAnimInfo(std::string animname, float fps, float start, Mesh* pMesh, Skeleton* pSkeleton);

		bool GetAnimationBounds(FbxScene* pScene, float& start, float& stop);

		Transform3<real_Num> getLocalTransform(FbxNode* pNode) const;

		Transform3<real_Num> getWorldTransform(FbxNode* pNode) const;

		ParamList m_params;

	private:

		bool GetFBXAnimationBounds(FbxScene* pScene, float& start, float& stop);

		bool Initialize(const char* fbxfilename, const char* skeletonfilename, const char* meshfilename, const char* logfilename, const char* animname);

		bool InitializeLogging(const char* logfilename);

		bool translateNode(FbxNode* pNode);

		// Adds the default FBX animation to m_params.skelClipList
		bool prepareAnimationClip(const char* clipName);
		bool prepareFrame0Anim(const char* clipName);
		bool writeOgreData();

		void addLightToScene(FbxNode* pNode);
		void addCameraToScene(FbxNode* pNode);
		FxOgreNode getFxOgreNode(FbxNode* pNode);

		bool stat = false;
		Mesh* m_pMesh = nullptr;
		FBXMaterialSet* m_pMaterialSet = nullptr;

		String m_fbxfilename;
		String m_meshfilepath;
		String m_skeletonfilepath;
		String m_animationname;

		FBXMeshScene m_FxOgreScene;
	};
}



#endif // FBXConverter_h__
