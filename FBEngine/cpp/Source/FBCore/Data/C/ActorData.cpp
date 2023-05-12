#include "FBCore/FBCorePCH.h"
#include "FBCore/Data/C/ActorData.h"

namespace fb
{
    ActorData::ActorData()
    {
        hashName = 0;
        numComponents = 0;
        value = 0.0f;
        value1 = 0.0f;

        isVisible = 0;
        reserved1 = 0;
        reserved2 = 0;
        reserved3 = 0;
    }
}  // namespace fb
