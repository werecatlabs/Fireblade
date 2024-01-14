#ifndef __FBIndexBuffer__H
#define __FBIndexBuffer__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Mesh/IIndexBuffer.h>

namespace fb
{

    /** Implementation of an index buffer. */
    class IndexBuffer : public IIndexBuffer
    {
    public:
        /** Constructor. */
        IndexBuffer();

        /** Destructor. */
        ~IndexBuffer() override;

        /** @copydoc IIndexBuffer::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        void setIndexType( Type indexType ) override;
        Type getIndexType() const override;

        void setNumIndices( u32 numIndices ) override;
        u32 getNumIndices() const override;

        u32 getIndexSize() const override;
        void setIndexSize( u32 size ) override;

        void *createIndexData() override;
        void *getIndexData() const override;

        SmartPtr<IIndexBuffer> clone() const override;

        bool compare( SmartPtr<IIndexBuffer> other ) const override;

    protected:
        // The index type.
        Type m_indexType = Type::IT_32BIT;

        // The number of indexes.
        u32 m_numIndices = 0;

        // The size of the index.
        u32 m_indexSize = 0;

        // The index data.
        void *m_indexData = nullptr;
    };

}  // end namespace fb

#endif
