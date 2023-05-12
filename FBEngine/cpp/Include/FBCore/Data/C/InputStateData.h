#ifndef SaracenInputState_h__
#define SaracenInputState_h__

#include <FBCore/FBCorePrerequisites.h>

#include "FBCore/Math/Vector3.h"
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Data/C/ObjectTransformData.h"
#include "FBCore/Data/C/CameraData.h"
#include "FBCore/Data/C/PhysicsStateData.h"
#include <FBCore/Data/C/ComponentData.h>
#include "FBCore/Data/C/StatsData.h"

#include "FBCore/Data/C/AudioListenerState.h"
#include "FBCore/Data/C/PlayerInputStateData.h"

namespace fb
{
    class InputStateData
    {
    public:
        InputStateData();

        AudioListenerState audioListenerState;
        PlayerInputStateData playerInputState;
        CameraData cameraData;
        TransformData cameraTransform;
        float reserved0 = 0.0f;
        float reserved1 = 0.0f;
        float reserved2 = 0.0f;
        float reserved3 = 0.0f;
    };
}  // namespace fb

#endif  // SaracenInputState_h__
