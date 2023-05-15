#include <FBMesh/FBMeshPCH.h>
#include <FBMesh/MeshGenerator.h>
#include <FBMesh/CMesh.h>
#include <FBMesh/CSubMesh.h>
#include <FBMesh/CVertexDeclaration.h>
#include <FBMesh/CVertexElement.h>
#include <FBMesh/CVertexBuffer.h>
#include <FBMesh/CIndexBuffer.h>
#include <FBCore/FBCore.h>

namespace fb
{
    SmartPtr<IVertexBuffer> MeshGenerator::createVertexBuffer( const Array<Vector3F> &positions,
                                                               const Array<Vector3F> &normals,
                                                               const Array<Vector2F> &uvs )
    {
        auto numVertices = positions.size();

        auto vertexDeclaration = fb::make_ptr<CVertexDeclaration>();
        u32 offset = 0;

        auto element = vertexDeclaration->addElement(
            0, offset, IVertexDeclaration::VertexElementSemantic::VES_POSITION,
            IVertexElement::VertexElementType::VET_FLOAT3 );
        offset = element->getSize();

        element = vertexDeclaration->addElement( 0, offset,
                                                 IVertexDeclaration::VertexElementSemantic::VES_NORMAL,
                                                 IVertexElement::VertexElementType::VET_FLOAT3 );
        offset += element->getSize();

        element = vertexDeclaration->addElement(
            0, offset, IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
            IVertexElement::VertexElementType::VET_FLOAT2, 0 );
        offset += element->getSize();

        auto vertexBuffer = fb::make_ptr<CVertexBuffer>();
        vertexBuffer->setVertexDeclaration( vertexDeclaration );
        vertexBuffer->setNumVerticies( numVertices );

        auto vertexData = static_cast<f32 *>( vertexBuffer->createVertexData() );
        f32 *vertexDataPtr = vertexData;

        for( size_t i = 0; i < numVertices; ++i )
        {
            const auto &position = positions[i];
            *vertexDataPtr++ = position.X();
            *vertexDataPtr++ = position.Y();
            *vertexDataPtr++ = position.Z();

            const auto &normal = normals[i];
            *vertexDataPtr++ = normal.X();
            *vertexDataPtr++ = normal.Y();
            *vertexDataPtr++ = normal.Z();

            const auto &uv = uvs[i];
            *vertexDataPtr++ = uv.X();
            *vertexDataPtr++ = uv.Y();
        }

        return vertexBuffer;
    }

    SmartPtr<IVertexBuffer> MeshGenerator::createVertexBuffer( const Array<Vertex> &vertices )
    {
        auto vertexDeclaration = fb::make_ptr<CVertexDeclaration>();
        u32 offset = 0;

        auto element = vertexDeclaration->addElement(
            0, offset, IVertexDeclaration::VertexElementSemantic::VES_POSITION,
            IVertexElement::VertexElementType::VET_FLOAT3 );
        offset = element->getSize();

        element = vertexDeclaration->addElement( 0, offset,
                                                 IVertexDeclaration::VertexElementSemantic::VES_NORMAL,
                                                 IVertexElement::VertexElementType::VET_FLOAT3 );
        offset += element->getSize();

        element = vertexDeclaration->addElement(
            0, offset, IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
            IVertexElement::VertexElementType::VET_FLOAT2, 0 );
        offset += element->getSize();

        auto vertexBuffer = fb::make_ptr<CVertexBuffer>();
        vertexBuffer->setVertexDeclaration( vertexDeclaration );
        vertexBuffer->setNumVerticies( 4 );

        auto vertexData = static_cast<f32 *>( vertexBuffer->createVertexData() );
        f32 *vertexDataPtr = vertexData;

        for( auto &vert : vertices )
        {
            auto &position = vert.position;
            *vertexDataPtr++ = position.X();
            *vertexDataPtr++ = position.Y();
            *vertexDataPtr++ = position.Z();

            auto &normal = vert.normal;
            *vertexDataPtr++ = normal.X();
            *vertexDataPtr++ = normal.Y();
            *vertexDataPtr++ = normal.Z();

            auto &uv = vert.texCoord;
            *vertexDataPtr++ = uv.X();
            *vertexDataPtr++ = uv.Y();
        }

        return vertexBuffer;
    }

