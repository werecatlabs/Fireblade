#include <FBCore/FBCorePCH.h>
#include <FBCore/Base/DataBinding.h>
#include <FBCore/Base/StringUtil.h>
#include <FBCore/Base/DataCoreTypes.h>
#include <FBCore/Base/Properties.h>
#include <cJSON.h>
#include <tinyxml.h>
#include <cstring>

namespace fb
{
    namespace data
    {
        template <class T>
        void TXB_array_bind( cJSON *json, bool from, Array<T> *ANI, const char *tagname )
        {
            if( from )  // from json
            {
                cJSON *arr = cJSON_GetObjectItem( json, tagname );
                if( !arr )
                    return;

                if( arr->type == cJSON_Array )
                    for( int i = 0, length = cJSON_GetArraySize( arr ); i < length; i++ )
                    {
                        cJSON *child = cJSON_GetArrayItem( arr, i );
                        T ani;
                        TXB_binding( &ani, child, 1 );
                        ANI->push_back( ani );
                    }
            }
            else  // to json
            {
                cJSON *arr = cJSON_CreateArray();
                cJSON_AddItemToObject( json, tagname, arr );
                int size = static_cast<int>( ANI->size() );
                for( int i = 0; i < size; ++i )
                {
                    T *ani = &ANI->at( i );
                    cJSON *jani = cJSON_CreateObject();
                    TXB_binding( ani, jani, 0 );
                    cJSON_AddItemToArray( arr, jani );
                }
            }
        }

        template <class T>
        void TXB_array_bind( TiXmlElement *xmle, bool from, Array<T> *ANI, const char *tagname )
        {
            if( from )  // from xml
            {
                for( TiXmlElement *child = xmle->FirstChildElement( tagname ); child;
                     child = child->NextSiblingElement( tagname ) )
                {
                    T ani;
                    TXB_binding( &ani, child, 1 );
                    ANI->push_back( ani );
                }
            }
            else  // to xml
            {
                for( int i = 0; i < static_cast<int>( ANI->size() ); i++ )
                {
                    T *ani = &ANI->at( i );
                    TiXmlElement Xani( tagname );
                    TXB_binding( ani, &Xani, 0 );
                    xmle->InsertEndChild( Xani );
                }
            }
        }

        template <class T>
        void TXB_array_bind( Properties *xmle, bool from, Array<T> *ANI, const char *tagname )
        {
            int stop = 0;
            stop = 0;
        }

        char *cJSON_strdup( const char *str )
        {
            size_t len;
            char *copy;

            len = strlen( str ) + 1;
            copy = static_cast<char *>( malloc( len ) );

            if( !copy )
            {
                return nullptr;
            }

            memcpy( copy, str, len );
            return copy;
        }

        void TXB_binding( int *v, TiXmlElement *exml, bool m )
        {
            using namespace fb;

            if( m )
            {
                auto str = exml->GetText();
                auto val = StringUtil::parseInt( str );
                *v = val;
            }
            else
            {
                auto str = StringUtil::toString( *v );
                auto text = exml->ToText();
                text->SetValue( str.c_str() );
            }
        }

        void TXB_binding( int *str, cJSON *json, bool m )
        {
            using namespace fb;

            if( m )
            {
                *str = json->valueint;
            }
            else
            {
                json->valueint = *str;
                json->valuedouble = *str;

                std::string valStr = StringUtil::toString( *str );
                json->valuestring = cJSON_strdup( valStr.c_str() );

                json->type = cJSON_Number;
            }
        }

        void TXB_binding( float *v, TiXmlElement *exml, bool m )
        {
            using namespace fb;

            if( m )
            {
                auto str = exml->GetText();
                auto val = StringUtil::parseFloat( str );
                *v = val;
            }
            else
            {
                auto str = StringUtil::toString( *v );
                auto text = exml->ToText();
                text->SetValue( str.c_str() );
            }
        }

        void TXB_binding( std::string *v, TiXmlElement *exml, bool m )
        {
            using namespace fb;

            if( m )
            {
                auto str = exml->GetText();
                *v = str ? str : "";
            }
            else
            {
                auto str = *v;

                auto text = exml->ToText();
                text->SetValue( str.c_str() );
            }
        }

