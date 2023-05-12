#ifndef SaracenDebugLine_h__
#define SaracenDebugLine_h__

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
    class DebugLineData
    {
    public:
        DebugLineData();

        VectorData start;
        VectorData end;
        int enabled = 0;
        int colour = 0;
        float reserved1 = 0.0f;
        float reserved2 = 0.0f;
    };
}  // namespace fb

#endif  // SaracenDebugLine_h__
