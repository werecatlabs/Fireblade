#ifndef FBLuaScriptError_h__
#define FBLuaScriptError_h__

#include "FBCore/FBCoreTypes.h"

namespace fb
{
    class FBLuaScriptError
    {
    public:
        static void checkCode( s32 returnCode );
    };
}  // namespace fb

#endif  // FBLuaScriptError_h__
