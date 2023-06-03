#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/IndexBuffer.h>
#include <FBCore/FBCore.h>

namespace fb
{
    IndexBuffer::IndexBuffer() :
        m_indexType( Type::IT_32BIT ),
        m_numIndexes( 0 ),
        m_indexSize( 0 ),
        m_indexData( nullptr )
    {
        m_indexSize = sizeof( u32 );
    }

    IndexBuffer::~IndexBuffer()
    {
        unload( nullptr );
    }

    void IndexBuffer::unload( SmartPtr<ISharedObject> data )
    {
        if( m_indexData )
        {
            delete[] m_indexData;
            m_indexData = nullptr;
        }
    }

    void IndexBuffer::setIndexType( Type indexType )
    {
        m_indexType = indexType;

        switch( m_indexType )
        {
        case Type::IT_16BIT:
            m_indexSize = sizeof( u16 );
            break;
        case Type::IT_32BIT:
            m_indexSize = sizeof( u32 );
            break;
        default:
        {
        }
        }
    }

    IIndexBuffer::Type IndexBuffer::getIndexType() const
    {
        return m_indexType;
    }

    void IndexBuffer::setNumIndices( u32 numIndices )
    {
        m_numIndexes = numIndices;
    }

    u32 IndexBuffer::getNumIndices() const
    {
        return m_numIndexes;
    }

    u32 IndexBuffer::getIndexSize() const
    {
        return m_indexSize;
    }

    void *IndexBuffer::createIndexData()
    {
        auto indexType = getIndexType();
        switch( indexType )
        {
        case Type::IT_16BIT:
        {
            m_indexData = new u16[m_numIndexes];
        }
        break;
        case Type::IT_32BIT:
        {
            m_indexData = new u32[m_numIndexes];
        }
        break;
        default:
        {
        }
        };

        return m_indexData;
    }

    void *IndexBuffer::getIndexData() const
    {
        return m_indexData;
    }

    SmartPtr<IIndexBuffer> IndexBuffer::clone() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        auto indexBuffer = factoryManager->make_ptr<IndexBuffer>();

        indexBuffer->setIndexType( m_indexType );
        indexBuffer->setNumIndices( m_numIndexes );
        indexBuffer->setIndexSize( m_indexSize );
        indexBuffer->createIndexData();

        switch( m_indexType )
        {
        case Type::IT_16BIT:
        {
            auto indices = static_cast<u16 *>( m_indexData );
            auto newIndices = static_cast<u16 *>( indexBuffer->getIndexData() );
            for( u32 i = 0; i < m_numIndexes; ++i )
            {
                newIndices[i] = indices[i];
            }
        }
        break;
        case Type::IT_32BIT:
        {
            auto indices = static_cast<u32 *>( m_indexData );
            auto newIndices = static_cast<u32 *>( indexBuffer->getIndexData() );
            for( u32 i = 0; i < m_numIndexes; ++i )
            {
                newIndices[i] = indices[i];
            }
        }
        break;
        default:
        {
            FB_ASSERT( false );
        }
        }

        return indexBuffer;
    }

    void IndexBuffer::setIndexSize( u32 size )
    {
        m_indexSize = size;
    }

    bool IndexBuffer::compare( SmartPtr<IIndexBuffer> other ) const
    {
        return false;
    }
}  // end namespace fb
