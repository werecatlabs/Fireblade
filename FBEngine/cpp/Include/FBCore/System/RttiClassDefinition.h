#ifndef RttiClassDefinition_h__
#define RttiClassDefinition_h__

#include <FBCore/Memory/TypeManager.h>

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
            auto iTypeInfo = typeManager->getNewTypeId( #TYPE, 0 ); \
            setTypeInfo( iTypeInfo ); \
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
            auto typeManager = TypeManager::instance(); \
            auto _typeInfo = typeManager->getNewTypeId( #TYPE, BASE_TYPE::typeInfo() ); \
            setTypeInfo( _typeInfo ); \
        } \
    } \
\
    u32 TYPE::getTypeInfo() const \
    { \
        return TYPE::typeInfo(); \
    } \
\
    u32 TYPE::m_typeInfo = 0; \
    ObjectSetup<TYPE> objectSetupTemplate##TYPE;

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
            auto typeManager = TypeManager::instance(); \
            auto baseTypeInfo = BASE_TYPE::typeInfo(); \
            auto templateTypeInfo = TYPE::typeInfo(); \
            auto templateTypeName = typeManager->getName( templateTypeInfo ); \
            auto classTypeName = String( #TEMPLATE_CLASS ) + "<" + templateTypeName + ">"; \
            auto classTypeInfo = typeManager->getNewTypeId( classTypeName, baseTypeInfo ); \
            setTypeInfo( classTypeInfo ); \
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
    u32 TEMPLATE_CLASS<T>::m_typeInfo = 0; \
    template <class T> \
    ObjectSetup<TEMPLATE_CLASS<TYPE> > TEMPLATE_CLASS<T>::objectSetupTemplate##TYPE;

#define FB_CLASS_REGISTER_DERIVED_TEMPLATE_PAIR( NAMESPACE, TEMPLATE_CLASS, TYPE, SECOND_TYPE, BASE_TYPE ) \
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
            auto typeManager = TypeManager::instance(); \
            auto baseTypeInfo = BASE_TYPE::typeInfo(); \
            auto templateTypeName = String(#TYPE) + String(", ") + String(#SECOND_TYPE); \
            auto classTypeName = String( #TEMPLATE_CLASS ) + "<" + templateTypeName + ">"; \
            auto classTypeInfo = typeManager->getNewTypeId( classTypeName, baseTypeInfo ); \
            setTypeInfo( classTypeInfo ); \
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
            auto typeManager = TypeManager::instance(); \
            auto _typeInfo = typeManager->getNewTypeId( #TEMPLATE_CLASS #TYPE, BASE_TYPE::typeInfo() ); \
            setTypeInfo( _typeInfo ); \
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
    u32 TEMPLATE_CLASS<TYPE>::m_typeInfo = 0; \
    template <> \
    ObjectSetup<TEMPLATE_CLASS<TYPE> > TEMPLATE_CLASS<T>::objectSetupTemplate##TYPE;

#endif  // RttiClassDefinition_h__
