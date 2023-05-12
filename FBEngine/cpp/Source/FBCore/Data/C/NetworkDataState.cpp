#include <FBCore/FBCorePCH.h>
#include "FBCore/Data/C/NetworkDataState.h"

namespace fb
{
    NetworkDataState MakeSaracenNetworkState()
    {
        NetworkDataState state;
        state.isConnected = 0;
        state.isInLobby = 0;
        state.isInRoom = 0;
        state.sceneryId = 0;

        return state;
    }

    NetworkDataState::NetworkDataState()
    {
        isConnected = 0;
        isInLobby = 0;
        isInRoom = 0;
        sceneryId = 0;

        isJoiningRoom = 0;
        isHost = 0;
        reserved1 = 0;
        reserved2 = 0;
    }
}  // namespace fb
