#undef FB_PASS_STRUCT
#undef FB_PASS_JSON
#undef FB_PASS_XML
#undef FB_STRUCT
#undef FB_STRUCT_INHERIT
#undef FB_CLASS
#undef FB_CLASS_INHERIT
#undef FB_CONSTRUCTOR
#undef FB_ATTR
#undef FB_ATTR_NAME
#undef FB_ATTR_PRIVATE
#undef FB_TEXT
#undef FB_CHILD
#undef FB_ARRAY_PROPERTY
#undef FB_INHERIT

#define FB_PASS_PROPERTIES
#define FB_STRUCT( TYPE ) void TXB_binding( TYPE *str, fb::Properties *json, bool m )
#define FB_STRUCT_INHERIT( TYPE, ... ) void TXB_binding( TYPE *str, fb::Properties *json, bool m )
#define FB_CONSTRUCTOR( X )
#define FB_ATTR( X, HOLDER ) TXB_attr_bind( json, m, &str->HOLDER, #HOLDER )
#define FB_ATTR_NAME( X, HOLDER, NAME ) TXB_attr_bind( json, m, &str->HOLDER, NAME )
#define FB_ATTR_PRIVATE( X, Y )
#define FB_TEXT( X, HOLDER ) TXB_text_bind( json, m, &str->HOLDER )
#define FB_CHILD( CHILD, HOLDER ) TXB_ele_bind( json, m, &str->HOLDER, #CHILD )
#define FB_ARRAY_PROPERTY( CHILD, HOLDER ) TXB_ele_bind( json, m, &str->HOLDER, #HOLDER )
#define FB_INHERIT( X ) TXB_binding( (X *)str, json, m )
