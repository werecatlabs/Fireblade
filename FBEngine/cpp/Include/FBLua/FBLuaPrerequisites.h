#ifndef FBLuaPrerequisites_h__
#define FBLuaPrerequisites_h__

#include <FBCore/FBCoreTypes.h>
#include <FBCore/Memory/SmartPtr.h>

//forward decs
struct lua_State;

namespace fb
{
    /// class forward decs
    class NullScriptObject;
    class LuaManager;
    class LuaObjectData;

    /// smart pointer forward decs
    using LuaManagerPtr = SmartPtr<LuaManager>;
} // end namespace fb

#endif  // FBLuaPrerequisites_h__
