#ifndef FBLua_h__
#define FBLua_h__

#include "FBCore/FBCoreAutolink.h"
#include "FBCore/Memory/SharedObject.h"

//#ifdef _DEBUG
//#pragma comment(lib, "FBLua_d.lib")
//#pragma comment(lib, "FBLuabind_d.lib")
//#pragma comment(lib, "lua_d.lib")
//#pragma comment(lib, "luabind_d.lib")
//#else
//#ifndef _FINAL_
//#pragma comment(lib, "FBLua.lib")
//#pragma comment(lib, "FBLuabind.lib")
//#pragma comment(lib, "lua.lib")
//#pragma comment(lib, "luabind.lib")
//#else
//#pragma comment(lib, "FBLua.lib")
//#pragma comment(lib, "FBLuabind.lib")
//#pragma comment(lib, "lua.lib")
//#pragma comment(lib, "luabind.lib")
//#endif
//#endif

namespace fb
{
    class FBLua : public SharedObject<ISharedObject>
    {
    public:
        FBLua() = default;
        ~FBLua() override = default;

        void load( SmartPtr<ISharedObject> data ) override;
        void unload( SmartPtr<ISharedObject> data ) override;
    };

}  // namespace fb

#endif  // FBLua_h__
