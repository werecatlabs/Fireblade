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

        Vector3F Position;
        Vector3F Normal;
        Vector2F TexCoord0;
        Vector2F TexCoord1;
    };
} // namespace fb

#endif
