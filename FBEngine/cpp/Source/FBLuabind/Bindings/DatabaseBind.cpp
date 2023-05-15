#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/DatabaseBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    void bindDatabase( lua_State *L )
    {
        using namespace luabind;

        module( L )[class_<IDatabaseQuery, ISharedObject>( "IDatabaseQuery" )
                        .def( "getNumFields", &IDatabaseQuery::getNumFields )
                        .def( "getFieldName", &IDatabaseQuery::getFieldName )
                        .def( "getFieldValue",
                              ( String( IDatabaseQuery::* )( u32 ) ) & IDatabaseQuery::getFieldValue )
                        .def( "getFieldValue", ( String( IDatabaseQuery::* )( const String & ) ) &
                                                   IDatabaseQuery::getFieldValue )
                        .def( "isFieldValueNull", &IDatabaseQuery::isFieldValueNull )
                        .def( "nextRow", &IDatabaseQuery::nextRow )
                        .def( "eof", &IDatabaseQuery::eof )];

        module(
            L )[class_<IDatabase, ISharedObject>( "IDatabase" )
                    .def( "loadFromFile",
                          ( void( IDatabase::* )( const String & ) ) & IDatabase::loadFromFile )
                    .def( "loadFromFile", ( void( IDatabase::* )( const String &, const String & ) ) &
                                              IDatabase::loadFromFile )
                    .def( "close", &IDatabase::close )
                    .def( "setKey", &IDatabase::setKey )
                    .def( "query", ( SmartPtr<IDatabaseQuery>( IDatabase::* )( const String & ) ) &
                                       IDatabase::query )
                    .def( "query", ( SmartPtr<IDatabaseQuery>( IDatabase::* )( const StringW & ) ) &
                                       IDatabase::query )
                    .def( "queryDML", ( void( IDatabase::* )( const String & ) ) & IDatabase::queryDML )
                    .def( "queryDML",
                          ( void( IDatabase::* )( const StringW & ) ) & IDatabase::queryDML )];
    }

}  // end namespace fb
