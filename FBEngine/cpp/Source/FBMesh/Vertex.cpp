#include <FBMesh/FBMeshPCH.h>
#include <FBMesh/Vertex.h>

namespace fb
{

    Vertex::Vertex()
    {
    }

    bool Vertex::isFinite() const
    {
        return MathF::isFinite( Position.squaredLength() ) &&
               MathF::isFinite( Normal.squaredLength() ) &&
               MathF::isFinite( TexCoord0.squaredLength() ) &&
               MathF::isFinite( TexCoord1.squaredLength() );
    }

}  // namespace fb
