#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Scene/Actor.h>
#include <tinyxml.h>
#include <cJSON.h>
#include <vector>
#include <string>
#include <iostream>
#include <rttr/registration>
#include <rttr/registration_friend>
#include <boost/json.hpp>

using namespace fb;

namespace boost
{
    namespace json
    {

        template <>
        rttr::variant value_to<rttr::variant>( const boost::json::value &val )
        {
            if( val.is_string() )
            {
                auto str = val.as_string();
                std::string s( str.data(), str.size() );
                return rttr::variant( s );
            }
            else if( val.is_double() )
            {
                double v = val.as_double();
                return rttr::variant( v );
            }
            else if( val.is_int64() )
            {
                s64 v = val.as_int64();
                return rttr::variant( v );
            }
            else if( val.is_bool() )
            {
                bool v = val.as_bool();
                return rttr::variant( v );
            }
            else if( val.is_object() )
            {
                const boost::json::object &obj = val.as_object();
                //rttr::variant var = rttr::variant::create( obj );
                return rttr::variant();
            }
            else if( val.is_array() )
            {
                const boost::json::array &arr = val.as_array();
                //rttr::variant var = rttr::variant::create( arr );
                return rttr::variant();
            }
            else
            {
                return rttr::variant();
            }
        }

    }  // namespace json
}  // namespace boost

template <class T>
void jsonToObject( const boost::json::object &json, T *ptr )
{
    rttr::type personType = rttr::type::get<T>();
    for( const auto &member : json )
    {
        std::string propertyName = member.key();
        rttr::property prop = personType.get_property( propertyName );
        if( prop.is_valid() )
        {
            rttr::variant value = boost::json::value_to<rttr::variant>( member.value() );
            prop.set_value( *ptr, value );
        }
    }
}

#define FB_DECLARE_DATA_CLASS( T ) \
    template <> \
    String DataUtil::toString( T *ptr, bool formatted ) \
    { \
        return objectToJsonStr( *ptr ); \
    } \
\
    template <> \
    void DataUtil::parse( const String &jsonDataStr, T *ptr ) \
    { \
        auto jsonData = boost::json::parse( jsonDataStr ); \
\
        if( jsonData.is_object() ) \
        { \
            const boost::json::object &obj = jsonData.as_object(); \
            DataUtil::parseJson( &obj, ptr ); \
        } \
    }

FB_DECLARE_DATA_CLASS( Vector2F )
FB_DECLARE_DATA_CLASS( Vector2D )
FB_DECLARE_DATA_CLASS( Vector3F )
FB_DECLARE_DATA_CLASS( Vector3D )

FB_DECLARE_DATA_CLASS( Transform3F )
FB_DECLARE_DATA_CLASS( Transform3D )

FB_DECLARE_DATA_CLASS( Property )
FB_DECLARE_DATA_CLASS( Properties )

SmartPtr<Properties> fromObject( const boost::json::object &obj )
{
    auto properties = fb::make_ptr<Properties>();

    bool isPropertyName = false;
    bool isPropertyValue = false;

    Property property;

    for( const auto &member : obj )
    {
        std::string key = member.key();
        const boost::json::value &value = member.value();

        /*
        if( key == "name" )
        {
            isPropertyName = true;

            if( value.is_string() )
            {
                std::string val = value.as_string().c_str();
                property.setName( val );
            }
        }
        else if( key == "value" )
        {
            isPropertyValue = true;

            if( value.is_string() )
            {
                std::string val = value.as_string().c_str();
                //properties->setProperty( key, val );
                property.setValue( val );
                property.setType( "string" );
            }
            else if( value.is_int64() )
            {
                auto val = value.as_int64();
                //properties->setProperty( key, (s32)val );

                auto valueStr = StringUtil::toString( val );
                property.setValue( valueStr );
                property.setType( "int" );
            }
            else if( value.is_double() )
            {
                double val = value.as_double();
                //properties->setProperty( key, val );

                auto valueStr = StringUtil::toString( val );
                property.setValue( valueStr );
                property.setType( "double" );
            }
            else if( value.is_bool() )
            {
                bool val = value.as_bool();
                //properties->setProperty( key, val );

                auto valueStr = StringUtil::toString( val );
                property.setValue( valueStr );
                property.setType( "bool" );
            }
        }
        else if( key == "attribute" )
        {
        }
        */

        if( value.is_string() )
        {
            std::string val = value.as_string().c_str();
            properties->setProperty( key, val );
        }
        else if( value.is_int64() )
        {
            auto val = value.as_int64();
            properties->setProperty( key, (s32)val );
        }
        else if( value.is_double() )
        {
            double val = value.as_double();
            properties->setProperty( key, val );
        }
        else if( value.is_bool() )
        {
            bool val = value.as_bool();
            properties->setProperty( key, val );
        }
        else if( value.is_object() )
        {
            auto val = value.as_object();
            auto child = fromObject( val );

            child->setName( key );
            properties->addChild( child );
        }
        else if( value.is_array() )
        {
            boost::json::array jsonArray = value.as_array();
            for( const auto &element : jsonArray )
            {
                if( element.is_object() )
                {
                    const auto &childObj = element.as_object();
                    auto child = fromObject( childObj );

                    child->setName( key );
                    properties->addChild( child );
                }
            }
        }
    }

    if( isPropertyName && isPropertyValue )
    {
        properties->addProperty( property );
    }

    return properties;
}

