#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/CIndexBuffer.h>
#include <FBCore/FBCore.h>

namespace fb
{
    CIndexBuffer::CIndexBuffer() :
        m_indexType( Type::IT_32BIT ),
        m_numIndexes( 0 ),
        m_indexSize( 0 ),
        m_indexData( nullptr )
    {
        m_indexSize = sizeof( u32 );
    }

    CIndexBuffer::~CIndexBuffer()
    {
        unload( nullptr );
    }

    void CIndexBuffer::unload( SmartPtr<ISharedObject> data )
    {
        if( m_indexData )
        {
            delete[] m_indexData;
            m_indexData = nullptr;
        }
    }

    void CIndexBuffer::setIndexType( Type indexType )
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

    IIndexBuffer::Type CIndexBuffer::getIndexType() const
    {
        return m_indexType;
    }

    void CIndexBuffer::setNumIndices( u32 numIndices )
    {
        m_numIndexes = numIndices;
    }

    u32 CIndexBuffer::getNumIndices() const
    {
        return m_numIndexes;
    }

    u32 CIndexBuffer::getIndexSize() const
    {
        return m_indexSize;
    }

    void *CIndexBuffer::createIndexData()
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

    void *CIndexBuffer::getIndexData() const
    {
        return m_indexData;
    }

    SmartPtr<IIndexBuffer> CIndexBuffer::clone() const
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto factoryManager = applicationManager->getFactoryManager();

        auto indexBuffer = factoryManager->make_ptr<CIndexBuffer>();

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

    void CIndexBuffer::setIndexSize( u32 size )
    {
        m_indexSize = size;
    }

    bool CIndexBuffer::compare( SmartPtr<IIndexBuffer> other ) const
    {
        return false;
    }
}  // end namespace fb
