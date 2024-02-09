#include <FBGraphicsOgreNext/FBGraphicsOgreNextPCH.h>
#include <FBGraphicsOgreNext/MeshLoader.h>
#include <FBCore/FBCore.h>
#include <FBMesh/FBMesh.h>
#include <Ogre.h>
#include <OgreMesh2.h>
#include <OgreMeshManager2.h>
#include <OgreHardwareIndexBuffer.h>
#include <OgreSubMesh2.h>
#include <OgreSubMesh.h>
#include <OgreMeshManager.h>
#include <OgreMeshSerializer.h>
#include <OgreMesh.h>
#include <OgreMesh2.h>
#include <OgreException.h>
#include <OgreHardwareBufferManager.h>
#include <OgreLogManager.h>
#include <OgreBitwise.h>
#include <Vao/OgreVaoManager.h>
#include <Vao/OgreAsyncTicket.h>
#include <OgreVertexShadowMapHelper.h>
#include <OgreStringConverter.h>
#include <OgreDefaultHardwareBufferManager.h>
#include <OgreVertexIndexData.h>
#include <OgreHardwareIndexBuffer.h>

#include "OgreMesh2Serializer.h"

namespace fb::render
{

    MeshLoader::MeshLoader()
    {
        setUseSingleMesh( true );
    }

    MeshLoader::~MeshLoader() = default;

    void MeshLoader::createV2Mesh( Ogre::Mesh *mesh, SmartPtr<IMesh> fbmesh )
    {
        Ogre::RenderSystem *renderSystem = Ogre::Root::getSingleton().getRenderSystem();
        Ogre::VaoManager *vaoManager = renderSystem->getVaoManager();

        auto bLogUVs = false;

        static int meshCount = 0;
        auto meshName = fbmesh->getName() + StringUtil::toString( meshCount++ );
        // Create the mesh:
        //Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual(
        //    meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

        auto subMeshList = fbmesh->getSubMeshes();
        auto numSubMeshes = subMeshList.size();

        Ogre::Vector3 minBB( Ogre::Vector3::UNIT_SCALE * FLT_MAX );
        Ogre::Vector3 maxBB( Ogre::Vector3::UNIT_SCALE * -FLT_MAX );

        for( size_t subMeshIdx = 0; subMeshIdx < numSubMeshes; ++subMeshIdx )
        {
            auto fbSubMesh = subMeshList[subMeshIdx];

            auto fbVertexBuffer = fbSubMesh->getVertexBuffer();
            auto fbIndexBuffer = fbSubMesh->getIndexBuffer();

            u32 fbVertexCount = fbVertexBuffer->getNumVertices();
            u32 fbIndexCount = fbIndexBuffer->getNumIndices();

            if( fbVertexCount <= 1 )
            {
                continue;
            }

            if( fbIndexCount <= 1 )
            {
                continue;
            }

            auto matName = fbSubMesh->getMaterialName();

            auto fbVertexDeclaration = fbVertexBuffer->getVertexDeclaration();
            const auto posElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_POSITION );
            const auto normalElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_NORMAL );

            Array<SmartPtr<IVertexElement>> texCoordElems;
            texCoordElems.reserve( 8 );

            for( size_t i = 0; i < 8; ++i )
            {
                auto texCoordElem = fbVertexDeclaration->findElementBySemantic(
                    IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, i );
                if( texCoordElem )
                {
                    texCoordElems.push_back( texCoordElem );
                }
            }

