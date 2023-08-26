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
    SubMesh::SubMesh()
    {
    }

    SubMesh::~SubMesh()
    {
    }

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

    String SubMesh::getMaterialName() const
    {
        return m_materialName;
    }

    void SubMesh::setVertexBuffer( SmartPtr<IVertexBuffer> vertexBuffer )
    {
        m_vertexBuffer = vertexBuffer;
    }

    SmartPtr<IVertexBuffer> SubMesh::getVertexBuffer() const
    {
        return m_vertexBuffer;
    }

    void SubMesh::setIndexBuffer( SmartPtr<IIndexBuffer> indexBuffer )
    {
        m_indexBuffer = indexBuffer;
    }

    SmartPtr<IIndexBuffer> SubMesh::getIndexBuffer() const
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

    AABB3F SubMesh::getAABB() const
    {
        return m_aabb;
    }

    SmartPtr<ISubMesh> SubMesh::clone() const
    {
        auto subMesh = fb::make_ptr<SubMesh>();

        subMesh->setMaterialName( m_materialName );
        subMesh->setVertexBuffer( m_vertexBuffer->clone() );
        subMesh->setIndexBuffer( m_indexBuffer->clone() );
        subMesh->setAABB( m_aabb );

        return subMesh;
    }

    bool SubMesh::getUseSharedVertices() const
    {
        return m_useSharedVertices;
    }

    void SubMesh::setUseSharedVertices( bool useSharedVertices )
    {
        m_useSharedVertices = useSharedVertices;
    }

    bool SubMesh::compare( SmartPtr<ISubMesh> other ) const
    {
        return m_vertexBuffer->compare( other->getVertexBuffer() ) &&
               m_indexBuffer->compare( other->getIndexBuffer() );
    }

    void SubMesh::setAABB( const AABB3F &aabb )
    {
        m_aabb = aabb;
    }
}  // end namespace fb
