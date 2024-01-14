#ifndef MeshGenerator_h__
#define MeshGenerator_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/LinearSpline3.h>

namespace fb
{

    /** Mesh generator class. */
    class MeshGenerator
    {
    public:
        struct Vertex
        {
            Vector3F position;
            Vector3F normal;
            Vector2F texCoord;
        };

        static SmartPtr<IVertexBuffer> createVertexBuffer( const Array<Vertex> &vertices );

        static SmartPtr<IVertexBuffer> createVertexBuffer( const Array<Vector3F> &positions,
                                                           const Array<Vector3F> &normals,
                                                           const Array<Vector2F> &uvs );

        static SmartPtr<IIndexBuffer> createIndexBuffer( const Array<u32> &indices );

        static SmartPtr<ISubMesh> createSubMesh( SmartPtr<IVertexBuffer> vertexBuffer,
                                                 SmartPtr<IIndexBuffer> indexBuffer );
        static SmartPtr<IMesh> createMesh( const Array<Vector3F> &vertices,
                                           const Array<Vector3F> &normals,
                                           const Array<Vector4F> &tangents, const Array<Vector3F> &uvs,
                                           const Array<u32> &indices );

        static SmartPtr<IMesh> createMesh( const Array<Vector3F> &vertices,
                                           const Array<Vector3F> &normals,
                                           const Array<Vector4F> &tangents, const Array<Vector3F> &uvs0,
                                           const Array<Vector3F> &uvs1, const Array<u32> &indices );

        static SmartPtr<IMesh> createMesh( const Array<Vector3F> &vertices,
                                           const Array<Vector3F> &normals, const Array<Vector2F> &uvs,
                                           const Array<u32> &indices );

        static SmartPtr<IMesh> createPlane( const Vector3F &halfExtent, const Vector3F &normal,
                                            const Vector3F &right );

        static SmartPtr<IMesh> createBox( f32 sizeX = 1.f, f32 sizeY = 1.f, f32 sizeZ = 1.f,
                                          u32 numSegX = 1, u32 numSegY = 1, u32 numSegZ = 1 );

        static SmartPtr<IMesh> createCylinder( f32 radius = 1.f, f32 height = 1.f, u32 numSegBase = 16,
                                               u32 numSegHeight = 1, bool capped = true );

        static SmartPtr<IMesh> createCylinderFromSpline( SmartPtr<LinearSpline3F> spline,
                                                         f32 radius = 1.f, f32 height = 1.f,
                                                         u32 numSegBase = 16, u32 numSegHeight = 1,
                                                         bool capped = true );

        static SmartPtr<IMesh> createRoadFromSpline( SmartPtr<LinearSpline3F> spline, f32 radius = 1.f,
                                                     f32 height = 1.f, u32 numSegBase = 16,
                                                     u32 numSegHeight = 1, bool capped = true );

        static SmartPtr<IMesh> createMeshFromSplines( const Array<LinearSpline3F> &splines );
    };
}  // end namespace fb

#endif  // MeshGenerator_h__
