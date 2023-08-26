#include <FBCore/FBCorePCH.h>
#include "FBCore/Mesh/CollisionSubMesh.h"
#include <FBCore/Core/Exception.h>
#include "FBCore/Mesh/SubMesh.h"
#include "FBCore/Mesh/Mesh.h"
#include "FBCore/Mesh/VertexDeclaration.h"

#if FB_USE_OPCODE_LIB
#    include <opcode/Opcode.h>
#endif

namespace fb
{
#if FB_USE_OPCODE_LIB
    //--------------------------------------------
    static void callback( unsigned int triangle_index, Opcode::VertexPointers &triangle, void *userData )
    {
        // CollisionSubMesh* mesh = (CollisionSubMesh*)userData;

        // triangle_index *= 3;

        // u32 indexCount = mesh->m_indexCount;

        // int v1i, v2i, v3i;
        // v1i = mesh->m_indices[triangle_index];
        // v2i = mesh->m_indices[triangle_index + 1];
        // v3i = mesh->m_indices[triangle_index + 2];

        // u32 vertexCount = mesh->m_vertexCount;

        // triangle.Vertex[0] = &mesh->m_points[v1i];
        // triangle.Vertex[1] = &mesh->m_points[v2i];
        // triangle.Vertex[2] = &mesh->m_points[v3i];
    }
#endif

    //--------------------------------------------
    CollisionSubMesh::CollisionSubMesh( const SmartPtr<IMesh> mesh, SmartPtr<ISubMesh> subMesh,
                                        Matrix4F transform )
    {
        if( mesh )
        {
            return;
        }

#if FB_USE_OPCODE_LIB
        m_rayCollider = new Opcode::RayCollider;
        m_tree = new Opcode::Model;
        m_mesh = new Opcode::MeshInterface;

        if( !mesh->getHasSharedVertexData() )
        {
            SmartPtr<IVertexBuffer> vertexBuffer = subMesh->getVertexBuffer();
            SmartPtr<IIndexBuffer> indexBuffer = subMesh->getIndexBuffer();

            u32 meshIndexCnt = indexBuffer->getNumIndices();
            u32 meshVertexCnt = vertexBuffer->getNumVerticies();

            float *vertices = new float[meshVertexCnt * 3];
            u32 *indices = new u32[meshIndexCnt];

            // fill indices
            switch( indexBuffer->getIndexType() )
            {
            case IndexBuffer::Type::IT_16BIT:
            {
                const u16 *indexData = static_cast<u16 *>( indexBuffer->getIndexData() );
                const u32 indexCount = indexBuffer->getNumIndices();

                for( u32 j = 0; j < indexCount; j++ )
                {
                    u32 index = indexData[j];
                    indices[j] = index;
                }
            }
            break;
            case IndexBuffer::Type::IT_32BIT:
            {
                const u32 *indexData = static_cast<u32 *>( indexBuffer->getIndexData() );
                const u32 indexCount = indexBuffer->getNumIndices();

                for( u32 j = 0; j < indexCount; j++ )
                {
                    u32 index = indexData[j];
                    indices[j] = index;
                }
            }
            break;
            default:
            {
                throw Exception( "Unknown index type." );
            }
            };

            // fill vertices
            {
                const VertexElement *posElem =
                    vertexBuffer->getVertexDeclaration()->findElementBySemantic(
                        VertexDeclaration::VertexElementSemantic::VES_POSITION, 0 );

                u32 numVerticies = vertexBuffer->getNumVerticies();
                u32 vertexSize = vertexBuffer->getVertexDeclaration()->getSize();
                u8 *vertexDataPtr = (u8 *)vertexBuffer->getVertexData();
                float *elementData = 0;

                for( u32 j = 0; j < numVerticies; ++j, vertexDataPtr += vertexSize )
                {
                    posElem->getElementData( vertexDataPtr, &elementData );

                    Vector3F position;
                    position.X() = *elementData++;
                    position.Y() = *elementData++;
                    position.Z() = *elementData++;
                    position = transform * position;

                    vertices[j * 3] = position.X();
                    vertices[j * 3 + 1] = position.Y();
                    vertices[j * 3 + 2] = position.Z();
                }
            }

            build( vertices, 3 * sizeof( float ), meshVertexCnt, indices, meshIndexCnt );

            delete[] vertices;
        }
        else
        {
            // u32 meshIndexCnt = 0;
            // u32 meshVertexCnt = 0;

            //// count vertices and indices

            // meshIndexCnt += subMesh->indexData->indexCount;
            // meshVertexCnt += mesh->sharedVertexData->vertexCount;

            // float* vertices = new float[meshVertexCnt * 3];
            // u32* indices = new u32[meshIndexCnt];

            // int ci = 0;
            // int cif = 0;
            // int cv = 0;

            // Ogre::IndexData* indexBuffer = subMesh->indexData;

            //// fill indices
            // switch ( indexBuffer->indexBuffer->getType() )
            //{
            //	case IndexBuffer::IT_16BIT:
            //	{
            //		const u16* indexData =
            //static_cast<u16*>(indexBuffer->indexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            //		const u32 indexCount = subMesh->indexData->indexCount;
            //		for ( u32 j = 0; j < indexCount; j++ )
            //		{
            //			u32 index = indexData[j];
            //			indices[ci] = (u16)cif + index;
            //			ci++;
            //		}

            //		indexBuffer->indexBuffer->unlock();
            //	}
            //	break;
            //	case IndexBuffer::IT_32BIT:
            //	{
            //		const u32* indexData =
            //static_cast<u32*>(indexBuffer->indexBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            //		const u32 indexCount = subMesh->indexData->indexCount;
            //		for ( u32 j = 0; j < indexCount; j++ )
            //		{
            //			u32 index = indexData[j];
            //			indices[ci] = (u32)cif + index;
            //			ci++;
            //		}

            //		indexBuffer->indexBuffer->unlock();
            //	}
            //	break;
            //	default:
            //	{
            //		throw Exception("Unknown index type.");
            //	}
            //}

            //// fill vertices

            // Ogre::VertexData* vertexData = mesh->sharedVertexData;
            // const Ogre::VertexElement* posElem =
            // vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION, 0);
            // Ogre::HardwareVertexBufferSharedPtr vbuf =
            //	vertexData->vertexBufferBinding->getBuffer(posElem->getSource());

            // u32 numVerticies = vertexData->vertexCount;
            // u32 vertexSize = vertexData->vertexDeclaration->getVertexSize(0);
            // u8* vertexDataPtr = static_cast<u8*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            // float* elementData = 0;

            // for ( u32 j = 0; j < numVerticies; ++j, vertexDataPtr += vertexSize )
            //{
            //	posElem->baseVertexPointerToElement(vertexDataPtr, &elementData);

            //	Ogre::Vector3 position;
            //	position.x = *elementData++;
            //	position.y = *elementData++;
            //	position.z = *elementData++;
            //	position = transform * position;

            //	vertices[cv * 3] = position.x;
            //	vertices[cv * 3 + 1] = position.y;
            //	vertices[cv * 3 + 2] = position.z;
            //	cv++;
            //}

            // vbuf->unlock();

            // build(vertices, 3 * sizeof(float), meshVertexCnt, indices, meshIndexCnt);

            // delete[] vertices;
        }
#endif
    }

