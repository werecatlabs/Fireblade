#ifndef SaracenSwitchIndicatorsData_h__
#define SaracenSwitchIndicatorsData_h__

#include <FBCore/FBCorePrerequisites.h>

namespace fb
{
    class SwitchIndicatorsData
    {
    public:
        SwitchIndicatorsData();

        int acroMode = 0;
        int horizonMode = 0;
        int angleMode = 0;
        int airMode = 0;

        int rpm1 = 0;
        int rpm2 = 0;
        int rpm3 = 0;
        int tx = 0;
        int adapter = 0;
        int norm = 0;
        int idle = 0;
        int hold = 0;
        int dualRate = 0;
        int gearDown = 0;
        int flapDown = 0;
        int bail = 0;
        int bailMode = 0;
        float batteryState = 0.0f;
        float batteryVolts = 0.0f;
        int mode = 0;
    };
}  // namespace fb

#endif  // SaracenSwitchIndicatorsData_h__
