#include <FBGraphicsOgre/FBGraphicsOgrePCH.h>
#include "FBGraphicsOgre/Ogre/NGPlantLoader.h"
#include <FBCore/Base/StringUtil.h>

#ifdef FB_OGRE_USE_NGPLANT

#    include <ngpcore/p3dmodel.h>
#    include <ngpcore/p3dmodelstemtube.h>
#    include <ngpcore/p3dmodelstemquad.h>
#    include <ngpcore/p3dbalgbase.h>
#    include <ngpcore/p3dbalgstd.h>
#    include <ngpcore/p3diostream.h>

#    include <ngpcore/p3dhli.h>

#    include <OgreDefaultHardwareBufferManager.h>
#    include <map>
#    include <Ogre.h>

namespace fb
{

    class TreeVertex
    {
    public:
        Vector3F Position;
        Vector3F Normal;
        Vector3F Tangent;
        Vector3F TexCoord;
        float VertexData[4];  // store properties used for wind calculations
    };

    class TreeSubMesh
    {
    public:
        Array<TreeVertex> Vertices;
        Array<u32> Indices;
    };

    typedef std::map<String, TreeSubMesh *> TreeSubMeshMap;

    class TreeMesh
    {
    public:
        TreeSubMeshMap SubMeshes;
    };

    class P3DMaterialInstanceSimple : public P3DMaterialInstance
    {
    public:
        virtual const P3DMaterialDef *GetMaterialDef() const
        {
            return ( &MatDef );
        }

        virtual P3DMaterialInstance *CreateCopy() const
        {
            return new P3DMaterialInstanceSimple;
        }

    private:
        P3DMaterialDef MatDef;
    };

    class FBP3DMaterialFactory : public P3DMaterialFactory
    {
    public:
        FBP3DMaterialFactory()
        {
        }

        ~FBP3DMaterialFactory()
        {
        }

        P3DMaterialInstance *CreateMaterial( const P3DMaterialDef &materialDef ) const
        {
            return new P3DMaterialInstanceSimple;
        }
    };

    static void GetBranchGroupMaterial( const String &materialName, const P3DMaterialDef *MaterialDef )
    {
        // video::SColorf matColour;
        // MaterialDef->GetColor(&matColour.r,&matColour.g,&matColour.b);

        const char *TexName = MaterialDef->GetTexName( P3D_TEX_DIFFUSE );

        if( TexName != 0 )
        {
        }

        bool DoubleSided = MaterialDef->IsDoubleSided();
        bool Transparent = MaterialDef->IsTransparent();

        /*FBMaterialPtr fbMaterial =
        FBMaterialManager::getSingletonPtr()->createRetrieveMaterial(materialName);
        if(!fbMaterial.isNull())
        {
            fbMaterial->setTextureName("Diffuse", TexName);
            fbMaterial->setTransparent(Transparent);
        }*/
    }

