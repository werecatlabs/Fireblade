#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/FBLuabindTypes.h"
#include "FBLuabind/FBLuaScriptError.h"
#include <FBCore/FBCore.h>

namespace fb
{

    
    String getDebugStr()
    {
        String debugStr;

        //core::IApplicationManager *engine = core::IApplicationManager::instance();
        //ScriptManagerPtr scriptMgr = engine->getScriptManager();

        //if( scriptMgr )
        //    debugStr += scriptMgr->getDebugInfo();

        return debugStr;
    }

}  // end namespace fb