        void TXB_binding( float *str, cJSON *json, bool m )
        {
            using namespace fb;

            if( m )
            {
                *str = static_cast<float>( json->valuedouble );
            }
            else
            {
                json->valueint = static_cast<int>( *str );
                json->valuedouble = static_cast<double>( *str );

                std::string valStr = StringUtil::toString( *str );
                json->valuestring = cJSON_strdup( valStr.c_str() );

                json->type = cJSON_Number;
            }
        }

        void TXB_binding( double *v, TiXmlElement *exml, bool m )
        {
            using namespace fb;

            if( m )
            {
                auto str = exml->GetText();
                auto val = StringUtil::parseDouble( str );
                *v = val;
            }
            else
            {
                auto str = StringUtil::toString( *v );
                auto text = exml->ToText();
                text->SetValue( str.c_str() );
            }
        }

        void TXB_binding( double *pValue, cJSON *json, bool m )
        {
            using namespace fb;

            if( m )
            {
                *pValue = json->valuedouble;
            }
            else
            {
                json->valueint = static_cast<int>( *pValue );
                json->valuedouble = *pValue;

                std::string valStr = StringUtil::toString( static_cast<float>( *pValue ) );
                json->valuestring = cJSON_strdup( valStr.c_str() );

                json->type = cJSON_Number;
            }
        }

        void TXB_binding( std::string *str, cJSON *json, bool m )
        {
            using namespace fb;

            if( m )
            {
                *str = json->valuestring;
            }
            else
            {
                json->valuestring = cJSON_strdup( str->c_str() );
                json->type = cJSON_String;
            }
        }

        void TXB_binding( [[maybe_unused]] int *str, [[maybe_unused]] Properties *properties,
                          [[maybe_unused]] bool m )
        {
            int stop = 0;
            stop = 0;
        }

        void TXB_binding( [[maybe_unused]] float *str, [[maybe_unused]] Properties *properties,
                          [[maybe_unused]] bool m )
        {
            int stop = 0;
            stop = 0;
        }

        void TXB_binding( [[maybe_unused]] double *str, [[maybe_unused]] Properties *properties,
                          [[maybe_unused]] bool m )
        {
            int stop = 0;
            stop = 0;
        }

        void TXB_binding( [[maybe_unused]] std::string *str, [[maybe_unused]] Properties *properties,
                          [[maybe_unused]] bool m )
        {
            int stop = 0;
            stop = 0;
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, bool *attr_content, const char *attr_name )
        {
            if( from )  // from xml
            {
                if( xmle->Attribute( attr_name ) )
                {
                    *attr_content =
                        ( std::string( xmle->Attribute( attr_name ) ).compare( "true" ) == 0 );
                }
            }
            else  // to xml
            {
                xmle->SetAttribute( attr_name, *attr_content ? "true" : "false" );
            }
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, float *attr_content, const char *attr_name )
        {
            TXB_attr_bind_float<float>( xmle, from, attr_content, attr_name );
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, double *attr_content, const char *attr_name )
        {
            TXB_attr_bind_float<double>( xmle, from, attr_content, attr_name );
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, std::string *attr_content,
                            const char *attr_name )
        {
            if( from )  // from xml
            {
                if( xmle->Attribute( attr_name ) )
                    *attr_content = xmle->Attribute( attr_name );
            }
            else  // to xml
            {
                if( attr_content->length() > 0 )
                    xmle->SetAttribute( attr_name, attr_content->c_str() );
            }
        }

        void TXB_attr_bind( cJSON *json, bool from, int *attr_content, const char *attr_name )
        {
            TXB_attr_bind_int<int>( json, from, attr_content, attr_name );
        }

        void TXB_attr_bind( cJSON *json, bool from, unsigned int *attr_content, const char *attr_name )
        {
            TXB_attr_bind_int<unsigned int>( json, from, attr_content, attr_name );
        }

        void TXB_attr_bind( cJSON *json, bool from, char *attr_content, const char *attr_name )
        {
            TXB_attr_bind_int<char>( json, from, attr_content, attr_name );
        }

