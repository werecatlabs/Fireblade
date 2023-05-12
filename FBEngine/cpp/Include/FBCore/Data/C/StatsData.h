#ifndef SaracenStats_h__
#define SaracenStats_h__

#include <FBCore/FBCorePrerequisites.h>

namespace fb
{
    class StatsData
    {
    public:
        StatsData();

        float fps = 0.0f;
        float controlsFPS = 0.0f;
        float physicsFPS = 0.0f;
        float velocityMPH = 0.0f;
        float rotorRPM = 0.0f;
        float reserved = 0.0f;
        float reserved1 = 0.0f;
        float reserved2 = 0.0f;
    };
}  // namespace fb

#endif  // SaracenStats_h__