SmartPtr<Properties> propertiesFromObject( const boost::json::object &obj )
{
    auto properties = fb::make_ptr<Properties>();

    bool isPropertyName = false;
    bool isPropertyValue = false;

    Property property;

    for( const auto &member : obj )
    {
        std::string key = member.key();
        const boost::json::value &value = member.value();

        if( key == "name" )
        {
            isPropertyName = true;

            if( value.is_string() )
            {
                std::string val = value.as_string().c_str();
                property.setName( val );
            }
        }
        else if( key == "value" )
        {
            isPropertyValue = true;

            if( value.is_string() )
            {
                std::string val = value.as_string().c_str();
                //properties->setProperty( key, val );
                property.setValue( val );
                property.setType( "string" );
            }
            else if( value.is_int64() )
            {
                auto val = value.as_int64();
                //properties->setProperty( key, (s32)val );

                auto valueStr = StringUtil::toString( val );
                property.setValue( valueStr );
                property.setType( "int" );
            }
            else if( value.is_double() )
            {
                double val = value.as_double();
                //properties->setProperty( key, val );

                auto valueStr = StringUtil::toString( val );
                property.setValue( valueStr );
                property.setType( "double" );
            }
            else if( value.is_bool() )
            {
                bool val = value.as_bool();
                //properties->setProperty( key, val );

                auto valueStr = StringUtil::toString( val );
                property.setValue( valueStr );
                property.setType( "bool" );
            }
        }
        else if( key == "attribute" )
        {
        }
        else if( value.is_string() )
        {
            std::string val = value.as_string().c_str();
            properties->setProperty( key, val );
        }
        else if( value.is_int64() )
        {
            auto val = value.as_int64();
            properties->setProperty( key, (s32)val );
        }
        else if( value.is_double() )
        {
            double val = value.as_double();
            properties->setProperty( key, val );
        }
        else if( value.is_bool() )
        {
            bool val = value.as_bool();
            properties->setProperty( key, val );
        }
        else if( value.is_object() )
        {
            auto val = value.as_object();
            auto child = propertiesFromObject( val );

            child->setName( key );
            properties->addChild( child );
        }
        else if( value.is_array() )
        {
            boost::json::array jsonArray = value.as_array();
            for( const auto &element : jsonArray )
            {
                if( element.is_object() )
                {
                    const auto &childObj = element.as_object();
                    auto child = propertiesFromObject( childObj );

                    child->setName( key );
                    properties->addChild( child );
                }
            }
        }
    }

    if( isPropertyName && isPropertyValue )
    {
        properties->addProperty( property );
    }

    return properties;
}

SmartPtr<Properties> DataUtil::parseJson( const String &jsonDataStr )
{
    auto jsonData = boost::json::parse( jsonDataStr );

    if( jsonData.is_object() )
    {
        const boost::json::object &obj = jsonData.as_object();
        return fromObject( obj );
    }

    return nullptr;
}

SmartPtr<Properties> DataUtil::parsePropertiesFromJson( const String &jsonDataStr )
{
    auto jsonData = boost::json::parse( jsonDataStr );

    if( jsonData.is_object() )
    {
        const boost::json::object &obj = jsonData.as_object();
        return propertiesFromObject( obj );
    }

    return nullptr;
}

RTTR_REGISTRATION
{
    using namespace rttr;

    registration::class_<Vector2<int>>( "Vector2Int" )
        .constructor<>()
        .property( "x", &Vector2<int>::x )
        .property( "y", &Vector2<int>::y );

    registration::class_<Vector2<float>>( "Vector2Float" )
        .constructor<>()
        .property( "x", &Vector2<float>::x )
        .property( "y", &Vector2<float>::y );

    registration::class_<Vector2<double>>( "Vector2Double" )
        .constructor<>()
        .property( "x", &Vector2<double>::x )
        .property( "y", &Vector2<double>::y );

    scene::Actor::registerClass();
    Property::registerClass();
    Properties::registerClass();
}