        void TXB_attr_bind( cJSON *json, bool from, unsigned char *attr_content, const char *attr_name )
        {
            TXB_attr_bind_int<unsigned char>( json, from, attr_content, attr_name );
        }

        void TXB_attr_bind( cJSON *json, bool from, short *attr_content, const char *attr_name )
        {
            TXB_attr_bind_int<short>( json, from, attr_content, attr_name );
        }

        void TXB_attr_bind( cJSON *json, bool from, unsigned short *attr_content, const char *attr_name )
        {
            TXB_attr_bind_int<unsigned short>( json, from, attr_content, attr_name );
        }

        void TXB_attr_bind( cJSON *json, bool from, double *attr_content, const char *attr_name )
        {
            TXB_attr_bind_float<double>( json, from, attr_content, attr_name );
        }

        void TXB_attr_bind( cJSON *json, bool from, float *attr_content, const char *attr_name )
        {
            TXB_attr_bind_float<float>( json, from, attr_content, attr_name );
        }

        void TXB_attr_bind( cJSON *json, bool from, std::string *attr_content, const char *attr_name )
        {
            if( from )  // from json
            {
                cJSON *item = cJSON_GetObjectItem( json, attr_name );
                if( item && item->type == cJSON_String )
                    *attr_content = item->valuestring;
            }
            else  // to json
            {
                if( attr_content->length() > 0 )
                    cJSON_AddStringToObject( json, attr_name, attr_content->c_str() );
            }
        }

        void TXB_text_bind( TiXmlElement *xmle, bool from, std::string *attr_content )
        {
            if( from )  // from xml
            {
                const TiXmlNode *child = xmle->FirstChild();
                if( child )
                {
                    const TiXmlText *childText = child->ToText();
                    if( childText )
                    {
                        *attr_content = childText->Value();
                    }
                }
            }
            else  // to xml
            {
                if( attr_content->length() > 0 )
                {
                    TiXmlText txt( attr_content->c_str() );
                    if( xmle->FirstChild() )
                        xmle->InsertBeforeChild( xmle->FirstChild(), txt );
                    else
                        xmle->InsertEndChild( txt );
                }
            }
        }

        void TXB_text_bind( cJSON *json, bool from, std::string *attr_content )
        {
            TXB_attr_bind( json, from, attr_content, "text" );
        }

        template <class T>
        void TXB_attr_bind_float( cJSON *json, bool from, T *attr_content, const char *attr_name )
        {
            if( from )  // from json
            {
                cJSON *item = cJSON_GetObjectItem( json, attr_name );
                if( item && item->type == cJSON_Number )
                    *attr_content = static_cast<T>( item->valuedouble );
            }
            else  // to json
            {
                cJSON_AddNumberToObject( json, attr_name, *attr_content );
            }
        }

        template <class T>
        void TXB_attr_bind_int( TiXmlElement *xmle, bool from, T *attr_content, const char *attr_name )
        {
            if( from )  // from xml
            {
                if( xmle->Attribute( attr_name ) )
                    *attr_content = static_cast<T>( strtol( xmle->Attribute( attr_name ), nullptr, 0 ) );
            }
            else  // to xml
            {
                char buf[100];
                snprintf( buf, 100, "%d", *attr_content );
                xmle->SetAttribute( attr_name, buf );
            }
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, char *attr_content, const char *attr_name )
        {
            TXB_attr_bind_int<char>( xmle, from, attr_content, attr_name );
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, unsigned char *attr_content,
                            const char *attr_name )
        {
            TXB_attr_bind_int<unsigned char>( xmle, from, attr_content, attr_name );
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, short *attr_content, const char *attr_name )
        {
            TXB_attr_bind_int<short>( xmle, from, attr_content, attr_name );
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, unsigned short *attr_content,
                            const char *attr_name )
        {
            TXB_attr_bind_int<unsigned short>( xmle, from, attr_content, attr_name );
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, int *attr_content, const char *attr_name )
        {
            TXB_attr_bind_int<int>( xmle, from, attr_content, attr_name );
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, unsigned int *attr_content,
                            const char *attr_name )
        {
            TXB_attr_bind_int<unsigned int>( xmle, from, attr_content, attr_name );
        }

