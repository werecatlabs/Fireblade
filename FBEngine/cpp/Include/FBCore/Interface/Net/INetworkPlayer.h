#ifndef INetworkPlayer_h__
#define INetworkPlayer_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{

    class INetworkPlayer : public ISharedObject
    {
    public:
        ~INetworkPlayer() override = default;
    };

}  // end namespace fb

#endif  // INetworkPlayer_h__
