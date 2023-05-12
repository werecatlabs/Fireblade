#ifndef SaracenActorEventData_h__
#define SaracenActorEventData_h__

#include <FBCore/Data/C/ActorInfoData.h>

namespace fb
{
    class ActorEventData
    {
    public:
        int eventHash = 0;
        char eventName[252];
        ActorInfoData actorData;
        char data[256];
    };
}  // namespace fb

#endif  // SaracenActorEventData_h__
