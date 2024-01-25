#ifndef RttiClassDefinition_h__
#define RttiClassDefinition_h__

#include <FBCore/Memory/TypeManager.h>

#define FB_STRINGIZE_HELPER( x ) #x
#define FB_STRINGIZE( x ) FB_STRINGIZE_HELPER( x )

#define FB_CLASS_REGISTER( NAMESPACE, TYPE ) \
    void TYPE::setTypeInfo( u32 id ) \
    { \
        m_typeInfo = id; \
    } \
\
    u32 TYPE::typeInfo() \
    { \
        if( m_typeInfo == 0 ) \
        { \
            setupTypeInfo(); \
        } \
\
        return TYPE::m_typeInfo; \
    } \
\
    void TYPE::setupTypeInfo() \
    { \
        if( TYPE::m_typeInfo == 0 ) \
        { \
            auto typeManager = TypeManager::instance(); \
            auto name = String( #NAMESPACE ) + String( "::" ) + String( #TYPE ); \
            auto iTypeInfo = typeManager->getNewTypeId( name, 0 ); \
            setTypeInfo( iTypeInfo ); \
            typeManager->setLabel( iTypeInfo, #TYPE ); \
        } \
    } \
\
    u32 TYPE::getTypeInfo() const \
    { \
        auto iTypeInfo = TYPE::typeInfo(); \
        return iTypeInfo; \
    } \
\
    u32 TYPE::m_typeInfo = 0;

#define FB_CLASS_REGISTER_DERIVED( NAMESPACE, TYPE, BASE_TYPE ) \
    void TYPE::setTypeInfo( u32 id ) \
    { \
        m_typeInfo = id; \
    } \
\
    u32 TYPE::typeInfo() \
    { \
        if( m_typeInfo == 0 ) \
        { \
            setupTypeInfo(); \
        } \
\
        return TYPE::m_typeInfo; \
    } \
\
    void TYPE::setupTypeInfo() \
    { \
        if( TYPE::m_typeInfo == 0 ) \
        { \
            BASE_TYPE::setupTypeInfo(); \
            auto typeManager = TypeManager::instance(); \
            auto baseType = BASE_TYPE::typeInfo(); \
            auto name = String( #NAMESPACE ) + String( "::" ) + String( #TYPE ); \
            auto iTypeInfo = typeManager->getNewTypeId( name, baseType ); \
            setTypeInfo( iTypeInfo ); \
            typeManager->setLabel( iTypeInfo, #TYPE ); \
        } \
    } \
\
    u32 TYPE::getTypeInfo() const \
    { \
        return TYPE::typeInfo(); \
    } \
\
    u32 TYPE::m_typeInfo = 0;

#define FB_CLASS_REGISTER_DERIVED_TEMPLATE( NAMESPACE, TEMPLATE_CLASS, TYPE, BASE_TYPE ) \
    template <class T> \
    void TEMPLATE_CLASS<T>::setTypeInfo( u32 id ) \
    { \
        m_typeInfo = id; \
    } \
\
    template <class T> \
    u32 TEMPLATE_CLASS<T>::typeInfo() \
    { \
        if( m_typeInfo == 0 ) \
        { \
            setupTypeInfo(); \
        } \
\
        return m_typeInfo; \
    } \
\
    template <class T> \
    void TEMPLATE_CLASS<T>::setupTypeInfo() \
    { \
        if( TEMPLATE_CLASS<T>::m_typeInfo == 0 ) \
        { \
            BASE_TYPE::setupTypeInfo(); \
            auto typeManager = TypeManager::instance(); \
            auto baseType = BASE_TYPE::typeInfo(); \
            auto classType = typeManager->getName( T::typeInfo() ); \
            auto name = String( #NAMESPACE ) + String( "::" ) + String( #TEMPLATE_CLASS ) + "<" + \
                        String( classType ) + ">"; \
            auto iTypeInfo = typeManager->getNewTypeId( name, baseType ); \
            setTypeInfo( iTypeInfo ); \
            typeManager->setLabel( iTypeInfo, #TEMPLATE_CLASS ); \
        } \
    } \
\
    template <class T> \
    u32 TEMPLATE_CLASS<T>::getTypeInfo() const \
    { \
        return TEMPLATE_CLASS<T>::typeInfo(); \
    } \
\
    template <class T> \
    u32 TEMPLATE_CLASS<T>::m_typeInfo = 0;

#define FB_CLASS_REGISTER_DERIVED_TEMPLATE_TYPEID( NAMESPACE, TEMPLATE_CLASS, TYPE, BASE_TYPE ) \
    template <class T> \
    void TEMPLATE_CLASS<T>::setTypeInfo( u32 id ) \
    { \
        m_typeInfo = id; \
    } \
\
    template <class T> \
    u32 TEMPLATE_CLASS<T>::typeInfo() \
    { \
        if( m_typeInfo == 0 ) \
        { \
            setupTypeInfo(); \
        } \
\
        return m_typeInfo; \
    } \
\
    template <class T> \
    void TEMPLATE_CLASS<T>::setupTypeInfo() \
    { \
        if( TEMPLATE_CLASS<T>::m_typeInfo == 0 ) \
        { \
            BASE_TYPE::setupTypeInfo(); \
            auto typeManager = TypeManager::instance(); \
            auto baseType = BASE_TYPE::typeInfo(); \
            auto classType = typeid( T ).name(); \
            auto name = String( #NAMESPACE ) + String( "::" ) + String( #TEMPLATE_CLASS ) + "<" + \
                        String( classType ) + ">"; \
            auto iTypeInfo = typeManager->getNewTypeId( name, baseType ); \
            setTypeInfo( iTypeInfo ); \
            typeManager->setLabel( iTypeInfo, #TEMPLATE_CLASS ); \
        } \
    } \
\
    template <class T> \
    u32 TEMPLATE_CLASS<T>::getTypeInfo() const \
    { \
        return TEMPLATE_CLASS<T>::typeInfo(); \
    } \
\
    template <class T> \
    u32 TEMPLATE_CLASS<T>::m_typeInfo = 0;

#define FB_CLASS_REGISTER_DERIVED_TEMPLATE_PAIR( NAMESPACE, TEMPLATE_CLASS, TYPE, SECOND_TYPE, \
                                                 BASE_TYPE ) \
    template <class T, class U> \
    void TEMPLATE_CLASS<T, U>::setTypeInfo( u32 id ) \
    { \
        m_typeInfo = id; \
    } \
\
    template <class T, class U> \
    u32 TEMPLATE_CLASS<T, U>::typeInfo() \
    { \
        if( m_typeInfo == 0 ) \
        { \
            setupTypeInfo(); \
        } \
\
        return m_typeInfo; \
    } \
\
    template <class T, class U> \
    void TEMPLATE_CLASS<T, U>::setupTypeInfo() \
    { \
        if( TEMPLATE_CLASS<T, U>::m_typeInfo == 0 ) \
        { \
            BASE_TYPE::setupTypeInfo(); \
            auto typeManager = TypeManager::instance(); \
            auto baseType = BASE_TYPE::typeInfo(); \
            auto classTypeT = typeManager->getName( T::typeInfo() ); \
            auto classTypeU = typeManager->getName( U::typeInfo() ); \
            auto name = String( #NAMESPACE ) + String( "::" ) + String( #TEMPLATE_CLASS ) + "<" + \
                        String( classTypeT ) + ", " + String( classTypeU ) + ">"; \
            auto iTypeInfo = typeManager->getNewTypeId( name, baseType ); \
            setTypeInfo( iTypeInfo ); \
            typeManager->setLabel( iTypeInfo, #TEMPLATE_CLASS ); \
        } \
    } \
\
    template <class T, class U> \
    u32 TEMPLATE_CLASS<T, U>::getTypeInfo() const \
    { \
        return TEMPLATE_CLASS<T, U>::typeInfo(); \
    } \
\
    template <class T, class U> \
    u32 TEMPLATE_CLASS<T, U>::m_typeInfo = 0;

#define FB_CLASS_REGISTER_DERIVED_TEMPLATE_PAIR_TYPEID( NAMESPACE, TEMPLATE_CLASS, TYPE, SECOND_TYPE, \
                                                 BASE_TYPE ) \
    template <class T, class U> \
    void TEMPLATE_CLASS<T, U>::setTypeInfo( u32 id ) \
    { \
        m_typeInfo = id; \
    } \
\
    template <class T, class U> \
    u32 TEMPLATE_CLASS<T, U>::typeInfo() \
    { \
        if( m_typeInfo == 0 ) \
        { \
            setupTypeInfo(); \
        } \
\
        return m_typeInfo; \
    } \
\
    template <class T, class U> \
    void TEMPLATE_CLASS<T, U>::setupTypeInfo() \
    { \
        if( TEMPLATE_CLASS<T, U>::m_typeInfo == 0 ) \
        { \
            BASE_TYPE::setupTypeInfo(); \
            auto typeManager = TypeManager::instance(); \
            auto baseType = BASE_TYPE::typeInfo(); \
            auto classTypeT = typeid( T ).name(); \
            auto classTypeU = typeid( U ).name(); \
            auto name = String( #NAMESPACE ) + String( "::" ) + String( #TEMPLATE_CLASS ) + "<" + \
                        String( classTypeT ) + ", " + String( classTypeU ) + ">"; \
            auto iTypeInfo = typeManager->getNewTypeId( name, baseType ); \
            setTypeInfo( iTypeInfo ); \
            typeManager->setLabel( iTypeInfo, #TEMPLATE_CLASS ); \
        } \
    } \
\
    template <class T, class U> \
    u32 TEMPLATE_CLASS<T, U>::getTypeInfo() const \
    { \
        return TEMPLATE_CLASS<T, U>::typeInfo(); \
    } \
\
    template <class T, class U> \
    u32 TEMPLATE_CLASS<T, U>::m_typeInfo = 0;

#define FB_CLASS_REGISTER_DERIVED_TEMPLATE_SPECIALISE( NAMESPACE, TEMPLATE_CLASS, TYPE, BASE_TYPE ) \
    template <> \
    void TEMPLATE_CLASS<TYPE>::setTypeInfo( u32 id ) \
    { \
        m_typeInfo = id; \
    } \
\
    template <> \
    u32 TEMPLATE_CLASS<TYPE>::typeInfo() \
    { \
        if( m_typeInfo == 0 ) \
        { \
            setupTypeInfo(); \
        } \
\
        return m_typeInfo; \
    } \
\
    template <> \
    void TEMPLATE_CLASS<TYPE>::setupTypeInfo() \
    { \
        if( TEMPLATE_CLASS<TYPE>::m_typeInfo == 0 ) \
        { \
            BASE_TYPE::setupTypeInfo(); \
            auto typeManager = TypeManager::instance(); \
            auto baseType = BASE_TYPE::typeInfo(); \
            auto classType = typeManager->getName( TYPE::typeInfo() ); \
            auto name = String( #NAMESPACE ) + String( "::" ) + String( #TEMPLATE_CLASS ) + "<" + \
                        String( classType ) + ">"; \
            auto iTypeInfo = typeManager->getNewTypeId( name, baseType ); \
            setTypeInfo( iTypeInfo ); \
            typeManager->setLabel( iTypeInfo, #TEMPLATE_CLASS ); \
        } \
    } \
\
    template <> \
    u32 TEMPLATE_CLASS<TYPE>::getTypeInfo() const \
    { \
        return TEMPLATE_CLASS<TYPE>::typeInfo(); \
    } \
\
    template <> \
    u32 TEMPLATE_CLASS<TYPE>::m_typeInfo = 0;

#endif  // RttiClassDefinition_h__
