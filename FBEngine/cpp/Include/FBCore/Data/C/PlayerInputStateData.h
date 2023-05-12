#ifndef SaracenPlayerInputState_h__
#define SaracenPlayerInputState_h__

#include <FBCore/FBCorePrerequisites.h>

namespace fb
{
    class PlayerInputStateData
    {
    public:
        static constexpr int NUM_AXES = 128;
        static constexpr int NUM_BUTTONS = 128;

        PlayerInputStateData();
        PlayerInputStateData( const PlayerInputStateData &other );

        void operator=( const PlayerInputStateData &other );

        float functions[NUM_AXES] = { 0 };
        float axes[NUM_AXES] = { 0 };
        float rawAxes[NUM_AXES] = { 0 };
        float prevRawAxes[NUM_AXES] = { 0 };

        int map[NUM_AXES] = { 0 };
        int channel[NUM_AXES] = { 0 };
        int values[NUM_AXES] = { 0 };
        int buttons[NUM_BUTTONS] = { 0 };
        int reverse[NUM_BUTTONS] = { 0 };

        int axisCount = 0;
        int buttonValues = 0;
        int prevButtonValues = 0;
        int numButtons = 0;
    };
}  // namespace fb

#endif  // SaracenPlayerInputState_h__