    CollisionSubMesh::CollisionSubMesh()
    {
    }

    //--------------------------------------------
    CollisionSubMesh::~CollisionSubMesh()
    {
#if FB_USE_OPCODE_LIB
        FB_SAFE_DELETE_ARRAY( m_points );
        FB_SAFE_DELETE_ARRAY( m_indices );
#endif
    }

    //--------------------------------------------
    void CollisionSubMesh::build( float *vertices, int vertexStide, int vertexCount, const void *indices,
                                  int indexCount )
    {
#if FB_USE_OPCODE_LIB
        using namespace Opcode;

        m_indices = (u32 *)indices;
        m_vertexCount = (u32)vertexCount;
        m_indexCount = (u32)indexCount;
        m_triangleCount = (u32)( indexCount / 3 );

        m_mesh->SetNbTriangles( m_triangleCount );
        m_mesh->SetNbVertices( vertexCount );
        m_mesh->SetCallback( callback, this );

        m_points = new IceMaths::Point[vertexCount];

        int temp_vertex_counter = 0;
        for( int i = 0; i < vertexCount; ++i )
        {
            Vector3F vec;
            m_points[i] =
                IceMaths::HPoint( vertices[i * 3], vertices[( i * 3 ) + 1], vertices[( i * 3 ) + 2] );
        }

        BuildSettings buildSettings;
        buildSettings.mRules = SPLIT_BEST_AXIS | SPLIT_SPLATTER_POINTS | SPLIT_GEOM_CENTER;
        buildSettings.mLimit = 1;

        OPCODECREATE treeBuilder;
        treeBuilder.mIMesh = m_mesh;
        treeBuilder.mSettings = buildSettings;
        treeBuilder.mNoLeaf = false;
        treeBuilder.mQuantized = true;
        treeBuilder.mKeepOriginal = false;
        treeBuilder.mCanRemap = false;

        if( !m_tree->Build( treeBuilder ) )
        {
            printf( "Could not build opcode mesh" );
        }
#endif
    }

    //--------------------------------------------
    bool CollisionSubMesh::rayCast( const Vector3F &origin, const Vector3F &dir, Array<float> &hits )
    {
        // IceMaths::Ray worldRay;
        // worldRay.mOrig.x = origin.x;
        // worldRay.mOrig.y = origin.y;
        // worldRay.mOrig.z = origin.z;
        // worldRay.mDir.x = dir.x;
        // worldRay.mDir.y = dir.y;
        // worldRay.mDir.z = dir.z;

        // Opcode::CollisionFaces CF;
        // CF.Reset();

        // m_rayCollider.SetDestination(&CF);
        // m_rayCollider.SetFirstContact(true);
        // m_rayCollider.SetTemporalCoherence(true);
        // m_rayCollider.SetClosestHit(true);
        // m_rayCollider.SetCulling(true);

        // BOOL IsOk = m_rayCollider.Collide(worldRay, m_tree);
        // BOOL Status = m_rayCollider.GetContactStatus();

        // if (Status)
        //{
        //	int NbTouchedPrimitives = CF.GetNbFaces();
        //	const Opcode::CollisionFace* faces = CF.GetFaces();

        //	if (NbTouchedPrimitives > 0)
        //	{
        //		for (int i = 0; i < NbTouchedPrimitives; ++i)
        //		{
        //			hits.push_back(faces[i].mDistance);
        //		}

        //		return true;
        //	}
        //}

        return false;
    }

    void *CollisionSubMesh::getUserData() const
    {
        return m_userData;
    }

    void CollisionSubMesh::setUserData( void *val )
    {
        m_userData = val;
    }
}  // namespace fb
