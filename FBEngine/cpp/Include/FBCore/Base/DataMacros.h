//
// Created by Zane Desir on 2019-02-27.
//

#ifndef FBENGINE_DATAMACROS_H
#define FBENGINE_DATAMACROS_H

struct cJSON;
class TiXmlElement;

namespace fb
{
    class Properties;
}

#define FB_REFLECT_DECL_STRUCT( X ) \
    extern void TXB_binding( X *str, cJSON *json, bool m ); \
    extern void TXB_binding( X *str, TiXmlElement *exml, bool m ); \
    extern void TXB_binding( X *str, fb::Properties *properties, bool m ); \
    extern void TXB_attr_bind( TiXmlElement *exml, bool from, X *attr_content, const char *attr_name ); \
    extern void TXB_attr_bind( cJSON *json, bool from, X *attr_content, const char *attr_name ); \
    extern void TXB_attr_bind( fb::Properties *properties, bool from, X *attr_content, \
                               const char *attr_name ); \
    extern void TXB_array_bind( TiXmlElement *exml, bool from, Array<X> *ANI, const char *tagname ); \
    extern void TXB_array_bind( cJSON *json, bool from, Array<X> *ANI, const char *tagname ); \
    extern void TXB_array_bind( fb::Properties *properties, bool from, Array<X> *ANI, \
                                const char *tagname ); \
    extern void TXB_ele_bind( TiXmlElement *exml, bool from, Array<X> *ANI, const char *tagname ); \
    extern void TXB_ele_bind( cJSON *json, bool from, Array<X> *ANI, const char *tagname ); \
    extern void TXB_ele_bind( fb::Properties *properties, bool from, Array<X> *ANI, \
                              const char *tagname );

#endif  // FBENGINE_DATAMACROS_H
