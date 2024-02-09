#ifndef MeshTransformData_h__
#define MeshTransformData_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Core/Array.h>
#include <FBCore/Math/Vector2.h>
#include <FBCore/Math/Vector3.h>
#include <FBCore/Math/Quaternion.h>

namespace fb
{

    /// A structure to store mesh transform data.
    struct MeshTransformData
    {
        MeshTransformData();

        MeshTransformData( const Vector3<real_Num> &position, const Vector3<real_Num> &scale,
                           const Quaternion<real_Num> &orientation, const SmartPtr<IMesh> &mesh );

        Vector3<real_Num> Position;
        Vector3<real_Num> Scale;
        Quaternion<real_Num> Orientation;
        SmartPtr<IMesh> Mesh;

        Array<Vector2<real_Num>> UVOffsets;
        Array<Vector2<real_Num>> UVScaleData;
    };
}  // end namespace fb

#endif  // MeshTransformData_h__
