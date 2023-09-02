#ifndef FBLua_h__
#define FBLua_h__

#include <FBCore/FBCoreAutolink.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBLua/FBLuaAutolink.h>

namespace fb
{
    class FBLua : public ISharedObject
    {
    public:
        FBLua() = default;
        ~FBLua() override = default;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;
    };

}  // namespace fb

#endif  // FBLua_h__
