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
    }

using namespace fb;

FB_DECLARE_DATA_CLASS( Vector2F )
FB_DECLARE_DATA_CLASS( Vector2D )
FB_DECLARE_DATA_CLASS( Vector3F )
FB_DECLARE_DATA_CLASS( Vector3D )
FB_DECLARE_DATA_CLASS( Property )
FB_DECLARE_DATA_CLASS( Properties )

SmartPtr<Properties> fromObject( const boost::json::object &obj )
{
    auto properties = fb::make_ptr<Properties>();

    for( const auto &member : obj )
    {
        std::string key = member.key();
        const boost::json::value &value = member.value();

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

    return properties;
}

SmartPtr<Properties> DataUtil::parse( const String &jsonDataStr )
{
    auto jsonData = boost::json::parse( jsonDataStr );

    if( jsonData.is_object() )
    {
        const boost::json::object &obj = jsonData.as_object();
        return fromObject( obj );
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

    String DataUtil::objectToJsonStr( const rttr::instance &instance )
    {
        auto j = objectToJson( instance );
        return boost::json::serialize( j );
    }

}  // end namespace fb