    SmartPtr<IIndexBuffer> MeshGenerator::createIndexBuffer( const Array<u32> &indices )
    {
        auto indexBuffer = fb::make_ptr<CIndexBuffer>();
        indexBuffer->setIndexType( CIndexBuffer::Type::IT_16BIT );
        indexBuffer->setNumIndices( 6 );

        auto indexDataPtr = static_cast<u16 *>( indexBuffer->createIndexData() );

        for( auto &index : indices )
        {
            *indexDataPtr++ = static_cast<u16>( index );
        }

        return indexBuffer;
    }

    SmartPtr<ISubMesh> MeshGenerator::createSubMesh( SmartPtr<IVertexBuffer> vertexBuffer,
                                                     SmartPtr<IIndexBuffer> indexBuffer )
    {
        auto subMesh = fb::make_ptr<CSubMesh>();
        subMesh->setVertexBuffer( vertexBuffer );
        subMesh->setIndexBuffer( indexBuffer );

        return subMesh;
    }

    SmartPtr<IMesh> MeshGenerator::createMesh( const Array<Vector3F> &vertices,
                                               const Array<Vector3F> &normals,
                                               const Array<Vector2F> &uvs, const Array<u32> &indices )
    {
        auto vertexBuffer = MeshGenerator::createVertexBuffer( vertices, normals, uvs );
        auto indexBuffer = MeshGenerator::createIndexBuffer( indices );

        auto subMesh = MeshGenerator::createSubMesh( vertexBuffer, indexBuffer );

        auto mesh = fb::make_ptr<CMesh>();
        mesh->addSubMesh( subMesh );
        return mesh;
    }

    SmartPtr<IMesh> MeshGenerator::createMesh( const Array<Vector3F> &vertices,
                                               const Array<Vector3F> &normals,
                                               const Array<Vector4F> &tangents,
                                               const Array<Vector3F> &uvs, const Array<u32> &indices )
    {
        return nullptr;
    }

    SmartPtr<IMesh> MeshGenerator::createMesh( const Array<Vector3F> &vertices,
                                               const Array<Vector3F> &normals,
                                               const Array<Vector4F> &tangents,
                                               const Array<Vector3F> &uvs0, const Array<Vector3F> &uvs1,
                                               const Array<u32> &indices )
    {
        return nullptr;
    }

    SmartPtr<IMesh> MeshGenerator::createPlane( const Vector3F &halfExtent, const Vector3F &normal,
                                                const Vector3F &right )
    {
        Vertex v[4];

        auto binormal = normal.crossProduct( right );

        // top left
        v[0].position = binormal - right;

        // top right
        v[1].position = binormal + right;

        // bottom left
        v[2].position = -binormal - right;

        // bottom right
        v[3].position = -binormal + right;

        v[0].normal = normal;
        v[1].normal = normal;
        v[2].normal = normal;
        v[3].normal = normal;

        auto texCoordScale = Vector2F::unit() * 1.0f;

        v[0].texCoord = Vector2F( 0, 0 ) * texCoordScale;
        v[1].texCoord = Vector2F( 1, 0 ) * texCoordScale;
        v[2].texCoord = Vector2F( 0, 1 ) * texCoordScale;
        v[3].texCoord = Vector2F( 1, 1 ) * texCoordScale;

        auto planeMesh = fb::make_ptr<CMesh>();

        auto subMesh = fb::make_ptr<CSubMesh>();
        planeMesh->addSubMesh( subMesh );

        // create sub mesh data
        auto vertexDeclaration = fb::make_ptr<CVertexDeclaration>();
        u32 offset = 0;

        offset = vertexDeclaration
                     ->addElement( 0, offset, IVertexDeclaration::VertexElementSemantic::VES_POSITION,
                                   IVertexElement::VertexElementType::VET_FLOAT3 )
                     ->getSize();
        offset += vertexDeclaration
                      ->addElement( 0, offset, IVertexDeclaration::VertexElementSemantic::VES_NORMAL,
                                    IVertexElement::VertexElementType::VET_FLOAT3 )
                      ->getSize();
        offset += vertexDeclaration
                      ->addElement( 0, offset,
                                    IVertexDeclaration::VertexElementSemantic::VES_TEXTURE_COORDINATES,
                                    IVertexElement::VertexElementType::VET_FLOAT2, 0 )
                      ->getSize();

        auto vertexBuffer = fb::make_ptr<CVertexBuffer>();
        vertexBuffer->setVertexDeclaration( vertexDeclaration );
        vertexBuffer->setNumVerticies( 4 );

        auto vertexData = static_cast<f32 *>( vertexBuffer->createVertexData() );
        f32 *vertexDataPtr = vertexData;

        for( u32 vertIdx = 0; vertIdx < 4; ++vertIdx )
        {
            auto &vert = v[vertIdx];

            auto &position = vert.position;
            *vertexDataPtr++ = position.X();
            *vertexDataPtr++ = position.Y();
            *vertexDataPtr++ = position.Z();

            auto &normal = vert.normal;
            *vertexDataPtr++ = normal.X();
            *vertexDataPtr++ = normal.Y();
            *vertexDataPtr++ = normal.Z();

            auto &uv = vert.texCoord;
            *vertexDataPtr++ = uv.X();
            *vertexDataPtr++ = uv.Y();
        }

        auto indexBuffer = fb::make_ptr<CIndexBuffer>();
        indexBuffer->setIndexType( CIndexBuffer::Type::IT_16BIT );
        indexBuffer->setNumIndices( 6 );

        auto indexDataPtr = static_cast<u16 *>( indexBuffer->createIndexData() );

        indexDataPtr[0] = 0;
        indexDataPtr[1] = 2;
        indexDataPtr[2] = 3;

        indexDataPtr[3] = 0;
        indexDataPtr[4] = 3;
        indexDataPtr[5] = 1;

        subMesh->setVertexBuffer( vertexBuffer );
        subMesh->setIndexBuffer( indexBuffer );

        return planeMesh;
    }