        template <class T>
        void TXB_attr_bind_float( TiXmlElement *xmle, bool from, T *attr_content, const char *attr_name )
        {
            if( from )  // from xml
            {
                if( xmle->Attribute( attr_name ) )
                    *attr_content = static_cast<T>( strtod( xmle->Attribute( attr_name ), nullptr ) );
            }
            else  // to xml
            {
                char buf[100];
                snprintf( buf, 100, "%f", *attr_content );
                xmle->SetAttribute( attr_name, buf );
            }
        }

        template <class T>
        void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<T> *ANI, const char *tagname )
        {
            if( from )  // from xml
            {
                for( TiXmlElement *child = xmle->FirstChildElement( tagname ); child;
                     child = child->NextSiblingElement( tagname ) )
                {
                    T ani;
                    TXB_binding( &ani, child, 1 );
                    ANI->push_back( ani );
                }
            }
            else  // to xml
            {
                for( int i = 0; i < static_cast<int>( ANI->size() ); i++ )
                {
                    T *ani = &ANI->at( i );
                    TiXmlElement Xani( tagname );
                    TXB_binding( ani, &Xani, 0 );
                    xmle->InsertEndChild( Xani );
                }
            }
        }

        cJSON *cJSON_FromFile( const char *filename )
        {
            FILE *f = fopen( filename, "rb" );
            fseek( f, 0, SEEK_END );
            long len = ftell( f );
            fseek( f, 0, SEEK_SET );
            auto data = static_cast<char *>( malloc( len + 1 ) );
            fread( data, 1, len, f );
            fclose( f );
            cJSON *json = cJSON_Parse( data );
            free( data );
            if( !json )
            {
                printf( "Error before: [%s]\n", cJSON_GetErrorPtr() );
                return nullptr;
            }
            return json;
        }

        cJSON *cJSON_CreateNode( cJSON *json, const char *name )
        {
            cJSON *node = cJSON_CreateObject();
            cJSON_AddItemToObject( json, name, node );
            return node;
        }

        void cJSON_Print( cJSON *json, FILE *out )
        {
            char *print = cJSON_Print( json );
            fprintf( out, "%s", print );
            free( print );
        }

        template <class T>
        void TXB_attr_bind_int( cJSON *json, bool from, T *attr_content, const char *attr_name )
        {
            if( from )  // from json
            {
                cJSON *item = cJSON_GetObjectItem( json, attr_name );
                if( item && item->type == cJSON_Number )
                {
                    *attr_content = static_cast<T>( item->valueint );
                }
            }
            else  // to json
            {
                cJSON_AddNumberToObject( json, attr_name, *attr_content );
            }
        }

        // void TXB_binding(int* str, fb::Properties* properties, bool m)
        //{
        //	int stop = 0;
        //	stop = 0;
        // }

        // void TXB_binding(float* str, fb::Properties* properties, bool m)
        //{
        //	int stop = 0;
        //	stop = 0;
        // }

        // void TXB_binding(double* str, fb::Properties* properties, bool m)
        //{
        //	int stop = 0;
        //	stop = 0;
        // }

        // void TXB_binding(std::string* str, fb::Properties* properties, bool m)
        //{
        //	int stop = 0;
        //	stop = 0;
        // }

        void TXB_attr_bind( Properties *properties, bool from, bool *attr_content,
                            const char *attr_name )
        {
            if( from )
            {
                auto name = attr_name ? attr_name : "";
                auto value = *attr_content;
                properties->setProperty( name, value );
            }
            else
            {
                auto name = attr_name ? attr_name : "";
                auto value = properties->getPropertyAsBool( name );
                *attr_content = value;
            }
        }

        void TXB_attr_bind( Properties *properties, bool from, int *attr_content, const char *attr_name )
        {
            if( from )
            {
                auto name = attr_name ? attr_name : "";
                auto value = *attr_content;
                properties->setProperty( name, value );
            }
            else
            {
                auto name = attr_name ? attr_name : "";
                auto value = properties->getPropertyAsInt( name );
                *attr_content = value;
            }
        }

