#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/ScriptObjectBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "FBLuabind/ScriptObjectFunctions.h"

namespace fb
{

    
    void bindScriptProperties( lua_State *L )
    {
        //using namespace luabind;

        //typedef ScriptObjectFunc<ScriptProperties> Functions;

        //module( L )[class_<ScriptProperties>( "ScriptProperties" )
        //                .def( constructor<>() )

        //                .def( "update", Functions::scriptObjectUpdate )
        //                .def( "setProperty", Functions::ScriptObject_setProperty )
        //                .def( "setProperty", Functions::ScriptObject_setPropertyHash )
        //                .def( "setAsString", Functions::ScriptObject_setProperty )
        //                .def( "setAsString", Functions::ScriptObject_setPropertyHash )
        //                .def( "setAsBool", Functions::setPropertyAsBool )
        //                .def( "setAsBool", Functions::setPropertyAsBoolHash )
        //                .def( "setAsInt", Functions::setPropertyAsInt )
        //                .def( "setAsInt", Functions::setPropertyAsIntHash )
        //                .def( "setAsUInt", Functions::setPropertyAsUInt )
        //                .def( "setAsUInt", Functions::setPropertyAsUIntHash )
        //                .def( "setAsNumber", Functions::setPropertyAsNumber )
        //                .def( "setAsNumber", Functions::setPropertyAsNumberHash )
        //                .def( "setAsVector2", Functions::setPropertyAsVector2 )
        //                .def( "setAsVector2", Functions::setPropertyAsVector2Hash )
        //                .def( "setAsVector3", Functions::setPropertyAsVector3 )
        //                .def( "setAsVector3", Functions::setPropertyAsVector3Hash )
        //                .def( "setAsQuaternion", Functions::setPropertyAsQuaternion )
        //                .def( "setAsQuaternion", Functions::setPropertyAsQuaternionHash )
        //                .def( "setAsColor", Functions::setPropertyAsColour )
        //                .def( "setAsColor", Functions::setPropertyAsColourHash )
        //                .def( "setAsColour", Functions::setPropertyAsColour )
        //                .def( "setAsColour", Functions::setPropertyAsColourHash )
        //                .def( "getAsBool", Functions::getPropertyAsBool )
        //                .def( "getAsBool", Functions::getPropertyAsBoolHash )
        //                .def( "getAsInt", Functions::getPropertyAsInt )
        //                .def( "getAsInt", Functions::getPropertyAsIntHash )
        //                .def( "getAsUInt", Functions::getPropertyAsUInt )
        //                .def( "getAsUInt", Functions::getPropertyAsUIntHash )
        //                .def( "getAsNumber", Functions::getPropertyAsNumber )
        //                .def( "getAsNumber", Functions::getPropertyAsNumberHash )
        //                .def( "getAsString", Functions::getPropertyAsString )
        //                .def( "getAsHash", Functions::getPropertyAsHash )
        //                .def( "getAsVector2", Functions::getPropertyAsVector2 )
        //                .def( "getAsVector2", Functions::getPropertyAsVector2Hash )
        //                .def( "getAsVector3", Functions::getPropertyAsVector3 )
        //                .def( "getAsVector3", Functions::getPropertyAsVector3Hash )
        //                .def( "getAsQuaternion", Functions::getPropertyAsQuaternion )
        //                .def( "getAsQuaternion", Functions::getPropertyAsQuaternionHash )
        //                .def( "getAsColor", Functions::getPropertyAsColour )
        //                .def( "getAsColor", Functions::getPropertyAsColourHash )
        //                .def( "getAsColour", Functions::getPropertyAsColour )
        //                .def( "getAsColour", Functions::getPropertyAsColourHash )
        //                .def( "callFunction", Functions::callObjectFunction )
        //                .def( "callFunction", Functions::callObjectFunctionHash )
        //                .def( "callFunction", Functions::callObjectFunctionParams )
        //                .def( "callFunction", Functions::callObjectFunctionParamsHash )
        //                .def( "callFunction", Functions::callObjectFunctionObj )
        //                .def( "callFunction", Functions::callObjectFunctionObjHash )
        //                .def( "getObject", Functions::getObject )
        //                .def( "getObject", Functions::getObjectFromHash )
        //                .def( "getObjectAsEntity", Functions::getObjectAsEntity )
        //                .def( "getObjectAsEntity", Functions::getObjectAsEntityFromHash )];
    }

}  // end namespace fb
