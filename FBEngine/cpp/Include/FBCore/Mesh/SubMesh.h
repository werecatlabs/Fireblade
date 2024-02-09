#ifndef __FBSubMesh__H
#define __FBSubMesh__H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Mesh/ISubMesh.h>

namespace fb
{

    /** Implementation of a submesh. */
    class SubMesh : public ISubMesh
    {
    public:
        /** Constructor. */
        SubMesh();

        /** Destructor. */
        ~SubMesh() override;

        /** @copydoc ISubMesh::unload */
        void unload( SmartPtr<ISharedObject> data ) override;

        void setMaterialName( const String &materialName ) override;
        String getMaterialName() const override;

        void setVertexBuffer( SmartPtr<IVertexBuffer> vertexBuffer ) override;
        SmartPtr<IVertexBuffer> getVertexBuffer() const override;

        void setIndexBuffer( SmartPtr<IIndexBuffer> indexBuffer ) override;
        SmartPtr<IIndexBuffer> getIndexBuffer() const override;

        void updateAABB() override;

        AABB3<real_Num> getAABB() const override;
        void setAABB( const AABB3<real_Num> &aabb ) override;

        SmartPtr<ISubMesh> clone() const override;

        bool getUseSharedVertices() const override;
        void setUseSharedVertices( bool useSharedVertices ) override;

        bool compare( SmartPtr<ISubMesh> other ) const override;

        void addBoneAssignment( SmartPtr<IVertexBoneAssignment> vba );
        void removeBoneAssignment( SmartPtr<IVertexBoneAssignment> vba );
        Array<SmartPtr<IVertexBoneAssignment>> getBoneAssignments() const;

    protected:
        Array<SmartPtr<IVertexBoneAssignment>> m_boneAssignments;
        SmartPtr<IVertexBuffer> m_vertexBuffer;
        SmartPtr<IIndexBuffer> m_indexBuffer;
        AABB3<real_Num> m_aabb;
        bool m_useSharedVertices = false;
        String m_materialName;
    };
}  // end namespace fb

#endif