            const auto diffuseElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_DIFFUSE );
            const auto tangentElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_TANGENT );

            Ogre::SubMesh *subMesh = mesh->createSubMesh();

            Ogre::VertexElement2Vec vertexElements;
            vertexElements.emplace_back( Ogre::VET_FLOAT3, Ogre::VES_POSITION );
            vertexElements.emplace_back( Ogre::VET_FLOAT3, Ogre::VES_NORMAL );

            if( tangentElem )
            {
                vertexElements.emplace_back( Ogre::VET_FLOAT3, Ogre::VES_TANGENT );
            }

            //uvs
            for( auto texCoordElem : texCoordElems )
            {
                if( texCoordElem )
                {
                    auto texCoordElemType = texCoordElem->getType();

                    if( texCoordElemType == IVertexElement::VertexElementType::VET_FLOAT3 )
                    {
                        vertexElements.emplace_back( Ogre::VET_FLOAT3, Ogre::VES_TEXTURE_COORDINATES );
                    }
                    else if( texCoordElemType == IVertexElement::VertexElementType::VET_FLOAT2 )
                    {
                        vertexElements.emplace_back( Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );
                    }
                }
            }

            size_t vertexSize = vaoManager->calculateVertexSize( vertexElements );

            auto vertexData = static_cast<Ogre::Real *>(
                OGRE_MALLOC_SIMD( vertexSize * fbVertexCount, Ogre::MEMCATEGORY_GEOMETRY ) );
            auto pVertex = vertexData;

            u32 fbVertexSize = fbVertexDeclaration->getSize();
            u8 *fbVertexDataPtr = static_cast<u8 *>( fbVertexBuffer->getVertexData() );
            f32 *fbElementData = nullptr;

            for( int i = 0; i < fbVertexCount; i++ )
            {
                posElem->getElementData( fbVertexDataPtr, &fbElementData );

                Ogre::Vector3 ogrePosition( fbElementData[0], fbElementData[1], fbElementData[2] );

                *pVertex++ = ogrePosition.x;
                *pVertex++ = ogrePosition.y;
                *pVertex++ = ogrePosition.z;

                if( normalElem )
                {
                    normalElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Ogre::Vector3 ogreNormal( fbElementData[0], fbElementData[1], fbElementData[2] );

                    //Normals
                    *pVertex++ = ogreNormal.x;
                    *pVertex++ = ogreNormal.y;
                    *pVertex++ = ogreNormal.z;
                }

                if( tangentElem )
                {
                    tangentElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Ogre::Vector3 ogreTangent( fbElementData[0], fbElementData[1], fbElementData[2] );

                    *pVertex++ = ogreTangent.x;
                    *pVertex++ = ogreTangent.y;
                    *pVertex++ = ogreTangent.z;
                }

                for( auto texCoordElem : texCoordElems )
                {
                    if( texCoordElem )
                    {
                        auto texCoordElemType = texCoordElem->getType();

                        if( texCoordElemType == IVertexElement::VertexElementType::VET_FLOAT3 )
                        {
                            texCoordElem->getElementData( fbVertexDataPtr, &fbElementData );
                            *pVertex++ = fbElementData[0];
                            *pVertex++ = fbElementData[1];
                            *pVertex++ = fbElementData[2];

                            if( bLogUVs )
                            {
                                FB_LOG( StringUtil::toString(
                                    Vector2F( fbElementData[0], fbElementData[1] ) ) );
                            }
                        }
                        else if( texCoordElemType == IVertexElement::VertexElementType::VET_FLOAT2 )
                        {
                            texCoordElem->getElementData( fbVertexDataPtr, &fbElementData );
                            *pVertex++ = fbElementData[0];
                            *pVertex++ = fbElementData[1];

                            if( bLogUVs )
                            {
                                FB_LOG( StringUtil::toString(
                                    Vector2F( fbElementData[0], fbElementData[1] ) ) );
                            }
                        }
                    }
                }

                //Calc Bounds
                minBB.makeFloor( ogrePosition );
                maxBB.makeCeil( ogrePosition );

                fbVertexDataPtr += fbVertexSize;
            }

            Ogre::VertexBufferPackedVec vertexBuffers;

            Ogre::VertexBufferPacked *pVertexBuffer = vaoManager->createVertexBuffer(
                vertexElements, fbVertexCount, Ogre::BT_IMMUTABLE, vertexData, true );
            vertexBuffers.push_back( pVertexBuffer );

            auto iBufSize = fbIndexBuffer->getNumIndices();
            constexpr auto index16BitClamp = std::numeric_limits<u16>::max();

            auto buff_type = fbVertexCount < index16BitClamp ? Ogre::IndexBufferPacked::IT_16BIT
                                                             : Ogre::IndexBufferPacked::IT_32BIT;

            auto indexBuffer = vaoManager->createIndexBuffer( buff_type, iBufSize, Ogre::BT_IMMUTABLE,
                                                              fbIndexBuffer->getIndexData(), false );

            auto vao = vaoManager->createVertexArrayObject( vertexBuffers, indexBuffer,
                                                            Ogre::OT_TRIANGLE_LIST );

            subMesh->mVao[0].push_back( vao );
            subMesh->mVao[1].push_back( vao );
        }

        Ogre::Aabb bounds;
        bounds.merge( minBB );
        bounds.merge( maxBB );
        mesh->_setBounds( bounds, false );
        mesh->_setBoundingSphereRadius( bounds.getRadius() );
    }

    void MeshLoader::loadFBMesh( Ogre::MeshPtr mesh, const String &meshPath )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto fileSystem = applicationManager->getFileSystem();
        auto meshManager = applicationManager->getMeshManager();
        if( meshManager )
        {
            auto resource = meshManager->loadFromFile( meshPath );
            if( resource )
            {
                auto meshResource = fb::static_pointer_cast<MeshResource>( resource );
                if( !meshResource->isLoaded() )
                {
                    meshResource->load( nullptr );
                }

                if( auto fbMesh = meshResource->getMesh() )
                {
                    createV2Mesh( mesh.get(), fbMesh );
                    return;
                }
            }

            auto fbMeshStream = fileSystem->open( meshPath, true, true, false, false );
            if( !fbMeshStream )
            {
                fbMeshStream = fileSystem->open( meshPath, true, true, false, true );
            }

            if( fbMeshStream )
            {
                MeshSerializer meshSerializer;
                auto fbMesh = meshSerializer.loadMesh( fbMeshStream );

                createV2Mesh( mesh.get(), fbMesh );
            }
        }
    }

    auto MeshLoader::loadFBMesh( SmartPtr<IStream> stream ) -> Ogre::Mesh *
    {
        return nullptr;
    }

    void MeshLoader::loadFBMesh( Ogre::MeshPtr meshPtr, SmartPtr<IMesh> mesh )
    {
        using namespace Ogre;

        FB_LOG( "Mesh2::importV1" );

        mesh->load( nullptr );

        auto loadingState = meshPtr->getLoadingState();

        if( loadingState != Ogre::Resource::LoadingState::LOADSTATE_UNLOADED &&
            loadingState != Ogre::Resource::LoadingState::LOADSTATE_LOADING )
        {
            OGRE_EXCEPT( Ogre::Exception::ERR_INVALID_STATE,
                         "To import a v1 mesh, the v2 mesh must be in unloaded state!",
                         "Mesh::importV1" );
        }

        if( mesh->getHasSharedVertexData() )
        {
            FB_LOG( "WARNING: Mesh '" + mesh->getName() +
                    "' has shared vertices. They're being "
                    "'unshared' for importing to v2" );
            MeshUtil::unshareVertices( mesh );
        }

        auto aabb = mesh->getAABB();
        auto minimum = aabb.getMinimum();
        auto maximum = aabb.getMaximum();

        auto ogreAabb = Ogre::AxisAlignedBox( Ogre::Vector3( minimum.X(), minimum.Y(), minimum.Z() ),
                                              Ogre::Vector3( maximum.X(), maximum.Y(), maximum.Z() ) );
        // auto ogreBoundingRadius = ogreAabb.getHalfSize().length();

        /*
        try
        {
            if( qTangents )
            {
                unsigned short sourceCoordSet;
                unsigned short index;
                bool alreadyHasTangents = mesh->suggestTangentVectorBuildParams( VES_TANGENT,
                        sourceCoordSet,
                        index );
                if( !alreadyHasTangents )
                    mesh->buildTangentVectors( VES_TANGENT, sourceCoordSet, index, false, false, true
        );
            }
        }
        catch( Exception & )
        {
        }*/

        bool halfPos = false;
        bool halfTexCoords = false;
        bool qTangents = false;
        bool halfPose = false;

        auto subMeshes = mesh->getSubMeshes();
        for( auto pSubMesh : subMeshes )
        {
            auto subMesh = meshPtr->createSubMesh();
            importFromV1( meshPtr, subMesh, mesh, pSubMesh, halfPos, halfTexCoords, qTangents,
                          halfPose );
        }
        /*
                    mSubMeshNameMap = mesh->getSubMeshNameMap();

                    mSkeletonName = mesh->getSkeletonName();
                    v1::SkeletonPtr v1Skeleton = mesh->getOldSkeleton();
                    if( !v1Skeleton.isNull() )
                        mSkeleton = SkeletonManager::getSingleton().getSkeletonDef( v1Skeleton.get()
           );

                    //So far we only import manual LOD levels. If the mesh had manual LOD levels,
                    //mLodValues will have more entries than Vaos, causing an out of bounds
           exception.
                    //Don't use LOD if the imported mesh had manual levels.
                    //Note: Mesh2 supports LOD levels that have their own vertex and index buffers,
                    //so it should be possible to import them as well.
                    if( !mesh->hasManualLodLevel() )
                        mLodValues = *mesh->_getLodValueArray();
                    else
                        mLodValues = MovableObject::c_DefaultLodMesh;
        */

        meshPtr->setManuallyLoaded( true );
        meshPtr->setToLoaded();
    }

    auto MeshLoader::convertFBMeshToOgreMesh( const String &newMeshName, SmartPtr<IMesh> mesh )
        -> Ogre::v1::MeshPtr
    {
        if( !mesh )
        {
            return {};
        }

        auto bLogUVs = StringUtil::contains( newMeshName, "cube" );

        static int meshIdx = 0;

        auto meshManager = Ogre::v1::MeshManager::getSingletonPtr();
        FB_ASSERT( meshManager );

        auto logManager = Ogre::LogManager::getSingletonPtr();
        FB_ASSERT( logManager );

        auto hardwareBufferManager = Ogre::v1::HardwareBufferManager::getSingletonPtr();
        FB_ASSERT( hardwareBufferManager );

        auto defaultHardwareBufferManager = Ogre::v1::DefaultHardwareBufferManager::getSingletonPtr();
        FB_ASSERT( defaultHardwareBufferManager );

        auto group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
        // Ogre::String meshName = "OutMesh" + Ogre::StringConverter::toString(meshIdx++);
        Ogre::String meshName = newMeshName.c_str();
        auto createOrRetriveMesh =
            meshManager->createOrRetrieve( meshName, Ogre::String( "General" ), true );
        if( !createOrRetriveMesh.second )
        {
            logManager->logMessage( "maxExporter::doMesh resource already exists" );
            logManager->logMessage( "maxExporter::doMesh end" );
            return {};
        }

        Ogre::v1::MeshPtr pMesh = createOrRetriveMesh.first.staticCast<Ogre::v1::Mesh>();

        // Ogre::v1::Mesh* pMesh = new Ogre::v1::Mesh(nullptr, newMeshName, -1, group);
        // Ogre::v1::VertexData* pData = nullptr;

        Ogre::v1::HardwareVertexBufferSharedPtr pBuf;
        // Ogre::Real* pReal = nullptr;
        Ogre::Vector3 currPos;
        Ogre::Real maxSquaredRadius = 0.0f;
        bool firstVert = true;

        Ogre::Vector3 aabbMin( 1e10, 1e10, 1e10 );
        Ogre::Vector3 aabbMax( -1e10, -1e10, -1e10 );

        auto subMeshList = mesh->getSubMeshes();
        auto numSubMeshes = subMeshList.size();

        for( size_t subMeshIdx = 0; subMeshIdx < numSubMeshes; ++subMeshIdx )
        {
            auto fbSubMesh = subMeshList[subMeshIdx];

            auto fbVertexBuffer = fbSubMesh->getVertexBuffer();
            auto fbIndexBuffer = fbSubMesh->getIndexBuffer();

            u32 fbVertexCount = fbVertexBuffer->getNumVertices();
            u32 fbIndexCount = fbIndexBuffer->getNumIndices();

            if( fbVertexCount <= 1 )
            {
                continue;
            }

            if( fbIndexCount <= 1 )
            {
                continue;
            }

            auto matName = fbSubMesh->getMaterialName();
            //auto matName = String( "" );

            // create subMesh
            auto ogreSubMesh = pMesh->createSubMesh( matName );
            ogreSubMesh->setMaterialName( matName );

            ogreSubMesh->useSharedVertices = false;
            ogreSubMesh->vertexData[Ogre::VpNormal] = new Ogre::v1::VertexData( nullptr );
            ogreSubMesh->indexData[Ogre::VpNormal] = new Ogre::v1::IndexData();

            ogreSubMesh->vertexData[Ogre::VpShadow] = ogreSubMesh->vertexData[Ogre::VpNormal];
            ogreSubMesh->indexData[Ogre::VpShadow] = ogreSubMesh->indexData[Ogre::VpNormal];

            size_t ogreNumVertices = ogreSubMesh->vertexData[0]->vertexCount = fbVertexCount;
            auto decl = ogreSubMesh->vertexData[0]->vertexDeclaration;

            FB_ASSERT_TRUE( ogreSubMesh->vertexData[0]->vertexCount == 0 );

            auto fbVertexDeclaration = fbVertexBuffer->getVertexDeclaration();
            const auto posElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_POSITION );
            const auto normalElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_NORMAL );
            const auto texCoordElem0 = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 0 );
            const auto texCoordElem1 = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 1 );
            const auto diffuseElem = fbVertexDeclaration->findElementBySemantic(
                IVertexDeclaration::VertexElementSemantic::VES_DIFFUSE );

            unsigned short iBinding = 0;
            u32 offset = 0;
            decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
            offset += sizeof( f32 ) * 3;

            if( normalElem )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL );
                offset += sizeof( f32 ) * 3;
            }

            if( texCoordElem0 )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_TEXTURE_COORDINATES, 0 );
                offset += sizeof( f32 ) * 3;
            }

            if( texCoordElem1 )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT3, Ogre::VES_TEXTURE_COORDINATES, 1 );
                offset += sizeof( f32 ) * 3;
            }

            if( diffuseElem )
            {
                decl->addElement( iBinding, offset, Ogre::VET_FLOAT4, Ogre::VES_DIFFUSE, 0 );
                offset += sizeof( f32 ) * 4;
            }

            auto ogreVertexSize = decl->getVertexSize( 0 );

            // Create new vertex buffer
            auto ogreVertexBuffer = defaultHardwareBufferManager->createVertexBuffer(
                ogreVertexSize, ogreNumVertices, Ogre::v1::HardwareBuffer::HBU_STATIC );

            // Bind buffer
            auto pBind = ogreSubMesh->vertexData[0]->vertexBufferBinding;
            pBind->setBinding( 0, ogreVertexBuffer );

            auto *ogreVertexDataPtr = static_cast<Ogre::Real *>(
                ogreVertexBuffer->lock( Ogre::v1::HardwareBuffer::HBL_DISCARD ) );

            u32 fbVertexSize = fbVertexDeclaration->getSize();
            u8 *fbVertexDataPtr = static_cast<u8 *>( fbVertexBuffer->getVertexData() );
            f32 *fbElementData = nullptr;

            for( u32 vertIdx = 0; vertIdx < fbVertexCount; ++vertIdx )
            {
                posElem->getElementData( fbVertexDataPtr, &fbElementData );

                Ogre::Vector3 ogrePosition( fbElementData[0], fbElementData[1], fbElementData[2] );

                // Deal with bounds
                currPos = ogrePosition;
                if( firstVert )
                {
                    aabbMin = aabbMax = currPos;
                    maxSquaredRadius = currPos.squaredLength();
                    firstVert = false;
                }
                else
                {
                    aabbMin.makeFloor( currPos );
                    aabbMax.makeCeil( currPos );
                    maxSquaredRadius = MathF::max( maxSquaredRadius, currPos.squaredLength() );
                }

                if( ogrePosition.x < aabbMin.x )
                {
                    aabbMin.x = ogrePosition.x;
                }
                if( ogrePosition.y < aabbMin.y )
                {
                    aabbMin.y = ogrePosition.y;
                }
                if( ogrePosition.z < aabbMin.z )
                {
                    aabbMin.z = ogrePosition.z;
                }

                if( ogrePosition.x > aabbMax.x )
                {
                    aabbMax.x = ogrePosition.x;
                }
                if( ogrePosition.y > aabbMax.y )
                {
                    aabbMax.y = ogrePosition.y;
                }
                if( ogrePosition.z > aabbMax.z )
                {
                    aabbMax.z = ogrePosition.z;
                }

                *ogreVertexDataPtr++ = ogrePosition.x;
                *ogreVertexDataPtr++ = ogrePosition.y;
                *ogreVertexDataPtr++ = ogrePosition.z;

                if( normalElem )
                {
                    normalElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Ogre::Vector3 ogreNormal( fbElementData[0], fbElementData[1], fbElementData[2] );

                    *ogreVertexDataPtr++ = ogreNormal.x;
                    *ogreVertexDataPtr++ = ogreNormal.y;
                    *ogreVertexDataPtr++ = ogreNormal.z;
                }

                if( texCoordElem0 )
                {
                    if( texCoordElem0->getType() == IVertexElement::VertexElementType::VET_FLOAT3 )
                    {
                        texCoordElem0->getElementData( fbVertexDataPtr, &fbElementData );
                        *ogreVertexDataPtr++ = fbElementData[0];
                        *ogreVertexDataPtr++ = fbElementData[1];
                        *ogreVertexDataPtr++ = fbElementData[2];

                        if( bLogUVs )
                        {
                            FB_LOG(
                                StringUtil::toString( Vector2F( fbElementData[0], fbElementData[1] ) ) );
                        }
                    }
                    else if( texCoordElem0->getType() == IVertexElement::VertexElementType::VET_FLOAT2 )
                    {
                        texCoordElem0->getElementData( fbVertexDataPtr, &fbElementData );
                        *ogreVertexDataPtr++ = fbElementData[0];
                        *ogreVertexDataPtr++ = fbElementData[1];
                        *ogreVertexDataPtr++ = 0.0f;

                        if( bLogUVs )
                        {
                            FB_LOG(
                                StringUtil::toString( Vector2F( fbElementData[0], fbElementData[1] ) ) );
                        }
                    }
                }

                if( texCoordElem1 )
                {
                    if( texCoordElem1->getType() == IVertexElement::VertexElementType::VET_FLOAT3 )
                    {
                        texCoordElem1->getElementData( fbVertexDataPtr, &fbElementData );
                        *ogreVertexDataPtr++ = fbElementData[0];
                        *ogreVertexDataPtr++ = fbElementData[1];
                        *ogreVertexDataPtr++ = fbElementData[2];
                    }
                    else if( texCoordElem1->getType() == IVertexElement::VertexElementType::VET_FLOAT2 )
                    {
                        texCoordElem1->getElementData( fbVertexDataPtr, &fbElementData );
                        *ogreVertexDataPtr++ = fbElementData[0];
                        *ogreVertexDataPtr++ = fbElementData[1];
                        *ogreVertexDataPtr++ = 0.0f;
                    }
                }

                if( diffuseElem )
                {
                    diffuseElem->getElementData( fbVertexDataPtr, &fbElementData );
                    *ogreVertexDataPtr++ = fbElementData[0];
                    *ogreVertexDataPtr++ = fbElementData[1];
                    *ogreVertexDataPtr++ = fbElementData[2];
                    *ogreVertexDataPtr++ = fbElementData[3];
                }

                fbVertexDataPtr += fbVertexSize;
            }

            ogreVertexBuffer->unlock();

            auto indexData = ogreSubMesh->indexData[0];
            indexData->indexCount = fbIndexCount;
            FB_ASSERT_TRUE( indexData->indexCount == 0 );

            bool use32bit = fbVertexCount > std::numeric_limits<u16>::max();

            auto ogreIndexBuffer = defaultHardwareBufferManager->createIndexBuffer(
                use32bit ? Ogre::v1::HardwareIndexBuffer::IT_32BIT
                         : Ogre::v1::HardwareIndexBuffer::IT_16BIT,
                indexData->indexCount, Ogre::v1::HardwareBuffer::HBU_STATIC );
            indexData->indexBuffer = ogreIndexBuffer;

            u16 *pWords = use32bit ? nullptr
                                   : static_cast<u16 *>( ogreIndexBuffer->lock(
                                         Ogre::v1::HardwareBuffer::HBL_DISCARD ) );
            u32 *pDWords = use32bit ? static_cast<u32 *>( ogreIndexBuffer->lock(
                                          Ogre::v1::HardwareBuffer::HBL_DISCARD ) )
                                    : nullptr;

            if( fbIndexBuffer->getIndexType() == IIndexBuffer::Type::IT_32BIT )
            {
                const u32 *fbIndexData = reinterpret_cast<u32 *>( fbIndexBuffer->getIndexData() );

                // for (s32 i = (s32)fbIndexCount - 1; i >= 0; --i)
                for( u32 i = 0; i < fbIndexCount; ++i )
                {
                    auto index = fbIndexData[i];
                    if( use32bit )
                    {
                        *pDWords++ = static_cast<u32>( index );
                    }
                    else
                    {
                        *pWords++ = static_cast<u16>( index );
                    }
                }
            }
            else
            {
                const u16 *fbIndexData = reinterpret_cast<u16 *>( fbIndexBuffer->getIndexData() );

                // for (s32 i = (s32)fbIndexCount - 1; i >= 0; --i)
                for( u32 i = 0; i < fbIndexCount; ++i )
                {
                    auto index = fbIndexData[i];
                    if( use32bit )
                    {
                        *pDWords++ = static_cast<u32>( index );
                    }
                    else
                    {
                        *pWords++ = index;
                    }
                }
            }

            ogreIndexBuffer->unlock();
        }

        // Set bounds
        if( !subMeshList.empty() )
        {
            pMesh->_setBoundingSphereRadius( Ogre::Math::Sqrt( maxSquaredRadius ) );
            pMesh->_setBounds( Ogre::AxisAlignedBox( aabbMin, aabbMax ) );
        }
        else
        {
            pMesh->_setBoundingSphereRadius( 0.1f );
            pMesh->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3::UNIT_SCALE * -1.0f,
                                                     Ogre::Vector3::UNIT_SCALE * 1.0f ) );
        }

        /*Ogre::Mesh::LodValueList lodList;
        lodList.push_back(500.0f);
        lodList.push_back(750.0f);
        lodList.push_back(900.0f);
        lodList.push_back(1100.0f);
        pMesh->generateLodLevels(lodList,
        Ogre::ProgressiveMesh::VertexReductionQuota::VRQ_PROPORTIONAL, 0.2f);*/

        auto applicationManager = core::ApplicationManager::instance();
        if( applicationManager->isEditor() )
        {
            auto cacheFolder = applicationManager->getCachePath();

            if( !StringUtil::isNullOrEmpty( cacheFolder ) )
            {
                auto meshFileName = newMeshName + ".mesh";
                auto meshFilePath = Path::lexically_normal( cacheFolder, meshFileName );

                Ogre::v1::MeshSerializer serializer;
                serializer.exportMesh( pMesh.get(), meshFilePath,
                                       Ogre::v1::MeshVersion::MESH_VERSION_1_10 );
            }
        }

        // meshManager->unload(meshName);

        return pMesh;
    }

    void MeshLoader::importBuffersFromV1( Ogre::MeshPtr newMesh, Ogre::SubMesh *pSubMesh,
                                          SmartPtr<IMesh> mesh, SmartPtr<ISubMesh> subMesh, bool halfPos,
                                          bool halfTexCoords, bool qTangents, bool halfPose,
                                          size_t vaoPassIdx )
    {
        using namespace Ogre;

        /*
        VertexElement2Vec vertexElements;
        char *data = _arrangeEfficient( subMesh, halfPos, halfTexCoords, qTangents, &vertexElements,
                vaoPassIdx );

        //Wrap the ptrs around these, because the VaoManager's call
        //can throw thus causing a leak if we don't free them.
        FreeOnDestructor dataPtrContainer( data );

        VaoManager* vaoManager = newMesh->_getVaoManager();
        VertexBufferPackedVec vertexBuffers;

        //Create the vertex buffer
        bool keepAsShadow = newMesh->mVertexBufferShadowBuffer;
        VertexBufferPacked *vertexBuffer = vaoManager->createVertexBuffer( vertexElements,
                subMesh->vertexData[vaoPassIdx]->vertexCount,
                mParent->mVertexBufferDefaultType,
                data, keepAsShadow );
        vertexBuffers.push_back( vertexBuffer );

        if( keepAsShadow ) //Don't free the pointer ourselves
            dataPtrContainer.ptr = 0;

        IndexBufferPacked *indexBuffer = importFromV1( subMesh->indexData[vaoPassIdx] );

        {
            VertexArrayObject *vao = vaoManager->createVertexArrayObject( vertexBuffers, indexBuffer,
                    subMesh->operationType );
            mVao[vaoPassIdx].push_back( vao );
        }

        //Now deal with the automatic LODs
        v1::SubMesh::LODFaceList::const_iterator itor = subMesh->mLodFaceList[vaoPassIdx].begin();
        v1::SubMesh::LODFaceList::const_iterator end  = subMesh->mLodFaceList[vaoPassIdx].end();

        while( itor != end )
        {
            IndexBufferPacked *lodIndexBuffer = importFromV1( *itor );

            VertexArrayObject *vao = vaoManager->createVertexArrayObject( vertexBuffers,
        lodIndexBuffer, subMesh->operationType );

            mVao[vaoPassIdx].push_back( vao );
            ++itor;
        }

        importPosesFromV1( subMesh, vertexBuffer, halfPose );
         */
    }

    auto MeshLoader::importFromV1( SmartPtr<IIndexBuffer> indexData ) -> Ogre::IndexBufferPacked *
    {
        /*
        if( !indexData || indexData->indexBuffer.isNull() )
            return 0;

        //Create & copy the index buffer
        const size_t indexSize = indexData->indexBuffer->getIndexSize();
        bool keepAsShadow = mParent->mIndexBufferShadowBuffer;
        VaoManager *vaoManager = mParent->mVaoManager;
        void *indexDataPtr = OGRE_MALLOC_SIMD( indexData->indexCount * indexSize,
                MEMCATEGORY_GEOMETRY );
        FreeOnDestructor indexDataPtrContainer( indexDataPtr );
        IndexBufferPacked::IndexType indexType = static_cast<IndexBufferPacked::IndexType>(
                indexData->indexBuffer->getType() );

        const uint8 *srcIndexDataPtr = reinterpret_cast<uint8*>(
                indexData->indexBuffer->lock( v1::HardwareBuffer::HBL_READ_ONLY ) );

        memcpy( indexDataPtr, srcIndexDataPtr + indexData->indexStart * indexSize,
                indexSize * indexData->indexCount );
        indexData->indexBuffer->unlock();

        IndexBufferPacked *indexBuffer = vaoManager->createIndexBuffer( indexType,
        indexData->indexCount, mParent->mIndexBufferDefaultType, indexDataPtr, keepAsShadow );

        if( keepAsShadow ) //Don't free the pointer ourselves
            indexDataPtrContainer.ptr = 0;

        return indexBuffer;
         */

        return nullptr;
    }

    void MeshLoader::importPosesFromV1( SmartPtr<ISubMesh> subMesh,
                                        Ogre::VertexBufferPacked *vertexBuffer, bool halfPrecision )
    {
        /*
        // Find the index of this subMesh and only process poses which have this
        // subMesh as their target.
        v1::Mesh::SubMeshList::const_iterator subMeshBegin
        =subMesh->parent->getSubMeshIterator().begin(); v1::Mesh::SubMeshList::const_iterator
        subMeshEnd = subMesh->parent->getSubMeshIterator().end();
        v1::Mesh::SubMeshList::const_iterator subMeshIt = std::find( subMeshBegin, subMeshEnd,
        subMesh );

        assert( subMeshIt != subMeshEnd && "Parent mesh does not contain this subMesh.");

        const size_t subMeshIndex = static_cast<size_t>( subMeshIt - subMeshBegin );

        const v1::PoseList &poseListOrig = subMesh->parent->getPoseList();
        v1::PoseList poseList;
        poseList.reserve( poseListOrig.size() );
        {
            v1::PoseList::const_iterator itor = poseListOrig.begin();
            v1::PoseList::const_iterator end  = poseListOrig.end();

            while( itor != end )
            {
                if( (*itor)->getTarget() == subMeshIndex )
                    poseList.push_back( *itor );
                ++itor;
            }
        }

        mNumPoses = static_cast<uint16>( poseList.size() );
        mPoseHalfPrecision = halfPrecision;

        if( mNumPoses > 0 )
        {
            mPoseNormals = poseList[0]->getIncludesNormals();
            size_t numVertices = vertexBuffer->getNumElements();
            size_t elementSize = halfPrecision ? sizeof( uint16 ) : sizeof( float );
            size_t elementsPerVertex = mPoseNormals ? 8 : 4;
            size_t singlePoseBufferSize = numVertices * elementSize * elementsPerVertex;
            size_t bufferSize = mNumPoses * singlePoseBufferSize;
            char *buffer = static_cast<char*>( OGRE_MALLOC_SIMD( bufferSize,
                    MEMCATEGORY_GEOMETRY ) );
            FreeOnDestructor bufferPtrContainer( buffer );
            memset( buffer, 0, bufferSize );

            v1::Mesh::PoseIterator poseIt = subMesh->parent->getPoseIterator();

            size_t index = 0u;

            while( poseIt.hasMoreElements() )
            {
                v1::Pose* pose = poseIt.getNext();
                v1::Pose::VertexOffsetMap::const_iterator v = pose->getVertexOffsets().begin();
                v1::Pose::NormalsIterator::const_iterator n = pose->getNormalsIterator().begin();

                if( halfPrecision )
                {
                    uint16* pHalf = reinterpret_cast<uint16*>( buffer +  index * singlePoseBufferSize
        ); while( v != pose->getVertexOffsets().end() )
                    {
                        size_t idx = v->first * elementsPerVertex;
                        pHalf[idx+0] = Bitwise::floatToHalf( v->second.x );
                        pHalf[idx+1] = Bitwise::floatToHalf( v->second.y );
                        pHalf[idx+2] = Bitwise::floatToHalf( v->second.z );
                        pHalf[idx+3] = Bitwise::floatToHalf( 0.f );
                        ++v;

                        if( mPoseNormals )
                        {
                            pHalf[idx+4] = Bitwise::floatToHalf( n->second.x );
                            pHalf[idx+5] = Bitwise::floatToHalf( n->second.y );
                            pHalf[idx+6] = Bitwise::floatToHalf( n->second.z );
                            pHalf[idx+7] = Bitwise::floatToHalf( 0.f );
                            ++n;
                        }
                    }
                }
                else
                {
                    float* pFloat = reinterpret_cast<float*>( buffer + index * singlePoseBufferSize
        ); while( v != pose->getVertexOffsets().end() )
                    {
                        size_t idx = v->first * elementsPerVertex;
                        pFloat[idx+0] = v->second.x;
                        pFloat[idx+1] = v->second.y;
                        pFloat[idx+2] = v->second.z;
                        pFloat[idx+3] = 0.f;
                        ++v;

                        if( mPoseNormals )
                        {
                            pFloat[idx+4] = n->second.x;
                            pFloat[idx+5] = n->second.y;
                            pFloat[idx+6] = n->second.z;
                            pFloat[idx+7] = 0.f;
                            ++n;
                        }
                    }
                }

                mPoseIndexMap[pose->getName()] = index++;
            }

            PixelFormatGpu pixelFormat = halfPrecision ? PFG_RGBA16_FLOAT : PFG_RGBA32_FLOAT;
            mPoseTexBuffer = mParent->mVaoManager->createTexBuffer( pixelFormat, bufferSize,
                    BT_IMMUTABLE, buffer, false );
        }
         */
    }

    void MeshLoader::importFromV1( Ogre::MeshPtr newMesh, Ogre::SubMesh *newSubMesh,
                                   SmartPtr<IMesh> mesh, SmartPtr<ISubMesh> subMesh, bool halfPos,
                                   bool halfTexCoords, bool qTangents, bool halfPose )
    {
        auto mMaterialName = subMesh->getMaterialName();

        if( mesh->hasSkeleton() )
        {
            // subMesh->_compileBoneAssignments();
        }

        /*
        const v1::SubMesh::VertexBoneAssignmentList& v1BoneAssignments =
        subMesh->getBoneAssignments(); mBoneAssignments.reserve(v1BoneAssignments.size());

        {
            v1::SubMesh::VertexBoneAssignmentList::const_iterator itor = v1BoneAssignments.begin();
            v1::SubMesh::VertexBoneAssignmentList::const_iterator end = v1BoneAssignments.end();

            while (itor != end)
            {
                mBoneAssignments.push_back(VertexBoneAssignment(itor->second));
                ++itor;
            }
        }
        */

        /*
        std::sort(mBoneAssignments.begin(), mBoneAssignments.end());
        mBlendIndexToBoneIndexMap = subMesh->blendIndexToBoneIndexMap;
        mBoneAssignmentsOutOfDate = false;
        */

        importBuffersFromV1( newMesh, newSubMesh, mesh, subMesh, halfPos, halfTexCoords, qTangents,
                             halfPose, 0 );

        // assert(subMesh->parent->hasValidShadowMappingBuffers());

        /*
        //Deal with shadow mapping optimized buffers
        if (subMesh->vertexData[VpNormal] != subMesh->vertexData[VpShadow] ||
            subMesh->indexData[VpNormal] != subMesh->indexData[VpShadow])
        {
            //Use the special version already built for v1
            importBuffersFromV1(subMesh, halfPos, halfTexCoords, qTangents, halfPose, 1);
        }
        else
        {
            //No special version in the v1 format, let the autogeneration routine decide.
            this->_prepareForShadowMapping(false);
        }
        */
    }

    void MeshLoader::importV1( IMesh *mesh, bool halfPos, bool halfTexCoords, bool qTangents,
                               bool halfPose )
    {
        using namespace Ogre;

        auto meshPtr = Ogre::MeshManager::getSingleton().createManual(
            "Barrel Imported", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

        FB_LOG( "Mesh2::importV1" );

        mesh->load( nullptr );

        auto loadingState = meshPtr->getLoadingState();

        if( loadingState != Ogre::Resource::LoadingState::LOADSTATE_UNLOADED &&
            loadingState != Ogre::Resource::LoadingState::LOADSTATE_LOADING )
        {
            OGRE_EXCEPT( Ogre::Exception::ERR_INVALID_STATE,
                         "To import a v1 mesh, the v2 mesh must be in unloaded state!",
                         "Mesh::importV1" );
        }

        if( mesh->getHasSharedVertexData() )
        {
            FB_LOG( "WARNING: Mesh '" + mesh->getName() +
                    "' has shared vertices. They're being "
                    "'unshared' for importing to v2" );
            MeshUtil::unshareVertices( mesh );
        }

        auto aabb = mesh->getAABB();
        auto minimum = aabb.getMinimum();
        auto maximum = aabb.getMaximum();

        auto ogreAabb = Ogre::AxisAlignedBox( Ogre::Vector3( minimum.X(), minimum.Y(), minimum.Z() ),
                                              Ogre::Vector3( maximum.X(), maximum.Y(), maximum.Z() ) );
        // auto ogreBoundingRadius = ogreAabb.getHalfSize().length();

        /*
        try
        {
            if( qTangents )
            {
                unsigned short sourceCoordSet;
                unsigned short index;
                bool alreadyHasTangents = mesh->suggestTangentVectorBuildParams( VES_TANGENT,
                        sourceCoordSet,
                        index );
                if( !alreadyHasTangents )
                    mesh->buildTangentVectors( VES_TANGENT, sourceCoordSet, index, false, false, true
        );
            }
        }
        catch( Exception & )
        {
        }*/

        auto subMeshes = mesh->getSubMeshes();
        for( auto pSubMesh : subMeshes )
        {
            auto subMesh = meshPtr->createSubMesh();
            importFromV1( meshPtr, subMesh, mesh, pSubMesh, halfPos, halfTexCoords, qTangents,
                          halfPose );
        }
        /*
                    mSubMeshNameMap = mesh->getSubMeshNameMap();

                    mSkeletonName = mesh->getSkeletonName();
                    v1::SkeletonPtr v1Skeleton = mesh->getOldSkeleton();
                    if( !v1Skeleton.isNull() )
                        mSkeleton = SkeletonManager::getSingleton().getSkeletonDef( v1Skeleton.get()
           );

                    //So far we only import manual LOD levels. If the mesh had manual LOD levels,
                    //mLodValues will have more entries than Vaos, causing an out of bounds
           exception.
                    //Don't use LOD if the imported mesh had manual levels.
                    //Note: Mesh2 supports LOD levels that have their own vertex and index buffers,
                    //so it should be possible to import them as well.
                    if( !mesh->hasManualLodLevel() )
                        mLodValues = *mesh->_getLodValueArray();
                    else
                        mLodValues = MovableObject::c_DefaultLodMesh;
        */

        meshPtr->setManuallyLoaded( true );
        meshPtr->setToLoaded();
    }

    void MeshLoader::loadMesh( const String &meshName )
    {
        /*
        String fileExt = Path::getFileExtension(meshName);
        if(fileExt==(".mesh"))
        {
            auto engine = core::ApplicationManager::instance();
            SmartPtr<IFileSystem> fileSystem = engine->getFileSystem();

            //String fileName = Path::getFileName(meshName);
            //SmartPtr<IStream> stream = fileSystem->open(fileName);
            //if(stream)
            //{
            //	MeshGeometryPtr meshResource = m_meshMgr->create(fileName,
        IResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

            //	SmartPtr<IMesh> mesh(new CMesh);
            //	mesh->setName(meshName);
            //	meshResource->setMesh(mesh);

            //	MeshSerializer meshSerializer;
            //	meshSerializer.importMesh(stream, (CMesh*)mesh.get());

            //	mesh->updateAABB(true);
            //	meshResource->setAABB(mesh->getAABB());
            //}
        }
        else
        {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile( meshName.c_str(),
        aiProcessPreset_TargetRealtime_Quality | aiProcess_TransformUVCoords | aiProcess_FlipUVs);

            String path = Path::getFilePath(meshName);
            loadDataFromNode(scene, scene->mRootNode, meshName.c_str());
        }
        */
    }

    auto MeshLoader::loadEngineMesh( const String &meshName ) -> SmartPtr<IMesh>
    {
        /*
        String fileExt = Path::getFileExtension(meshName);
        if(fileExt==(".mesh"))
        {
            auto engine = core::ApplicationManager::instance();
            SmartPtr<IFileSystem> fileSystem = engine->getFileSystem();

            String fileName = Path::getFileName(meshName);
            SmartPtr<IStream> stream = fileSystem->open(fileName);
            if(stream)
            {

                SmartPtr<IMesh> mesh(new CMesh);

                MeshSerializer meshSerializer;
                meshSerializer.importMesh(stream, (CMesh*)mesh.get());

                return mesh;
            }
        }
        */

        return nullptr;
    }

    auto MeshLoader::load( const String &meshName, SmartPtr<ISceneNode> fbParent,
                           SmartPtr<render::IGraphicsScene> smgr ) -> SmartPtr<IMesh>
    {
        /*
        String fileExt = Path::getFileExtension(meshName);
        if(fileExt==(".mesh"))
        {
            auto engine = core::ApplicationManager::instance();
            SmartPtr<IFileSystem> fileSystem = engine->getFileSystem();

            String fileName = Path::getFileName(meshName);
            String path = Path::getFilePath(meshName);

            //SmartPtr<IStream> stream = fileSystem->open(fileName, path);
            //if(stream)
            //{
            //	MeshGeometryPtr meshResource = m_meshMgr->create(fileName,
        IResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

            //	SmartPtr<IMesh> mesh(new CMesh);
            //	meshResource->setMesh(mesh);

            //	MeshSerializer meshSerializer;
            //	meshSerializer.importMesh(stream, (CMesh*)mesh.get());
            //}
        }
        else
        {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile( meshName.c_str(),
        aiProcessPreset_TargetRealtime_Quality | aiProcess_TransformUVCoords | aiProcess_FlipUVs);

            fb::String path = Path::getFilePath(meshName);
            loadDataFromNode(scene, scene->mRootNode, meshName.c_str());

            if(scene->mRootNode)
            {
                fb::SmartPtr<ISceneNode> fbNode = fbParent->addChildSceneNode();
                loadNode(scene->mRootNode, fbNode, smgr);
            }
        }
        */

        return nullptr;
    }

#if FB_USE_ASSET_IMPORT

    auto MeshLoader::loadNode( aiNode *node, SmartPtr<ISceneNode> fbNode,
                               SmartPtr<render::IGraphicsScene> smgr ) -> SmartPtr<ISceneNode>
    {
        /*
        Ogre::SceneNode* ogreNode;
        fbNode->_getObject((void**)&ogreNode);
        Ogre::SceneManager* ogreSmgr = ogreNode->getCreator();
        fb::SmartPtr<CSceneManagerOgre> fbSceneManger = smgr;


        for(int i=0; i<node->mNumChildren; ++i)
        {
        aiNode* child = node->mChildren[i];

        fb::SmartPtr<ISceneNode> fbChild = fbNode->addChildSceneNode();
        fb::String name = child->mName.C_Str();

        //set transform
        const aiMatrix4x4& trans = child->mTransformation;
        fbChild->setPosition(fb::Vector3F(trans.a4, trans.c4, trans.b4));

        // meshes
        for(int meshIdx=0; meshIdx<child->mNumMeshes; ++meshIdx)
        {
        unsigned int meshIndex = child->mMeshes[meshIdx];
        MeshVector::iterator it = mMeshes.begin();
        std::advance(it, meshIndex);

        Ogre::Entity* ogreEntity;// = ogreSmgr->createEntity((*it));

        fb::CMeshObjectPtr graphicsObject(new fb::CMeshObject(fbSceneManger.get()), true);
        graphicsObject->initialise(ogreEntity);
        graphicsObject->setName(ogreEntity->getName().c_str());
        fbSceneManger->addExistingGraphicsObject(graphicsObject);
        fbChild->attachObject(graphicsObject);
        }


        loadNode(child, fbChild, smgr);
        }
        */

        return nullptr;
    }

#endif

#if FB_USE_ASSET_IMPORT

    void MeshLoader::loadDataFromNode( const aiScene *mScene, const aiNode *pNode, const String &mDir )
    {
        // auto engine = core::ApplicationManager::instance();
        // SmartPtr<IGraphicsSystem> graphicsSystem = engine->getGraphicsSystem();
        // SmartPtr<IMeshManager> meshMgr = graphicsSystem->getMeshManager();

        if( pNode->mNumMeshes > 0 )
        {
            // MeshGeometryPtr mesh;

            if( getUseSingleMesh() )
            {
                // if(mMeshes.size() == 0)
                //{
                //	static int nameExt = 0;
                //	mesh = m_meshMgr->create(String("ROOTMesh") + StringUtil::toString(nameExt++),
                //IResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

                //	SmartPtr<IMesh> pMesh(new CMesh);
                //	mesh->setMesh(pMesh);

                //	mMeshes.push_back(mesh);
                //}
                // else
                //{
                //	mesh = mMeshes[0];
                //}
            }

            for( u32 idx = 0; idx < pNode->mNumMeshes; ++idx )
            {
                // aiMesh* pAIMesh = mScene->mMeshes[pNode->mMeshes[idx]];
                if( !mQuietMode )
                {
                    FB_LOG_MESSAGE( "MeshLoader", String( "SubMesh " ) + StringUtil::toString( idx ) +
                                                      String( " for mesh '" ) +
                                                      String( pNode->mName.data ) + "'" );
                }

                // Create a material instance for the mesh.
                // const aiMaterial* pAIMaterial = mScene->mMaterials[pAIMesh->mMaterialIndex];
                // createSubMesh(pNode->mName.data, idx, pNode, pAIMesh, pAIMaterial,
                // mesh->getMesh(), mDir);
            }
        }

        // Traverse all child nodes of the current node instance
        for( u32 childIdx = 0; childIdx < pNode->mNumChildren; childIdx++ )
        {
            const aiNode *pChildNode = pNode->mChildren[childIdx];
            loadDataFromNode( mScene, pChildNode, mDir );
        }
    }

#endif

#if FB_USE_ASSET_IMPORT

    auto MeshLoader::createSubMesh( const String &name, int index, const aiNode *pNode,
                                    const aiMesh *mesh, const aiMaterial *mat, SmartPtr<IMesh> mMesh,
                                    const String &mDir ) -> bool
    {
        // if animated all submeshes must have bone weights
        if( mBonesByName.size() && !mesh->HasBones() )
        {
            if( !mQuietMode )
            {
                FB_LOG_MESSAGE( "MeshLoader", String( "Skipping Mesh " ) + String( mesh->mName.data ) +
                                                  String( "with no bone weights" ) );
            }
            return false;
        }

        SmartPtr<IMaterial> matptr;

        // if((mLoaderParams & LP_GENERATE_MATERIALS_AS_CODE) == 0)
        //{
        //     matptr = createMaterial(mesh->mMaterialIndex, mat, mDir);
        // }
        // else
        //{
        //     matptr = createMaterialByScript(mesh->mMaterialIndex, mat);
        // }

        // now begin the object definition
        // We create a submesh per material
        SmartPtr<ISubMesh> submesh( new SubMesh );
        mMesh->addSubMesh( submesh );

        // prime pointers to vertex related data
        aiVector3D *vec = mesh->mVertices;
        aiVector3D *norm = mesh->mNormals;
        aiVector3D *uv = mesh->mTextureCoords[0];
        // aiColor4D *col = mesh->mColors[0];

        // We must create the vertex data, indicating how many vertices there will be
        // submesh->useSharedVertices = false;

        SmartPtr<IVertexBuffer> vertexData( new VertexBuffer );
        submesh->setVertexBuffer( vertexData );
        // submesh->vertexData->vertexStart = 0;
        // submesh->vertexData->vertexCount = mesh->mNumVertices;
        //
        vertexData->setNumVertices( mesh->mNumVertices );

        // We must now declare what the vertex data contains
        boost::shared_ptr<IVertexDeclaration> declaration( new VertexDeclaration );
        // vertexData->setVertexDeclaration(declaration);

        static const unsigned short source = 0;
        u32 offset = 0;
        offset += declaration
                      ->addElement( 0, sizeof( Vector3F ),
                                    VertexDeclaration::VertexElementSemantic::VES_POSITION,
                                    IVertexElement::VertexElementType::VET_FLOAT3 )
                      ->getOffset();

        // mLog->logMessage((boost::format(" %d vertices ") % m->mNumVertices).str());
        if( !mQuietMode )
        {
            FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumVertices ) + " vertices" );
        }
        if( norm )
        {
            if( !mQuietMode )
            {
                FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumVertices ) + " normals" );
            }
            // mLog->logMessage((boost::format(" %d normals ") % m->mNumVertices).str() );
            offset += declaration
                          ->addElement( 0, sizeof( Vector3F ),
                                        VertexDeclaration::VertexElementSemantic::VES_NORMAL,
                                        IVertexElement::VertexElementType::VET_FLOAT3 )
                          ->getOffset();
        }

        if( uv )
        {
            if( !mQuietMode )
            {
                FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumVertices ) + " uvs" );
            }
            // mLog->logMessage((boost::format(" %d uvs ") % m->mNumVertices).str() );
            offset +=
                declaration
                    ->addElement( 0, sizeof( Vector2F ),
                                  VertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
                                  IVertexElement::VertexElementType::VET_FLOAT2, 0 )
                    ->getOffset();
        }

        /*
        if (col)
        {
        Ogre::LogManager::getSingleton().logMessage(Ogre::StringConverter::toString(mesh->mNumVertices)
        + " colours");
        //mLog->logMessage((boost::format(" %d colours ") % m->mNumVertices).str() );
        offset +=
        declaration->addElement(source,offset,Ogre::VET_FLOAT3,Ogre::VES_DIFFUSE).getSize();
        }
        */

        // We create the hardware vertex buffer
        // Ogre::HardwareVertexBufferSharedPtr vbuffer =
        //    Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(declaration->getVertexSize(source),
        //    // == offset submesh->vertexData->vertexCount,   // == nbVertices
        //    Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        aiMatrix4x4 aiM = mNodeDerivedTransformByName.find( pNode->mName.data )->second;

        // Now we get access to the buffer to fill it.  During so we record the bounding box.
        //  float* vdata = static_cast<float*>(vbuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
        auto *vdata = static_cast<float *>( vertexData->createVertexData() );
        for( u32 i = 0; i < mesh->mNumVertices; ++i )
        {
            // Position
            aiVector3D vect;
            vect.x = vec->x;
            vect.y = vec->y;
            vect.z = vec->z;

            // vect *= aiM;

            /*
            if(NULL != mSkeletonRootNode)
            {
            vect *= mSkeletonRootNode->mTransformation;
            }
            */

            Vector3F position( vect.x, vect.z, vect.y );
            *vdata++ = vect.x;
            *vdata++ = vect.z;
            *vdata++ = -vect.y;
            vec++;

            // Normal
            if( norm )
            {
                vect.x = norm->x;
                vect.y = norm->y;
                vect.z = norm->z;

                // vect *= aiM;

                *vdata++ = vect.x;
                *vdata++ = vect.z;
                *vdata++ = -vect.y;
                norm++;

                //*vdata++ = norm->x;
                //*vdata++ = norm->y;
                //*vdata++ = norm->z;
                // norm++;
            }

            // uvs
            if( uv )
            {
                *vdata++ = uv->x;
                *vdata++ = uv->y;
                uv++;
            }

            /*
            if (col)
            {
            *vdata++ = col->r;
            *vdata++ = col->g;
            *vdata++ = col->b;
            //*vdata++ = col->a;
            //col++;
            }
            */
        }

        // submesh->vertexData->vertexBufferBinding->setBinding(source,vbuffer);

        if( !mQuietMode )
        {
            FB_LOG_MESSAGE( "MeshLoader", StringUtil::toString( mesh->mNumFaces ) + " faces" );
        }

        aiFace *f = mesh->mFaces;

        // Creates the index data
        SmartPtr<IIndexBuffer> indexBuffer( new IndexBuffer );
        submesh->setIndexBuffer( indexBuffer );
        // submesh->indexData->indexStart = 0;
        // submesh->indexData->indexCount = mesh->mNumFaces * 3;
        // submesh->indexData->indexBuffer =
        //     Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT,
        //     submesh->indexData->indexCount,
        //     Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
        // Ogre::uint16* idata =
        // static_cast<Ogre::uint16*>(submesh->indexData->indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));

        indexBuffer->setIndexType( IndexBuffer::Type::IT_16BIT );
        indexBuffer->setNumIndices( mesh->mNumFaces * 3 );
        u16 *idata = static_cast<u16 *>( indexBuffer->createIndexData() );

        // poke in the face data
        for( u32 i = 0; i < mesh->mNumFaces; ++i )
        {
            //		wxASSERT(f->mNumIndices == 3);
            *idata++ = static_cast<u16>( f->mIndices[0] );
            *idata++ = static_cast<u16>( f->mIndices[1] );
            *idata++ = static_cast<u16>( f->mIndices[2] );
            f++;
        }
        // submesh->indexData->indexBuffer->unlock();

        // set bone weigths
        // if(mesh->HasBones())
        //{
        //    for ( Ogre::uint32 i=0; i < mesh->mNumBones; i++ )
        //    {
        //        aiBone *pAIBone = mesh->mBones[ i ];
        //        if ( NULL != pAIBone )
        //        {
        //            Ogre::String bname = pAIBone->mName.data;
        //            for ( Ogre::uint32 weightIdx = 0; weightIdx < pAIBone->mNumWeights; weightIdx++
        //            )
        //            {
        //                aiVertexWeight aiWeight = pAIBone->mWeights[ weightIdx ];

        //                Ogre::VertexBoneAssignment vba;
        //                vba.vertexIndex = aiWeight.mVertexId;
        //                vba.boneIndex = mSkeleton->getBone(bname)->getHandle();
        //                vba.weight= aiWeight.mWeight;

        //                submesh->addBoneAssignment(vba);
        //            }
        //        }
        //    }
        //} // if mesh has bones

        //// Finally we set a material to the submesh
        // submesh->setMaterialName(matptr->getName());

        return true;
    }

#endif

    auto MeshLoader::getUseSingleMesh() const -> bool
    {
        return m_useSingleMesh;
    }

    void MeshLoader::setUseSingleMesh( bool val )
    {
        m_useSingleMesh = val;
    }

    auto MeshLoader::getQuietMode() const -> bool
    {
        return mQuietMode;
    }

    void MeshLoader::setQuietMode( bool val )
    {
        mQuietMode = val;
    }

    auto MeshLoader::getMeshMgr() const -> SmartPtr<IMeshManager>
    {
        return m_meshMgr;
    }

    void MeshLoader::setMeshMgr( SmartPtr<IMeshManager> val )
    {
        m_meshMgr = val;
    }

}  // namespace fb::render
