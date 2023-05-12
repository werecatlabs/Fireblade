#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/StatsData.h"

namespace fb
{
    StatsData::StatsData() :
        fps( 0.0f ),
        controlsFPS( 0.0f ),
        physicsFPS( 0.0f ),
        velocityMPH( 0.0f ),
        rotorRPM( 0.0f )
    {
        constexpr int size = sizeof( StatsData );
        static_assert( ( size % 16 ) == 0, "Struct needs padding." );
    }
}  // namespace fb
