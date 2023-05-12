#ifndef __FBCore_DataBindings_h__
#define __FBCore_DataBindings_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/StringTypes.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>

namespace fb
{
    namespace data
    {
        extern char *cJSON_strdup( const char *str );
        extern void TXB_binding( int *str, TiXmlElement *json, bool m );
        extern void TXB_binding( int *str, cJSON *json, bool m );
        extern void TXB_binding( float *str, TiXmlElement *json, bool m );
        extern void TXB_binding( std::string *str, TiXmlElement *json, bool m );
        extern void TXB_binding( float *str, cJSON *json, bool m );
        extern void TXB_binding( double *str, TiXmlElement *json, bool m );
        extern void TXB_binding( double *pValue, cJSON *json, bool m );
        extern void TXB_binding( std::string *str, cJSON *json, bool m );

        extern void TXB_binding( int *str, Properties *properties, bool m );
        extern void TXB_binding( float *str, Properties *properties, bool m );
        extern void TXB_binding( double *str, Properties *properties, bool m );
        extern void TXB_binding( std::string *str, Properties *properties, bool m );

        extern void TXB_attr_bind( Properties *xmle, bool from, bool *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( Properties *xmle, bool from, float *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( Properties *xmle, bool from, double *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( Properties *xmle, bool from, std::string *attr_content,
                                   const char *attr_name );

        template <class T>
        void TXB_array_bind( cJSON *json, bool from, Array<T> *ANI, const char *tagname );

        template <class T>
        void TXB_array_bind( TiXmlElement *xmle, bool from, Array<T> *ANI, const char *tagname );

        template <class T>
        void TXB_array_bind( Properties *xmle, bool from, Array<T> *ANI, const char *tagname );

        //-----------------------------------XML---------------------------------------//

#ifndef WIN32
        // to solve "error: �stricmp� was not declared in this scope"
#    define stricmp strcasecmp
#    define strnicmp strncasecmp
#endif

#define TXB_fromxml( X, Y ) TXB_binding( X, Y, 1 )
#define TXB_toxml( X, Y ) TXB_binding( X, Y, 0 )
#define TXB_fromxmldoc( X, Y ) TXB_binding( X, TiXmlHandle( Y ).FirstChild().ToElement(), 1 )
#define TXB_toxmldoc( X, Y ) TXB_binding( X, TiXmlHandle( Y ).FirstChild().ToElement(), 0 )

        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, bool *attr_content,
                                   const char *attr_name );

        template <class T>
        void TXB_attr_bind_int( TiXmlElement *xmle, bool from, T *attr_content, const char *attr_name );

        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, int *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, unsigned int *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, char *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, unsigned char *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, short *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, unsigned short *attr_content,
                                   const char *attr_name );

        template <class T>
        void TXB_attr_bind_float( TiXmlElement *xmle, bool from, T *attr_content,
                                  const char *attr_name );

        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, float *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, double *attr_content,
                                   const char *attr_name );

        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, std::string *attr_content,
                                   const char *attr_name );

        extern void TXB_text_bind( TiXmlElement *xmle, bool from, std::string *attr_content );

        //-----------------------------------JSON---------------------------------------//

        // cJSON patches
        extern cJSON *cJSON_FromFile( const char *filename );
        extern cJSON *cJSON_CreateNode( cJSON *json, const char *name );
        extern void cJSON_Print( cJSON *json, FILE *out );
        // end cJSON patches

#define TXB_fromjson( X, Y ) TXB_binding( X, Y, 1 )
#define TXB_tojson( X, Y ) TXB_binding( X, Y, 0 )

        template <class T>
        void TXB_attr_bind_int( cJSON *json, bool from, T *attr_content, const char *attr_name );

