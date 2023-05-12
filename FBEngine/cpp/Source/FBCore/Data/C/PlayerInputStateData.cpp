#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/PlayerInputStateData.h"

namespace fb
{
    PlayerInputStateData::PlayerInputStateData()
    {
        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            functions[i] = 0;
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            axes[i] = 0.0f;
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            rawAxes[i] = 0.0f;
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            prevRawAxes[i] = 0.0f;
        }

        for( size_t i = 0; i < NUM_BUTTONS; ++i )
        {
            map[i] = 0;
        }

        for( size_t i = 0; i < NUM_BUTTONS; ++i )
        {
            channel[i] = 0;
        }

        for( size_t i = 0; i < NUM_BUTTONS; ++i )
        {
            values[i] = 0;
        }

        for( size_t i = 0; i < NUM_BUTTONS; ++i )
        {
            buttons[i] = 0;
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            reverse[i] = 0;
        }

        axisCount = 0;
        buttonValues = 0;
        prevButtonValues = 0;
        numButtons = 0;

        constexpr int size = sizeof( PlayerInputStateData );
    }

    PlayerInputStateData::PlayerInputStateData( const PlayerInputStateData &other )
    {
        *this = other;
    }

    void PlayerInputStateData::operator=( const PlayerInputStateData &other )
    {
        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            functions[i] = other.functions[i];
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            axes[i] = other.axes[i];
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            rawAxes[i] = other.rawAxes[i];
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            prevRawAxes[i] = other.prevRawAxes[i];
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            map[i] = other.map[i];
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            channel[i] = other.channel[i];
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            values[i] = other.values[i];
        }

        for( size_t i = 0; i < NUM_BUTTONS; ++i )
        {
            buttons[i] = other.buttons[i];
        }

        for( size_t i = 0; i < NUM_AXES; ++i )
        {
            reverse[i] = other.reverse[i];
        }

        axisCount = other.axisCount;
        buttonValues = other.buttonValues;
        prevButtonValues = other.prevButtonValues;
        numButtons = other.numButtons;
    }
}  // namespace fb
