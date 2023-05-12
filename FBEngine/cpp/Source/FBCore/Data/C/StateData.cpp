#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/StateData.h"

namespace fb
{
    StateData MakeSaracenState()
    {
        StateData saracenState;
        saracenState.cameraData = MakeSaracenCameraData();
        saracenState.modelData = MakeSaracenModelData();
        saracenState.appState = 0;
        saracenState.isModelLoading = 0;
        saracenState.isSceneLoading = 0;
        saracenState.isMouseOver = 0;

        return saracenState;
    }

    StateData::StateData()
    {
        constexpr int size = sizeof( StateData );
    }

    StateData::StateData( const StateData &other )
    {
        *this = other;
    }

    StateData &StateData::operator=( const StateData &other )
    {
        cameraData = other.cameraData;
        modelData = other.modelData;
        stats = other.stats;
        playerInputState = other.playerInputState;
        switchIndicatorsData = other.switchIndicatorsData;

        appState = other.appState;
        isModelLoading = other.isModelLoading;
        isSceneLoading = other.isSceneLoading;
        isMouseOver = other.isMouseOver;

        reserved0 = other.reserved0;
        reserved1 = other.reserved1;
        reserved2 = other.reserved2;

        return *this;
    }
}  // namespace fb
