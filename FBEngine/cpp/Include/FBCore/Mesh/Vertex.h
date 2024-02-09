#ifndef __FBVertex__H
#define __FBVertex__H

#include <FBMesh/FBMeshPrerequisites.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Vector2.h>

namespace fb
{
    /** A vertex. Use mainly for debugging. */
    struct Vertex
    {
        Vertex();

        bool isFinite() const;

        Vector3<real_Num> Position;
        Vector3<real_Num> Normal;
        Vector2<real_Num> TexCoord0;
        Vector2<real_Num> TexCoord1;
    };
}  // namespace fb

#endif
