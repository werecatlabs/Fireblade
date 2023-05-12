#ifndef SaracenCameraData_h__
#define SaracenCameraData_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Data/C/VectorData.h>
#include "FBCore/Data/C/TransformData.h"

namespace fb
{
    class CameraData
    {
    public:
        CameraData();

        TransformData transform;
        VectorData up;
        VectorData forward;
        VectorData right;

        float viewMatrix[16] = { 0.0f };
        float projectionMatrix[16] = { 0.0f };
        float yFOV = 0.0f;
        int type = 0;
        float reserved1 = 0.0f;
        float reserved2 = 0.0f;
    };

    CameraData MakeSaracenCameraData();
}  // namespace fb

#endif  // SaracenCameraData_h__
