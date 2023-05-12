#ifndef SaracenAudioListenerState_h__
#define SaracenAudioListenerState_h__

#include <FBCore/FBCorePrerequisites.h>
#include "FBCore/Data/C/ObjectTransformData.h"
#include "FBCore/Data/C/CameraData.h"
#include "FBCore/Data/C/PhysicsStateData.h"
#include <FBCore/Data/C/ComponentData.h>
#include "FBCore/Data/C/StatsData.h"

namespace fb
{
    struct AudioListenerState
    {
        VectorData up;
        VectorData forward;
        VectorData position;
        VectorData orientation;
        VectorData veloclity;
    };
}  // namespace fb

#endif  // SaracenAudioListenerState_h__
