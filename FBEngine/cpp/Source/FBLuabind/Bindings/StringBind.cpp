#include "FBLuabind/FBLuabindPCH.h"
#include "FBLuabind/Bindings/ScriptObjectBind.h"
#include <luabind/luabind.hpp>
#include "FBLuabind/SmartPtrConverter.h"
#include "FBLuabind/ParamConverter.h"
#include <FBCore/FBCore.h>

namespace fb
{

    //typedef ArrayFunctions<String> stringc_array_functions;
    //typedef Array<String> StringVector;

    //class StringVector : public SharedObject<ISharedObject>
    //{
    //public:
    //    StringVector()
    //    {
    //    }

    //    void push_back( const String &value )
    //    {
    //
    //    }
    //};
    ;

    bool hashCheck( lua_Integer hash, const char *str )
    {
        return *reinterpret_cast<hash32 *>( &hash ) == StringUtil::getHash( str );
    }

    lua_Integer _getStringHash( const char *value )
    {
        auto hash = StringUtil::getHash( value );
        lua_Integer iHash = *reinterpret_cast<lua_Integer *>( &hash );
        return iHash;
    }

    lua_Integer _getStringHashLower( const char *value )
    {
        auto hash = StringUtil::getHash( value );
        lua_Integer iHash = *reinterpret_cast<lua_Integer *>( &hash );
        return iHash;
    }

    char *_c_str( const String &str )
    {
        return (c8 *)str.c_str();
    }

    //char *_get( const StringVector &strings, lua_Integer idx )
    //{
    //    return 0;//(c8 *)strings[idx].c_str();
    //}

    String _toStringInt( lua_Integer value )
    {
        return StringUtil::toString( (s32)value );
    }

    String _toStringNumber( lua_Number value )
    {
        return StringUtil::toString( value );
    }

    template <class T>
    String _toStringObject( T *value )
    {
        return "";
    }

    //void _push_back( StringVector *strings, const String &str )
    //{
    //    strings->push_back( str );
    //}

    //SmartPtr<StringVector> make_shared_strarray()
    //{
    //    return fb::make_ptr<StringVector>();
    //}

    void bindString( lua_State *L )
    {
        using namespace luabind;

        //module( L )[class_<String>( "String" )
        //                .def( luabind::constructor<>() )
        //                .def( luabind::constructor<const char *>() )
        //                .def( luabind::constructor<const String &>() )
        //                .def( "empty", &String::empty )
        //                .def( "length", &String::length )
        //                .def( "clear", &String::clear )
        //                .def( "c_str", &String::c_str )
        //                //.def( "append",
        //                //      ( void( String::* )( const String & ) ) & String::append )
        //                //.def( "append", ( void( String::* )( const char * ) ) & String::append )
        //                .def( "substr", &String::substr )
        //                //.def( "find", &String::find )
        //                //.def( "rfind", &String::rfind )
        //                //.def( "replace", &String::replace )
        //                //.def( "compare", &String::compare )
        //                //.def( "operator+", ( String( String::* )( const String & ) const ) &
        //                //                       String::operator+ )
        //                //.def( "operator+", ( String( String::* )( const char * ) const ) &
        //                //                       String::operator+ )
        //                .def( "operator=", ( String & (String::*)(const String &)) & String::operator= )
        //                .def( "operator=", ( String & (String::*)(const char *)) & String::operator= )
        //            //.def( "operator==", ( bool( String::* )( const String & ) const ) &
        //            //                        String::operator== )
        //            //.def( "operator!=", ( bool( String::* )( const String & ) const ) &
        //            //                        String::operator!= )
        //            //.def( "operator<", ( bool( String::* )( const String & ) const ) &
        //            //                       String::operator< )
        //            //.def( "operator<=", ( bool( String::* )( const String & ) const ) &
        //            //                        String::operator<= )
        //            //.def( "operator>", ( bool( String::* )( const String & ) const ) &
        //            //                       String::operator> )
        //            //.def( "operator>=", ( bool( String::* )( const String & ) const ) &
        //            //                        String::operator>= )
        //];

        //module( L )[class_<StringVector, SmartPtr<StringVector>>( "StringVector" )
        //                //.def( "push_back", ( void( StringVector::* )( const String & ) ) &
        //                //                       StringVector::push_back )
        //                //.def( "push_back", ( void( StringVector::* )( const String && ) ) &
        //                //                      StringVector::push_back )
        //                .def( "push_back", &StringVector::push_back )
        //                //.def( "pop_back", &StringVector::pop_back )
        //                //.def( "clear", &StringVector::clear )
        //                //.def( "size", &StringVector::size )
        //                //.def( "empty", &StringVector::empty )
        //                //.def( "at",
        //                //      ( String & (StringVector::*)( size_t ) ) & StringVector::at )
        //                .scope[def( "make_shared", make_shared_strarray )]];

        module( L )
            [class_<StringUtil>( "StringUtil" )
                 .scope[def( "isEqual", &StringUtil::isEqual ),
                        def( "isNullOrEmpty", &StringUtil::isNullOrEmpty ),
                        def( "contains", &StringUtil::contains ),
                        def( "parseBool", &StringUtil::parseBool ),
                        def( "parseInt", &StringUtil::parseInt ),
                        def( "parseUInt", &StringUtil::parseUInt ),
                        def( "parseFloat", &StringUtil::parseFloat ),
                        def( "parseDouble", &StringUtil::parseDouble ),
                        def( "toString", ( String( * )( int ) ) & StringUtil::toString ),
                        def( "toString", ( String( * )( float ) ) & StringUtil::toString ),
                        def( "toString", ( String( * )( double ) ) & StringUtil::toString ),
                        def( "toString", ( String( * )( const Vector2I & ) ) & StringUtil::toString ),
                        def( "toString", ( String( * )( const Vector2F & ) ) & StringUtil::toString ),
                        def( "toString", ( String( * )( const Vector3I & ) ) & StringUtil::toString ),
                        def( "toString", ( String( * )( const Vector3F & ) ) & StringUtil::toString )
                        //,def( "toString", ( String( * )( const Vector4F & ) ) & StringUtil::toString )
                        ,
                        def( "toString", ( String( * )( const ColourI & ) ) & StringUtil::toString ),
                        def( "toString", ( String( * )( const ColourF & ) ) & StringUtil::toString ),
                        def( "parseVector2", &StringUtil::parseVector2<f32> )
                        //,def( "parseVector3", &StringUtil::parseVector3<f32> )
                        //,def( "parseVector4", &StringUtil::parseVector4<f32> )
                        ,
                        def( "parseColour", &StringUtil::parseColour ),
                        def( "parseColourf", &StringUtil::parseColourf ),
                        def( "split", &StringUtil::split ), def( "replaceAll", &StringUtil::replaceAll ),
                        def( "trim", &StringUtil::trim ), def( "toBuffer", &StringUtil::toBuffer )]];

        module( L )[def( "getStringHash", _getStringHash )];

        module( L )[def( "hash", _getStringHash )];

        module( L )[def( "hashCheck", hashCheck )];
    }

}  // end namespace fb
