#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/ScriptObjectBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    //
    //ISpecialFxManager *getSpecialFxManager()
    //{
    //    core::IApplicationManager *engine = core::IApplicationManager::instance();
    //    SpecialFxManagerPtr &sfxMgr = engine->getSpecialFxManager();
    //    return sfxMgr.get();
    //}

    //
    //IScriptObject *addSpecialFx( ISpecialFxManager *sfxMgr, const char *type )
    //{
    //    return sfxMgr->addSpecialFx( type ).get();
    //}

    //
    //IScriptObject *addSpecialFxFromHash( ISpecialFxManager *sfxMgr, lua_Integer type )
    //{
    //    hash32 hashId = type;
    //    return sfxMgr->addSpecialFx( hashId ).get();
    //}

    
    void bindSpecialFx( lua_State *L )
    {
        //using namespace luabind;

        //module(
        //    L )[class_<ISpecialFxManager, IScriptObject, SpecialFxManagerPtr>( "ISpecialFxManager" )
        //            .def( "addSpecialFx", addSpecialFx )
        //            .def( "addSpecialFx", addSpecialFxFromHash )];

        //module( L )[def( "getSpecialFxManager", getSpecialFxManager )];
    }

}  // end namespace fb
