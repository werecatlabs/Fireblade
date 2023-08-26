#ifndef __FBVertexBuffer__H
#define __FBVertexBuffer__H

#include <FBCore/Interface/Mesh/IVertexBuffer.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    class VertexBuffer : public IVertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer() override;

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
