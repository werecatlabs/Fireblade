#ifndef RttiClass_h__
#define RttiClass_h__

#include <boost/describe.hpp>
#include <boost/mp11.hpp>

/*template <class T>
class ObjectSetup
{
public:
    ObjectSetup()
    {
        T::setupTypeInfo();
    }
};*/

#define FB_OBJECT_CLASS_REGISTER_DECL \
public: \
    static void setTypeInfo( u32 id ); \
    static u32 typeInfo(); \
    static void setupTypeInfo(); \
    virtual u32 getTypeInfo() const; \
    static u32 m_typeInfo;

#define FB_CLASS_REGISTER_DECL \
public: \
    static void setTypeInfo( u32 id ); \
    static u32 typeInfo(); \
    static void setupTypeInfo(); \
    virtual u32 getTypeInfo() const override; \
    static u32 m_typeInfo;

#define FB_CLASS_REGISTER_TEMPLATE_DECL( TEMPLATE_CLASS, TYPE ) \
public: \
    static void setTypeInfo( u32 id ); \
    static u32 typeInfo(); \
    static void setupTypeInfo(); \
    virtual u32 getTypeInfo() const override; \
    static u32 m_typeInfo; \
    //static ObjectSetup<TEMPLATE_CLASS<TYPE>> objectSetupTemplate##TYPE;

#define FB_CLASS_REGISTER_TEMPLATE_PAIR_DECL( TEMPLATE_CLASS, TYPE, SECOND_TYPE ) \
public: \
    static void setTypeInfo( u32 id ); \
    static u32 typeInfo(); \
    static void setupTypeInfo(); \
    virtual u32 getTypeInfo() const override; \
    static u32 m_typeInfo; 

#define FB_REGISTER_FACTORY_DECL \
public: \
    static void registerFactory(); \
    static void unregisterFactory();

#define FB_DESCRIBE_CLASS( C, Bases, Public, Protected, Private ) \
    BOOST_DESCRIBE_CLASS( C, Bases, Public, Protected, Private )


#endif  // RttiClass_h__
