#ifndef __FBVertexBuffer__H
#define __FBVertexBuffer__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/FBCoreTypes.h>
#include <FBCore/Memory/CSharedObject.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Interface/Mesh/IVertexBuffer.h>

namespace fb
{
    //---------------------------------------------
    class CVertexBuffer : public CSharedObject<IVertexBuffer>
    {
    public:
        CVertexBuffer();
        ~CVertexBuffer() override;

        /** @copydoc IObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        void setVertexDeclaration( SmartPtr<IVertexDeclaration> vertexDeclaration ) override;
        SmartPtr<IVertexDeclaration> getVertexDeclaration() const override;

        void setNumVerticies( u32 numVerticies ) override;
        u32 getNumVerticies() const override;

        void *createVertexData( s32 index = 0 ) override;
        void *getVertexData( s32 index = 0 ) const override;

        Array<u8 *> getDataArray() const override;

        SmartPtr<IVertexBuffer> clone() const override;

        bool compare( SmartPtr<IVertexBuffer> other ) const override;

    protected:
        SmartPtr<IVertexDeclaration> m_vertexDeclaration;
        Array<u8 *> m_vertexData;
        u32 m_numVerticies = 0;
    };
} // end namespace fb

#endif