        void TXB_attr_bind( Properties *properties, bool from, float *attr_content,
                            const char *attr_name )
        {
            if( from )
            {
                auto name = attr_name ? attr_name : "";
                auto value = *attr_content;
                properties->setProperty( name, value );
            }
            else
            {
                auto name = attr_name ? attr_name : "";
                auto value = properties->getPropertyAsFloat( name );
                *attr_content = value;
            }
        }

        void TXB_attr_bind( Properties *properties, bool from, double *attr_content,
                            const char *attr_name )
        {
            if( from )
            {
                auto name = attr_name ? attr_name : "";
                auto value = static_cast<f32>( *attr_content );
                properties->setProperty( name, value );
            }
            else
            {
                auto name = attr_name ? attr_name : "";
                auto value = properties->getPropertyAsFloat( name );
                *attr_content = static_cast<f64>( value );
            }
        }

        void TXB_attr_bind( Properties *properties, bool from, std::string *attr_content,
                            const char *attr_name )
        {
            if( from )
            {
                auto name = attr_name ? attr_name : "";
                auto value = *attr_content;
                properties->setProperty( name, value );
            }
            else
            {
                auto name = attr_name ? attr_name : "";
                auto value = properties->getProperty( name, "" );
                *attr_content = value;
            }
        }

        void TXB_attr_bind( cJSON *json, bool from, bool *attr_content, const char *attr_name )
        {
            if( from )  // from json
            {
                cJSON *item = cJSON_GetObjectItem( json, attr_name );
                if( item )
                {
                    *attr_content = item->valueint == 1 ? true : false;
                }
            }
            else  // to json
            {
                cJSON_AddBoolToObject( json, attr_name, *attr_content );
            }
        }

        template <class T>
        void TXB_ele_bind( cJSON *json, bool from, Array<T> *ANI, const char *tagname )
        {
            if( from )  // from json
            {
                cJSON *arr = cJSON_GetObjectItem( json, tagname );
                if( !arr )
                    return;

                if( arr->type == cJSON_Array )
                {
                    for( int i = 0, length = cJSON_GetArraySize( arr ); i < length; i++ )
                    {
                        cJSON *child = cJSON_GetArrayItem( arr, i );
                        T ani;
                        TXB_binding( &ani, child, 1 );
                        ANI->push_back( ani );
                    }
                }
            }
            else  // to json
            {
                cJSON *arr = cJSON_CreateArray();
                cJSON_AddItemToObject( json, tagname, arr );
                for( int i = 0; i < static_cast<int>( ANI->size() ); i++ )
                {
                    T *ani = &ANI->at( i );
                    cJSON *jani = cJSON_CreateObject();
                    TXB_binding( ani, jani, 0 );
                    cJSON_AddItemToArray( arr, jani );
                }
            }
        }

        template <class T>
        void TXB_ele_bind( Properties *properties, bool from, Array<T> *ANI,
                           [[maybe_unused]] const char *tagname )
        {
            if( from )
            {
                auto propertyArray = properties->getPropertiesAsArray();
                for( auto property : propertyArray )
                {
                    auto name = property.getName();
                    auto value = property.getValue();
                    // ANI->push_back(std::pair<std::string, std::string>(name, value));
                }
            }
            else
            {
                auto &strVector = *ANI;

                auto it = strVector.begin();
                for( ; it != strVector.end(); ++it )
                {
                    // auto value = (*it);
                    // properties->addProperty(value.first.c_str(), value.second.c_str());
                }
            }
        }

