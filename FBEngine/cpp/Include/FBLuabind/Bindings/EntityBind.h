#ifndef EntityBind_h__
#define EntityBind_h__

#include "FBLuabind/FBLuabindPrerequisites.h"

namespace fb
{

    void bindEntity( lua_State *L );
    void bindEntityMgr( lua_State *L );

}  // end namespace fb

#endif  // EntityBind_h__