    SmartPtr<IMesh> MeshGenerator::createBox( f32 sizeX, f32 sizeY, f32 sizeZ, u32 numSegX, u32 numSegY,
                                              u32 numSegZ )
    {
        return nullptr;
    }

    SmartPtr<IMesh> MeshGenerator::createCylinder( f32 radius, f32 height, u32 numSegBase,
                                                   u32 numSegHeight, bool capped )
    {
        Array<Vector3F> vertices;
        Array<Vector3F> normals;
        Array<Vector2F> uvs;
        Array<u32> indices;

        vertices.reserve( 1000 );
        normals.reserve( 1000 );
        uvs.reserve( 1000 );
        indices.reserve( 1000 );

        auto deltaAngle = ( ( Math<real_Num>::pi() * static_cast<real_Num>( 2.0 ) ) / numSegBase );
        auto deltaHeight = height / static_cast<real_Num>( numSegHeight );
        auto offset = 0;

        for( u32 i = 0; i <= numSegHeight; i++ )
        {
            for( u32 j = 0; j <= numSegBase; j++ )
            {
                auto x0 = radius * Math<real_Num>::Cos( j * deltaAngle );
                auto z0 = radius * Math<real_Num>::Sin( j * deltaAngle );

                auto position = Vector3F( x0, i * deltaHeight, z0 );
                auto normal = Vector3F( x0, 0, z0 ).normaliseCopy();
                auto texCoord =
                    Vector2F( j / static_cast<f32>( numSegBase ), i / static_cast<f32>( numSegHeight ) );

                vertices.push_back( position );
                normals.push_back( normal );
                uvs.push_back( texCoord );

                if( i != numSegHeight )
                {
                    indices.push_back( offset + numSegBase + 1 );
                    indices.push_back( offset );
                    indices.push_back( offset + numSegBase );
                    indices.push_back( offset + numSegBase + 1 );
                    indices.push_back( offset + 1 );
                    indices.push_back( offset );
                }

                offset++;
            }
        }

        return MeshGenerator::createMesh( vertices, normals, uvs, indices );
    }

