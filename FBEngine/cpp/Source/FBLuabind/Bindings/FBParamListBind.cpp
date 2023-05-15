#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/FBParamBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>
#include "luabind/operator.hpp"

namespace fb
{

    Parameters createParams()
    {
        return Parameters();
    }

    Parameters createIntParam( s32 value )
    {
        Parameters params( 1 );
        Parameter param( value );
        params.push_back( param );
        return params;
    }

    Parameters createF64Param( f64 value )
    {
        Parameters params( 1 );
        Parameter param( value );
        params.push_back( param );
        return params;
    }

    Parameters createParams( Parameter &param )
    {
        Parameters params( 1 );
        params.push_back( param );
        return params;
    }

    Parameters createParams( Parameter &param0, Parameter &param1 )
    {
        Parameters params( 2 );
        params.push_back( param0 );
        params.push_back( param1 );
        return params;
    }

    Parameters createParams( Parameter &param0, Parameter &param1, Parameter &param2 )
    {
        Parameters params( 3 );
        params.push_back( param0 );
        params.push_back( param1 );
        params.push_back( param2 );
        return params;
    }

    Parameters createParams( Parameter &param0, Parameter &param1, Parameter &param2, Parameter &param3 )
    {
        Parameters params( 4 );
        params.push_back( param0 );
        params.push_back( param1 );
        params.push_back( param2 );
        params.push_back( param3 );
        return params;
    }

    void addParamAsBool( Parameters &params, bool value )
    {
        Parameter param( value );
        params.push_back( param );
    }

    void addParamAsInt( Parameters &params, lua_Integer value )
    {
        Parameter param( (s32)value );
        params.push_back( param );
    }

    void addParamAsNumber( Parameters &params, lua_Number value )
    {
        Parameter param( value );
        params.push_back( param );
    }

    void addParamAsString( Parameters &params, const char *value )
    {
        Parameter param;
        param.str = value;
        params.push_back( param );
    }

    Parameter getParam( Parameters &params, lua_Integer index )
    {
        if( (size_t)index < params.size() )
        {
            return params[index];
        }

        return Parameter();
    }

    bool getParamAsBool( Parameters &params, lua_Integer index )
    {
        return params[index].data.bData;
    }

    lua_Integer getParamAsInt( Parameters &params, lua_Integer index )
    {
        return (lua_Integer)params[index].data.iData;
    }

    lua_Number getParamAsNumber( Parameters &params, lua_Integer index )
    {
        return (lua_Number)params[index].data.fData;
    }

    SmartPtr<ISharedObject> getParamAsObject( Parameters &params, lua_Integer index )
    {
        SmartPtr<ISharedObject> pObject( (ISharedObject *)params[index].data.pData );
        return pObject;
    }

    Array<String> getParamAsStringArray( Parameters &params )
    {
        Array<String> array;

        for( auto &param : params )
        {
            array.push_back( param.str );
        }

        return array;
    }

    //
    //IEntity *getParamAsEntity( Parameters &params, lua_Integer index )
    //{
    //    return (IEntity *)params[index].data.pData;
    //}

    //
    //StateMessagePtr getParamAsStateMessage( Parameters &params, lua_Integer index )
    //{
    //    return StateMessagePtr( (IStateMessage *)params[index].data.pData );
    //}

    lua_Integer getListSize( Parameters &params )
    {
        return (lua_Integer)params.size();
    }

    //
    //template <class T>
    //void _push_back_ptr(ParamArray& params, SmartPtr<T> val)
    //{
    //	Parameter param;
    //	param.setPtr(val.get());
    //	params.push_back(param);
    //}

    //
    //void _push_back(ParamArray& params, Parameter& param)
    //{
    //	params.push_back(param);
    //}

    //
    //Parameter _getParamFromArray(ParamArray& params, lua_Integer index)
    //{
    //	return params[index];
    //}

