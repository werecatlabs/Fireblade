#ifndef CVertexDeclaration_h__
#define CVertexDeclaration_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Interface/Mesh/IVertexDeclaration.h>

namespace fb
{
    //---------------------------------------------
    class CVertexDeclaration : public CSharedObject<IVertexDeclaration>
    {
    public:
        CVertexDeclaration();
        ~CVertexDeclaration() override;

        void unload( SmartPtr<ISharedObject> data ) override;

        SmartPtr<IVertexElement> addElement( u16 source, u32 offset,
                                             VertexElementSemantic elementSemantic,
                                             IVertexElement::VertexElementType elementType,
                                             u32 index = 0 ) override;
        u32 getSize( u16 source = 0 ) const override;

        SmartPtr<IVertexElement> findElementBySemantic(
            VertexElementSemantic elementSemantic, u32 index = 0 ) override;

        Array<SmartPtr<IVertexElement>> findElementsBySource( u16 source ) const override;

        SmartPtr<IVertexDeclaration> clone() const override;

        Array<SmartPtr<IVertexElement>> &getElements() override;
        const Array<SmartPtr<IVertexElement>> &getElements() const override;

        bool compare( SmartPtr<IVertexDeclaration> other ) const override;

    private:
        Array<SmartPtr<IVertexElement>> m_elements;
    };
} // end namespace fb

#endif  // CVertexDeclaration_h__