    static bool GetBranchGroupGeometry( AABB3F &box, Ogre::SubMesh *subMesh,
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

        if( AttrCount == 0 )
        {
            return ( true );
        }

        // meshBuffer->getVertexBuffer().set_used(AttrCount);

        f32 *PosBuffer = new f32[3 * AttrCount];
        f32 *NormalBuffer = new f32[3 * AttrCount];
        f32 *TexCoordBuffer = new f32[2 * AttrCount];

        if( ( PosBuffer != 0 ) && ( NormalBuffer != 0 ) )
        {
            VAttrBuffers.AddAttr( P3D_ATTR_VERTEX, (void *)PosBuffer, 0, sizeof( float ) * 3 );
            VAttrBuffers.AddAttr( P3D_ATTR_NORMAL, (void *)NormalBuffer, 0, sizeof( float ) * 3 );
            VAttrBuffers.AddAttr( P3D_ATTR_TEXCOORD0, (void *)TexCoordBuffer, 0, sizeof( float ) * 2 );
        }
        else
        {
            return ( false );
        }

        PlantInstance->FillVAttrBuffersI( &VAttrBuffers, GroupIndex );

        u32 numVerts = AttrCount;
        Vector3F *vertPositions = (Vector3F *)PosBuffer;
        Vector3F *vertNormals = (Vector3F *)NormalBuffer;
        Vector2F *vertTexCoords = (Vector2F *)TexCoordBuffer;
        /*for(u32 vertIdx=0; vertIdx<numVerts; ++vertIdx)
        {
            video::S3DVertex irrVert;
            irrVert.Pos = vertPositions[vertIdx];
            irrVert.Normal = vertNormals[vertIdx];
            irrVert.TCoords = vertTexCoords[(numVerts-1)-vertIdx];	//why do uvs have to be reversed?
            meshBuffer->getVertexBuffer()[vertIdx] = irrVert;
        }*/

        Ogre::HardwareVertexBufferSharedPtr pBuf;  // = NULL;
        Ogre::VertexBufferBinding *pBind = NULL;
        Ogre::VertexDeclaration *pDecl = NULL;
        Ogre::Real *pReal = NULL;

        size_t numVert = subMesh->vertexData->vertexCount = numVerts;
        pDecl = subMesh->vertexData->vertexDeclaration;
        pBind = subMesh->vertexData->vertexBufferBinding;

        //
        unsigned short iBinding = 0;
        // do positions first
        pDecl->addElement( iBinding, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
        pBuf = Ogre::DefaultHardwareBufferManager::getSingletonPtr()->createVertexBuffer(
            pDecl->getVertexSize( iBinding ), numVert, Ogre::HardwareBuffer::HBU_DYNAMIC, false );
        pBind->setBinding( iBinding, pBuf );
        pReal = static_cast<Ogre::Real *>( pBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );

        // fill vertices
        for( int vertIdx = 0; vertIdx < numVert; vertIdx++ )
        {
            const Vector3F &v = vertPositions[vertIdx];
            box.merge( v );

            Ogre::Vector3 pos = Ogre::Vector3( v.X(), v.Y(), v.Z() );

            *pReal++ = pos.x;
            *pReal++ = pos.y;
            *pReal++ = pos.z;

            // Deal with bounds
            /*currPos = pos;
            if (firstVert)
            {
                vMin = vMax = currPos;
                maxSquaredRadius = currPos.squaredLength();
                firstVert = false;
            }
            else
            {
                vMin.makeFloor(currPos);
                vMax.makeCeil(currPos);
                maxSquaredRadius = max_(maxSquaredRadius, currPos.squaredLength());
            }*/
        }

        // do normals
        iBinding++;
        pDecl->addElement( iBinding, 0, Ogre::VET_FLOAT3, Ogre::VES_NORMAL );
        pBuf = Ogre::DefaultHardwareBufferManager::getSingletonPtr()->createVertexBuffer(
            pDecl->getVertexSize( iBinding ), numVert, Ogre::HardwareBuffer::HBU_DYNAMIC, false );
        pBind->setBinding( iBinding, pBuf );
        pReal = static_cast<Ogre::Real *>( pBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );
        for( int vertIdx = 0; vertIdx < numVert; vertIdx++ )
        {
            Vector3F irrNor = vertNormals[vertIdx];

            Ogre::Vector3 nor;
            nor.x = irrNor.X();
            nor.y = irrNor.Y();
            nor.z = irrNor.Z();
            // nor =  world2objN * nor;
            *pReal++ = nor.x;
            *pReal++ = nor.y;
            *pReal++ = nor.z;
        }

        iBinding++;
        pDecl->addElement( iBinding, 0, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0 );
        pBuf = Ogre::DefaultHardwareBufferManager::getSingletonPtr()->createVertexBuffer(
            pDecl->getVertexSize( iBinding ), numVert, Ogre::HardwareBuffer::HBU_DYNAMIC, false );
        pBind->setBinding( iBinding, pBuf );
        pReal = static_cast<Ogre::Real *>( pBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );
        for( int vertIdx = 0; vertIdx < numVert; vertIdx++ )
        {
            Vector2F tc = vertTexCoords[( numVerts - 1 ) - vertIdx];

            *pReal++ = tc.X();
            *pReal++ = tc.Y();

        }  // for vertex

        BranchCount = PlantInstance->GetBranchCount( GroupIndex );
        IndexCount = PlantTemplate->GetIndexCount( GroupIndex, P3D_TRIANGLE_LIST );

        u32 meshIndexCount = IndexCount * BranchCount;
        u32 *IndexBuffer = new u32[meshIndexCount];

        BranchAttrCount = PlantTemplate->GetVAttrCountI( GroupIndex );

        for( BranchIndex = 0; BranchIndex < BranchCount; BranchIndex++ )
        {
            PlantTemplate->FillIndexBuffer( &( IndexBuffer[BranchIndex * IndexCount] ), GroupIndex,
                                            P3D_TRIANGLE_LIST, P3D_UNSIGNED_INT,
                                            BranchAttrCount * BranchIndex );
        }

        bool useWords = ( subMesh->vertexData->vertexCount >= 65535 ) ? false : true;
        subMesh->indexData->indexCount = meshIndexCount;
        Ogre::HardwareIndexBufferSharedPtr pIBuf =
            Ogre::DefaultHardwareBufferManager::getSingletonPtr()->createIndexBuffer(
                useWords ? Ogre::HardwareIndexBuffer::IT_16BIT : Ogre::HardwareIndexBuffer::IT_32BIT,
                subMesh->indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY );
        subMesh->indexData->indexBuffer = pIBuf;

        unsigned short *pWords =
            useWords ? static_cast<unsigned short *>( pIBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) )
                     : NULL;
        unsigned int *pDWords =
            useWords ? NULL
                     : static_cast<unsigned int *>( pIBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );
        for( int i = 0; i < (int)meshIndexCount; i++ )
        // for (int i = (int)pSubMesh->indexData->indexCount - 1; i >= 0; i--)
        {
            u32 indexValue = IndexBuffer[i];
            if( useWords )
                *pWords++ = indexValue;
            else
                *pDWords++ = indexValue;
        }

        pIBuf->unlock();

        delete[] IndexBuffer;
        delete[] PosBuffer;
        delete[] NormalBuffer;
        delete[] TexCoordBuffer;

        return true;
    }

