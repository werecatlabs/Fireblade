#ifndef __FB_LogManager_h__
#define __FB_LogManager_h__

#include <FBCore/Interface/IApplicationManager.h>
#include <FBCore/Interface/System/ILogManager.h>

#if FB_VERBOSE_LOG
#    define FB_LOG_MESSAGE( x, y ) \
        using namespace fb; \
        using namespace fb::core; \
        if( auto applicationManager = IApplicationManager::instance() ) \
        { \
            if( auto logManager = applicationManager->getLogManager() ) \
            { \
                logManager->logMessage( ( y ), fb::ILogManager::Type::Info ); \
            } \
        }
#    define FB_LOG( x ) \
        using namespace fb; \
        using namespace fb::core; \
        if( auto applicationManager = IApplicationManager::instance() ) \
        { \
            if( auto logManager = applicationManager->getLogManager() ) \
            { \
                logManager->logMessage( ( x ), fb::ILogManager::Type::Info ); \
            } \
        }
#else
#    define FB_LOG_MESSAGE( x, y )
#    define FB_LOG( x )
#endif

#if FB_ERROR_LOG
#    define FB_LOG_ERROR( x ) \
        using namespace fb; \
        using namespace fb::core; \
        if( auto applicationManager = IApplicationManager::instance() ) \
        { \
            auto logManager = applicationManager->getLogManager(); \
            logManager->logMessage( ( x ), fb::ILogManager::Type::ErrorMessage ); \
        }
#    define FB_LOG_EXCEPTION( ex ) \
        using namespace fb; \
        using namespace fb::core; \
        if( auto applicationManager = IApplicationManager::instance() ) \
        { \
            if( auto logManager = applicationManager->getLogManager() ) \
            { \
                auto description = ex.what(); \
                logManager->logMessage( description, ILogManager::Type::Exception ); \
            } \
        }
#else
#    define FB_LOG_ERROR( x )
#    define FB_LOG_EXCEPTION( ex )
#endif

#define FB_LOG_SOURCE_MESSAGE( x )

#endif  // LogManager_h__