namespace fb
{
    // SmartPtr<Properties> DataUtil::jsonToProperties(const std::string& jsonData)
    //{
    //	FB_ASSERT(!StringUtil::isNullOrEmpty(jsonData));

    //	auto properties = fb::make_ptr<Properties>();

    //	auto json = cJSON_Parse(jsonData.c_str());
    //	auto data = json->child;

    //	while (data)
    //	{
    //		auto name = String(data->string ? data->string : "");
    //		auto value = String("");

    //		if (data->type == cJSON_String)
    //		{
    //			value = data->valuestring ? data->valuestring : "";
    //		}

    //		properties->setProperty(name, value);
    //		data = data->next;
    //	}

    //	return properties;
    //}

    boost::json::object objectToJson( const rttr::instance &obj )
    {
        boost::json::object j;
        rttr::instance obj_instance =
            obj.get_type().get_raw_type().is_wrapper() ? obj.get_wrapped_instance() : obj;

        for( auto &prop : obj_instance.get_derived_type().get_properties() )
        {
            std::cout << "name: " << prop.get_name();

            if( prop.get_metadata( "NO_SERIALIZE" ) )
            {
                return boost::json::object();
            }

            rttr::variant prop_value = prop.get_value( obj_instance );
            if( !prop_value )
            {
                return boost::json::object();
            }

            if( prop_value.get_type().is_sequential_container() )
            {
                boost::json::array array;
                rttr::variant_sequential_view view = prop_value.create_sequential_view();
                for( const auto &item : view )
                {
                    array.push_back( objectToJson( item ) );
                }
                j[prop.get_name().to_string()] = array;
            }
            else if( prop_value.get_type().is_associative_container() )
            {
                boost::json::object map;
                rttr::variant_associative_view view = prop_value.create_associative_view();
                for( const auto &item : view )
                {
                    //map[objectToJson( item.first )] = objectToJson( item.second );
                }

                j[prop.get_name().to_string()] = map;
            }
            else
            {
                j[prop.get_name().to_string()] = prop_value.to_string();
            }
        }

        for( auto &meth : obj_instance.get_derived_type().get_methods() )
            std::cout << "name: " << meth.get_name();

        //rttr::for_each( obj_instance.get_derived_type().get_properties(), [&]( rttr::property &prop ) {
        //    if( prop.get_metadata( "NO_SERIALIZE" ) )
        //    {
        //        return;
        //    }

        //    rttr::variant prop_value = prop.get_value( obj_instance );
        //    if( !prop_value )
        //    {
        //        return;
        //    }

        //    if( prop_value.get_type().is_sequential_container() )
        //    {
        //        boost::json::array array;
        //        rttr::variant_sequential_view view = prop_value.create_sequential_view();
        //        for( const auto &item : view )
        //        {
        //            array.push_back( objectToJson( item ) );
        //        }
        //        j[prop.get_name().to_string()] = array;
        //    }
        //    else if( prop_value.get_type().is_associative_container() )
        //    {
        //        boost::json::object map;
        //        rttr::variant_associative_view view = prop_value.create_associative_view();
        //        for( const auto &item : view )
        //        {
        //            map[objectToJson( item.first )] = objectToJson( item.second );
        //        }
        //        j[prop.get_name().to_string()] = map;
        //    }
        //    else
        //    {
        //        j[prop.get_name().to_string()] = prop_value.to_string();
        //    }
        //} );

        return j;
    }

    void DataUtil::parse( SmartPtr<Properties> properties, ColourF &value )
    {
        if( properties )
        {
            value.r = properties->getPropertyAsFloat( "x" );
            value.g = properties->getPropertyAsFloat( "y" );
            value.b = properties->getPropertyAsFloat( "z" );
            value.a = properties->getPropertyAsFloat( "w" );
        }
    }

    void DataUtil::parse( SmartPtr<Properties> properties, QuaternionF &value )
    {
        if( properties )
        {
            value.x = properties->getPropertyAsFloat( "x" );
            value.y = properties->getPropertyAsFloat( "y" );
            value.z = properties->getPropertyAsFloat( "z" );
            value.w = properties->getPropertyAsFloat( "w" );
        }
    }

    void DataUtil::parse( SmartPtr<Properties> properties, Vector4F &value )
    {
        if( properties )
        {
            value.x = properties->getPropertyAsFloat( "x" );
            value.y = properties->getPropertyAsFloat( "y" );
            value.z = properties->getPropertyAsFloat( "z" );
            value.w = properties->getPropertyAsFloat( "w" );
        }
    }

    void DataUtil::parse( SmartPtr<Properties> properties, Vector3F &value )
    {
        if( properties )
        {
            value.x = properties->getPropertyAsFloat( "x" );
            value.y = properties->getPropertyAsFloat( "y" );
            value.z = properties->getPropertyAsFloat( "z" );
        }
    }

