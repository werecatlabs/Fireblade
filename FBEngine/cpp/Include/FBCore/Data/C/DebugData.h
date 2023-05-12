#ifndef SaracenDebugData_h__
#define SaracenDebugData_h__

#include <FBCore/FBCorePrerequisites.h>

#include "FBCore/Math/Vector3.h"
#include "FBCore/Math/Quaternion.h"
#include "FBCore/Data/C/ObjectTransformData.h"
#include "FBCore/Data/C/CameraData.h"
#include "FBCore/Data/C/PhysicsStateData.h"
#include <FBCore/Data/C/ComponentData.h>
#include "FBCore/Data/C/StatsData.h"
#include "FBCore/Data/C/DebugLineData.h"

#include "FBCore/Data/C/AudioListenerState.h"
#include "FBCore/Data/C/PlayerInputStateData.h"

namespace fb
{
    class DebugData
    {
    public:
        DebugLineData lines[8192] = { DebugLineData() };
    };
}  // namespace fb

#endif  // SaracenDebugData_h__
