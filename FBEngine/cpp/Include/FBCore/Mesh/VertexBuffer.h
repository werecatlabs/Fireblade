#ifndef __FBVertexBuffer__H
#define __FBVertexBuffer__H

#include <FBCore/Interface/Mesh/IVertexBuffer.h>
#include <FBCore/Core/Array.h>

namespace fb
{

    /** Implementation of a vertex buffer. */
    class VertexBuffer : public IVertexBuffer
    {
    public:
        /** Constructor. */
        VertexBuffer();

        /** Destructor. */
        ~VertexBuffer() override;

        /** @copydoc IVertexBuffer::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        /** @copydoc IVertexBuffer::setVertexDeclaration */
        void setVertexDeclaration( SmartPtr<IVertexDeclaration> vertexDeclaration ) override;

        /** @copydoc IVertexBuffer::getVertexDeclaration */
        SmartPtr<IVertexDeclaration> getVertexDeclaration() const override;

        /** @copydoc IVertexBuffer::setNumVertices */
        void setNumVertices( u32 numVertices ) override;

        /** @copydoc IVertexBuffer::getNumVertices */
        u32 getNumVertices() const override;

        /** @copydoc IVertexBuffer::createVertexData */
        void *createVertexData( s32 index = 0 ) override;

        /** @copydoc IVertexBuffer::getVertexData */
        void *getVertexData( s32 index = 0 ) const override;

        /** @copydoc IVertexBuffer::getDataArray */
        Array<u8 *> getDataArray() const override;

        /** @copydoc IVertexBuffer::clone */
        SmartPtr<IVertexBuffer> clone() const override;

        /** @copydoc IVertexBuffer::compare */
        bool compare( SmartPtr<IVertexBuffer> other ) const override;

        FB_CLASS_REGISTER_DECL;

    protected:
        // The vertex declaration.
        SmartPtr<IVertexDeclaration> m_vertexDeclaration;

        // The vertex data.
        Array<u8 *> m_vertexData;

        // The number of vertices.
        u32 m_numVertices = 0;
    };
}  // end namespace fb

#endif