    void bindParamList( lua_State *L )
    {
        using namespace luabind;

        module(
            L )[class_<std::vector<std::string>>( "StringVector" )
                    .def( constructor<>() )
                    .def( "push_back", ( void( std::vector<std::string>::* )( const std::string & ) ) &
                                           std::vector<std::string>::push_back )
                    .def( "pop_back", &std::vector<std::string>::pop_back )
                    .def( "clear", &std::vector<std::string>::clear )
                    .def( "size", &std::vector<std::string>::size )
                    .def( "empty", &std::vector<std::string>::empty )
                    .def( "at", ( std::string & (std::vector<std::string>::*)( size_t ) ) &
                                    std::vector<std::string>::at )];

        module( L )[class_<Parameters>( "Parameters" )
                        .def( constructor<>() )
                        .def( "addAsBool", addParamAsBool )
                        .def( "addAsInt", addParamAsInt )
                        .def( "addAsNumber", addParamAsNumber )
                        .def( "push_back", addParamAsString )
                        .def( "get", getParam )
                        .def( "at", getParam )
                        .def( "getAsBool", getParamAsBool )
                        .def( "getAsInt", getParamAsInt )
                        .def( "getAsNumber", getParamAsNumber )
                        .def( "getAsObject", getParamAsObject )
                        .def( "getAsStringArray", getParamAsStringArray )

                        .def( "size", getListSize )];

        //module(L)
        //	[
        //		class_<ParamArray>( "ParamArray" )
        //		.def(constructor<>())

        //		.def("reserve", &ParamArray::reserve )
        //		.def("set_used", &ParamArray::set_used )

        //		.def("push_back", _push_back )
        //		.def("push_front", &ParamArray::push_front )

        //		.def("erase", (void (ParamArray::*)(u32))&ParamArray::erase )
        //		.def("erase_element", &ParamArray::erase_element )

        //		.def("get", _getParamFromArray )

        //		//convenience functions
        //		.def("addAsBool", addParamAsBool )
        //		.def("addAsInt", addParamAsInt )
        //		.def("addAsNumber", addParamAsNumber )
        //
        //		.def("getAsBool", getParamAsBool )
        //		.def("getAsInt", getParamAsInt )
        //		.def("getAsNumber", getParamAsNumber )
        //		.def("getAsObject", getParamAsObject )
        //		.def("getAsEntity", getParamAsEntity )
        //		.def("getAsStateMessage", getParamAsStateMessage )

        //		.def("size", getListSize )
        //	];

        //typedef Array<SmartPtr<IScriptObject>> ScriptObjectArray;

        //module( L )[class_<ScriptObjectArray>( "ScriptObjectArray" )
        //                .def( constructor<>() )

        //                .def( "reserve", &ScriptObjectArray::reserve )
        //                //.def("set_used", &ScriptObjectArray::set_used )

        //                //.def("push_back", &ScriptObjectArray::push_back )

        //                //.def("erase", (void (ParamArray::*)(u32))&ScriptObjectArray::erase )
        //                //.def("erase_element", &ScriptObjectArray::erase_element )

        //                //.def("get", _getParamFromArray )

        //                //convenience functions
        //                .def( "addAsBool", addParamAsBool )
        //                .def( "addAsInt", addParamAsInt )
        //                .def( "addAsNumber", addParamAsNumber )

        //                .def( "getAsBool", getParamAsBool )
        //                .def( "getAsInt", getParamAsInt )
        //                .def( "getAsNumber", getParamAsNumber )
        //                .def( "getAsObject", getParamAsObject )
        //                .def( "getAsEntity", getParamAsEntity )
        //                .def( "getAsStateMessage", getParamAsStateMessage )

        //                 .def( "size", getListSize )];

        //module( L )[def( "params", ( Parameters( * )() ) & createParams )];

        //module( L )[def( "params", ( Parameters( * )( Parameter & ) ) & createParams )];

        //module( L )[def( "params", ( Parameters( * )( Parameter &, Parameter & ) ) & createParams )];

        //module( L )[def( "params",
        //                 ( Parameters( * )( Parameter &, Parameter &, Parameter & ) ) & createParams )];

        //module( L )[def(
        //    "params",
        //    ( Parameters( * )( Parameter &, Parameter &, Parameter &, Parameter & ) ) & createParams )];

        //module( L )[def( "params", ( Parameters( * )( s32 ) ) & createIntParam )];

        //module( L )[def( "params", ( Parameters( * )( f64 ) ) & createF64Param )];
    }

}  // end namespace fb