        extern void TXB_attr_bind( cJSON *json, bool from, bool *attr_content, const char *attr_name );
        extern void TXB_attr_bind( cJSON *json, bool from, int *attr_content, const char *attr_name );
        extern void TXB_attr_bind( cJSON *json, bool from, unsigned int *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( cJSON *json, bool from, char *attr_content, const char *attr_name );
        extern void TXB_attr_bind( cJSON *json, bool from, unsigned char *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( cJSON *json, bool from, short *attr_content, const char *attr_name );
        extern void TXB_attr_bind( cJSON *json, bool from, unsigned short *attr_content,
                                   const char *attr_name );

        template <class T>
        static void TXB_attr_bind_float( cJSON *json, bool from, T *attr_content,
                                         const char *attr_name );

        extern void TXB_attr_bind( cJSON *json, bool from, double *attr_content, const char *attr_name );
        extern void TXB_attr_bind( cJSON *json, bool from, float *attr_content, const char *attr_name );

        extern void TXB_attr_bind( cJSON *json, bool from, std::string *attr_content,
                                   const char *attr_name );
        extern void TXB_text_bind( cJSON *json, bool from, std::string *attr_content );

        extern void TXB_binding( int *str, Properties *properties, bool m );
        extern void TXB_binding( float *str, Properties *properties, bool m );
        extern void TXB_binding( double *str, Properties *properties, bool m );
        extern void TXB_binding( std::string *str, Properties *properties, bool m );

        extern void TXB_attr_bind( Properties *xmle, bool from, bool *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( Properties *xmle, bool from, int *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( Properties *xmle, bool from, float *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( Properties *xmle, bool from, double *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( Properties *xmle, bool from, std::string *attr_content,
                                   const char *attr_name );

        extern void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<int> *ANI, const char *tagname );
        extern void TXB_ele_bind( cJSON *json, bool from, Array<int> *ANI, const char *tagname );
        extern void TXB_ele_bind( Properties *properties, bool from, Array<int> *ANI,
                                  const char *tagname );

        extern void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<float> *ANI,
                                  const char *tagname );
        extern void TXB_ele_bind( cJSON *json, bool from, Array<float> *ANI, const char *tagname );
        extern void TXB_ele_bind( Properties *properties, bool from, Array<float> *ANI,
                                  const char *tagname );

        extern void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<double> *ANI,
                                  const char *tagname );
        extern void TXB_ele_bind( cJSON *json, bool from, Array<double> *ANI, const char *tagname );
        extern void TXB_ele_bind( Properties *properties, bool from, Array<double> *ANI,
                                  const char *tagname );

        extern void TXB_ele_bind( TiXmlElement *xmle, bool from, Array<std::string> *ANI,
                                  const char *tagname );
        extern void TXB_ele_bind( cJSON *json, bool from, Array<std::string> *ANI, const char *tagname );
        extern void TXB_ele_bind( Properties *properties, bool from, Array<std::string> *ANI,
                                  const char *tagname );

        // template <class T>
        // void TXB_ele_bind(TiXmlElement* xmle, bool from, Array<T>* ANI, const char* tagname);

        // template <class T>
        // void TXB_ele_bind(cJSON* json, bool from, Array<T>* ANI, const char* tagname);

        // template <class T>
        // void TXB_ele_bind(Properties* properties, bool from, Array<T>* ANI, const char*
        // tagname);

        extern void TXB_ele_bind( TiXmlElement *xmle, bool from, string_pairs *ANI,
                                  const char *tagname );
        extern void TXB_ele_bind( cJSON *json, bool from, string_pairs *ANI, const char *tagname );
        extern void TXB_ele_bind( Properties *properties, bool from, string_pairs *ANI,
                                  const char *tagname );

        extern void TXB_attr_bind( TiXmlElement *xmle, bool from, string_pairs *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( cJSON *json, bool from, string_pairs *attr_content,
                                   const char *attr_name );
        extern void TXB_attr_bind( Properties *properties, bool from, string_pairs *attr_content,
                                   const char *attr_name );
    }  // end namespace data
}  // end namespace fb

#endif  // __FBCore_DataBindings_h__
