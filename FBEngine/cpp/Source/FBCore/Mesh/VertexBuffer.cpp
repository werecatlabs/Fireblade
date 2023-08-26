#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/VertexBuffer.h>
#include <FBCore/FBCore.h>

namespace fb
{
    VertexBuffer::VertexBuffer() : m_vertexData( 0 ), m_numVerticies( 0 )
    {
    }

    VertexBuffer::~VertexBuffer()
    {
        unload( nullptr );
    }

    void VertexBuffer::unload( SmartPtr<ISharedObject> data )
    {
        for( u32 i = 0; i < m_vertexData.size(); ++i )
        {
            u8 *data = m_vertexData[i];
            if( data )
            {
                delete[] data;
                m_vertexData[i] = nullptr;
            }
        }

        m_vertexData.clear();

        if( m_vertexDeclaration )
        {
            m_vertexDeclaration->unload( nullptr );
            m_vertexDeclaration = nullptr;
        }
    }

    void VertexBuffer::setVertexDeclaration( SmartPtr<IVertexDeclaration> vertexDeclaration )
    {
        m_vertexDeclaration = vertexDeclaration;
    }

    SmartPtr<IVertexDeclaration> VertexBuffer::getVertexDeclaration() const
    {
        return m_vertexDeclaration;
    }

    void VertexBuffer::setNumVerticies( u32 numVerticies )
    {
        m_numVerticies = numVerticies;
    }

    u32 VertexBuffer::getNumVerticies() const
    {
        return m_numVerticies;
    }

    void *VertexBuffer::createVertexData( s32 index )
    {
        if( m_vertexDeclaration )
        {
            auto newSize = index + 1;
            if( newSize >= static_cast<s32>( m_vertexData.size() ) )
            {
                auto copy = m_vertexData;
                m_vertexData.resize( newSize );

                for( u32 i = 0; i < copy.size(); ++i )
                {
                    m_vertexData[i] = copy[i];
                }
            }

            auto numBytes = m_vertexDeclaration->getSize( index ) * m_numVerticies;
            auto buffer = new u8[numBytes];
            m_vertexData[index] = buffer;
            return buffer;
        }

        return nullptr;
    }

    void *VertexBuffer::getVertexData( int index ) const
    {
        return m_vertexData[index];
    }

    Array<u8 *> VertexBuffer::getDataArray() const
    {
        return m_vertexData;
    }

    SmartPtr<IVertexBuffer> VertexBuffer::clone() const
    {
        auto newVertexBuffer = fb::make_ptr<VertexBuffer>();

        newVertexBuffer->setVertexDeclaration( m_vertexDeclaration->clone() );
        newVertexBuffer->setNumVerticies( m_numVerticies );
        newVertexBuffer->createVertexData();

        u32 numBytes = m_vertexDeclaration->getSize() * m_numVerticies;
        Memory::Memcpy( newVertexBuffer->m_vertexData[0], m_vertexData[0], numBytes );

        return newVertexBuffer;
    }

    bool VertexBuffer::compare( SmartPtr<IVertexBuffer> other ) const
    {
        if( m_numVerticies != other->getNumVerticies() )
        {
            return false;
        }

        return m_vertexDeclaration->compare( other->getVertexDeclaration() );
    }
}  // end namespace fb
