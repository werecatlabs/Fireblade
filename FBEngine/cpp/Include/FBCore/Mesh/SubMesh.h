#ifndef __FBSubMesh__H
#define __FBSubMesh__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Mesh/ISubMesh.h>
#include <FBCore/Memory/SharedObject.h>

namespace fb
{
    
    class SubMesh : public SharedObject<ISubMesh>
    {
    public:
        SubMesh();
        ~SubMesh() override;

        /** @copydoc IObject::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        void setMaterialName( const String &materialName ) override;
        String getMaterialName() const override;

        void setVertexBuffer( SmartPtr<IVertexBuffer> vertexBuffer ) override;
        SmartPtr<IVertexBuffer> getVertexBuffer() const override;

        void setIndexBuffer( SmartPtr<IIndexBuffer> indexBuffer ) override;
        SmartPtr<IIndexBuffer> getIndexBuffer() const override;

        void updateAABB() override;

        AABB3F getAABB() const override;
        void setAABB( const AABB3F &aabb ) override;

        SmartPtr<ISubMesh> clone() const override;

        bool getUseSharedVertices() const override;
        void setUseSharedVertices( bool useSharedVertices ) override;

        bool compare( SmartPtr<ISubMesh> other ) const override;

    protected:
        SmartPtr<IVertexBuffer> m_vertexBuffer;
        SmartPtr<IIndexBuffer> m_indexBuffer;
        AABB3F m_aabb;
        bool m_useSharedVertices = false;
        String m_materialName;
    };
} // end namespace fb

#endif
