#ifndef SaracenRaycastResult_h__
#define SaracenRaycastResult_h__

#include <FBCore/Data/C/VectorData.h>

namespace fb
{
    class RaycastResultData
    {
    public:
        VectorData position;
        int hasHit = 0;
        float distance = 0;
        int reserved1 = 0;
        int reserved2 = 0;
    };

    extern RaycastResultData MakeSaracenRaycastResult();
}  // namespace fb

#endif  // SaracenRaycastResult_h__
