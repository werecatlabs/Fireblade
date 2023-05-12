#ifndef SaracenActorInfoData_h__
#define SaracenActorInfoData_h__

#include <FBCore/Data/C/GameObjectTransformData.h>

namespace fb
{
    class ActorInfoData
    {
    public:
        int actorId = 0;
        char actorName[252];
        GameObjectTransformData transformData;
    };
}  // namespace fb

#endif  // SaracenActorInfoData_h__
