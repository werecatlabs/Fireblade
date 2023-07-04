#ifndef FBMesh_Prerequisites_h__
#define FBMesh_Prerequisites_h__

#include <FBCore/Memory/SmartPtr.h>
#include <FBCore/FBCorePrerequisites.h>

namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
} // namespace tinyxml2

namespace fb
{
    // mesh
    class Mesh;
    class SubMesh;
    class VertexBuffer;
    class VertexDeclaration;
    class IndexBuffer;
    class CollisionMesh;
    class CollisionSubMesh;
} // end namespace fb

namespace Opcode
{
    class RayCollider;
    class Model;
    class MeshInterface;
}  // namespace Opcode

namespace IceMaths
{
    class Point;
}

#endif  // FBMesh_Prerequisites_h__
