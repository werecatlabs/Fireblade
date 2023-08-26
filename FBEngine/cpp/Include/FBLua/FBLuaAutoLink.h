#ifndef FBLuaAutoLink_h__
#define FBLuaAutoLink_h__

#    if FB_USE_AUTO_LINK
#        if defined _FB_STATIC_LIB_
#        ifdef _DEBUG
#            pragma comment( lib, "FBLua_d.lib" )
#            pragma comment( lib, "FBLuabind_d.lib" )
#            pragma comment( lib, "lua_d.lib" )
#            pragma comment( lib, "luabind_d.lib" )
#        else
#            ifndef _FINAL_
#                pragma comment( lib, "FBLua.lib" )
#                pragma comment( lib, "FBLuabind.lib" )
#                pragma comment( lib, "lua.lib" )
#                pragma comment( lib, "luabind.lib" )
#            else
#                pragma comment( lib, "FBLua.lib" )
#                pragma comment( lib, "FBLuabind.lib" )
#                pragma comment( lib, "lua.lib" )
#                pragma comment( lib, "luabind.lib" )
#            endif
#        endif
#    endif
#endif

#endif // FBLuaAutoLink_h__
