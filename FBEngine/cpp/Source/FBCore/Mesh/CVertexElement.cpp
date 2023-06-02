#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/CVertexElement.h>
#include <FBCore/Memory/PointerUtil.h>

namespace fb
{
    CVertexElement::CVertexElement()
    {
    }

    CVertexElement::CVertexElement( u16 source, u32 size, u32 offset,
                                    IVertexDeclaration::VertexElementSemantic semantic,
                                    VertexElementType type, u8 index ) :
        m_source( source ),
        m_size( size )
    {
        m_offset = offset;
        m_semantic = static_cast<u32>(semantic);
        m_type = type;
        m_index = index;
    }

    CVertexElement::~CVertexElement()
    {
    }

    void CVertexElement::getElementData( void *vertexData, void **elementData ) const
    {
        *elementData = static_cast<void *>(static_cast<unsigned char *>(vertexData) + m_offset);
    }

    void CVertexElement::getElementData( void *vertexData, f32 **elementData ) const
    {
        // The only way we can do this is to cast to char* in order to use byte offset
        // then cast back to float*. However we have to go via void* because casting
        // directly is not allowed
        *elementData = static_cast<float *>(
            static_cast<void *>(static_cast<unsigned char *>(vertexData) + m_offset));
    }

    u32 CVertexElement::getOffset() const
    {
        return m_offset;
    }

    u32 CVertexElement::getSemantic() const
    {
        return m_semantic;
    }

    IVertexElement::VertexElementType CVertexElement::getType() const
    {
        return m_type;
    }

    u8 CVertexElement::getIndex() const
    {
        return m_index;
    }

    void CVertexElement::setSource( u16 source )
    {
        m_source = source;
    }

    void CVertexElement::setSize( u32 size )
    {
        m_size = size;
    }

    void CVertexElement::setOffset( u32 offset )
    {
        m_offset = offset;
    }

    void CVertexElement::setSemantic( u32 semantic )
    {
        m_semantic = semantic;
    }

    void CVertexElement::setType( VertexElementType type )
    {
        m_type = type;
    }

    void CVertexElement::setIndex( u8 index )
    {
        m_index = index;
    }

    bool CVertexElement::compare( SmartPtr<IVertexElement> other ) const
    {
        return *this == *fb::static_pointer_cast<CVertexElement>( other );
    }

    bool CVertexElement::operator==( const CVertexElement &rhs ) const
    {
        if(m_type != rhs.m_type || m_index != rhs.m_index || m_offset != rhs.m_offset ||
           m_semantic != rhs.m_semantic || m_source != rhs.m_source)
        {
            return false;
        }

        return true;
    }

    u32 CVertexElement::getSize() const
    {
        return m_size;
    }

    u16 CVertexElement::getSource() const
    {
        return m_source;
    }
} // end namespace fb