        void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<int> *ANI, const char *tagname )
        {
            TXB_ele_bind<s32>( xmle, from, ANI, tagname );
        }

        void TXB_ele_bind( cJSON *json, bool from, Array<int> *ANI, const char *tagname )
        {
            TXB_ele_bind<s32>( json, from, ANI, tagname );
        }

        void TXB_ele_bind( Properties *properties, bool from, Array<int> *ANI, const char *tagname )
        {
            TXB_ele_bind<s32>( properties, from, ANI, tagname );
        }

        void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<float> *ANI, const char *tagname )
        {
            TXB_ele_bind<f32>( xmle, from, ANI, tagname );
        }

        void TXB_ele_bind( cJSON *json, bool from, Array<float> *ANI, const char *tagname )
        {
            TXB_ele_bind<f32>( json, from, ANI, tagname );
        }

        void TXB_ele_bind( Properties *properties, bool from, Array<float> *ANI, const char *tagname )
        {
            TXB_ele_bind<f32>( properties, from, ANI, tagname );
        }

        void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<double> *ANI, const char *tagname )
        {
            TXB_ele_bind<f64>( xmle, from, ANI, tagname );
        }

        void TXB_ele_bind( cJSON *json, bool from, Array<double> *ANI, const char *tagname )
        {
            TXB_ele_bind<f64>( json, from, ANI, tagname );
        }

        void TXB_ele_bind( Properties *properties, bool from, Array<double> *ANI, const char *tagname )
        {
            TXB_ele_bind<f64>( properties, from, ANI, tagname );
        }

        void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<std::string> *ANI, const char *tagname )
        {
            TXB_ele_bind<std::string>( xmle, from, ANI, tagname );
        }

        void TXB_ele_bind( cJSON *json, bool from, Array<std::string> *ANI, const char *tagname )
        {
            TXB_ele_bind<std::string>( json, from, ANI, tagname );
        }

        void TXB_ele_bind( Properties *properties, bool from, Array<std::string> *ANI,
                           const char *tagname )
        {
            TXB_ele_bind<std::string>( properties, from, ANI, tagname );
        }

        void TXB_attr_bind( TiXmlElement *xmle, bool from, string_pairs *attr_content,
                            [[maybe_unused]] const char *attr_name )
        {
            if( from )  // from xml
            {
                auto child = xmle->FirstChildElement();
                while( child )
                {
                    auto name = child->Value();
                    auto value = child->GetText();

                    attr_content->push_back(
                        std::pair<std::string, std::string>( name ? name : "", value ? value : "" ) );

                    child = child->NextSiblingElement();
                }
            }
            else  // to xml
            {
                if( attr_content->size() > 0 )
                {
                    string_pairs &strVector = *attr_content;
                    auto it = strVector.begin();

                    for( ; it != strVector.end(); ++it )
                    {
                        string_pairs::value_type value = ( *it );

                        TiXmlElement elem( "" );
                        elem.SetAttribute( value.first.c_str(), value.second.c_str() );
                        xmle->InsertEndChild( elem );
                    }
                }
            }
        }

        void TXB_attr_bind( cJSON *json, bool from, string_pairs *attr_content, const char *attr_name )
        {
            if( from )
            {
                cJSON *item = cJSON_GetObjectItem( json, attr_name );
                if( item && item->type == 5 )
                {
                    cJSON *child = item->child;
                    while( child )
                    {
                        std::string name = child->string ? child->string : "";
                        std::string value = child->valuestring ? child->valuestring : "";
                        attr_content->push_back( std::pair<std::string, std::string>( name, value ) );
                        child = child->child;
                    }
                }
            }
            else
            {
                if( attr_content->size() > 0 )
                {
                    string_pairs &strVector = *attr_content;
                    auto it = strVector.begin();

                    for( ; it != strVector.end(); ++it )
                    {
                        string_pairs::value_type value = ( *it );
                        cJSON_AddStringToObject( json, value.first.c_str(), value.second.c_str() );
                    }
                }
            }
        }

        void TXB_attr_bind( Properties *properties, bool from, string_pairs *attr_content,
                            [[maybe_unused]] const char *attr_name )
        {
            if( from )
            {
                auto propertyArray = properties->getPropertiesAsArray();
                for( auto property : propertyArray )
                {
                    auto name = property.getName();
                    auto value = property.getValue();
                    attr_content->push_back( std::pair<std::string, std::string>( name, value ) );
                }
            }
            else
            {
                auto &strVector = *attr_content;

                auto it = strVector.begin();
                for( ; it != strVector.end(); ++it )
                {
                    auto value = ( *it );
                    properties->addProperty( value.first.c_str(), value.second.c_str() );
                }
            }
        }
    }  // end namespace data
}  // end namespace fb
