#include <FBCore/FBCorePCH.h>
#include "FBCore/Mesh/SubMesh.h"
#include <FBCore/Math/Vector3.h>
#include <FBCore/Interface/Mesh/IVertexBuffer.h>
#include <FBCore/Interface/Mesh/IIndexBuffer.h>
#include <FBCore/Interface/Mesh/IVertexDeclaration.h>
#include <FBCore/Interface/Mesh/IVertexElement.h>
#include "FBCore/Memory/PointerUtil.h"

namespace fb
{
    SubMesh::SubMesh() = default;

    SubMesh::~SubMesh() = default;

    void SubMesh::unload( SmartPtr<ISharedObject> data )
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

    void SubMesh::setMaterialName( const String &materialName )
    {
        m_materialName = materialName;
    }

    auto SubMesh::getMaterialName() const -> String
    {
        return m_materialName;
    }

    void SubMesh::setVertexBuffer( SmartPtr<IVertexBuffer> vertexBuffer )
    {
        m_vertexBuffer = vertexBuffer;
    }

    auto SubMesh::getVertexBuffer() const -> SmartPtr<IVertexBuffer>
    {
        return m_vertexBuffer;
    }

    void SubMesh::setIndexBuffer( SmartPtr<IIndexBuffer> indexBuffer )
    {
        m_indexBuffer = indexBuffer;
    }

    auto SubMesh::getIndexBuffer() const -> SmartPtr<IIndexBuffer>
    {
        return m_indexBuffer;
    }

    void SubMesh::updateAABB()
    {
        auto vertexBuffer = getVertexBuffer();
        auto vertexData = static_cast<u8 *>( vertexBuffer->getVertexData() );
        auto vertexDec = vertexBuffer->getVertexDeclaration();
        auto posElem =
            vertexDec->findElementBySemantic( IVertexDeclaration::VertexElementSemantic::VES_POSITION );

        auto numVerticies = vertexBuffer->getNumVertices();

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

    auto SubMesh::getAABB() const -> AABB3F
    {
        return m_aabb;
    }

    auto SubMesh::clone() const -> SmartPtr<ISubMesh>
    {
        auto subMesh = fb::make_ptr<SubMesh>();

        subMesh->setMaterialName( m_materialName );
        subMesh->setVertexBuffer( m_vertexBuffer->clone() );
        subMesh->setIndexBuffer( m_indexBuffer->clone() );
        subMesh->setAABB( m_aabb );

        return subMesh;
    }

    auto SubMesh::getUseSharedVertices() const -> bool
    {
        return m_useSharedVertices;
    }

    void SubMesh::setUseSharedVertices( bool useSharedVertices )
    {
        m_useSharedVertices = useSharedVertices;
    }

    auto SubMesh::compare( SmartPtr<ISubMesh> other ) const -> bool
    {
        return m_vertexBuffer->compare( other->getVertexBuffer() ) &&
               m_indexBuffer->compare( other->getIndexBuffer() );
    }

    void SubMesh::setAABB( const AABB3F &aabb )
    {
        m_aabb = aabb;
    }
}  // end namespace fb
