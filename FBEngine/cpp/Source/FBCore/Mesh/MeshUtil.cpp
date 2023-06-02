#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/MeshUtil.h>
#include <FBCore/Mesh/CVertexElement.h>
#include <FBCore/Mesh/Vertex.h>
#include <FBCore/Mesh/CMesh.h>
#include <FBCore/Mesh/CSubMesh.h>
#include <FBCore/Mesh/CVertexDeclaration.h>
#include <FBCore/Mesh/CVertexBuffer.h>
#include <FBCore/Mesh/CIndexBuffer.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Core/Map.h>
#include <FBCore/Math/Math.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    SmartPtr<IMesh> MeshUtil::clean( SmartPtr<IMesh> mesh, real_Num weldTolerance )
    {
        FB_ASSERT( mesh );
        FB_ASSERT( mesh->isValid() );
        FB_ASSERT( weldTolerance >= static_cast<real_Num>( 0.0 ) );
        FB_ASSERT( Math<real_Num>::isFinite( weldTolerance ) );

        auto cleanMesh = fb::make_ptr<CMesh>();

        auto subMeshes = mesh->getSubMeshes();
        for(auto subMesh : subMeshes)
        {
            auto cleanSubMesh = clean( subMesh, weldTolerance );
            FB_ASSERT( cleanSubMesh );
            FB_ASSERT( cleanSubMesh->isValid() );

            cleanMesh->addSubMesh( cleanSubMesh );
        }

        return cleanMesh;
    }

    SmartPtr<ISubMesh> MeshUtil::clean( SmartPtr<ISubMesh> submesh, real_Num weldTolerance )
    {
        FB_ASSERT( submesh );
        FB_ASSERT( submesh->isValid() );

        auto cleanSubMesh = fb::make_ptr<CSubMesh>();

        auto vertexBuffer = submesh->getVertexBuffer();
        auto indexBuffer = submesh->getIndexBuffer();

        cleanSubMesh->setVertexBuffer( vertexBuffer->clone() );
        cleanSubMesh->setIndexBuffer( indexBuffer->clone() );

        auto vertexCount = vertexBuffer->getNumVerticies();
        auto indexCount = indexBuffer->getNumIndices();

        auto fbVertexDeclaration = vertexBuffer->getVertexDeclaration();
        const auto posElem = fbVertexDeclaration->findElementBySemantic(
            IVertexDeclaration::VertexElementSemantic::VES_POSITION );

        u32 fbVertexSize = fbVertexDeclaration->getSize();
        auto fbVertexDataPtr = static_cast<u8 *>(vertexBuffer->getVertexData());

        f32 *fbElementData = nullptr;
        f32 *fbElementData2 = nullptr;

        Array<u32> cleanVertices;
        cleanVertices.reserve( vertexCount );

        Array<std::pair<u32, u32>> weldedVertices;
        weldedVertices.reserve( vertexCount );

        for(u32 x = 0; x < vertexCount; ++x, fbVertexDataPtr += fbVertexSize)
        {
            auto wasWelded = false;
            for(auto p : weldedVertices)
            {
                if(p.first == x)
                {
                    wasWelded = true;
                }
            }

            if(wasWelded)
            {
                continue;
            }

            posElem->getElementData( fbVertexDataPtr, &fbElementData );
            auto vertexPosition1 = Vector3F( fbElementData[0], fbElementData[1], fbElementData[2] );

            auto welded = false;
            auto fbVertexDataPtr2 = static_cast<u8 *>(vertexBuffer->getVertexData());

            for(u32 y = 0; y < vertexCount; ++y, fbVertexDataPtr2 += fbVertexSize)
            {
                if(x == y)
                {
                    continue;
                }

                posElem->getElementData( fbVertexDataPtr2, &fbElementData2 );
                auto vertexPosition2 =
                    Vector3F( fbElementData2[0], fbElementData2[1], fbElementData2[2] );

                auto diff = ( vertexPosition2 - vertexPosition1 ).length();
                if(diff < weldTolerance)
                {
                    auto weldedPair = std::make_pair( y, x );
                    weldedVertices.push_back( weldedPair );
                    welded = true;
                }
            }

            cleanVertices.push_back( x );
        }

        auto cleanVertexBuffer = cleanSubMesh->getVertexBuffer();
        auto cleanIndexBuffer = cleanSubMesh->getIndexBuffer();

        auto numCleanVertices = cleanVertices.size();
        cleanVertexBuffer->setNumVerticies( static_cast<u32>(numCleanVertices) );

        auto cleanVertexDataPtr = static_cast<f32 *>(vertexBuffer->getVertexData());

        fbVertexDataPtr = static_cast<u8 *>(vertexBuffer->getVertexData());
        for(u32 x = 0; x < vertexCount; ++x, fbVertexDataPtr += fbVertexSize)
        {
            if(std::find( cleanVertices.begin(), cleanVertices.end(), x ) != cleanVertices.end())
            {
                posElem->getElementData( fbVertexDataPtr, &fbElementData );
                auto vertexPosition1 = Vector3F( fbElementData[0], fbElementData[1], fbElementData[2] );

                cleanVertexDataPtr[0] = fbElementData[0];
                cleanVertexDataPtr[1] = fbElementData[1];
                cleanVertexDataPtr[2] = fbElementData[2];

                cleanVertexDataPtr += fbVertexSize;
            }
        }

        Array<u32> cleanIndices;
        cleanIndices.reserve( indexCount );

        // bool useWords = (vertexCount >= std::numeric_limits<u16>::max()) ? false : true;

        if(indexBuffer->getIndexType() == IIndexBuffer::Type::IT_32BIT)
        {
            const u32 *fbIndexData = reinterpret_cast<u32 *>(indexBuffer->getIndexData());

            for(u32 i = 0; i < indexCount; ++i)
            {
                auto index = fbIndexData[i];

                auto wasWelded = false;
                for(auto p : weldedVertices)
                {
                    if(p.first == index)
                    {
                        index = p.second;
                        wasWelded = true;
                    }
                }

                if(wasWelded)
                {
                    continue;
                }

                cleanIndices.push_back( index );
            }

            auto numCleanIndices = cleanIndices.size();
            cleanIndexBuffer->setNumIndices( static_cast<u32>(numCleanIndices) );

            auto cleanIndexData = reinterpret_cast<u32 *>(cleanIndexBuffer->getIndexData());
            for(u32 i = 0; i < numCleanIndices; ++i)
            {
                auto index = cleanIndices[i];
                *cleanIndexData++ = index;
            }
        }
        else
        {
            const u16 *fbIndexData = reinterpret_cast<u16 *>(indexBuffer->getIndexData());

            for(u32 i = 0; i < indexCount; ++i)
            {
                auto index = fbIndexData[i];

                auto wasWelded = false;
                for(auto p : weldedVertices)
                {
                    if(p.first == index)
                    {
                        index = static_cast<u16>(p.second);
                        wasWelded = true;
                    }
                }

                if(wasWelded)
                {
                    continue;
                }

                cleanIndices.push_back( index );
            }

            auto numCleanIndices = cleanIndices.size();
            cleanIndexBuffer->setNumIndices( static_cast<u32>(numCleanIndices) );

            auto cleanIndexData = reinterpret_cast<u16 *>(cleanIndexBuffer->getIndexData());
            for(u32 i = 0; i < numCleanIndices; ++i)
            {
                auto index = cleanIndices[i];
                *cleanIndexData++ = static_cast<u16>(index);
            }
        }

        FB_ASSERT( cleanSubMesh );
        FB_ASSERT( cleanSubMesh->isValid() );
        return cleanSubMesh;
    }

    Array<Vector3F> MeshUtil::getPoints( SmartPtr<IMesh> mesh )
    {
        return Array<Vector3F>();
    }

    Array<Vector3F> MeshUtil::getPoints( SmartPtr<ISubMesh> submesh )
    {
        auto points = Array<Vector3F>();

        auto fbVertexBuffer = submesh->getVertexBuffer();
        auto fbIndexBuffer = submesh->getIndexBuffer();

        auto fbVertexCount = fbVertexBuffer->getNumVerticies();
        // auto fbIndexCount = fbIndexBuffer->getNumIndices();

        points.reserve( fbVertexCount );

        auto fbVertexDeclaration = fbVertexBuffer->getVertexDeclaration();
        const auto posElem = fbVertexDeclaration->findElementBySemantic(
            IVertexDeclaration::VertexElementSemantic::VES_POSITION );
        const auto normalElem = fbVertexDeclaration->findElementBySemantic(
            IVertexDeclaration::VertexElementSemantic::VES_NORMAL );
        const auto texCoordElem0 = fbVertexDeclaration->findElementBySemantic(
            IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 0 );
        const auto texCoordElem1 = fbVertexDeclaration->findElementBySemantic(
            IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 1 );

        u32 fbVertexSize = fbVertexDeclaration->getSize();
        auto fbVertexDataPtr = static_cast<u8 *>(fbVertexBuffer->getVertexData());
        f32 *fbElementData = nullptr;

        for(u32 vertIdx = 0; vertIdx < fbVertexCount; ++vertIdx, fbVertexDataPtr += fbVertexSize)
        {
            posElem->getElementData( fbVertexDataPtr, &fbElementData );

            auto vertexPosition = Vector3F( fbElementData[0], fbElementData[1], fbElementData[2] );
            points.push_back( vertexPosition );
        }

        return points;
    }

    Array<u32> MeshUtil::getIndices( SmartPtr<IMesh> mesh )
    {
        return Array<u32>();
    }

    Array<u32> MeshUtil::getIndices( SmartPtr<ISubMesh> submesh )
    {
        FB_ASSERT( submesh );
        FB_ASSERT( submesh->isValid() );

        auto indices = Array<u32>();

        auto fbVertexBuffer = submesh->getVertexBuffer();
        auto fbIndexBuffer = submesh->getIndexBuffer();

        auto fbVertexCount = fbVertexBuffer->getNumVerticies();
        auto fbIndexCount = fbIndexBuffer->getNumIndices();

        indices.reserve( fbIndexCount );

        auto fbVertexDeclaration = fbVertexBuffer->getVertexDeclaration();
        const auto posElem = fbVertexDeclaration->findElementBySemantic(
            IVertexDeclaration::VertexElementSemantic::VES_POSITION );
        const auto normalElem = fbVertexDeclaration->findElementBySemantic(
            IVertexDeclaration::VertexElementSemantic::VES_NORMAL );
        const auto texCoordElem0 = fbVertexDeclaration->findElementBySemantic(
            IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 0 );
        const auto texCoordElem1 = fbVertexDeclaration->findElementBySemantic(
            IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 1 );

        // u32 fbVertexSize = fbVertexDeclaration->getSize();
        // u8* fbVertexDataPtr = (u8*)fbVertexBuffer->getVertexData();
        // f32* fbElementData = 0;

        // bool useWords = (fbVertexCount >= std::numeric_limits<u16>::max()) ? false : true;

        if(fbIndexBuffer->getIndexType() == IIndexBuffer::Type::IT_32BIT)
        {
            const u32 *fbIndexData = reinterpret_cast<u32 *>(fbIndexBuffer->getIndexData());

            // for (s32 i = (s32)fbIndexCount - 1; i >= 0; --i)
            for(u32 i = 0; i < fbIndexCount; ++i)
            {
                auto index = fbIndexData[i];
                indices.push_back( index );
            }
        }
        else
        {
            const u16 *fbIndexData = reinterpret_cast<u16 *>(fbIndexBuffer->getIndexData());
            // for (s32 i = (s32)fbIndexCount - 1; i >= 0; --i)
            for(u32 i = 0; i < fbIndexCount; ++i)
            {
                auto index = fbIndexData[i];
                indices.push_back( index );
            }
        }

        return indices;
    }

    void MeshUtil::meshToHeightMap( const String &meshName, const String &textureName,
                                    const Vector2I &textureSize )
    {
        // auto engine = core::IApplicationManager::instance();
        // SmartPtr<IPhysicsManager3> physicsMgr = engine->getPhysicsManager3();

        // PhysicsWorld3Ptr world = physicsMgr->createScene();

        // PhysicsMeshPtr physicsMesh = physicsMgr->createCollisionShape("mesh");
        // physicsMesh->load(meshName);

        // SmartPtr<IRigidBody3> rigidBody = physicsMgr->createRigidBody(physicsMesh);
        // rigidBody->setMass(physics_Num(0.0));
    }

    SmartPtr<IMesh> MeshUtil::buildMesh( const Array<Vector3F> &positions, const Array<Vector2F> &uvs,
                                         const Array<Vector2F> &uvs2 )
    {
        SmartPtr<IMesh> mesh( new CMesh );
        SmartPtr<ISubMesh> subMesh( new CSubMesh );
        mesh->addSubMesh( subMesh );

        auto numVertices = positions.size();

        // create sub mesh data
        SmartPtr<IVertexDeclaration> vertexDeclaration = fb::make_ptr<CVertexDeclaration>();
        vertexDeclaration->addElement( 0, sizeof( Vector3F ),
                                       CVertexDeclaration::VertexElementSemantic::VES_POSITION,
                                       IVertexElement::VertexElementType::VET_FLOAT3 );
        vertexDeclaration->addElement( 0, sizeof( Vector3F ),
                                       CVertexDeclaration::VertexElementSemantic::VES_NORMAL,
                                       IVertexElement::VertexElementType::VET_FLOAT3 );
        vertexDeclaration->addElement(
            0, sizeof( Vector2F ), CVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
            IVertexElement::VertexElementType::VET_FLOAT2, 0 );
        vertexDeclaration->addElement(
            0, sizeof( Vector2F ), CVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
            IVertexElement::VertexElementType::VET_FLOAT2, 1 );

        SmartPtr<IVertexBuffer> vertexBuffer( new CVertexBuffer );
        vertexBuffer->setVertexDeclaration( vertexDeclaration );

        vertexBuffer->setNumVerticies( static_cast<u32>(numVertices) );
        auto vertexData = static_cast<f32 *>(vertexBuffer->createVertexData());
        f32 *vertexDataPtr = vertexData;

        for(u32 vertIdx = 0; vertIdx < numVertices; ++vertIdx)
        {
            Vector3F position = positions[vertIdx];
            *vertexDataPtr++ = position.X();
            *vertexDataPtr++ = position.Y();
            *vertexDataPtr++ = position.Z();

            Vector3F normal( 0, 1, 0 );
            *vertexDataPtr++ = normal.X();
            *vertexDataPtr++ = normal.Y();
            *vertexDataPtr++ = normal.Z();

            Vector2F uv = uvs[vertIdx];
            *vertexDataPtr++ = uv.X();
            *vertexDataPtr++ = uv.Y();

            Vector2F uv2 = uvs2[vertIdx];
            *vertexDataPtr++ = uv2.X();
            *vertexDataPtr++ = uv2.Y();
        }

        auto numIndices = positions.size();
        SmartPtr<IIndexBuffer> indexBuffer( new CIndexBuffer );
        indexBuffer->setNumIndices( static_cast<u32>(numIndices) );
        auto indexDataPtr = static_cast<u32 *>(indexBuffer->createIndexData());

        for(u32 indexIdx = 0; indexIdx < numIndices; ++indexIdx)
        {
            *indexDataPtr++ = indexIdx;
        }

        subMesh->setVertexBuffer( vertexBuffer );
        subMesh->setIndexBuffer( indexBuffer );
        return mesh;
    }

    SmartPtr<ISubMesh> MeshUtil::buildSubMesh( const Array<Vector3F> &positions,
                                               const Array<Vector2F> &uvs, const Array<Vector2F> &uvs2 )
    {
        SmartPtr<ISubMesh> subMesh( new CSubMesh );

        auto numVertices = positions.size();

        // create sub mesh data
        SmartPtr<IVertexDeclaration> vertexDeclaration = fb::make_ptr<CVertexDeclaration>();
        vertexDeclaration->addElement( 0, sizeof( Vector3F ),
                                       CVertexDeclaration::VertexElementSemantic::VES_POSITION,
                                       IVertexElement::VertexElementType::VET_FLOAT3 );
        vertexDeclaration->addElement( 0, sizeof( Vector3F ),
                                       CVertexDeclaration::VertexElementSemantic::VES_NORMAL,
                                       IVertexElement::VertexElementType::VET_FLOAT3 );
        vertexDeclaration->addElement(
            0, sizeof( Vector2F ), CVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
            IVertexElement::VertexElementType::VET_FLOAT2, 0 );
        vertexDeclaration->addElement(
            0, sizeof( Vector2F ), CVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
            IVertexElement::VertexElementType::VET_FLOAT2, 1 );

        SmartPtr<IVertexBuffer> vertexBuffer( new CVertexBuffer );
        vertexBuffer->setVertexDeclaration( vertexDeclaration );

        vertexBuffer->setNumVerticies( static_cast<u32>(numVertices) );
        auto vertexData = static_cast<f32 *>(vertexBuffer->createVertexData());
        f32 *vertexDataPtr = vertexData;

        for(u32 vertIdx = 0; vertIdx < numVertices; ++vertIdx)
        {
            Vector3F position = positions[vertIdx];
            *vertexDataPtr++ = position.X();
            *vertexDataPtr++ = position.Y();
            *vertexDataPtr++ = position.Z();

            Vector3F normal( 0, 1, 0 );
            *vertexDataPtr++ = normal.X();
            *vertexDataPtr++ = normal.Y();
            *vertexDataPtr++ = normal.Z();

            Vector2F uv = uvs[vertIdx];
            *vertexDataPtr++ = uv.X();
            *vertexDataPtr++ = uv.Y();

            Vector2F uv2 = uvs2[vertIdx];
            *vertexDataPtr++ = uv2.X();
            *vertexDataPtr++ = uv2.Y();
        }

        auto numIndices = positions.size();
        SmartPtr<IIndexBuffer> indexBuffer( new CIndexBuffer );
        indexBuffer->setNumIndices( static_cast<u32>(numIndices) );
        auto indexDataPtr = static_cast<u32 *>(indexBuffer->createIndexData());

        for(u32 indexIdx = 0; indexIdx < numIndices; ++indexIdx)
        {
            *indexDataPtr++ = indexIdx;
        }

        subMesh->setVertexBuffer( vertexBuffer );
        subMesh->setIndexBuffer( indexBuffer );
        return subMesh;
    }

    SmartPtr<IMesh> MeshUtil::mergeMeshes( const Array<MeshTransformData> &meshTransformData )
    {
        SmartPtr<IMesh> newMesh( new CMesh );

        for(u32 meshIdx = 0; meshIdx < meshTransformData.size(); ++meshIdx)
        {
            const MeshTransformData &transformData = meshTransformData[meshIdx];
            const SmartPtr<IMesh> &curMesh = transformData.Mesh;

            const Array<SmartPtr<ISubMesh>> subMeshes = curMesh->getSubMeshes();
            for(u32 subMeshIdx = 0; subMeshIdx < subMeshes.size(); ++subMeshIdx)
            {
                SmartPtr<ISubMesh> subMesh = subMeshes[subMeshIdx];

                SmartPtr<ISubMesh> newSubMesh = subMesh->clone();
                newMesh->addSubMesh( newSubMesh );

                SmartPtr<IIndexBuffer> indexBuffer = newSubMesh->getIndexBuffer();
                SmartPtr<IVertexBuffer> vertexBuffer = newSubMesh->getVertexBuffer();

                u32 vertexCount = vertexBuffer->getNumVerticies();
                // u32 indexCount = indexBuffer->getNumIndices();

                const SmartPtr<IVertexElement> posElem =
                    vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                        CVertexDeclaration::VertexElementSemantic::VES_POSITION );
                const SmartPtr<IVertexElement> normalElem =
                    vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                        CVertexDeclaration::VertexElementSemantic::VES_NORMAL );
                const SmartPtr<IVertexElement> texCoordElem0 =
                    vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                        CVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 0 );
                const SmartPtr<IVertexElement> texCoordElem1 =
                    vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                        CVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 1 );

                u32 fbVertexSize = vertexBuffer->getVertexDeclaration()->getSize();
                auto fbVertexDataPtr = static_cast<u8 *>(vertexBuffer->getVertexData());
                f32 *fbElementData = nullptr;

                for(u32 j = 0; j < vertexCount; ++j, fbVertexDataPtr += fbVertexSize)
                {
                    posElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Vector3F position( fbElementData[0], fbElementData[1], fbElementData[2] );

                    position = transformData.Scale * position;
                    position = transformData.Orientation * position;
                    position = transformData.Position + position;
                    position.Z() = -position.Z();

                    fbElementData[0] = position.X();
                    fbElementData[1] = position.Y();
                    fbElementData[2] = position.Z();

                    normalElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Vector3F normal( fbElementData[0], fbElementData[1], fbElementData[2] );
                    normal = transformData.Orientation * normal;

                    fbElementData[0] = normal.X();
                    fbElementData[1] = normal.Y();
                    fbElementData[2] = normal.Z();

                    if(texCoordElem0)
                    {
                        texCoordElem0->getElementData( fbVertexDataPtr, &fbElementData );
                        Vector2F texCoord0( fbElementData[0], fbElementData[1] );
                        fbElementData[0] = texCoord0.X() * transformData.UVScaleData[0].X();
                        fbElementData[1] = texCoord0.Y() * transformData.UVScaleData[0].Y();
                        fbElementData[0] += transformData.UVOffsets[0].X();
                        fbElementData[1] += transformData.UVOffsets[0].Y();
                    }

                    if(texCoordElem1)
                    {
                        texCoordElem1->getElementData( fbVertexDataPtr, &fbElementData );
                        Vector2F texCoord1( fbElementData[0], fbElementData[1] );
                        texCoord1.X() = MathF::Mod( texCoord1.X(), 1.0f );
                        texCoord1.Y() = MathF::Mod( texCoord1.Y(), 1.0f );
                        if(texCoord1.X() < 0.0f)
                            texCoord1.X() = 1.0f + texCoord1.X();

                        if(texCoord1.Y() < 0.0f)
                            texCoord1.Y() = 1.0f + texCoord1.Y();

                        fbElementData[0] = texCoord1.X() * transformData.UVScaleData[1].X();
                        fbElementData[1] = texCoord1.Y() * transformData.UVScaleData[1].Y();
                        fbElementData[0] += transformData.UVOffsets[1].X();
                        fbElementData[1] += transformData.UVOffsets[1].Y();

                        // FB_ASSERT_TRUE(fbElementData[0] < 0.0f || fbElementData[0] > 1.0f);
                        // FB_ASSERT_TRUE(fbElementData[1] < 0.0f || fbElementData[1] > 1.0f);
                    }
                }
            }
        }

        return newMesh;
    }

    SmartPtr<IMesh> MeshUtil::mergeMeshes( const Array<SmartPtr<IMesh>> &meshes )
    {
        SmartPtr<IMesh> newMesh( new CMesh );

        for(u32 meshIdx = 0; meshIdx < meshes.size(); ++meshIdx)
        {
            const SmartPtr<IMesh> &curMesh = meshes[meshIdx];

            const Array<SmartPtr<ISubMesh>> subMeshes = curMesh->getSubMeshes();
            for(u32 subMeshIdx = 0; subMeshIdx < subMeshes.size(); ++subMeshIdx)
            {
                SmartPtr<ISubMesh> subMesh = subMeshes[subMeshIdx];

                SmartPtr<ISubMesh> newSubMesh = subMesh->clone();
                newMesh->addSubMesh( newSubMesh );

                SmartPtr<IIndexBuffer> indexBuffer = newSubMesh->getIndexBuffer();
                SmartPtr<IVertexBuffer> vertexBuffer = newSubMesh->getVertexBuffer();

                u32 vertexCount = vertexBuffer->getNumVerticies();
                // u32 indexCount = indexBuffer->getNumIndices();

                const SmartPtr<IVertexElement> posElem =
                    vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                        CVertexDeclaration::VertexElementSemantic::VES_POSITION );
                const SmartPtr<IVertexElement> normalElem =
                    vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                        CVertexDeclaration::VertexElementSemantic::VES_NORMAL );

                u32 fbVertexSize = vertexBuffer->getVertexDeclaration()->getSize();
                auto fbVertexDataPtr = static_cast<u8 *>(vertexBuffer->getVertexData());
                f32 *fbElementData = nullptr;

                for(u32 j = 0; j < vertexCount; ++j, fbVertexDataPtr += fbVertexSize)
                {
                    posElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Vector3F position( fbElementData[0], fbElementData[1], fbElementData[2] );
                    fbElementData[0] = position.X();
                    fbElementData[1] = position.Y();
                    fbElementData[2] = position.Z();
                }
            }
        }

        return newMesh;
    }

    SmartPtr<IMesh> MeshUtil::mergeMeshes( const Array<SmartPtr<IMesh>> &meshes,
                                           const Array<Matrix4F> &transformations )
    {
        SmartPtr<IMesh> newMesh( new CMesh );

        if(meshes.size() != transformations.size())
        {
            // MessageBoxUtil::show("MeshUtil::mergeMeshes - error transformations do not match the
            // number of meshes");
        }

        for(u32 meshIdx = 0; meshIdx < meshes.size() && meshIdx < transformations.size(); ++meshIdx)
        {
            const SmartPtr<IMesh> &curMesh = meshes[meshIdx];
            const Matrix4F &transformation = transformations[meshIdx];

            const Array<SmartPtr<ISubMesh>> subMeshes = curMesh->getSubMeshes();
            for(u32 subMeshIdx = 0; subMeshIdx < subMeshes.size(); ++subMeshIdx)
            {
                SmartPtr<ISubMesh> subMesh = subMeshes[subMeshIdx];

                SmartPtr<ISubMesh> newSubMesh = subMesh->clone();
                newMesh->addSubMesh( newSubMesh );

                SmartPtr<IIndexBuffer> indexBuffer = newSubMesh->getIndexBuffer();
                SmartPtr<IVertexBuffer> vertexBuffer = newSubMesh->getVertexBuffer();

                u32 vertexCount = vertexBuffer->getNumVerticies();
                // u32 indexCount = indexBuffer->getNumIndices();

                const SmartPtr<IVertexElement> posElem =
                    vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                        CVertexDeclaration::VertexElementSemantic::VES_POSITION );
                // const SmartPtr<IVertexElement> normalElem =
                // vertexBuffer->getVertexDeclaration()->findElementBySemantic(fb::VertexDeclaration::VES_NORMAL);

                u32 fbVertexSize = vertexBuffer->getVertexDeclaration()->getSize();
                auto fbVertexDataPtr = static_cast<u8 *>(vertexBuffer->getVertexData());
                f32 *fbElementData = nullptr;

                for(u32 j = 0; j < vertexCount; ++j, fbVertexDataPtr += fbVertexSize)
                {
                    posElem->getElementData( fbVertexDataPtr, &fbElementData );
                    Vector3F position( fbElementData[0], fbElementData[1], fbElementData[2] );
                    position = transformation * position;

                    fbElementData[0] = position.X();
                    fbElementData[1] = position.Y();
                    fbElementData[2] = position.Z();
                }
            }
        }

        return newMesh;
    }

    SmartPtr<IMesh> MeshUtil::mergeSubMeshesByMaterial( SmartPtr<IMesh> mesh )
    {
        using MatMeshMap = std::map<String, Array<SmartPtr<ISubMesh>>>;
        MatMeshMap matMeshMap;

        // sort sub meshes by material name
        Array<SmartPtr<ISubMesh>> subMeshList = mesh->getSubMeshes();
        for(u32 i = 0; i < subMeshList.size(); ++i)
        {
            SmartPtr<ISubMesh> subMesh = subMeshList[i];
            String materialName = subMesh->getMaterialName();

            matMeshMap[materialName].push_back( subMesh );
        }

        SmartPtr<IMesh> newMesh( new CMesh );

        auto it = matMeshMap.begin();
        for(; it != matMeshMap.end(); ++it)
        {
            u32 numTotalIndices = 0;
            u32 numTotalVertices = 0;

            const Array<SmartPtr<ISubMesh>> &matSubMeshList = it->second;
            for(u32 i = 0; i < matSubMeshList.size(); ++i)
            {
                SmartPtr<ISubMesh> subMesh = matSubMeshList[i];
                SmartPtr<IVertexBuffer> vertexBuffer = subMesh->getVertexBuffer();
                SmartPtr<IIndexBuffer> indexBuffer = subMesh->getIndexBuffer();

                numTotalVertices += vertexBuffer->getNumVerticies();
                numTotalIndices += indexBuffer->getNumIndices();
            }

            // create sub mesh
            SmartPtr<ISubMesh> subMesh( new CSubMesh );
            newMesh->addSubMesh( subMesh );

            subMesh->setMaterialName( it->first );

            // create sub mesh vertex data
            SmartPtr<IVertexDeclaration> vertexDeclaration = fb::make_ptr<CVertexDeclaration>();
            vertexDeclaration->addElement( 0, sizeof( Vector3F ),
                                           IVertexDeclaration::VertexElementSemantic::VES_POSITION,
                                           IVertexElement::VertexElementType::VET_FLOAT3 );
            vertexDeclaration->addElement( 0, sizeof( Vector3F ),
                                           IVertexDeclaration::VertexElementSemantic::VES_NORMAL,
                                           IVertexElement::VertexElementType::VET_FLOAT3 );
            vertexDeclaration->addElement(
                0, sizeof( Vector2F ),
                IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
                IVertexElement::VertexElementType::VET_FLOAT2, 0 );
            vertexDeclaration->addElement(
                0, sizeof( Vector2F ),
                IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
                IVertexElement::VertexElementType::VET_FLOAT2, 1 );

            // a pointer to the vertex data
            f32 *newVertexDataPtr = nullptr;

            {
                SmartPtr<IVertexBuffer> vertexBuffer( new CVertexBuffer );
                subMesh->setVertexBuffer( vertexBuffer );

                vertexBuffer->setVertexDeclaration( vertexDeclaration );

                vertexBuffer->setNumVerticies( numTotalVertices );
                auto newVertexData = static_cast<f32 *>(vertexBuffer->createVertexData());
                newVertexDataPtr = newVertexData;
            }

            // a pointer to the new index buffer
            u32 *newIndexDataPtr = nullptr;
            u32 indexOffset = 0;

            {
                // create sub mesh index data
                SmartPtr<IIndexBuffer> indexBuffer( new CIndexBuffer );
                subMesh->setIndexBuffer( indexBuffer );

                indexBuffer->setNumIndices( numTotalIndices );
                newIndexDataPtr = static_cast<u32 *>(indexBuffer->createIndexData());
            }

            // populate new vertex buffer
            for(u32 i = 0; i < matSubMeshList.size(); ++i)
            {
                SmartPtr<ISubMesh> subMesh = matSubMeshList[i];

                SmartPtr<IVertexBuffer> vertexBuffer = subMesh->getVertexBuffer();
                auto vertexData = static_cast<f32 *>(vertexBuffer->getVertexData());
                u32 numVerts = vertexBuffer->getNumVerticies();
                u32 vertSize = vertexBuffer->getVertexDeclaration()->getSize();
                // u32 totalVertexDataSize = numVerts * vertSize;

                u32 numFloats = numVerts * vertSize / sizeof( f32 );
                for(u32 vertIdx = 0; vertIdx < numFloats; ++vertIdx)
                {
                    *newVertexDataPtr++ = vertexData[vertIdx];
                }

                SmartPtr<IIndexBuffer> indexBuffer = subMesh->getIndexBuffer();
                auto indexData = static_cast<u32 *>(indexBuffer->getIndexData());
                u32 numIndices = indexBuffer->getNumIndices();

                for(u32 idx = 0; idx < numIndices; ++idx)
                {
                    u32 indexValue = indexData[idx] + indexOffset;
                    // FB_ASSERT_TRUE(!(indexValue < numTotalVertices));

                    *newIndexDataPtr++ = indexValue;
                }

                indexOffset += numVerts;
            }
        }

        return newMesh;
    }

    bool MeshUtil::isMeshValid( SmartPtr<IMesh> mesh )
    {
        const Array<SmartPtr<ISubMesh>> subMeshes = mesh->getSubMeshes();
        for(u32 subMeshIdx = 0; subMeshIdx < subMeshes.size(); ++subMeshIdx)
        {
            SmartPtr<ISubMesh> subMesh = subMeshes[subMeshIdx];

            Array<Vertex> vertices;

            SmartPtr<IVertexBuffer> vertexBuffer = subMesh->getVertexBuffer();
            // f32* vertexData = (f32*)vertexBuffer->getVertexData();
            u32 numVerts = vertexBuffer->getNumVerticies();
            u32 vertSize = vertexBuffer->getVertexDeclaration()->getSize();
            u32 totalVertexDataSize = numVerts * vertSize;

            vertices.resize( numVerts );

            const SmartPtr<IVertexElement> posElem =
                vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                    CVertexDeclaration::VertexElementSemantic::VES_POSITION );
            const SmartPtr<IVertexElement> normalElem =
                vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                    CVertexDeclaration::VertexElementSemantic::VES_NORMAL );
            const SmartPtr<IVertexElement> texCoordElem0 =
                vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                    CVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 0 );
            const SmartPtr<IVertexElement> texCoordElem1 =
                vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                    CVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES, 1 );

            u32 fbVertexSize = vertexBuffer->getVertexDeclaration()->getSize();
            auto fbVertexDataPtr = static_cast<u8 *>(vertexBuffer->getVertexData());
            f32 *fbElementData = nullptr;

            for(u32 j = 0; j < numVerts; ++j, fbVertexDataPtr += fbVertexSize)
            {
                Vertex vertex;

                posElem->getElementData( fbVertexDataPtr, &fbElementData );
                Vector3F position( fbElementData[0], fbElementData[1], fbElementData[2] );
                vertex.Position = position;

                normalElem->getElementData( fbVertexDataPtr, &fbElementData );
                Vector3F normal( fbElementData[0], fbElementData[1], fbElementData[2] );
                vertex.Normal = normal;

                if(texCoordElem0)
                {
                    texCoordElem0->getElementData( fbVertexDataPtr, &fbElementData );
                    Vector2F texCoord0( fbElementData[0], fbElementData[1] );
                    vertex.TexCoord0 = texCoord0;
                }

                if(texCoordElem1)
                {
                    texCoordElem1->getElementData( fbVertexDataPtr, &fbElementData );
                    Vector2F texCoord1( fbElementData[0], fbElementData[1] );
                    vertex.TexCoord1 = texCoord1;
                }

                vertices[j] = vertex;
            }

            // check if vertices are finite
            for(u32 i = 0; i < vertices.size(); ++i)
            {
                Vertex &vertex = vertices[i];
                if(!vertex.isFinite())
                {
                    // FB_ASSERT_TRUE(true);
                    FB_LOG_MESSAGE( "MeshUtil", "Error" );
                }
            }

            SmartPtr<IIndexBuffer> indexBuffer = subMesh->getIndexBuffer();
            const u32 *fbIndexData = reinterpret_cast<u32 *>(indexBuffer->getIndexData());
            for(u32 i = 0; i < indexBuffer->getNumIndices(); ++i)
            {
                u32 index = fbIndexData[i];
                if(index >= vertices.size())
                {
                    // FB_ASSERT_TRUE(true);
                    FB_LOG_MESSAGE( "MeshUtil", "Error" );
                }

                Vertex &vertex = vertices[index];
                if(!vertex.isFinite())
                {
                    // FB_ASSERT_TRUE(true);
                    FB_LOG_MESSAGE( "MeshUtil", "Error" );
                }
            }
        }

        return false;
    }

    
    SmartPtr<IMesh> MeshUtil::getMesh( const Array<f32> &heightData, f32 worldScale, f32 heightScale,
                                       u32 tileSize )
    {
        SmartPtr<IMesh> mesh( new CMesh );
        SmartPtr<ISubMesh> subMesh( new CSubMesh );
        mesh->addSubMesh( subMesh );

        // create sub mesh data
        SmartPtr<IVertexDeclaration> vertexDeclaration( new CVertexDeclaration );
        vertexDeclaration->addElement( 0, sizeof( Vector3F ),
                                       CVertexDeclaration::VertexElementSemantic::VES_POSITION,
                                       IVertexElement::VertexElementType::VET_FLOAT3 );
        // vertexDeclaration->addElement(sizeof(Vector3F), VertexDeclaration::VES_NORMAL,
        // VertexDeclaration::VET_FLOAT3); vertexDeclaration->addElement(sizeof(Vector2F),
        // VertexDeclaration::VES_TEXTURE_COORDINATES, VertexDeclaration::VET_FLOAT2, 0);
        // vertexDeclaration->addElement(sizeof(Vector2F), VertexDeclaration::VES_TEXTURE_COORDINATES,
        // VertexDeclaration::VET_FLOAT2, 1);

        SmartPtr<IVertexBuffer> vertexBuffer( new CVertexBuffer );
        vertexBuffer->setVertexDeclaration( vertexDeclaration );

        u32 numVerticies = tileSize * tileSize;

        vertexBuffer->setNumVerticies( numVerticies );
        auto vertexData = static_cast<f32 *>(vertexBuffer->createVertexData());
        f32 *vertexDataPtr = vertexData;

        u32 numVerticiesAdded = 0;
        const f32 tdSize = 1.0f / static_cast<f32>(tileSize - 1);
        for(s32 x = 0; x < static_cast<s32>(tileSize); ++x)
        {
            for(s32 z = 0; z < static_cast<s32>(tileSize); ++z)
            {
                // s32 indexZ = -z;

                s32 zIdx = ( tileSize - 1 ) - z;
                zIdx = z;

                f32 height = heightData[x + z * tileSize];

                Vector3F position;
                position.X() = static_cast<f32>(x);
                position.Y() = height * heightScale;
                position.Z() = static_cast<f32>(z);
                position = position * worldScale;

                *vertexDataPtr++ = position.X();
                *vertexDataPtr++ = position.Y();
                *vertexDataPtr++ = position.Z();

                /*				Vector3F normal;
                                normal = Vector3F::UNIT_Y;
                                *vertexDataPtr++ = normal.X();
                                *vertexDataPtr++ = normal.Y();
                                *vertexDataPtr++ = normal.Z();

                                Vector2F texCoord0;
                                texCoord0.X() = x;
                                texCoord0.Y() = z;
                                *vertexDataPtr++ = texCoord0.X();
                                *vertexDataPtr++ = texCoord0.Y();

                                Vector2F texCoord1;
                                texCoord1.X() = (f32)x / (f32)tileSize;
                                texCoord1.Y() = (f32)z / (f32)tileSize;
                                *vertexDataPtr++ = texCoord1.X();
                                *vertexDataPtr++ = texCoord1.Y();*/

                numVerticiesAdded++;
            }
        }

        u32 numIndices = tileSize * tileSize * 6;
        SmartPtr<IIndexBuffer> indexBuffer( new CIndexBuffer );
        indexBuffer->setNumIndices( numIndices );
        indexBuffer->setIndexType( CIndexBuffer::Type::IT_32BIT );
        auto indexDataPtr = static_cast<u32 *>(indexBuffer->createIndexData());

        u32 index11;
        u32 index21;
        u32 index12;
        u32 index22;
        s32 step = 6;

        u32 numIndicesSet = 0;
        for(s32 z = 0; z < static_cast<s32>(tileSize) - 1; z += step)
        {
            for(s32 x = 0; x < static_cast<s32>(tileSize) - 1; x += step)
            {
                index11 = getIndex( tileSize, x, z );
                index21 = getIndex( tileSize, x + step, z );
                index12 = getIndex( tileSize, x, z + step );
                index22 = getIndex( tileSize, x + step, z + step );

                *indexDataPtr++ = index22;
                *indexDataPtr++ = index11;
                *indexDataPtr++ = index12;

                *indexDataPtr++ = index21;
                *indexDataPtr++ = index11;
                *indexDataPtr++ = index22;

                numIndicesSet += 6;
            }
        }

        // size_t newLength = (tileSize / step) * (tileSize / step) * 2 * 2 * 2;
        indexBuffer->setNumIndices( numIndicesSet );

        subMesh->setVertexBuffer( vertexBuffer );
        subMesh->setIndexBuffer( indexBuffer );

        return mesh;
    }

    u32 MeshUtil::getIndex( u32 tileSize, u32 x, u32 z )
    {
        return x + z * tileSize;
    }

    void MeshUtil::unshareVertices( SmartPtr<IMesh> mesh )
    {
        /*
        // Retrieve data to copy bone assignments
        //const Mesh::VertexBoneAssignmentList& boneAssignments = mesh->getBoneAssignments();

        // Access shared vertices
        VertexData* sharedVertexData = mesh->getSha;
        if (!sharedVertexData)
            return;

        VerticesRemapInfo remapInfo;
        for (unsigned subMeshIdx = 0; subMeshIdx < mesh->getNumSubMeshes(); subMeshIdx++)
        {
            SubMesh *subMesh = mesh->getSubMesh(subMeshIdx);
            if (!subMesh->useSharedVertices)
                continue;

            IndexData *indexData = subMesh->indexData[VpNormal];
            if (!indexData)
                continue;

            remapInfo.initialize( sharedVertexData->vertexCount, false );
            remapInfo.markUsedIndices( indexData );

            VertexData *newVertexData = new VertexData(mesh->getHardwareBufferManager());
            newVertexData->vertexCount = remapInfo.usedCount;
            mesh->getHardwareBufferManager()->destroyVertexDeclaration(newVertexData->vertexDeclaration);
            newVertexData->vertexDeclaration =
        sharedVertexData->vertexDeclaration->clone(mesh->getHardwareBufferManager());

            for (size_t bufIdx = 0; bufIdx < sharedVertexData->vertexBufferBinding->getBufferCount();
        bufIdx++)
            {
                HardwareVertexBufferSharedPtr sharedVertexBuffer =
        sharedVertexData->vertexBufferBinding->getBuffer(bufIdx); HardwareVertexBufferSharedPtr
        newVertexBuffer = remapInfo.getRemappedVertexBuffer( mesh->getHardwareBufferManager(),
        sharedVertexBuffer, sharedVertexData->vertexStart, sharedVertexData->vertexCount );

                newVertexData->vertexBufferBinding->setBinding(bufIdx, newVertexBuffer);
            }

            ushort numLods = mesh->hasManualLodLevel() ? 1 : mesh->getNumLodLevels();
            for (ushort lod = 1; lod < numLods; ++lod)
            {
                v1::IndexData *lodIndexData = subMesh->mLodFaceList[VpNormal][lod - 1]; // lod0 is stored
        separately remapInfo.performIndexDataRemap(mesh->getHardwareBufferManager(), lodIndexData);
            }
            remapInfo.performIndexDataRemap(mesh->getHardwareBufferManager(), indexData);

            // Store new attributes
            subMesh->useSharedVertices = false;
            subMesh->vertexData[VpNormal] = newVertexData;

            // Transfer bone assignments to the submesh
            remapInfo.performBoneAssignmentRemap(subMesh, mesh);
        }

        // Release shared vertex data
        if( mesh->sharedVertexData[VpNormal] == mesh->sharedVertexData[VpShadow] )
            mesh->sharedVertexData[VpShadow] = 0;

        delete mesh->sharedVertexData[VpNormal];
        delete mesh->sharedVertexData[VpShadow];
        mesh->sharedVertexData[VpNormal] = 0;
        mesh->sharedVertexData[VpShadow] = 0;
        mesh->clearBoneAssignments();

        mesh->prepareForShadowMapping( false );

        if( mesh->isEdgeListBuilt() )
        {
            mesh->freeEdgeList();
            mesh->buildEdgeList();
        }
         */
    }

    u16 MeshUtil::getTypeCount( IVertexElement::VertexElementType etype )
    {
        switch(etype)
        {
        case IVertexElement::VertexElementType::VET_FLOAT1:
        case IVertexElement::VertexElementType::VET_SHORT1:
        case IVertexElement::VertexElementType::VET_USHORT1:
        case IVertexElement::VertexElementType::VET_UINT1:
        case IVertexElement::VertexElementType::VET_INT1:
        case IVertexElement::VertexElementType::VET_DOUBLE1:
            return 1;
        case IVertexElement::VertexElementType::VET_FLOAT2:
        case IVertexElement::VertexElementType::VET_SHORT2:
        case IVertexElement::VertexElementType::VET_SHORT2_NORM:
        case IVertexElement::VertexElementType::VET_USHORT2:
        case IVertexElement::VertexElementType::VET_USHORT2_NORM:
        case IVertexElement::VertexElementType::VET_UINT2:
        case IVertexElement::VertexElementType::VET_INT2:
        case IVertexElement::VertexElementType::VET_DOUBLE2:
            return 2;
        case IVertexElement::VertexElementType::VET_FLOAT3:
        case IVertexElement::VertexElementType::VET_SHORT3:
        case IVertexElement::VertexElementType::VET_USHORT3:
        case IVertexElement::VertexElementType::VET_UINT3:
        case IVertexElement::VertexElementType::VET_INT3:
        case IVertexElement::VertexElementType::VET_DOUBLE3:
            return 3;
        case IVertexElement::VertexElementType::VET_FLOAT4:
        case IVertexElement::VertexElementType::VET_SHORT4:
        case IVertexElement::VertexElementType::VET_SHORT4_NORM:
        case IVertexElement::VertexElementType::VET_USHORT4:
        case IVertexElement::VertexElementType::VET_USHORT4_NORM:
        case IVertexElement::VertexElementType::VET_UINT4:
        case IVertexElement::VertexElementType::VET_INT4:
        case IVertexElement::VertexElementType::VET_DOUBLE4:
        case IVertexElement::VertexElementType::VET_BYTE4:
        case IVertexElement::VertexElementType::VET_UBYTE4:
        case IVertexElement::VertexElementType::VET_BYTE4_NORM:
        case IVertexElement::VertexElementType::VET_UBYTE4_NORM:
        case IVertexElement::VertexElementType::_DETAIL_SWAP_RB:
            return 4;
        }

        FB_LOG_ERROR( "VertexElement::getTypeCount: Invalid type" );
        return 0;
    }

    IVertexElement::VertexElementType MeshUtil::multiplyTypeCount(
        IVertexElement::VertexElementType baseType, u16 count )
    {
        FB_ASSERT( count > 0 && count < 5 ); // Count out of range

        switch(baseType)
        {
        case IVertexElement::VertexElementType::VET_FLOAT1:
        case IVertexElement::VertexElementType::VET_DOUBLE1:
        case IVertexElement::VertexElementType::VET_INT1:
        case IVertexElement::VertexElementType::VET_UINT1:
            // evil enumeration arithmetic
            return static_cast<IVertexElement::VertexElementType>(static_cast<u16>(baseType) + count -
                                                                  1);

        case IVertexElement::VertexElementType::VET_SHORT1:
        case IVertexElement::VertexElementType::VET_SHORT2:
            if(count <= 2)
            {
                return IVertexElement::VertexElementType::VET_SHORT2;
            }
            return IVertexElement::VertexElementType::VET_SHORT4;

        case IVertexElement::VertexElementType::VET_USHORT1:
        case IVertexElement::VertexElementType::VET_USHORT2:
            if(count <= 2)
            {
                return IVertexElement::VertexElementType::VET_USHORT2;
            }
            return IVertexElement::VertexElementType::VET_USHORT4;

        case IVertexElement::VertexElementType::VET_SHORT2_NORM:
            if(count <= 2)
            {
                return IVertexElement::VertexElementType::VET_SHORT2_NORM;
            }
            return IVertexElement::VertexElementType::VET_SHORT4_NORM;

        case IVertexElement::VertexElementType::VET_USHORT2_NORM:
            if(count <= 2)
            {
                return IVertexElement::VertexElementType::VET_USHORT2_NORM;
            }
            return IVertexElement::VertexElementType::VET_USHORT4_NORM;

        case IVertexElement::VertexElementType::VET_BYTE4:
        case IVertexElement::VertexElementType::VET_BYTE4_NORM:
        case IVertexElement::VertexElementType::VET_UBYTE4:
        case IVertexElement::VertexElementType::VET_UBYTE4_NORM:
            return baseType;
        default:
            break;
        }

        FB_LOG_ERROR( "VertexElement::multiplyTypeCount: Invalid base type" );
        return static_cast<IVertexElement::VertexElementType>(0);
    }

    IVertexElement::VertexElementType MeshUtil::getBaseType(
        IVertexElement::VertexElementType multiType )
    {
        switch(multiType)
        {
        case IVertexElement::VertexElementType::VET_FLOAT1:
        case IVertexElement::VertexElementType::VET_FLOAT2:
        case IVertexElement::VertexElementType::VET_FLOAT3:
        case IVertexElement::VertexElementType::VET_FLOAT4:
            return IVertexElement::VertexElementType::VET_FLOAT1;
        case IVertexElement::VertexElementType::VET_DOUBLE1:
        case IVertexElement::VertexElementType::VET_DOUBLE2:
        case IVertexElement::VertexElementType::VET_DOUBLE3:
        case IVertexElement::VertexElementType::VET_DOUBLE4:
            return IVertexElement::VertexElementType::VET_DOUBLE1;
        case IVertexElement::VertexElementType::VET_INT1:
        case IVertexElement::VertexElementType::VET_INT2:
        case IVertexElement::VertexElementType::VET_INT3:
        case IVertexElement::VertexElementType::VET_INT4:
            return IVertexElement::VertexElementType::VET_INT1;
        case IVertexElement::VertexElementType::VET_UINT1:
        case IVertexElement::VertexElementType::VET_UINT2:
        case IVertexElement::VertexElementType::VET_UINT3:
        case IVertexElement::VertexElementType::VET_UINT4:
            return IVertexElement::VertexElementType::VET_UINT1;
        case IVertexElement::VertexElementType::VET_SHORT1:
        case IVertexElement::VertexElementType::VET_SHORT2:
        case IVertexElement::VertexElementType::VET_SHORT3:
        case IVertexElement::VertexElementType::VET_SHORT4:
            return IVertexElement::VertexElementType::VET_SHORT1;
        case IVertexElement::VertexElementType::VET_USHORT1:
        case IVertexElement::VertexElementType::VET_USHORT2:
        case IVertexElement::VertexElementType::VET_USHORT3:
        case IVertexElement::VertexElementType::VET_USHORT4:
            return IVertexElement::VertexElementType::VET_USHORT1;
        case IVertexElement::VertexElementType::VET_SHORT2_NORM:
        case IVertexElement::VertexElementType::VET_SHORT4_NORM:
            return IVertexElement::VertexElementType::VET_SHORT2_NORM;
        case IVertexElement::VertexElementType::VET_USHORT2_NORM:
        case IVertexElement::VertexElementType::VET_USHORT4_NORM:
            return IVertexElement::VertexElementType::VET_USHORT2_NORM;
        case IVertexElement::VertexElementType::VET_BYTE4:
            return IVertexElement::VertexElementType::VET_BYTE4;
        case IVertexElement::VertexElementType::VET_BYTE4_NORM:
            return IVertexElement::VertexElementType::VET_BYTE4_NORM;
        case IVertexElement::VertexElementType::VET_UBYTE4:
            return IVertexElement::VertexElementType::VET_UBYTE4;
        case IVertexElement::VertexElementType::VET_UBYTE4_NORM:
        case IVertexElement::VertexElementType::_DETAIL_SWAP_RB:
            return IVertexElement::VertexElementType::VET_UBYTE4_NORM;
        }

        // To keep compiler happy
        return IVertexElement::VertexElementType::VET_FLOAT1;
    }

    u32 MeshUtil::convertColourValue( const ColourF &src, IVertexElement::VertexElementType dst )
    {
        return 0;
    }

    void MeshUtil::convertColourValue( IVertexElement::VertexElementType srcType,
                                       IVertexElement::VertexElementType dstType, u32 *ptr )
    {
        if(srcType == dstType)
        {
            return;
        }

        // Conversion between ARGB and ABGR is always a case of flipping R/B
        *ptr = ( ( *ptr & 0x00FF0000 ) >> 16 ) | ( ( *ptr & 0x000000FF ) << 16 ) | ( *ptr & 0xFF00FF00 );
    }

    IVertexElement::VertexElementType MeshUtil::getBestColourVertexElementType( void )
    {
        return IVertexElement::VertexElementType::VET_UBYTE4_NORM;
    }
} // end namespace fb
