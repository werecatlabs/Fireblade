#ifndef SaracenActorData_h__
#define SaracenActorData_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Data/C/InterfaceConfigData.h>
#include <FBCore/Data/C/VectorData.h>
#include <FBCore/Data/C/ComponentData.h>

namespace fb
{
    struct ActorData
    {
        ActorData();

        int hashName = 0;
        int numComponents;
        float value = 0.0f;
        float value1 = 0.0f;

        int isVisible = 0;
        int reserved1 = 0;
        int reserved2 = 0;
        int reserved3 = 0;

        ComponentData componentData[256];
    };
}  // namespace fb

#endif  // SaracenActorData_h__
