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

        MeshTransformData( const Vector3F &position, const Vector3F &scale,
                           const QuaternionF &orientation, const SmartPtr<IMesh> &mesh );

        Vector3F Position;
        Vector3F Scale;
        QuaternionF Orientation;
        SmartPtr<IMesh> Mesh;

        Array<Vector2F> UVOffsets;
        Array<Vector2F> UVScaleData;
    };
}  // end namespace fb

#endif  // MeshTransformData_h__
