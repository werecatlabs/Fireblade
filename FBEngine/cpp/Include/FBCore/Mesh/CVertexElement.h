#ifndef VertexElement_h__
#define VertexElement_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/Mesh/IVertexElement.h>
#include <FBCore/Interface/Mesh/IVertexDeclaration.h>

namespace fb
{
    //---------------------------------------------
    class CVertexElement : public CSharedObject<IVertexElement>
    {
    public:
        CVertexElement();

        CVertexElement( u16 source, u32 size, u32 offset,
                        IVertexDeclaration::VertexElementSemantic semantic,
                        VertexElementType type, u8 index = 0 );

        ~CVertexElement() override;

        void getElementData( void *vertexData, void **elementData ) const override;
        void getElementData( void *vertexData, f32 **elementData ) const override;

        u16 getSource() const override;
        u32 getSize() const override;
        u32 getOffset() const override;
        u32 getSemantic() const override;
        VertexElementType getType() const override;
        u8 getIndex() const override;

        void setSource( u16 source ) override;
        void setSize( u32 size ) override;
        void setOffset( u32 offset ) override;
        void setSemantic( u32 semantic ) override;
        void setType( VertexElementType type ) override;
        void setIndex( u8 index ) override;

        bool operator==( const CVertexElement &rhs ) const;

        bool compare( SmartPtr<IVertexElement> other ) const override;

    private:
        u32 m_size;
        u32 m_offset;
        u32 m_semantic;
        VertexElementType m_type;
        u16 m_source;
        u8 m_index; // Index of the item, only applicable for some elements like texture coords
    };
} // end namespace fb

#endif  // VertexElement_h__
