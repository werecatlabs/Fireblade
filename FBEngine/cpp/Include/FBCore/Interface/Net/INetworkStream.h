#ifndef INetworkStream_h__
#define INetworkStream_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class INetworkStream : public ISharedObject
    {
    public:
        ~INetworkStream() override = default;
    };
}  // end namespace fb

#endif  // INetworkStream_h__