    SmartPtr<IMesh> MeshGenerator::createCylinderFromSpline( SmartPtr<LinearSpline3F> spline, f32 radius,
                                                             f32 height, u32 numSegBase,
                                                             u32 numSegHeight, bool capped )
    {
        Array<Vector3F> vertices;
        Array<Vector3F> normals;
        Array<Vector2F> uvs;
        Array<u32> indices;

        vertices.reserve( 1000 );
        normals.reserve( 1000 );
        uvs.reserve( 1000 );
        indices.reserve( 1000 );

        auto deltaAngle = ( ( Math<real_Num>::pi() * static_cast<real_Num>( 2.0 ) ) / numSegBase );
        auto deltaHeight = height / static_cast<real_Num>( numSegHeight );
        auto offset = 0;

        for( u32 i = 0; i <= numSegHeight; i++ )
        {
            for( u32 j = 0; j <= numSegBase; j++ )
            {
                auto x0 = radius * Math<real_Num>::Cos( j * deltaAngle );
                auto z0 = radius * Math<real_Num>::Sin( j * deltaAngle );

                auto position = Vector3F( x0, i * deltaHeight, z0 );
                auto normal = Vector3F( x0, 0, z0 ).normaliseCopy();
                auto texCoord =
                    Vector2F( j / static_cast<f32>( numSegBase ), i / static_cast<f32>( numSegHeight ) );

                vertices.push_back( position );
                normals.push_back( normal );
                uvs.push_back( texCoord );

                if( i != numSegHeight )
                {
                    indices.push_back( offset + numSegBase + 1 );
                    indices.push_back( offset );
                    indices.push_back( offset + numSegBase );
                    indices.push_back( offset + numSegBase + 1 );
                    indices.push_back( offset + 1 );
                    indices.push_back( offset );
                }

                offset++;
            }
        }

        return MeshGenerator::createMesh( vertices, normals, uvs, indices );
    }

    SmartPtr<IMesh> MeshGenerator::createRoadFromSpline( SmartPtr<LinearSpline3F> spline, f32 radius,
                                                         f32 height, u32 numSegBase, u32 numSegHeight,
                                                         bool capped )
    {
        Array<Vector3F> vertices;
        Array<Vector3F> normals;
        Array<Vector2F> uvs;
        Array<u32> indices;

        vertices.reserve( 12 );
        uvs.reserve( 12 );
        indices.reserve( 12 );

        auto widthA = 5.0f;
        auto widthB = 5.0f;

        auto index = 0;
        auto segments = 8;
        for( size_t i = 0; i < segments; ++i )
        {
            auto d0 = i / segments;
            auto d1 = ( i + 1 ) / segments;

            auto pointA = spline->interpolate( d0 );
            auto pointB = spline->interpolate( d1 );

            auto vec = ( pointB - pointA ).normaliseCopy();
            auto tangentA = vec.crossProduct( Vector3F::up() );
            auto tangentB = -tangentA;

            auto p0 = pointA - tangentA * widthA;
            auto p1 = pointA + tangentA * widthA;
            auto p2 = pointB + tangentB * widthB;
            auto p3 = pointB - tangentB * widthB;

            vertices.push_back( Vector3F( p0.X(), p0.Y(), p0.Z() ) );
            vertices.push_back( Vector3F( p1.X(), p1.Y(), p1.Z() ) );
            vertices.push_back( Vector3F( p2.X(), p2.Y(), p2.Z() ) );

            vertices.push_back( Vector3F( p2.X(), p2.Y(), p2.Z() ) );
            vertices.push_back( Vector3F( p3.X(), p3.Y(), p3.Z() ) );
            vertices.push_back( Vector3F( p0.X(), p0.Y(), p0.Z() ) );

            normals.push_back( Vector3F::up() );
            normals.push_back( Vector3F::up() );
            normals.push_back( Vector3F::up() );

            normals.push_back( Vector3F::up() );
            normals.push_back( Vector3F::up() );
            normals.push_back( Vector3F::up() );

            const auto segXRatio = 0.0f;
            const auto segYRatio = 0.0f;

            const auto nextSegXRatio = 1.0f;
            const auto nextSegYRatio = 1.0f;

            uvs.push_back( Vector2F( segXRatio, segYRatio ) );
            uvs.push_back( Vector2F( nextSegXRatio, segYRatio ) );
            uvs.push_back( Vector2F( nextSegXRatio, nextSegYRatio ) );

            uvs.push_back( Vector2F( nextSegXRatio, nextSegYRatio ) );
            uvs.push_back( Vector2F( segXRatio, nextSegYRatio ) );
            uvs.push_back( Vector2F( segXRatio, segYRatio ) );

            indices.push_back( index++ );
            indices.push_back( index++ );
            indices.push_back( index++ );

            indices.push_back( index++ );
            indices.push_back( index++ );
            indices.push_back( index++ );
        }

        return MeshGenerator::createMesh( vertices, normals, uvs, indices );
    }

    SmartPtr<IMesh> MeshGenerator::createMeshFromSplines( const Array<LinearSpline3F> &splines )
    {
        return createCylinder();
        return nullptr;
    }
}  // end namespace fb
