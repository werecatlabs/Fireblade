#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/SwitchIndicatorsData.h"

namespace fb
{
    SwitchIndicatorsData::SwitchIndicatorsData()
    {
        rpm1 = 0;
        rpm2 = 0;
        rpm3 = 0;

        tx = 0;
        adapter = 0;
        norm = 0;
        idle = 0;
        hold = 0;

        dualRate = 0;
        gearDown = 0;
        flapDown = 0;
        bail = 0;

        bailMode = 0;
        batteryState = 0;
        batteryVolts = 0;

        constexpr int size = sizeof( SwitchIndicatorsData );
    }
}  // namespace fb
