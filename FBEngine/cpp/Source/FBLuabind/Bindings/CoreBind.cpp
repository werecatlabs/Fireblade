#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/CoreBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    lua_Integer _or( lua_Integer val0, lua_Integer val1 )
    {
        return val0 | val1;
    }

    lua_Integer _shiftLeft( lua_Integer val0, lua_Integer val1 )
    {
        return val0 << val1;
    }

    void include( const String &fileName )
    {
        auto applicationManager = core::ApplicationManager::instance();
        auto scriptManager = applicationManager->getScriptManager();
        scriptManager->loadScript( fileName );
    }

    void bind_Handle( lua_State *L )
    {
    }

    void bindCore( lua_State *L )
    {
        using namespace luabind;

        module( L )[def( "include", include )];

        module( L )[namespace_( "BitUtil" )[def( "bit_and", &BitUtil::bit_and<lua_Integer> ),
                                            def( "bit_or", _or ), def( "shift_left", _shiftLeft )]];

        module( L )[class_<IFactory, ISharedObject, SmartPtr<IFactory>>( "IFactory" )
                        .def( "getTypeName", &IFactory::getTypeName )
                        .def( "setTypeName", &IFactory::setTypeName )];

        module( L )[class_<Handle>( "Handle" )
                        .def( "getHash", &Handle::getHash )
                        .def( "setHash", &Handle::setHash )
                        .def( "getName", &Handle::getName )
                        .def( "setName", &Handle::setName )
                        .def( "getUUID", &Handle::getUUID )
                        .def( "setUUID", &Handle::setUUID )
                        .def( "getId", &Handle::getId )
                        .def( "setId", &Handle::setId )
                        .def( "getInstanceId", &Handle::getInstanceId )
                        .def( "setInstanceId", &Handle::setInstanceId )
                        .def( "operator==", &Handle::operator== )
                        .def( "operator=", &Handle::operator= )
                        .def( "toString", &Handle::toString )];

        module( L )
            [class_<Properties, ISharedObject>( "Properties" )
                 .def( "setName", &Properties::setName )
                 .def( "getName", &Properties::getName )
                 .def( "clearAll", &Properties::clearAll )
                 .def( "addProperty",
                       ( void( Properties::* )( const Property & ) ) & Properties::addProperty )
                 .def( "addProperty", ( void( Properties::* )( const String &, const String &,
                                                               const String &, bool ) ) &
                                          Properties::addProperty )
                 .def( "setProperty",
                       ( void( Properties::* )( const String &, const String &, const String & ) ) &
                           Properties::setProperty )
                 .def( "setProperty", ( void( Properties::* )( const String &, const String &,
                                                               const String &, bool ) ) &
                                          Properties::setProperty )
                 .def( "setPropertyType", &Properties::setPropertyType )
                 .def( "removeProperty", &Properties::removeProperty )
                 //.def("getProperty", &Properties::getProperty)
                 .def( "hasProperty", &Properties::hasProperty )
                 .def( "getPropertyObject",
                       ( Property & (Properties::*)(const String &)) & Properties::getPropertyObject )
                 .def( "getPropertyObject", ( const Property &(Properties::*)( const String & ) const ) &
                                                Properties::getPropertyObject )
                 .def( "propertyValueEquals", &Properties::propertyValueEquals )
                 .def( "getProperty", ( String( Properties::* )( const String &, String ) const ) &
                                          Properties::getProperty )
                 .def( "getPropertyAsBool", &Properties::getPropertyAsBool )
                 .def( "getPropertyAsInt", &Properties::getPropertyAsInt )
                 .def( "getPropertyAsFloat", &Properties::getPropertyAsFloat )
                 .def( "getPropertyAsVector3", &Properties::getPropertyAsVector3 )
                 .def( "getPropertyAsVector3D", &Properties::getPropertyAsVector3D )
                 .def( "getPropertiesAsArray", &Properties::getPropertiesAsArray )
                 .def( "toData", &Properties::toData )
                 .def( "fromData", &Properties::fromData )
                 .def( "operator=", &Properties::operator= )
                 .def( "getChildren", &Properties::getChildren )
                 .def( "addChild", &Properties::addChild )
                 .def( "removeChild", &Properties::removeChild )
                 .def( "getNumChildren", &Properties::getNumChildren )
                 .def( "hasChild", &Properties::hasChild )
                 .def( "getChild",
                       ( SmartPtr<Properties>( Properties::* )( u32 ) const ) & Properties::getChild )
                 .def( "getChild", ( SmartPtr<Properties>( Properties::* )( const String & ) const ) &
                                       Properties::getChild )
                 .def( "setProperty", ( void( Properties::* )( const String &, const String &, bool ) ) &
                                          Properties::setProperty )
                 .def( "setProperty",
                       ( void( Properties::* )( const String &, const Array<String> &, bool ) ) &
                           Properties::setProperty )
                 .def( "setProperty",
                       ( void( Properties::* )( const String &, const char *const &, bool ) ) &
                           Properties::setProperty )
                 .def( "setProperty", ( void( Properties::* )( const String &, const bool &, bool ) ) &
                                          Properties::setProperty )
                 .def( "setProperty", ( void( Properties::* )( const String &, const s32 &, bool ) ) &
                                          Properties::setProperty )
                 .def( "setProperty", ( void( Properties::* )( const String &, const u32 &, bool ) ) &
                                          Properties::setProperty )];
    }

}  // end namespace fb
