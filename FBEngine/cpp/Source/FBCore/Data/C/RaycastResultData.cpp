#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/RaycastResultData.h"

namespace fb
{
    RaycastResultData MakeSaracenRaycastResult()
    {
        RaycastResultData result;
        result.position = MakeSaracenVec();
        result.hasHit = 0;
        result.distance = 0.0f;
        result.reserved1 = 0;
        result.reserved2 = 0;
        return result;
    }
}  // namespace fb
