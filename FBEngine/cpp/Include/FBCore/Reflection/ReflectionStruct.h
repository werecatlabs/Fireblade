#define FB_PASS_STRUCT
#define FB_STRUCT( TYPE ) struct TYPE
#define FB_STRUCT_INHERIT( TYPE, ... ) struct TYPE : __VA_ARGS__
#define FB_CLASS( TYPE ) class TYPE
#define FB_CLASS_INHERIT( TYPE, ... ) class TYPE : __VA_ARGS__
#define FB_CONSTRUCTOR( X ) X
#define FB_ATTR( X, Y ) X Y
#define FB_ATTR_NAME( X, Y, NAME ) X Y
#define FB_ATTR_PRIVATE( X, Y ) X Y
#define FB_TEXT( X, Y ) X Y
#define FB_CHILD( X, Y ) Array<X> Y
#define FB_ARRAY_PROPERTY( X, Y ) Array<X> Y
#define FB_INHERIT( X )
