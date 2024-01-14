#include <FBCore/FBCorePCH.h>
#include <FBCore/Mesh/Vertex.h>

namespace fb
{

    Vertex::Vertex() = default;

    auto Vertex::isFinite() const -> bool
    {
        return MathF::isFinite( Position.squaredLength() ) &&
               MathF::isFinite( Normal.squaredLength() ) &&
               MathF::isFinite( TexCoord0.squaredLength() ) &&
               MathF::isFinite( TexCoord1.squaredLength() );
    }

}  // namespace fb
