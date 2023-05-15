#ifndef FBLuaScriptCommon_h__
#define FBLuaScriptCommon_h__

#include <FBCore/FBCore.h>

// Some helpful macros for defining constants  (sort of) in Lua. Similar to this code:
// object g = globals(L);
// object table = g["class"];
// table["constant"] = class::constant;
#define LUA_CONST_START( class ) \
    { \
        object g = globals( L ); \
        object table = g[#class];
#define LUA_CONST( class, name ) table[#name] = class ::name
#define LUA_CONST_END }

namespace fb
{

    //typedef Vector2<lua_Integer> Vector2I;
    //typedef fb::Array<String> stringc_array;

}  // end namespace fb

#endif  // FBLuaScriptCommon_h__
