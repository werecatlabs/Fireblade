#ifndef _FBCore_H_
#define _FBCore_H_

#include <FBCore/FBCoreAutolink.h>
#include <FBCore/FBCoreHeaders.h>

namespace fb
{
    class FBCore : public CSharedObject<ISharedObject>
    {
    public:
        FBCore() = default;
        ~FBCore() override = default;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;
    };
}  // end namespace fb

#endif
