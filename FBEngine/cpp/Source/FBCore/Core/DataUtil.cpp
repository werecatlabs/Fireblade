#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/DataUtil.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Core/LogManager.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Scene/Actor.h>
#include <tinyxml.h>
#include <cJSON.h>
#include <vector>
#include <string>
#include <iostream>
#include <rttr/registration>
#include <rttr/registration_friend>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/json.hpp>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/src/nodebuilder.h>

using namespace fb;

namespace boost::json
{

    template <>
    auto value_to<rttr::variant>( const boost::json::value &val ) -> rttr::variant
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
            return {};
        }
        else if( val.is_array() )
        {
            const boost::json::array &arr = val.as_array();
            //rttr::variant var = rttr::variant::create( arr );
            return {};
        }
        else
        {
            return {};
        }
    }

}  // namespace boost::json

void parseYAML( const String &filePath, Vector2F *ptr )
{
}

void parseYAML( const String &filePath, Vector2D *ptr )
{
}

void parseYAML( const String &filePath, Vector3F *ptr )
{
}

void parseYAML( const String &filePath, Vector3D *ptr )
{
}

void parseYAML( const String &filePath, Transform3F *ptr )
{
}

void parseYAML( const String &filePath, Transform3D *ptr )
{
}

void parseYAML( const String &filePath, Property *ptr )
{
}