    static bool GetBranchGroupGeometry2( AABB3F &box, Ogre::SubMesh *subMesh,
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

        if( AttrCount == 0 )
        {
            return ( true );
        }

        // meshBuffer->getVertexBuffer().set_used(AttrCount);

        f32 *PosBuffer = new f32[3 * AttrCount];
        f32 *NormalBuffer = new f32[3 * AttrCount];
        f32 *TexCoordBuffer = new f32[2 * AttrCount];

        if( ( PosBuffer != 0 ) && ( NormalBuffer != 0 ) )
        {
            VAttrBuffers.AddAttr( P3D_ATTR_VERTEX, (void *)PosBuffer, 0, sizeof( float ) * 3 );
            VAttrBuffers.AddAttr( P3D_ATTR_NORMAL, (void *)NormalBuffer, 0, sizeof( float ) * 3 );
            VAttrBuffers.AddAttr( P3D_ATTR_TEXCOORD0, (void *)TexCoordBuffer, 0, sizeof( float ) * 2 );
        }
        else
        {
            return ( false );
        }

        PlantInstance->FillVAttrBuffersI( &VAttrBuffers, GroupIndex );

        u32 numVerts = AttrCount;
        Vector3F *vertPositions = (Vector3F *)PosBuffer;
        Vector3F *vertNormals = (Vector3F *)NormalBuffer;
        Vector2F *vertTexCoords = (Vector2F *)TexCoordBuffer;
        /*for(u32 vertIdx=0; vertIdx<numVerts; ++vertIdx)
        {
            video::S3DVertex irrVert;
            irrVert.Pos = vertPositions[vertIdx];
            irrVert.Normal = vertNormals[vertIdx];
            irrVert.TCoords = vertTexCoords[(numVerts-1)-vertIdx];	//why do uvs have to be reversed?
            meshBuffer->getVertexBuffer()[vertIdx] = irrVert;
        }*/

        Ogre::HardwareVertexBufferSharedPtr pBuf;  // = NULL;
        Ogre::VertexBufferBinding *pBind = NULL;
        Ogre::VertexDeclaration *decl = NULL;
        Ogre::Real *pReal = NULL;

        size_t numVert = subMesh->vertexData->vertexCount = numVerts;
        decl = subMesh->vertexData->vertexDeclaration;
        pBind = subMesh->vertexData->vertexBufferBinding;

        unsigned short iBinding = 0;
        u32 offset = 0;
        decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
        offset += sizeof( f32 ) * 3;
        decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL );
        offset += sizeof( f32 ) * 3;
        decl->addElement( iBinding, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0 );
        offset += sizeof( f32 ) * 2;

