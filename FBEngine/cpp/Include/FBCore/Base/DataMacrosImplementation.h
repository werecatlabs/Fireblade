#ifndef FBENGINE_DATAMACROS_Implementation_H
#define FBENGINE_DATAMACROS_Implementation_H

struct cJSON;
class TiXmlElement;

namespace fb
{
    class Properties;
}

#define FB_REFLECT_STRUCT( X ) \
    void TXB_attr_bind( TiXmlElement *xmle, bool from, X *attr_content, const char *attr_name ) \
    { \
        if( from ) \
        { \
            TXB_binding( attr_content, xmle, 1 ); \
        } \
        else \
        { \
            TiXmlElement Xani( attr_name ); \
            TXB_binding( attr_content, &Xani, 0 ); \
            xmle->InsertEndChild( Xani ); \
        } \
    } \
    void TXB_attr_bind( cJSON *json, bool from, X *attr_content, const char *attr_name ) \
    { \
        if( from ) \
        { \
            cJSON *item = cJSON_GetObjectItem( json, attr_name ); \
            if( item ) \
            { \
                TXB_fromjson( attr_content, item ); \
            } \
        } \
        else \
        { \
            cJSON *child = cJSON_CreateObject(); \
            TXB_tojson( attr_content, child ); \
            cJSON_AddItemToObject( json, attr_name, child ); \
        } \
    } \
    void TXB_attr_bind( fb::Properties *properties, bool from, X *attr_content, \
                        [[maybe_unused]] const char *attr_name ) \
    { \
        if( from ) \
        { \
            TXB_binding( attr_content, properties, 1 ); \
        } \
        else \
        { \
            TXB_binding( attr_content, properties, 0 ); \
        } \
    }

#define FB_REFLECT_ARRAY_STRUCT( X ) \
    void TXB_array_bind( cJSON *json, bool from, Array<X> *ANI, const char *tagname ) \
    { \
        if( from ) \
        { \
            cJSON *arr = cJSON_GetObjectItem( json, tagname ); \
            if( !arr ) \
                return; \
\
            if( arr->type == cJSON_Array ) \
                for( int i = 0, length = cJSON_GetArraySize( arr ); i < length; i++ ) \
                { \
                    cJSON *child = cJSON_GetArrayItem( arr, i ); \
                    X ani; \
                    TXB_binding( &ani, child, 1 ); \
                    ANI->push_back( ani ); \
                } \
        } \
        else \
        { \
            cJSON *arr = cJSON_CreateArray(); \
            cJSON_AddItemToObject( json, tagname, arr ); \
            for( int i = 0; i < (int)ANI->size(); i++ ) \
            { \
                X *ani = &ANI->at( i ); \
                cJSON *jani = cJSON_CreateObject(); \
                TXB_binding( ani, jani, 0 ); \
                cJSON_AddItemToArray( arr, jani ); \
            } \
        } \
    }

#define FB_REFLECT_REGISTER_STRUCT_PARSE( X ) \
    template <> \
    void fb::DataUtil::parse( const std::string &jsonData, X *ptr ) \
    { \
        cJSON *json = cJSON_Parse( jsonData.c_str() ); \
        if( json ) \
        { \
            TXB_fromjson( ptr, json ); \
            cJSON_Delete( json ); \
        } \
        else \
        { \
            FB_LOG_ERROR( "Error parsing json: " + jsonData ); \
        } \
    } \
    template <> \
    void fb::DataUtil::parse( const std::string &jsonData, RawPtr<X> ptr ) \
    { \
        cJSON *json = cJSON_Parse( jsonData.c_str() ); \
        if( json ) \
        { \
            TXB_fromjson( ptr, json ); \
            cJSON_Delete( json ); \
        } \
        else \
        { \
            FB_LOG_ERROR( "Error parsing json: " + jsonData ); \
        } \
    } \
    template <> \
    void fb::DataUtil::parseXML( const std::string &data, X *ptr ) \
    { \
        TiXmlDocument doc; \
        doc.Parse( data.c_str() ); \
        if( !doc.Error() ) \
        { \
            TXB_fromxmldoc( ptr, &doc ); \
        } \
        else \
        { \
            FB_LOG_ERROR( "Error parsing json: " + data ); \
        } \
    } \
    template <> \
    void fb::DataUtil::parseXML( const std::string &data, RawPtr<X> ptr ) \
    { \
        TiXmlDocument doc; \
        doc.Parse( data.c_str() ); \
        if( !doc.Error() ) \
        { \
            TXB_fromxmldoc( ptr, &doc ); \
        } \
        else \
        { \
            FB_LOG_ERROR( "Error parsing json: " + data ); \
        } \
    }

