#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/FBParamBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "luabind/operator.hpp"

namespace fb
{
    bool getBool( const Parameter &param )
    {
        return param.data.bData;
    }

    lua_Integer getInt( const Parameter &param )
    {
        return param.data.iData;
    }

    lua_Number getNumber( const Parameter &param )
    {
        return param.data.fData;
    }

    IScriptObject *getScriptObject( const Parameter &param )
    {
        return static_cast<IScriptObject *>( param.data.pData );
    }

    //
    //IEntity *getEntity( const Parameter &param )
    //{
    //    return (IEntity *)param.data.pData;
    //}

    StateMessageDefault *getStateMessage( const Parameter &param )
    {
        return static_cast<StateMessageDefault *>( param.data.pData );
    }

    void bindParam( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<Parameter>( "Parameter" )
                        .def( constructor<>() )
                        .def( constructor<bool>() )
                        .def( constructor<const c8 *>() )
                        .def( constructor<u8>() )
                        .def( constructor<u16>() )
                        .def( constructor<s32>() )
                        .def( constructor<u32>() )
                        .def( constructor<f32>() )
                        .def( constructor<s64>() )
                        .def( constructor<f64>() )
                        .def( constructor<void *>() )
                        .def( constructor<const String &>() )
                        .def( constructor<SmartPtr<ISharedObject>>() )
                        .def( constructor<const Array<Parameter> &>() )
                        .def( "setBool", &Parameter::setBool )
                        .def( "setCharPtr", &Parameter::setCharPtr )
                        .def( "setU8", &Parameter::setU8 )
                        .def( "setU16", &Parameter::setU16 )
                        .def( "setS32", &Parameter::setS32 )
                        .def( "setU32", &Parameter::setU32 )
                        .def( "setF32", &Parameter::setF32 )
                        .def( "setS64", &Parameter::setS64 )
                        .def( "setF64", &Parameter::setF64 )
                        .def( "setPtr", &Parameter::setPtr )
                        .def( "setObject", &Parameter::setObject )
                        .def( "setArray", &Parameter::setArray )
                        .def( "getBool", &Parameter::getBool )
                        .def( "getCharPtr", &Parameter::getCharPtr )
                        .def( "getU8", &Parameter::getU8 )
                        .def( "getU16", &Parameter::getU16 )
                        .def( "getS32", &Parameter::getS32 )
                        .def( "getU32", &Parameter::getU32 )
                        .def( "getF32", &Parameter::getF32 )
                        .def( "getS64", &Parameter::getS64 )
                        .def( "getF64", &Parameter::getF64 )
                        .def( "getPtr", &Parameter::getPtr )
                        .def( "getObject", &Parameter::getObject )
                        .def( "getArray", &Parameter::getArray )
                        .def( "operator==", &Parameter::operator== )];

        /*
        module( L )[class_<Parameter>( "Param" )
                        .def( constructor<>() )
                        .def( constructor<Parameter &>() )
                        .def( constructor<const c8 *>() )
                        .def( constructor<bool>() )
                        //.def(constructor<s32>())
                        //.def(constructor<u32>())
                        //.def(constructor<f32>())
                        .def( constructor<f64>() )
                        //.def(constructor<void*>())
                        .def( "setBool", &Parameter::setBool )
                        .def( "setInt", &Parameter::setS64 )
                        .def( "setNumber", &Parameter::setF64 )
                        .def( "getBool", getBool )
                        .def( "getInt", getInt )
                        .def( "getNumber", getNumber )
                        .def( "getScriptObject", getScriptObject )
                        //.def( "getEntity", getEntity )
                        .def( "getStateMessage", getStateMessage )];
                        */
    }
}  // end namespace fb
