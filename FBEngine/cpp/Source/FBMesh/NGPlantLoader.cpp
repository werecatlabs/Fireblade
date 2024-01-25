#include <FBMesh/FBMeshPCH.h>
#include "FBMesh/NGPlantLoader.h"
#include <FBCore/Core/Path.h>
#include <FBCore/Interface/Mesh/IVertexElement.h>
#include "FBCore/FBCore.h"

#if FB_USE_NGPLANT
#    include <ngpcore/p3dmodel.h>
#    include <ngpcore/p3dmodelstemtube.h>
#    include <ngpcore/p3dmodelstemquad.h>
#    include <ngpcore/p3dbalgbase.h>
#    include <ngpcore/p3dbalgstd.h>
#    include <ngpcore/p3diostream.h>
#    include <ngpcore/p3dhli.h>
#endif

#include <map>

namespace fb
{
#if FB_USE_NGPLANT
    class TreeVertex
    {
    public:
        Vector3F Position;
        Vector3F Normal;
        Vector3F Tangent;
        Vector3F TexCoord;
        float VertexData[4]; // store properties used for wind calculations
    };

    class TreeSubMesh
    {
    public:
        Array<TreeVertex> Vertices;
        Array<u32> Indices;
    };

    using TreeSubMeshMap = std::map<String, TreeSubMesh *>;

    class TreeMesh
    {
    public:
        TreeSubMeshMap SubMeshes;
    };

    //--------------------------------------------
    class P3DMaterialInstanceSimple : public P3DMaterialInstance
    {
    public:
        const P3DMaterialDef *GetMaterialDef() const override
        {
            return ( &MatDef );
        }

        P3DMaterialInstance *CreateCopy() const override
        {
            return new P3DMaterialInstanceSimple;
        }

    private:
        P3DMaterialDef MatDef;
    };

    //--------------------------------------------
    class FBP3DMaterialFactory : public P3DMaterialFactory
    {
    public:
        FBP3DMaterialFactory()
        {
        }

        ~FBP3DMaterialFactory() override
        {
        }

        P3DMaterialInstance *CreateMaterial( const P3DMaterialDef &materialDef ) const override
        {
            return new P3DMaterialInstanceSimple;
        }
    };

    //--------------------------------------------
    static void GetBranchGroupMaterial( const String &materialName, const P3DMaterialDef *MaterialDef )
    {
        // video::SColorf matColour;
        // MaterialDef->GetColor(&matColour.r,&matColour.g,&matColour.b);

        const char *TexName = MaterialDef->GetTexName( P3D_TEX_DIFFUSE );

        if(TexName != nullptr)
        {
        }

        bool DoubleSided = MaterialDef->IsDoubleSided();
        bool Transparent = MaterialDef->IsTransparent();

        /*SmartPtr<render::IMaterial> fbMaterial =
        FBMaterialManager::getSingletonPtr()->createRetrieveMaterial(materialName);
        if(!fbMaterial.isNull())
        {
            fbMaterial->setTextureName("Diffuse", TexName);
            fbMaterial->setTransparent(Transparent);
        }*/
    }

