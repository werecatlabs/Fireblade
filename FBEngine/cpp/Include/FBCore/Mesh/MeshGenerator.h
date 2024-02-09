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
            Vector3<real_Num> position;
            Vector3<real_Num> normal;
            Vector2<real_Num> texCoord;
        };

        static SmartPtr<IVertexBuffer> createVertexBuffer( const Array<Vertex> &vertices );

        static SmartPtr<IVertexBuffer> createVertexBuffer( const Array<Vector3<real_Num>> &positions,
                                                           const Array<Vector3<real_Num>> &normals,
                                                           const Array<Vector2<real_Num>> &uvs );

        static SmartPtr<IIndexBuffer> createIndexBuffer( const Array<u32> &indices );

        static SmartPtr<ISubMesh> createSubMesh( SmartPtr<IVertexBuffer> vertexBuffer,
                                                 SmartPtr<IIndexBuffer> indexBuffer );
        static SmartPtr<IMesh> createMesh( const Array<Vector3<real_Num>> &vertices,
                                           const Array<Vector3<real_Num>> &normals,
                                           const Array<Vector4F> &tangents, const Array<Vector3<real_Num>> &uvs,
                                           const Array<u32> &indices );

        static SmartPtr<IMesh> createMesh( const Array<Vector3<real_Num>> &vertices,
                                           const Array<Vector3<real_Num>> &normals,
                                           const Array<Vector4F> &tangents, const Array<Vector3<real_Num>> &uvs0,
                                           const Array<Vector3<real_Num>> &uvs1, const Array<u32> &indices );

        static SmartPtr<IMesh> createMesh( const Array<Vector3<real_Num>> &vertices,
                                           const Array<Vector3<real_Num>> &normals, const Array<Vector2<real_Num>> &uvs,
                                           const Array<u32> &indices );

        static SmartPtr<IMesh> createPlane( const Vector3<real_Num> &halfExtent, const Vector3<real_Num> &normal,
                                            const Vector3<real_Num> &right );

        static SmartPtr<IMesh> createBox( f32 sizeX = 1.f, f32 sizeY = 1.f, f32 sizeZ = 1.f,
                                          u32 numSegX = 1, u32 numSegY = 1, u32 numSegZ = 1 );

        static SmartPtr<IMesh> createCylinder( f32 radius = 1.f, f32 height = 1.f, u32 numSegBase = 16,
                                               u32 numSegHeight = 1, bool capped = true );

        static SmartPtr<IMesh> createCylinderFromSpline( SmartPtr<LinearSpline3<real_Num>> spline,
                                                         f32 radius = 1.f, f32 height = 1.f,
                                                         u32 numSegBase = 16, u32 numSegHeight = 1,
                                                         bool capped = true );

        static SmartPtr<IMesh> createRoadFromSpline( SmartPtr<LinearSpline3<real_Num>> spline, f32 radius = 1.f,
                                                     f32 height = 1.f, u32 numSegBase = 16,
                                                     u32 numSegHeight = 1, bool capped = true );

        static SmartPtr<IMesh> createMeshFromSplines( const Array<LinearSpline3<real_Num>> &splines );
    };
}  // end namespace fb

#endif  // MeshGenerator_h__
