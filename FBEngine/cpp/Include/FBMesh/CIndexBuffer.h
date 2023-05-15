#ifndef __FBIndexBuffer__H
#define __FBIndexBuffer__H

#include <FBMesh/FBMeshPrerequisites.h>
#include <FBCore/Interface/Mesh/IIndexBuffer.h>
#include <FBCore/Memory/CSharedObject.h>

namespace fb
{
    //---------------------------------------------
    class CIndexBuffer : public CSharedObject<IIndexBuffer>
    {
    public:
        CIndexBuffer();
        ~CIndexBuffer() override;

        /** @copydoc IObject::unload */
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
        Type m_indexType = Type::IT_32BIT;
        u32 m_numIndexes = 0;
        u32 m_indexSize = 0;
        void *m_indexData = nullptr;
    };
} // end namespace fb

#endif