#define FB_REFLECT_REGISTER_STRUCT_TOSTRING( X ) \
    template <> \
    std::string fb::DataUtil::toString( X *action, bool formatted ) \
    { \
        cJSON *newJSON = cJSON_CreateObject(); \
        TXB_tojson( action, newJSON ); \
        char *out = nullptr; \
        if( formatted ) \
        { \
            out = cJSON_Print( newJSON ); \
        } \
        else \
        { \
            out = cJSON_PrintUnformatted( newJSON ); \
        } \
        std::string output = out ? out : ""; \
        free( out ); \
        cJSON_Delete( newJSON ); \
        return output; \
    } \
    template <> \
    std::string fb::DataUtil::toString( RawPtr<X> action, bool formatted ) \
    { \
        cJSON *newJSON = cJSON_CreateObject(); \
        TXB_tojson( action.get(), newJSON ); \
        char *out = nullptr; \
        if( formatted ) \
        { \
            out = cJSON_Print( newJSON ); \
        } \
        else \
        { \
            out = cJSON_PrintUnformatted( newJSON ); \
        } \
        std::string output = out ? out : ""; \
        free( out ); \
        cJSON_Delete( newJSON ); \
        return output; \
    }

#define FB_REFLECT_STRUCT_EXPLICIT_INSTANTIATION( X ) \
    template <> \
    void TXB_ele_bind<X>( cJSON * json, bool from, Array<X> *ANI, const char *tagname );

#define FB_REFLECT_STRUCT_TEMPLATE_SPECIALISE( X ) \
    void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<X> *ANI, const char *tagname ) \
    { \
        XmlUtil::bind<X>( xmle, from, ANI, tagname ); \
    } \
    void TXB_ele_bind( cJSON *json, bool from, Array<X> *ANI, const char *tagname ) \
    { \
        JsonUtil::bind<X>( json, from, ANI, tagname ); \
    } \
    void TXB_ele_bind( fb::Properties *properties, bool from, Array<X> *ANI, const char *tagname ) \
    { \
        PropertiesUtil::bind( properties, from, ANI, tagname ); \
    }

#define FB_REFLECT_STRUCT_PROPERTIES( X ) \
    template <> \
    fb::SmartPtr<fb::Properties> fb::PropertiesUtil::getProperties( X *ptr ) \
    { \
        auto applicationManager = fb::core::IApplicationManager::instance(); \
        auto factoryManager = applicationManager->getFactoryManager(); \
        auto properties = factoryManager->make_ptr<Properties>(); \
        data::TXB_binding( ptr, properties.get(), true ); \
        return properties; \
    } \
    template <> \
    void fb::PropertiesUtil::fromProperties( fb::SmartPtr<fb::Properties> properties, X *ptr ) \
    { \
        data::TXB_binding( ptr, properties.get(), false ); \
    }

#define FB_REFLECT_REGISTER_DATAUTIL( X ) \
    FB_REFLECT_REGISTER_STRUCT_PARSE( X ) \
    FB_REFLECT_REGISTER_STRUCT_TOSTRING( X ) \
    FB_REFLECT_STRUCT_PROPERTIES( X )

#define FB_REFLECT_REGISTER_STRUCT( X ) \
    FB_REFLECT_STRUCT( X ) \
    FB_REFLECT_ARRAY_STRUCT( X ) \
    FB_REFLECT_STRUCT_TEMPLATE_SPECIALISE( X )

#endif  // FBENGINE_DATAMACROS_H