    //--------------------------------------------
    static bool GetBranchGroupGeometry( AABB3F &box, SmartPtr<ISubMesh> subMesh,
                                        const P3DHLIPlantTemplate *PlantTemplate,
                                        const P3DHLIPlantInstance *PlantInstance,
                                        unsigned int GroupIndex )
    {
        /*
        unsigned int                         AttrCount;
        unsigned int                         BranchAttrCount;
        P3DHLIVAttrBuffers                   VAttrBuffers;
        unsigned int                         IndexCount;
        unsigned int                         BranchIndex;
        unsigned int                         BranchCount;

        AttrCount = PlantInstance->GetVAttrCountI(GroupIndex);

        if (AttrCount == 0)
        {
            return(true);
        }

        //meshBuffer->getVertexBuffer().set_used(AttrCount);

        f32* PosBuffer    = new f32[3 * AttrCount];
        f32* NormalBuffer = new f32[3 * AttrCount];
        f32* TexCoordBuffer = new f32[2 * AttrCount];

        if ((PosBuffer    != 0) &&
            (NormalBuffer != 0))
        {
            VAttrBuffers.AddAttr(P3D_ATTR_VERTEX,(void*)PosBuffer,0,sizeof(float) * 3);
            VAttrBuffers.AddAttr(P3D_ATTR_NORMAL,(void*)NormalBuffer,0,sizeof(float) * 3);
            VAttrBuffers.AddAttr(P3D_ATTR_TEXCOORD0,(void*)TexCoordBuffer,0,sizeof(float) * 2);
        }
        else
        {
            return(false);
        }

        PlantInstance->FillVAttrBuffersI(&VAttrBuffers,GroupIndex);

        u32 numVerts = AttrCount;
        Vector3F* vertPositions = (Vector3F*)PosBuffer;
        Vector3F* vertNormals = (Vector3F*)NormalBuffer;
        Vector2F* vertTexCoords = (Vector2F*)TexCoordBuffer;
        //for(u32 vertIdx=0; vertIdx<numVerts; ++vertIdx)
        //{
        //	video::S3DVertex irrVert;
        //	irrVert.Pos = vertPositions[vertIdx];
        //	irrVert.Normal = vertNormals[vertIdx];
        //	irrVert.TCoords = vertTexCoords[(numVerts-1)-vertIdx];	//why do uvs have to be reversed?
        //	meshBuffer->getVertexBuffer()[vertIdx] = irrVert;
        //}

        SmartPtr<IVertexBuffer> pBuf;// = NULL;
        VertexDeclaration *pDecl = NULL;
        Real *pReal= NULL;

        u32 numVert = subMesh->vertexData->vertexCount = numVerts;
        pDecl = subMesh->vertexData->vertexDeclaration;
        pBind = subMesh->vertexData->vertexBufferBinding;


        //
        unsigned short iBinding = 0;
        // do positions first
        pDecl->addElement(iBinding, 0, VertexDeclaration::VET_FLOAT3, VertexDeclaration::VES_POSITION);
        pBuf =
        DefaultHardwareBufferManager::getSingletonPtr()->createVertexBuffer(pDecl->getVertexSize(iBinding),
        numVert, HardwareBuffer::HBU_DYNAMIC, false); pBind->setBinding(iBinding, pBuf); pReal =
        static_cast<Real*>(pBuf->lock(HardwareBuffer::HBL_DISCARD));


        // fill vertices
        for (int vertIdx = 0; vertIdx < numVert; vertIdx++)
        {
            const Vector3F& v = vertPositions[vertIdx];
            box.merge(v);

            Vector3 pos = Vector3(v.X(), v.Y(), v.Z());

            *pReal++ = pos.x;
            *pReal++ = pos.y;
            *pReal++ = pos.z;

            // Deal with bounds
            //currPos = pos;
            //if (firstVert)
            //{
            //	vMin = vMax = currPos;
            //	maxSquaredRadius = currPos.squaredLength();
            //	firstVert = false;
            //}
            //else
            //{
            //	vMin.makeFloor(currPos);
            //	vMax.makeCeil(currPos);
            //	maxSquaredRadius = max_(maxSquaredRadius, currPos.squaredLength());
            //}
        }

        // do normals
        iBinding++;
        pDecl->addElement(iBinding, 0, VertexDeclaration::VET_FLOAT3, VertexDeclaration::VES_NORMAL);
        pBuf =
        DefaultHardwareBufferManager::getSingletonPtr()->createVertexBuffer(pDecl->getVertexSize(iBinding),
        numVert, HardwareBuffer::HBU_DYNAMIC, false); pBind->setBinding(iBinding, pBuf); pReal =
        static_cast<Real*>(pBuf->lock(HardwareBuffer::HBL_DISCARD)); for (int vertIdx = 0; vertIdx <
        numVert; vertIdx++)
        {
            Vector3F irrNor = vertNormals[vertIdx];

            Vector3 nor;
            nor.x = irrNor.X();
            nor.y = irrNor.Y();
            nor.z = irrNor.Z();
            //nor =  world2objN * nor;
            *pReal++ = nor.x;
            *pReal++ = nor.y;
            *pReal++ = nor.z;
        }

        iBinding++;
        pDecl->addElement(iBinding, 0, VertexDeclaration::VET_FLOAT2,
        VertexDeclaration::VES_TEXTURE_COORDINATES, 0); pBuf =
        DefaultHardwareBufferManager::getSingletonPtr()->createVertexBuffer(pDecl->getVertexSize(iBinding),
        numVert, HardwareBuffer::HBU_DYNAMIC, false); pBind->setBinding(iBinding, pBuf); pReal =
        static_cast<Real*>(pBuf->lock(HardwareBuffer::HBL_DISCARD)); for (int vertIdx = 0; vertIdx <
        numVert; vertIdx++)
        {
            Vector2F tc = vertTexCoords[(numVerts-1)-vertIdx];

            *pReal++ = tc.X();
            *pReal++ = tc.Y();

        }// for vertex


        BranchCount = PlantInstance->GetBranchCount(GroupIndex);
        IndexCount  = PlantTemplate->GetIndexCount(GroupIndex,P3D_TRIANGLE_LIST);

        u32 meshIndexCount = IndexCount * BranchCount;
        u32* IndexBuffer = new u32[meshIndexCount];

        BranchAttrCount = PlantTemplate->GetVAttrCountI(GroupIndex);

        for (BranchIndex = 0; BranchIndex < BranchCount; BranchIndex++)
        {
            PlantTemplate->FillIndexBuffer
                (&(IndexBuffer[BranchIndex * IndexCount]),
                GroupIndex,
                P3D_TRIANGLE_LIST,
                P3D_UNSIGNED_INT,
                BranchAttrCount * BranchIndex);
        }

        bool useWords = (subMesh->vertexData->vertexCount >= 65535) ? false : true;
        subMesh->indexData->indexCount = meshIndexCount;
        HardwareIndexBufferSharedPtr pIBuf =
            DefaultHardwareBufferManager::getSingletonPtr()->createIndexBuffer(	useWords ?
        HardwareIndexBuffer::IT_16BIT : HardwareIndexBuffer::IT_32BIT, subMesh->indexData->indexCount,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        subMesh->indexData->indexBuffer = pIBuf;

        unsigned short *pWords = useWords ? static_cast<unsigned
        short*>(pIBuf->lock(HardwareBuffer::HBL_DISCARD)) : NULL; unsigned int *pDWords = useWords ? NULL
        : static_cast<unsigned int*>(pIBuf->lock(HardwareBuffer::HBL_DISCARD)); for (int i = 0; i <
        (int)meshIndexCount; i++)
        //for (int i = (int)pSubMesh->indexData->indexCount - 1; i >= 0; i--)
        {
            u32 indexValue = IndexBuffer[i];
            if (useWords)
                *pWords++ = indexValue;
            else
                *pDWords++ = indexValue;
        }

        pIBuf->unlock();

        delete[] IndexBuffer;
        delete[] PosBuffer;
        delete[] NormalBuffer;
        delete[] TexCoordBuffer;
        */

        return true;
    }

