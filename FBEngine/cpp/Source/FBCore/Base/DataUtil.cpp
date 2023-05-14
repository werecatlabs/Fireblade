#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/DataUtil.h>
#include <FBCore/Base/Properties.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Base/LogManager.h>
#include <FBCore/Memory/RawPtr.h>
#include <FBCore/Scene/CActor.h>
#include <tinyxml.h>
#include <cJSON.h>
#include <vector>
#include <string>
#include <iostream>
#include <rttr/registration>
#include <rttr/registration_friend>
#include <boost/json.hpp>

        //using namespace rttr;

//// Parse the JSON string into a JSON object
//auto jsonData = boost::json::parse(jsonDataStr);

//// Get the type of the T
//auto classType = type::get<T>();

//// Iterate over the properties of the T
//for( const auto &property : classType.get_properties() )
//{
//    // Get the property name
//    auto propertyName = property.get_name().to_string();

//    // Check if the JSON object contains the property
//    auto jsonDataObject = jsonData.as_object();
//    if( jsonDataObject.contains( propertyName ) )
//    {
//        // Get the value from the JSON object
//        auto jsonValue = jsonDataObject.at(propertyName);

//        // Set the value of the property in the class object
//        //property.set_value( *ptr, jsonValue.as_object().as_float() );
//    }
//}

#define FB_DECLARE_DATA_CLASS(T) \
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

//template <typename T, typename U>
//void setName(T* t, U u)
//{
//    t->setName(u);
//}

using namespace fb;

FB_DECLARE_DATA_CLASS(Vector2F)
FB_DECLARE_DATA_CLASS(Vector2D)
FB_DECLARE_DATA_CLASS(Vector3F)
FB_DECLARE_DATA_CLASS(Vector3D)
FB_DECLARE_DATA_CLASS(Property)
FB_DECLARE_DATA_CLASS(Properties)

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

    scene::CActor::registerClass();
    Property::registerClass();
    Properties::registerClass();
    //registration::class_<Properties>( "Properties" )
       //.property( "name", &Properties::getName );
    //.property( "properties", &Properties::getPropertiesAsArray, &Properties::setPropertiesAsArray );
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
        auto p = (ISharedObject*)0;
        auto ins = rttr::instance(*p);

        auto j = objectToJson( instance );
        return boost::json::serialize( j );
    }

}  // end namespace fb
