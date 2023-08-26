#ifndef CollisionSubMesh_h__
#define CollisionSubMesh_h__

#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Math/Matrix4.h>
#include <FBCore/Math/Vector3.h>

namespace fb
{
    
    class CollisionSubMesh : public ISharedObject
    {
    public:
        CollisionSubMesh();
        CollisionSubMesh( SmartPtr<IMesh> mesh, SmartPtr<ISubMesh> subMesh, Matrix4F transform );
        ~CollisionSubMesh() override;

        bool rayCast( const Vector3F &origin, const Vector3F &dir, Array<float> &hits );

        void *getUserData() const override;
        void setUserData( void *val ) override;

    private:
        void build( float *vertices, int vertexStide, int vertexCount, const void *indices,
                    int indexCount );

        Opcode::RayCollider *m_rayCollider;
        Opcode::Model *m_tree;
        Opcode::MeshInterface *m_mesh;

        u32 *m_indices;
        IceMaths::Point *m_points;

        Matrix4F m_transformation;

        u32 m_vertexCount;
        u32 m_indexCount;
        u32 m_triangleCount;

        void *m_userData;
    };
}  // end namespace fb

#endif  // CollisionSubMesh_h__
