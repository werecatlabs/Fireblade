#ifndef _MESH_H
#define _MESH_H



#include "FBXSubmesh.h"
#include "skeleton.h"

#include "vertex.h"
#include "FBXOptions.h"



namespace fb
{



	void FbxVector4ToPoint3(FbxVector4& normal, Point3& n, bool opposite);

	/***** structures to store shared geometry *****/
	typedef struct dagInfotag
	{
		long offset;
		long numVertices;
		FbxNode* pNode;
		FBXBlendShape* pBlendShape;
	} dagInfo;

	typedef struct sharedGeometrytag
	{
		std::vector<vertex> vertices;
		std::vector<dagInfo> dagMap;
	} sharedGeometry;


	/***** Class Mesh *****/
	class Mesh
	{
	public:
		//constructor
		Mesh(const std::string& name = "");
		
		//destructor
		~Mesh();

		//clear data
		void clear();

		//get pointer to linked skeleton
		Skeleton* getSkeleton();

		void createSkeleton();

		bool preprocessNode(FbxNode* pNode, FbxMesh* pMesh, FbxScene* pScene);

		//load mesh data 
		bool load(FbxNode* pNode, FbxMesh* pMesh, ParamList& params);
		
		//load vertex animations
		bool loadAnims(ParamList& params);
		
		//load blend shape deformers
		bool loadBlendShapes(ParamList& params);
		
		//load blend shape animations
		bool loadBlendShapeAnimations(ParamList& params);
		
		//write to a OGRE binary mesh
		bool writeOgreBinary(ParamList& params, Ogre::MeshManager& meshManager);

		void getKeyedFrames(std::vector<int>& keyedFrames, ParamList& params);

		int getNumSubmeshes();

		BoundingBox calculateBoundingBox(ParamList& params);

		// Adds the animation from the FBX to the mesh file.
		bool AddFBXAnimationToExisting(OgreManagers& managers, std::string& meshFilePath, std::string& clipName, ParamList& params, float start, float stop);

		// Deletes animation from existing mesh file.
		bool DeleteAnimationFromExisting(OgreManagers& managers, std::string& meshFilePath, std::string& clipName);

	protected:
#if FB_BUILD_OGRE
		Ogre::MeshPtr loadOgreMeshFromFile(OgreManagers& managers, std::string& meshFilePath);
#endif

		//get uvsets info from the mesh
		bool getUVSets(FbxNode* pNode, FbxMesh* pMesh);

		//get modifiers (skin and morph)
		bool getModifiers(FbxNode* pNode, FbxMesh* pMesh, ParamList& params);

		//get vertex data
		bool getVertices(FbxNode* pNode, FbxMesh* pMesh, ParamList& params);
		//get vertex bone assignements
		bool getVertexBoneWeights(FbxNode* pNode, FbxMesh* pMesh, ParamList& params);
		//get faces data
		bool getFaces(FbxNode* pNode, FbxMesh* pMesh, ParamList& params);
		// Set default values for data (rigidly skinned meshes, bone weights, position indices)
		bool cleanupData(FbxNode* pNode, ParamList& params);
		//build shared geometry
		bool buildSharedGeometry(FbxNode* pNode, ParamList& params);
		//create submeshes
		bool createSubmeshes(FbxNode* pNode, FbxMesh* pMesh, ParamList& params);

#if FB_BUILD_OGRE
		bool addOgreMeshAnimation(Ogre::Mesh& mesh, fb::Animation& anim);
#endif

		bool loadBSClip(std::string clipName, float start, float stop, float rate, ParamList& params);
		bool loadBSClipAnim(std::string clipName, float start, float stop, float rate, ParamList& params, Animation& a);
		
#if FB_BUILD_OGRE
		bool replaceBSAnimation(Ogre::MeshPtr pMesh, const std::string& clipName, float start, float stop, float rate, ParamList& params);
#endif

		//load a vertex animation clip
		bool loadVertexClip(std::string& clipName, float start, float stop, float rate, ParamList& params);
		//load a vertex animation track for the whole mesh
		bool loadMeshTrack(Animation& a, std::vector<float>& times, ParamList& params);
		//load all submesh animation tracks (one for each submesh)
		bool loadSubmeshTracks(Animation& a, std::vector<float>& times, ParamList& params);
		//load a keyframe for the whole mesh
		bool loadKeyframe(Track& t, float time, ParamList& params);

#if FB_BUILD_OGRE
		//write shared geometry data to an Ogre compatible mesh
		bool createOgreSharedGeometry(Ogre::MeshPtr pMesh, ParamList& params);

		//create an Ogre compatible vertex buffer
		bool createOgreVertexBuffer(Ogre::MeshPtr pMesh, Ogre::VertexDeclaration* pDecl, const std::vector<vertex>& vertices);
		//create Ogre poses for pose animation
		bool createOgrePoses(Ogre::MeshPtr pMesh, ParamList& params);
		//create vertex animations for an Ogre mesh
		bool createOgreVertexAnimations(Ogre::MeshPtr pMesh, ParamList& params);
		//create pose animations for an Ogre mesh
		bool createOgrePoseAnimations(Ogre::MeshPtr pMesh, ParamList& params);
		// Create a single pose animation from an Animation object
		bool createOgrePoseAnimation(Ogre::MeshPtr pMesh, Animation& a, ParamList& params);
#endif

		// return the bind pose of the mesh.  If no skin, then get the mesh's global transform at time 0.  
		FbxAMatrix getBindPose(FbxNode* pNode, FbxMesh* pMesh, ParamList& params);

		//internal members
		std::string m_name;
		long m_numTriangles;
		std::vector<uvset> m_uvsets;
		std::vector<FBXSubMesh*> m_submeshes;
		Skeleton* m_pSkeleton;
		sharedGeometry m_sharedGeom;
		std::vector<Animation> m_vertexClips;
		std::vector<Animation> m_BSClips;
		//temporary members (existing only during translation from max mesh)
		std::vector<vertexInfo> newvertices;
		std::vector< std::vector<float> > newweights;
		std::vector< std::vector<int> > newjointIds;
		std::vector<FbxVector4> newpoints;
		std::vector<FbxVector4> newnormals;
		std::vector<FbxVector4> newbinormals;
		std::vector<FbxVector4> newtangents;
		std::vector<int> newuvsets;
		int* pSkinCluster;
		FBXBlendShape* pBlendShape;
		int m_numBlendShapes;

		std::vector<int*> shaders;
		std::vector<faceArray> polygonSets;

		bool opposite;
		BoundingBox m_bindBox;
	};



	// Helper function for getting the color of a vertex.
	FbxColor GetFBXColor(FbxMesh* pMesh, int polyIndex, int polyPointIndex);



}; // end of namespace



#endif
