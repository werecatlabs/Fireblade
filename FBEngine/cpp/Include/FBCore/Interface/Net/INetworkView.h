#ifndef INetworkView_h__
#define INetworkView_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Interface/Memory/ISharedObject.h>

namespace fb
{
    class INetworkView : public ISharedObject
    {
    public:
        ~INetworkView() override = default;

        virtual void RPC() = 0;
    };
}  // end namespace fb

#endif  // INetworkView_h__
