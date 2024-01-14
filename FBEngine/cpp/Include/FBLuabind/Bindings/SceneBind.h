#ifndef SceneBind_h__
#define SceneBind_h__

#include "FBLuabind/FBLuabindPrerequisites.h"

namespace fb
{

    void bindScene( lua_State *L );
    void bindSceneManager( lua_State *L );

}  // end namespace fb

#endif  // EntityBind_h__
