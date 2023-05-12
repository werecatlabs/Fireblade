#ifndef SaracenNetworkState_h__
#define SaracenNetworkState_h__

#include <FBCore/Data/C/VectorData.h>

// extern "C" {

//}

namespace fb
{
    class NetworkDataState
    {
    public:
        NetworkDataState();

        int isConnected = 0;
        int isInLobby = 0;
        int isInRoom = 0;
        int sceneryId = 0;

        int isJoiningRoom = 0;
        int isHost = 0;
        int reserved1 = 0;
        int reserved2 = 0;
    };

    extern NetworkDataState MakeSaracenNetworkState();
}  // namespace fb

#endif  // SaracenNetworkState_h__