    //--------------------------------------------
    static bool GetBranchGroupGeometry2( AABB3F &box, SmartPtr<ISubMesh> subMesh,
                                         const P3DHLIPlantTemplate *PlantTemplate,
                                         const P3DHLIPlantInstance *PlantInstance,
                                         unsigned int GroupIndex )
    {
        unsigned int AttrCount;
        unsigned int BranchAttrCount;
        P3DHLIVAttrBuffers VAttrBuffers;
        unsigned int IndexCount;
        unsigned int BranchIndex;
        unsigned int BranchCount;

        AttrCount = PlantInstance->GetVAttrCountI( GroupIndex );

        if(AttrCount == 0)
        {
            return ( true );
        }

        // meshBuffer->getVertexBuffer().set_used(AttrCount);

        auto PosBuffer = new f32[3 * AttrCount];
        auto NormalBuffer = new f32[3 * AttrCount];
        auto TexCoordBuffer = new f32[2 * AttrCount];

        if(( PosBuffer != nullptr ) && ( NormalBuffer != nullptr ))
        {
            VAttrBuffers.AddAttr( P3D_ATTR_VERTEX, PosBuffer, 0, sizeof( float ) * 3 );
            VAttrBuffers.AddAttr( P3D_ATTR_NORMAL, NormalBuffer, 0, sizeof( float ) * 3 );
            VAttrBuffers.AddAttr( P3D_ATTR_TEXCOORD0, TexCoordBuffer, 0, sizeof( float ) * 2 );
        }
        else
        {
            return ( false );
        }

        PlantInstance->FillVAttrBuffersI( &VAttrBuffers, GroupIndex );

        u32 numVerts = AttrCount;
        auto vertPositions = (Vector3F *)PosBuffer;
        auto vertNormals = (Vector3F *)NormalBuffer;
        auto vertTexCoords = (Vector2F *)TexCoordBuffer;
        // for(u32 vertIdx=0; vertIdx<numVerts; ++vertIdx)
        //{
        //	video::S3DVertex irrVert;
        //	irrVert.Pos = vertPositions[vertIdx];
        //	irrVert.Normal = vertNormals[vertIdx];
        //	irrVert.TCoords = vertTexCoords[(numVerts-1)-vertIdx];	//why do uvs have to be reversed?
        //	meshBuffer->getVertexBuffer()[vertIdx] = irrVert;
        // }

        SmartPtr<IVertexBuffer> pBuf( new VertexBuffer );

        SmartPtr<IVertexDeclaration> decl( new VertexDeclaration );
        pBuf->setVertexDeclaration( decl );

        f32 *pReal = nullptr;

        pBuf->setNumVerticies( numVerts );

        u32 numVert = numVerts;

        unsigned short iBinding = 0;
        u32 offset = 0;
        decl->addElement( 0, offset, VertexDeclaration::VertexElementSemantic::VES_POSITION,
                          IVertexElement::VertexElementType::VET_FLOAT3 );
        offset += sizeof( f32 ) * 3;
        decl->addElement( 0, offset, VertexDeclaration::VertexElementSemantic::VES_NORMAL,
                          IVertexElement::VertexElementType::VET_FLOAT3 );
        offset += sizeof( f32 ) * 3;
        decl->addElement( 0, offset, VertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
                          IVertexElement::VertexElementType::VET_FLOAT2, 0 );
        offset += sizeof( f32 ) * 2;

        auto vertSize = decl->getSize();

        // Create new vertex buffer
        auto ogreVertexDataPtr = static_cast<float *>(pBuf->createVertexData());

        // fill vertices
        for(u32 vertIdx = 0; vertIdx < numVert; ++vertIdx)
        {
            const Vector3F &position = vertPositions[vertIdx];
            const Vector3F &normal = vertNormals[vertIdx];
            const Vector2F &texCoord = vertTexCoords[( numVerts - 1 ) - vertIdx];

            box.merge( position );

            *ogreVertexDataPtr++ = position.X();
            *ogreVertexDataPtr++ = position.Y();
            *ogreVertexDataPtr++ = position.Z();

            *ogreVertexDataPtr++ = normal.X();
            *ogreVertexDataPtr++ = normal.Y();
            *ogreVertexDataPtr++ = normal.Z();

            *ogreVertexDataPtr++ = texCoord.X();
            *ogreVertexDataPtr++ = texCoord.Y();
        }

        BranchCount = PlantInstance->GetBranchCount( GroupIndex );
        IndexCount = PlantTemplate->GetIndexCount( GroupIndex, P3D_TRIANGLE_LIST );

        u32 meshIndexCount = IndexCount * BranchCount;
        auto pIndexBuffer = new u32[meshIndexCount];

        BranchAttrCount = PlantTemplate->GetVAttrCountI( GroupIndex );

        for(BranchIndex = 0; BranchIndex < BranchCount; BranchIndex++)
        {
            PlantTemplate->FillIndexBuffer( &( pIndexBuffer[BranchIndex * IndexCount] ), GroupIndex,
                                            P3D_TRIANGLE_LIST, P3D_UNSIGNED_INT,
                                            BranchAttrCount * BranchIndex );
        }

        bool useWords = ( pBuf->getNumVerticies() >= 65535 ) ? false : true;
        SmartPtr<IIndexBuffer> pIBuf( new IndexBuffer );
        pIBuf->setNumIndices( meshIndexCount );
        pIBuf->setIndexType( useWords ? IndexBuffer::Type::IT_16BIT : IndexBuffer::Type::IT_32BIT );

        subMesh->setIndexBuffer( pIBuf );
        pIBuf->createIndexData();

        unsigned short *pWords =
            useWords ? static_cast<unsigned short *>(pIBuf->getIndexData()) : nullptr;
        unsigned int *pDWords = useWords ? nullptr : static_cast<unsigned int *>(pIBuf->getIndexData());
        for(int i = 0; i < static_cast<int>(meshIndexCount); ++i)
        // for (int i = (int)pSubMesh->indexData->indexCount - 1; i >= 0; i--)
        {
            u32 indexValue = pIndexBuffer[i];
            if(useWords)
                *pWords++ = indexValue;
            else
                *pDWords++ = indexValue;
        }

        delete[] pIndexBuffer;
        delete[] PosBuffer;
        delete[] NormalBuffer;
        delete[] TexCoordBuffer;

        return true;
    }

