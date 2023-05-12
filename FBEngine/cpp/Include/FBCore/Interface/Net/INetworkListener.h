#ifndef _INetworkListener_H
#define _INetworkListener_H

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class INetworkListener : public ISharedObject
    {
    public:
        ~INetworkListener() override = default;

        virtual void handlePacket( SmartPtr<IPacket> packet ) = 0;
        virtual void connect( u32 playerId ) = 0;
        virtual void disconnect( u32 playerId ) = 0;
    };
}  // end namespace fb

#endif
