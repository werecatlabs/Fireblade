#include <FBMesh/FBMeshPCH.h>
#include "FBMesh/CSubMesh.h"
#include <FBCore/Math/Vector3.h>
#include <FBCore/Interface/Mesh/IVertexBuffer.h>
#include <FBCore/Interface/Mesh/IIndexBuffer.h>
#include <FBCore/Interface/Mesh/IVertexDeclaration.h>
#include <FBCore/Interface/Mesh/IVertexElement.h>

namespace fb
{
    CSubMesh::CSubMesh()
    {
    }

    CSubMesh::~CSubMesh()
    {
    }

    void CSubMesh::unload( SmartPtr<ISharedObject> data )
    {
        if( m_vertexBuffer )
        {
            m_vertexBuffer->unload( nullptr );
            m_vertexBuffer = nullptr;
        }

        if( m_indexBuffer )
        {
            m_indexBuffer->unload( nullptr );
            m_indexBuffer = nullptr;
        }
    }

    void CSubMesh::setMaterialName( const String &materialName )
    {
        m_materialName = materialName;
    }

    String CSubMesh::getMaterialName() const
    {
        return m_materialName;
    }

    void CSubMesh::setVertexBuffer( SmartPtr<IVertexBuffer> vertexBuffer )
    {
        m_vertexBuffer = vertexBuffer;
    }

    SmartPtr<IVertexBuffer> CSubMesh::getVertexBuffer() const
    {
        return m_vertexBuffer;
    }

    void CSubMesh::setIndexBuffer( SmartPtr<IIndexBuffer> indexBuffer )
    {
        m_indexBuffer = indexBuffer;
    }

    SmartPtr<IIndexBuffer> CSubMesh::getIndexBuffer() const
    {
        return m_indexBuffer;
    }

    void CSubMesh::updateAABB()
    {
        auto vertexBuffer = getVertexBuffer();
        auto vertexData = static_cast<u8 *>( vertexBuffer->getVertexData() );
        auto vertexDec = vertexBuffer->getVertexDeclaration();
        auto posElem =
            vertexDec->findElementBySemantic( IVertexDeclaration::VertexElementSemantic::VES_POSITION );

        auto numVerticies = vertexBuffer->getNumVerticies();

        if( numVerticies > 0 )
        {
            m_aabb.setMinimum( Vector3F( 1e10, 1e10, 1e10 ) );
            m_aabb.setMaximum( Vector3F( -1e10, -1e10, -1e10 ) );
        }
        else
        {
            m_aabb.reset( Vector3F::zero() );
        }

        for( u32 j = 0; j < numVerticies; ++j, vertexData += vertexDec->getSize() )
        {
            f32 *pReal = nullptr;
            posElem->getElementData( vertexData, &pReal );
            Vector3F position( pReal[0], pReal[1], pReal[2] );

            m_aabb.merge( position );
        }
    }

    AABB3F CSubMesh::getAABB() const
    {
        return m_aabb;
    }

    SmartPtr<ISubMesh> CSubMesh::clone() const
    {
        auto subMesh = fb::make_ptr<CSubMesh>();

        subMesh->setMaterialName( m_materialName );
        subMesh->setVertexBuffer( m_vertexBuffer->clone() );
        subMesh->setIndexBuffer( m_indexBuffer->clone() );
        subMesh->setAABB( m_aabb );

        return subMesh;
    }

    bool CSubMesh::getUseSharedVertices() const
    {
        return m_useSharedVertices;
    }

    void CSubMesh::setUseSharedVertices( bool useSharedVertices )
    {
        m_useSharedVertices = useSharedVertices;
    }

    bool CSubMesh::compare( SmartPtr<ISubMesh> other ) const
    {
        return m_vertexBuffer->compare( other->getVertexBuffer() ) &&
               m_indexBuffer->compare( other->getIndexBuffer() );
    }

    void CSubMesh::setAABB( const AABB3F &aabb )
    {
        m_aabb = aabb;
    }
}  // end namespace fb