void parseYAML( const YAML::Node *value, Properties *ptr, const String &name )
{
    try
    {
        if( value->IsScalar() )
        {
            auto s = value->as<std::string>();
            ptr->setProperty( name, s );
        }
        else if( value->IsSequence() )
        {
            auto child = fb::make_ptr<Properties>();
            child->setName( name );

            for( auto sequenceNode : *value )
            {
                parseYAML( &sequenceNode, child.get(), name );
            }

            ptr->addChild( child );
        }
        else if( value->IsMap() )
        {
            try
            {
                auto child = fb::make_ptr<Properties>();
                child->setName( name );

                for( YAML::const_iterator itValue = ( *value ).begin(); itValue != ( *value ).end();
                     ++itValue )
                {
                    try
                    {
                        // Access the key-value pair
                        const std::string &keyValue = itValue->first.as<std::string>();
                        const YAML::Node &valueValue = itValue->second;

                        if( valueValue.IsScalar() )
                        {
                            auto s = valueValue.as<std::string>();
                            child->setProperty( keyValue, s );
                        }
                        else if( valueValue.IsSequence() )
                        {
                            for( auto sequenceNode : valueValue )
                            {
                                parseYAML( &sequenceNode, child.get(), keyValue );
                            }
                        }
                        else if( valueValue.IsMap() )
                        {
                            parseYAML( &valueValue, child.get(), keyValue );
                        }
                    }
                    catch( const YAML::Exception &e )
                    {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                }

                ptr->addChild( child );
            }
            catch( const YAML::Exception &e )
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    catch( const YAML::Exception &e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void parseYAML( const String &input, Properties *ptr )
{
    try
    {
        auto docs = std::vector<YAML::Node>();

        try
        {
            std::stringstream stream( input );
            YAML::Parser parser( stream );

            while( true )
            {
                try
                {
                    YAML::NodeBuilder builder;
                    try
                    {
                        if( !parser.HandleNextDocument( builder ) )
                        {
                            break;
                        }
                    }
                    catch( const YAML::Exception &e )
                    {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }

                    docs.push_back( builder.Root() );
                }
                catch( const YAML::Exception &e )
                {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }
        }
        catch( const YAML::Exception &e )
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        for( const YAML::Node &yamlNode : docs )
        {
            try
            {
                for( YAML::const_iterator it = yamlNode.begin(); it != yamlNode.end(); ++it )
                {
                    try
                    {
                        // Access the key-value pair
                        const std::string &key = it->first.as<std::string>();
                        const YAML::Node &value = it->second;

                        if( value.IsMap() )
                        {
                            try
                            {
                                auto gameObject = fb::make_ptr<Properties>();
                                gameObject->setName( key );

                                for( YAML::const_iterator itValue = value.begin();
                                     itValue != value.end(); ++itValue )
                                {
                                    // Access the key-value pair
                                    const std::string &keyValue = itValue->first.as<std::string>();
                                    const YAML::Node &valueValue = itValue->second;

                                    if( valueValue.IsScalar() )
                                    {
                                        auto s = valueValue.as<std::string>();
                                        gameObject->setProperty( keyValue, s );
                                    }
                                    else
                                    {
                                        parseYAML( &valueValue, gameObject.get(), keyValue );
                                    }
                                }

                                ptr->addChild( gameObject );
                            }
                            catch( const YAML::Exception &e )
                            {
                                std::cerr << "Error: " << e.what() << std::endl;
                            }
                        }
                    }
                    catch( const YAML::Exception &e )
                    {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                }
            }
            catch( const YAML::Exception &e )
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    catch( const YAML::Exception &e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

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
    String DataUtil::toString( T *ptr, bool formatted, Format fmt ) \
    { \
        switch( fmt ) \
        { \
        case Format::JSON: \
        { \
            return objectToJsonStr( *ptr, formatted ); \
        } \
        default: \
        { \
            return ""; \
        } \
        } \
\
        return ""; \
    } \
\
    template <> \
    void DataUtil::parse( const String &str, T *ptr, Format fmt ) \
    { \
        switch( fmt ) \
        { \
        case Format::JSON: \
        { \
            auto jsonData = boost::json::parse( str ); \
\
            if( jsonData.is_object() ) \
            { \
                const boost::json::object &obj = jsonData.as_object(); \
                DataUtil::parseJson( &obj, ptr ); \
            } \
        } \
        break; \
        case Format::YAML: \
        { \
            parseYAML( str, ptr ); \
        } \
        break; \
        } \
    }

FB_DECLARE_DATA_CLASS( Vector2F )
FB_DECLARE_DATA_CLASS( Vector2D )
FB_DECLARE_DATA_CLASS( Vector3F )
FB_DECLARE_DATA_CLASS( Vector3D )

FB_DECLARE_DATA_CLASS( Transform3F )
FB_DECLARE_DATA_CLASS( Transform3D )

FB_DECLARE_DATA_CLASS( Property )
//FB_DECLARE_DATA_CLASS( Properties )

auto fromObject( const boost::json::object &obj ) -> SmartPtr<Properties>
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
            properties->setProperty( key, static_cast<s32>( val ) );
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

auto propertiesFromObject( const boost::json::object &obj ) -> SmartPtr<Properties>
{
    auto properties = fb::make_ptr<Properties>();

    bool isPropertyName = false;
    bool isPropertyValue = false;

    Property property;

    for( const auto &member : obj )
    {
        std::string key = member.key();
        const boost::json::value &value = member.value();

        if( key == "handle_name" )
        {
            if( value.is_string() )
            {
                std::string val = value.as_string().c_str();
                properties->setName( val );
            }
        }
        else if( key == "name" )
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
            properties->setProperty( key, static_cast<s32>( val ) );
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

                    auto childName = child->getName();
                    if( StringUtil::isNullOrEmpty( childName ) )
                    {
                        child->setName( key );
                    }

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

auto DataUtil::parseJson( const String &jsonDataStr ) -> SmartPtr<Properties>
{
    auto jsonData = boost::json::parse( jsonDataStr );

    if( jsonData.is_object() )
    {
        const boost::json::object &obj = jsonData.as_object();
        return fromObject( obj );
    }

    return nullptr;
}

auto DataUtil::parsePropertiesFromJson( const String &jsonDataStr ) -> SmartPtr<Properties>
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

    auto propertiesToJson( Properties *ptr ) -> boost::json::object
    {
        if( !ptr )
        {
            return {};
        }

        rttr::instance obj = *ptr;
        auto objType = obj.get_type();
        auto rawType = objType.get_raw_type();
        auto objIsWrapper = rawType.is_wrapper();

        boost::json::object j;
        rttr::instance obj_instance = objIsWrapper ? obj.get_wrapped_instance() : obj;

        auto objInstanceType = obj_instance.get_derived_type();

        j["handle_name"] = ptr->getName();

        auto propertiesArray = ptr->getPropertiesAsArray();
        for( auto &prop : propertiesArray )
        {
            auto &name = prop.getName();
            auto &value = prop.getValue();
            j[name] = value;
        }

        // Handling Smart Pointers
        if( objType.is_pointer() )
        {
            rttr::instance propInstance = objInstanceType;
            rttr::instance wrappedInstance = obj.get_wrapped_instance();
            if( wrappedInstance.is_valid() )
            {
                if( auto p = wrappedInstance.try_convert<SmartPtr<Properties>>() )
                {
                    auto pProperties = *p;
                    auto &rProperties = *pProperties;
                    return propertiesToJson( pProperties.get() );
                }
            }
        }

        auto propList = objInstanceType.get_properties();

        for( auto &prop : propList )
        {
            // Check for metadata "NO_SERIALIZE"
            if( prop.get_metadata( "NO_SERIALIZE" ) )
            {
                return {};  // You might want to handle this differently.
            }

            rttr::variant prop_value = prop.get_value( obj_instance );
            if( !prop_value )
            {
                continue;
            }

            auto propName = prop.get_name();
            auto propNameStr = propName.to_string();
            auto propType = prop_value.get_type();

            if( propName == "properties" )
            {
                continue;
            }

            // Handling Smart Pointers
            if( propType.is_pointer() || propType.is_wrapper() )
            {
                rttr::instance propInstance = prop_value;
                rttr::instance wrappedInstance = propInstance.get_wrapped_instance();
                if( wrappedInstance.is_valid() )
                {
                    //j[propNameStr] = propertiesToJson( wrappedInstance );
                    continue;
                }
            }

            // Handling Sequential Containers
            if( propType.is_sequential_container() )
            {
                rttr::variant_sequential_view view = prop_value.create_sequential_view();

                boost::json::array array;
                array.reserve( view.get_size() );

                for( const auto &item : view )
                {
                    rttr::instance itemInstance = item;
                    if( auto p = itemInstance.try_convert<SmartPtr<Properties>>() )
                    {
                        auto pProperties = *p;
                        array.push_back( propertiesToJson( pProperties.get() ) );
                    }
                }

                j[propNameStr] = array;
            }
            // Handling Associative Containers
            else if( propType.is_associative_container() )
            {
                boost::json::object map;
                rttr::variant_associative_view view = prop_value.create_associative_view();
                for( const auto &item : view )
                {
                    auto mapKey = item.first.to_string();
                    //auto mapObject = propertiesToJson( item.second );
                    //map[mapKey] = mapObject;
                }

                // j[propNameStr] = map;
            }
            // Handling Basic Types
            else
            {
                //j[propNameStr] = prop_value.to_string();
            }
        }

        return j;
    }

    auto objectToJson( const rttr::instance &obj ) -> boost::json::object
    {
        auto objType = obj.get_type();
        auto rawType = objType.get_raw_type();
        auto objIsWrapper = rawType.is_wrapper();

        boost::json::object j;
        rttr::instance obj_instance = objIsWrapper ? obj.get_wrapped_instance() : obj;

        auto objInstanceType = obj_instance.get_derived_type();

        // Handling Smart Pointers
        if( objType.is_pointer() )
        {
            rttr::instance propInstance = objInstanceType;
            rttr::instance wrappedInstance = obj.get_wrapped_instance();
            if( wrappedInstance.is_valid() )
            {
                if( auto p = wrappedInstance.try_convert<SmartPtr<Properties>>() )
                {
                    auto pProperties = *p;
                    auto &rProperties = *pProperties;
                    return objectToJson( rProperties );
                }
            }
        }

        auto propList = objInstanceType.get_properties();

        for( auto &prop : propList )
        {
            // Check for metadata "NO_SERIALIZE"
            if( prop.get_metadata( "NO_SERIALIZE" ) )
            {
                return {};  // You might want to handle this differently.
            }

            rttr::variant prop_value = prop.get_value( obj_instance );
            if( !prop_value )
            {
                continue;
            }

            auto propName = prop.get_name();
            auto propNameStr = propName.to_string();
            auto propType = prop_value.get_type();

            // Handling Smart Pointers
            if( propType.is_pointer() || propType.is_wrapper() )
            {
                rttr::instance propInstance = prop_value;
                rttr::instance wrappedInstance = propInstance.get_wrapped_instance();
                if( wrappedInstance.is_valid() )
                {
                    j[propNameStr] = objectToJson( wrappedInstance );
                    continue;
                }
            }

            // Handling Sequential Containers
            if( propType.is_sequential_container() )
            {
                rttr::variant_sequential_view view = prop_value.create_sequential_view();

                boost::json::array array;
                array.reserve( view.get_size() );

                for( const auto &item : view )
                {
                    array.push_back( objectToJson( item ) );
                }

                j[propNameStr] = array;
            }
            // Handling Associative Containers
            else if( propType.is_associative_container() )
            {
                boost::json::object map;
                rttr::variant_associative_view view = prop_value.create_associative_view();
                for( const auto &item : view )
                {
                    auto mapKey = item.first.to_string();
                    auto mapObject = objectToJson( item.second );
                    map[mapKey] = mapObject;
                }

                j[propNameStr] = map;
            }
            // Handling Basic Types
            else
            {
                j[propNameStr] = prop_value.to_string();
            }
        }

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

    auto DataUtil::formatJson( const String &jsonString ) -> String
    {
        auto jsonValue = boost::json::parse( jsonString );

        std::ostringstream formatted;
        formatted << std::setw( 4 ) << std::setfill( ' ' ) << boost::json::serialize( jsonValue );
        return formatted.str();
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

    void pretty_print( std::ostream &os, boost::json::value const &jv, std::string *indent = nullptr )
    {
        using namespace boost;

        std::string indent_;
        if( !indent )
        {
            indent = &indent_;
        }
        switch( jv.kind() )
        {
        case json::kind::object:
        {
            os << "{\n";
            indent->append( 4, ' ' );
            auto const &obj = jv.get_object();
            if( !obj.empty() )
            {
                auto it = obj.begin();
                for( ;; )
                {
                    os << *indent << json::serialize( it->key() ) << " : ";
                    pretty_print( os, it->value(), indent );
                    if( ++it == obj.end() )
                    {
                        break;
                    }
                    os << ",\n";
                }
            }
            os << "\n";
            indent->resize( indent->size() - 4 );
            os << *indent << "}";
            break;
        }

        case json::kind::array:
        {
            os << "[\n";
            indent->append( 4, ' ' );
            auto const &arr = jv.get_array();
            if( !arr.empty() )
            {
                auto it = arr.begin();
                for( ;; )
                {
                    os << *indent;
                    pretty_print( os, *it, indent );
                    if( ++it == arr.end() )
                    {
                        break;
                    }
                    os << ",\n";
                }
            }
            os << "\n";
            indent->resize( indent->size() - 4 );
            os << *indent << "]";
            break;
        }

        case json::kind::string:
        {
            os << json::serialize( jv.get_string() );
            break;
        }

        case json::kind::uint64:
            os << jv.get_uint64();
            break;

        case json::kind::int64:
            os << jv.get_int64();
            break;

        case json::kind::double_:
            os << jv.get_double();
            break;

        case json::kind::bool_:
            if( jv.get_bool() )
            {
                os << "true";
            }
            else
            {
                os << "false";
            }
            break;

        case json::kind::null:
            os << "null";
            break;
        }

        if( indent->empty() )
        {
            os << "\n";
        }
    }

    auto DataUtil::objectToJsonStr( const rttr::instance &instance, bool formatted ) -> String
    {
        if( !formatted )
        {
            auto j = objectToJson( instance );
            return boost::json::serialize( j );
        }
        else
        {
            auto jsonValue = objectToJson( instance );

            std::ostringstream formatted;
            pretty_print( formatted, jsonValue );
            return formatted.str();
        }

        return "";
    }

    template <>
    auto DataUtil::toString( Properties *ptr, bool formatted, Format fmt ) -> String
    {
        if( !formatted )
        {
            auto j = propertiesToJson( ptr );
            return boost::json::serialize( j );
        }
        else
        {
            auto jsonValue = propertiesToJson( ptr );

            std::ostringstream formatted;
            pretty_print( formatted, jsonValue );
            return formatted.str();
        }

        return "";
    }

    template <>
    void DataUtil::parse( const String &dataStr, Properties *ptr, Format fmt )
    {
        switch( fmt )
        {
        case Format::JSON:
        {
            *ptr = *parsePropertiesFromJson( dataStr );
        }
        break;
        case Format::YAML:
        {
            parseYAML( dataStr, ptr );
        }
        break;
        };
    }

    template <>
    auto DataUtil::toString( ISharedObject *ptr, bool formatted, Format fmt ) -> String
    {
        if( ptr->isDerived<Properties>() )
        {
            auto properties = (Properties *)ptr;
            return toString( properties, formatted, fmt );
        }

        return "";
    }

    template <>
    void DataUtil::parse( const String &dataStr, ISharedObject *ptr, Format fmt )
    {
        if( ptr->isDerived<Properties>() )
        {
            auto properties = (Properties *)ptr;
            parse( dataStr, properties, fmt );
        }
    }

}  // end namespace fb