    SmartPtr<IMesh> NGPlantLoader::loadMesh( const String &meshName )
    {
        // try
        //{
        //	auto engine = core::ApplicationManager::instance();
        //	SmartPtr<IFileSystem> fileSystem = engine->getFileSystem();
        //
        //	String plantFilePath = fileSystem->getFileDir(meshName);

        //	FBP3DMaterialFactory materialFactory;

        //	P3DInputStringStreamFile sourceStream;
        //	sourceStream.Open(plantFilePath.c_str());

        //	P3DHLIPlantTemplate PlantTemplate(&sourceStream);

        //	P3DHLIPlantInstance *PlantInstance;
        //	PlantInstance = PlantTemplate.CreateInstance();

        //	u32 BranchGroupCount = PlantTemplate.GetGroupCount();

        //	AABB3F aabb;
        //	aabb.m_minimum = Vector3F(1e10,1e10,1e10);
        //	aabb.m_maximum = Vector3F(-1e10,-1e10,-1e10);

        //	SmartPtr<IMesh> pMesh(new CMesh);
        //	SmartPtr<ISubMesh> pSubMesh;
        //	Vector3F vMin(-20.f, 0.f, -20.f);
        //	Vector3F vMax(20.f, 20.f, 20.f);
        //	Vector3F currPos;
        //	f32 maxSquaredRadius = 20 * 20;
        //	bool firstVert = true;

        //	if (BranchGroupCount > 0)
        //	{
        //		bool Result = true;
        //		u32 BranchGroupIndex = 0;
        //		while ((BranchGroupIndex < BranchGroupCount) && (Result))
        //		{
        //			const P3DMaterialDef* materialDef = PlantTemplate.GetMaterial(BranchGroupIndex);
        //			String textureName = materialDef->GetTexName(0);

        //			String materialName = Path::getFileNameWithoutExtension(meshName) + "_" +
        //Path::getFileNameWithoutExtension(textureName);

        //			// create subMesh
        //			pSubMesh = SmartPtr<ISubMesh>(new CSubMesh);
        //			pSubMesh->setMaterialName(materialName.c_str());

        //			Result = GetBranchGroupGeometry2(aabb, pSubMesh, &PlantTemplate, PlantInstance,
        //BranchGroupIndex);

        //			BranchGroupIndex++;
        //		}
        //	}
        //	else
        //	{
        //		LOG_MESSAGE("FoliageGenerator", "Plant model is empty");
        //	}

        //	//vMin = Vector3F(aabb.Min);
        //	//vMax = Vector3F(aabb.Max);
        //	//maxSquaredRadius = MathF::Max(vMin.squaredLength(), vMax.squaredLength());

        //	//// Set bounds
        //	//pMesh->_setBoundingSphereRadius(Math::Sqrt(maxSquaredRadius));
        //	//pMesh->_setBounds(AxisAlignedBox(vMin, vMax));

        //	return pMesh;
        //}
        // catch (P3DException& e)
        //{
        //	LOG_MESSAGE( "Graphics", e.GetMessage());
        //}

        return nullptr;
    }
#else

    SmartPtr<IMesh> NGPlantLoader::loadMesh( const String &meshName )
    {
        return nullptr;
    }

#endif
} // end namespace fb