        u32 vertSize = decl->getVertexSize( 0 );

        // Create new vertex buffer
        Ogre::HardwareVertexBufferSharedPtr vbuf =
            Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
                vertSize, numVert, Ogre::HardwareBuffer::HBU_STATIC );
        // Bind buffer
        pBind->setBinding( 0, vbuf );
        Ogre::Real *ogreVertexDataPtr =
            static_cast<Ogre::Real *>( vbuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );

        // fill vertices
        for( int vertIdx = 0; vertIdx < numVert; ++vertIdx )
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

        vbuf->unlock();

        BranchCount = PlantInstance->GetBranchCount( GroupIndex );
        IndexCount = PlantTemplate->GetIndexCount( GroupIndex, P3D_TRIANGLE_LIST );

        u32 meshIndexCount = IndexCount * BranchCount;
        u32 *IndexBuffer = new u32[meshIndexCount];

        BranchAttrCount = PlantTemplate->GetVAttrCountI( GroupIndex );

        for( BranchIndex = 0; BranchIndex < BranchCount; BranchIndex++ )
        {
            PlantTemplate->FillIndexBuffer( &( IndexBuffer[BranchIndex * IndexCount] ), GroupIndex,
                                            P3D_TRIANGLE_LIST, P3D_UNSIGNED_INT,
                                            BranchAttrCount * BranchIndex );
        }

        bool useWords = ( subMesh->vertexData->vertexCount >= 65535 ) ? false : true;
        subMesh->indexData->indexCount = meshIndexCount;
        Ogre::HardwareIndexBufferSharedPtr pIBuf =
            Ogre::DefaultHardwareBufferManager::getSingletonPtr()->createIndexBuffer(
                useWords ? Ogre::HardwareIndexBuffer::IT_16BIT : Ogre::HardwareIndexBuffer::IT_32BIT,
                subMesh->indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY );
        subMesh->indexData->indexBuffer = pIBuf;

        unsigned short *pWords =
            useWords ? static_cast<unsigned short *>( pIBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) )
                     : NULL;
        unsigned int *pDWords =
            useWords ? NULL
                     : static_cast<unsigned int *>( pIBuf->lock( Ogre::HardwareBuffer::HBL_DISCARD ) );
        for( int i = 0; i < (int)meshIndexCount; i++ )
        // for (int i = (int)pSubMesh->indexData->indexCount - 1; i >= 0; i--)
        {
            u32 indexValue = IndexBuffer[i];
            if( useWords )
                *pWords++ = indexValue;
            else
                *pDWords++ = indexValue;
        }

        pIBuf->unlock();

        delete[] IndexBuffer;
        delete[] PosBuffer;
        delete[] NormalBuffer;
        delete[] TexCoordBuffer;

        return true;
    }

    String OgreNGPlantLoader::loadMesh( const String &meshName )
    {
        try
        {
            String ogreMeshFileName = meshName.subString( 0, meshName.length() - 4 );
            Ogre::String ogreMeshFileNameFull =
                Ogre::String( ogreMeshFileName.c_str() ) + Ogre::String( ".mesh" );

            auto engine = core::IApplicationManager::instance();
            SmartPtr<IFileSystem> fileSystem = engine->getFileSystem();
            if( fileSystem->isExistingFile( ogreMeshFileNameFull.c_str() ) )
            {
                return ogreMeshFileNameFull.c_str();
            }

            // will need to attempt a mesh load
            Ogre::MeshManager::ResourceCreateOrRetrieveResult createOrRetriveMesh =
                Ogre::MeshManager::getSingletonPtr()->createOrRetrieve(
                    ogreMeshFileNameFull, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                    true );
            if( !createOrRetriveMesh.second )
            {
                return ogreMeshFileNameFull.c_str();
            }

            String plantFilePath = fileSystem->getFileDir( meshName );

            FBP3DMaterialFactory materialFactory;

            P3DInputStringStreamFile sourceStream;
            sourceStream.Open( plantFilePath.c_str() );

            P3DHLIPlantTemplate PlantTemplate( &sourceStream );

            P3DHLIPlantInstance *PlantInstance;
            PlantInstance = PlantTemplate.CreateInstance();

            u32 BranchGroupCount = PlantTemplate.GetGroupCount();

            AABB3F aabb;
            aabb.Min = Vector3F( 1e10, 1e10, 1e10 );
            aabb.Max = Vector3F( -1e10, -1e10, -1e10 );

            Ogre::MeshPtr pMesh;  // = createOrRetriveMesh.first.staticCast<Ogre::Mesh>();
            Ogre::SubMesh *pSubMesh = NULL;
            Ogre::VertexData *pData = NULL;
            // Ogre::VertexBufferBinding *pBind = NULL;
            // Ogre::VertexDeclaration *pDecl = NULL;
            Ogre::HardwareVertexBufferSharedPtr pBuf;  // = NULL;
            Ogre::Real *pReal = NULL;
            Ogre::Vector3 vMin( -20.f, 0.f, -20.f );
            Ogre::Vector3 vMax( 20.f, 20.f, 20.f );
            Ogre::Vector3 currPos;
            Ogre::Real maxSquaredRadius = 20 * 20;
            bool firstVert = true;

            if( BranchGroupCount > 0 )
            {
                bool Result = true;
                u32 BranchGroupIndex = 0;
                while( ( BranchGroupIndex < BranchGroupCount ) && ( Result ) )
                {
                    String materialName = Path::getFileNameWithoutExtension( meshName );
                    materialName += "_branch_";
                    materialName += StringUtil::toString( BranchGroupIndex );

                    Ogre::MaterialPtr material =
                        Ogre::MaterialManager::getSingletonPtr()->getByName( materialName.c_str() );
                    if( material.isNull() )
                    {
                        LOG_MESSAGE( "FoliageGenerator",
                                     ( String( "Missing Material " ) + materialName ).c_str() );
                    }

                    // create subMesh
                    pSubMesh = pMesh->createSubMesh( materialName.c_str() );
                    pSubMesh->setMaterialName( materialName.c_str() );
                    pSubMesh->useSharedVertices = false;
                    pSubMesh->vertexData = new Ogre::VertexData();
                    pSubMesh->indexData = new Ogre::IndexData();
                    // size_t numVert = pSubMesh->vertexData->vertexCount = meshBuffer->getVertexCount();
                    // pDecl = pSubMesh->vertexData->vertexDeclaration;
                    // pBind = pSubMesh->vertexData->vertexBufferBinding;

                    Result = GetBranchGroupGeometry2( aabb, pSubMesh, &PlantTemplate, PlantInstance,
                                                      BranchGroupIndex );

                    BranchGroupIndex++;
                }
            }
            else
            {
                LOG_MESSAGE( "FoliageGenerator", "Plant model is empty" );
            }

            vMin = Ogre::Vector3( aabb.Min );
            vMax = Ogre::Vector3( aabb.Max );
            maxSquaredRadius = MathF::max( vMin.squaredLength(), vMax.squaredLength() );

            // Set bounds
            pMesh->_setBoundingSphereRadius( Ogre::Math::Sqrt( maxSquaredRadius ) );
            pMesh->_setBounds( Ogre::AxisAlignedBox( vMin, vMax ) );

            // save mesh and finish ...
            Ogre::MeshSerializer *meshSer = new Ogre::MeshSerializer;
            meshSer->exportMesh( pMesh.getPointer(), pMesh->getName() );
            delete meshSer;

            return String( ogreMeshFileNameFull.c_str() );
        }
        catch( P3DException &e )
        {
            LOG_MESSAGE( "Graphics", e.GetMessage() );
        }

        return StringUtil::EmptyString;
    }

}  // end namespace fb

#else
namespace fb
{
    String OgreNGPlantLoader::loadMesh( const String &meshName )
    {
        return fb::StringUtil::EmptyString;
    }
}  // namespace fb
#endif