    void DataUtil::parse( SmartPtr<Properties> properties, Vector2F &value )
    {
        if( properties )
        {
            value.x = properties->getPropertyAsFloat( "x" );
            value.y = properties->getPropertyAsFloat( "y" );
        }
    }

    template <class T>
    void parseVector2( const boost::json::object *obj, Vector2<T> *value )
    {
        auto &jsonValue = *obj;
        auto structType = rttr::type::get<Vector2<T>>();
        if( structType )
        {
            for( const auto &property : *obj )
            {
                std::string propertyName = property.key();
                rttr::property prop = structType.get_property( propertyName );

                if( jsonValue.contains( propertyName ) )
                {
                    rttr::variant propertyValue =
                        boost::json::value_to<rttr::variant>( property.value() );
                    prop.set_value( value, propertyValue );
                }
            }
        }
    }

    void DataUtil::parseJson( const boost::json::object *obj, Vector2I *value )
    {
        parseVector2( obj, value );
    }

    void DataUtil::parseJson( const boost::json::object *obj, Vector2F *value )
    {
        parseVector2( obj, value );
    }

    void DataUtil::parseJson( const boost::json::object *obj, Vector2D *value )
    {
        parseVector2( obj, value );
    }

    template <class T>
    void parseVector3( const boost::json::object *obj, Vector3<T> *value )
    {
        auto &jsonValue = *obj;
        auto structType = rttr::type::get<Vector3<T>>();
        if( structType )
        {
            for( const auto &property : *obj )
            {
                std::string propertyName = property.key();
                rttr::property prop = structType.get_property( propertyName );

                if( jsonValue.contains( propertyName ) )
                {
                    rttr::variant propertyValue =
                        boost::json::value_to<rttr::variant>( property.value() );
                    prop.set_value( value, propertyValue );
                }
            }
        }
    }

    void DataUtil::parseJson( const boost::json::object *obj, Vector3I *value )
    {
        parseVector3( obj, value );
    }

    void DataUtil::parseJson( const boost::json::object *obj, Vector3F *value )
    {
        parseVector3( obj, value );
    }

    void DataUtil::parseJson( const boost::json::object *obj, Vector3D *value )
    {
        parseVector3( obj, value );
    }

    void DataUtil::parseJson( const boost::json::object *val, Transform3F *ptr )
    {
    }

    void DataUtil::parseJson( const boost::json::object *val, Transform3D *ptr )
    {
    }

    void DataUtil::parseJson( const boost::json::object *val, Properties *ptr )
    {
        const boost::json::object &obj = *val;
        rttr::type personType = rttr::type::get<Properties>();
        for( const auto &member : obj )
        {
            std::string propertyName = member.key();
            rttr::property prop = personType.get_property( propertyName );
            if( prop.is_valid() )
            {
                auto &val = member.value();

                if( propertyName == "properties" )
                {
                    if( val.is_array() )
                    {
                        boost::json::array jsonArray = val.as_array();
                        for( const auto &element : jsonArray )
                        {
                            if( element.is_object() )
                            {
                                const auto &childObj = element.as_object();

                                Property newProp;
                                DataUtil::parseJson( &childObj, &newProp );

                                ptr->addProperty( newProp );
                            }
                        }
                    }
                }
                else if( propertyName == "children" )
                {
                    if( val.is_array() )
                    {
                        boost::json::array jsonArray = val.as_array();
                        for( const auto &element : jsonArray )
                        {
                            if( element.is_object() )
                            {
                                const auto &childObj = element.as_object();

                                auto properties = fb::make_ptr<Properties>();
                                DataUtil::parseJson( &childObj, properties.get() );

                                ptr->addChild( properties );
                            }
                        }
                    }
                }
                else if( val.is_string() )
                {
                    rttr::variant value = boost::json::value_to<rttr::variant>( member.value() );
                    prop.set_value( ptr, value );
                }
            }
        }
    }

    void DataUtil::parseJson( const boost::json::object *obj, Property *ptr )
    {
        auto &jsonValue = *obj;
        auto structType = rttr::type::get<Property>();
        if( structType )
        {
            for( const auto &property : *obj )
            {
                std::string propertyName = property.key();
                rttr::property prop = structType.get_property( propertyName );

                if( jsonValue.contains( propertyName ) )
                {
                    rttr::variant propertyValue =
                        boost::json::value_to<rttr::variant>( property.value() );
                    prop.set_value( ptr, propertyValue );
                }
            }
        }
    }

    String DataUtil::objectToJsonStr( const rttr::instance &instance )
    {
        auto j = objectToJson( instance );
        return boost::json::serialize( j );
    }

}  // end namespace fb
