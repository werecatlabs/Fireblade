#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/CVertexBuffer.h>
#include <FBCore/FBCore.h>

namespace fb
{
    CVertexBuffer::CVertexBuffer() :
        m_vertexData( 0 ),
        m_numVerticies( 0 )
    {
    }

    CVertexBuffer::~CVertexBuffer()
    {
        unload( nullptr );
    }

    void CVertexBuffer::unload( SmartPtr<ISharedObject> data )
    {
        for(u32 i = 0; i < m_vertexData.size(); ++i)
        {
            u8 *data = m_vertexData[i];
            if(data)
            {
                delete[] data;
                m_vertexData[i] = nullptr;
            }
        }

        m_vertexData.clear();

        if(m_vertexDeclaration)
        {
            m_vertexDeclaration->unload( nullptr );
            m_vertexDeclaration = nullptr;
        }
    }

    void CVertexBuffer::setVertexDeclaration( SmartPtr<IVertexDeclaration> vertexDeclaration )
    {
        m_vertexDeclaration = vertexDeclaration;
    }

    SmartPtr<IVertexDeclaration> CVertexBuffer::getVertexDeclaration() const
    {
        return m_vertexDeclaration;
    }

    void CVertexBuffer::setNumVerticies( u32 numVerticies )
    {
        m_numVerticies = numVerticies;
    }

    u32 CVertexBuffer::getNumVerticies() const
    {
        return m_numVerticies;
    }

    void *CVertexBuffer::createVertexData( s32 index )
    {
        if(m_vertexDeclaration)
        {
            auto newSize = index + 1;
            if(newSize >= static_cast<s32>(m_vertexData.size()))
            {
                auto copy = m_vertexData;
                m_vertexData.resize( newSize );

                for(u32 i = 0; i < copy.size(); ++i)
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

    void *CVertexBuffer::getVertexData( int index ) const
    {
        return m_vertexData[index];
    }

    Array<u8 *> CVertexBuffer::getDataArray() const
    {
        return m_vertexData;
    }

    SmartPtr<IVertexBuffer> CVertexBuffer::clone() const
    {
        auto newVertexBuffer = fb::make_ptr<CVertexBuffer>();

        newVertexBuffer->setVertexDeclaration( m_vertexDeclaration->clone() );
        newVertexBuffer->setNumVerticies( m_numVerticies );
        newVertexBuffer->createVertexData();

        u32 numBytes = m_vertexDeclaration->getSize() * m_numVerticies;
        Memory::Memcpy( newVertexBuffer->m_vertexData[0], m_vertexData[0], numBytes );

        return newVertexBuffer;
    }

    bool CVertexBuffer::compare( SmartPtr<IVertexBuffer> other ) const
    {
        if(m_numVerticies != other->getNumVerticies())
        {
            return false;
        }

        return m_vertexDeclaration->compare( other->getVertexDeclaration() );
    }
} // end namespace fb
