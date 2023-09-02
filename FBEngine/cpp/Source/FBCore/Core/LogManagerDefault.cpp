#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/LogManagerDefault.h>
#include <FBCore/FBCore.h>
#include <iostream>

#if FB_ENABLE_TRACE
#    include <boost/stacktrace.hpp>
#endif

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, LogManagerDefault, ILogManager );

    SmartPtr<LogManagerDefault> LogManagerDefault::m_instance;

    void LogManagerDefault::logMessage( const String &message, [[maybe_unused]] Type type )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            if( applicationManager )
            {
                if( auto pluginInterface = applicationManager->getPluginInterface() )
                {
                    auto factoryManager = applicationManager->getFactoryManager();
                    auto pluginEvent = factoryManager->make_ptr<core::PluginEvent>();
                    pluginEvent->setArg1( message );
                    pluginInterface->sendEvent( "logInfo", pluginEvent );
                }
            }

            std::cout << message << std::endl;

#if FB_ENABLE_TRACE
            static const auto newLineStr = String( "\n" );

            std::stringstream strStream;
            strStream << boost::stacktrace::stacktrace();
            auto trace = strStream.str();

            auto currentTimeStr = StringUtil::getCurrentTime() + String( ": " );
            auto logEntryStr = currentTimeStr + message + newLineStr + trace;
#else
            auto currentTimeStr = StringUtil::getCurrentTime() + String( ": " );
            auto logEntryStr = currentTimeStr + message;
#endif

            if( getEnableQueue() || !m_log.is_open() )
            {
                m_messages.push( logEntryStr );
            }
            else
            {
                String curMessage;
                while( m_messages.try_pop( curMessage ) )
                {
                    m_log << curMessage.c_str() << std::endl;
                }

                m_log << logEntryStr.c_str() << std::endl;
                m_log.flush();
            }
        }
        catch( std::exception &e )
        {
            std::cout << e.what() << std::endl;
        }
    }

    void LogManagerDefault::logMessage( const StringW &message, Type type )
    {
        try
        {
            auto applicationManager = core::IApplicationManager::instance();
            if( applicationManager )
            {
                if( auto pluginInterface = applicationManager->getPluginInterface() )
                {
                    auto factoryManager = applicationManager->getFactoryManager();
                    auto pluginEvent = factoryManager->make_ptr<core::PluginEvent>();
                    pluginEvent->setArg1( StringUtil::toUTF16to8( message ) );
                    pluginInterface->sendEvent( "logInfo", pluginEvent );
                }
            }

            StringW currentTimeStr = StringUtilW::getCurrentTime() + StringW( L": " );
            StringW logEntryStr = currentTimeStr + message;

            if( getEnableQueue() || !m_log.is_open() )
            {
                m_messages.push( StringUtil::toStringC( logEntryStr ) );
            }
            else
            {
                String message;
                while( m_messages.try_pop( message ) )
                {
                    m_log << message.c_str() << std::endl;
                }

                m_log << StringUtil::toUTF16to8( logEntryStr ) << std::endl;
                m_log.flush();
            }
        }
        catch( std::exception &e )
        {
            std::cout << e.what() << std::endl;
        }
    }

    void LogManagerDefault::setEnableQueue( bool queue )
    {
        m_queue = queue;
    }

    bool LogManagerDefault::getEnableQueue() const
    {
        return m_queue;
    }

    void LogManagerDefault::close()
    {
        m_log.close();
    }

    void LogManagerDefault::open( const String &filePath )
    {
        if( !m_log.is_open() )
        {
            m_log.open( filePath );
        }
    }

    void LogManagerDefault::open( const StringW &filePath )
    {
#if defined FB_PLATFORM_WIN32
        if( !m_log.is_open() )
        {
            m_log.open( filePath );
        }
#else
        auto filePathUTF8 = StringUtil::toUTF16to8( filePath );
        if( !m_log.is_open() )
        {
            m_log.open( filePathUTF8 );
        }
#endif
    }

    void LogManagerDefault::flush()
    {
        String message;
        while( m_messages.try_pop( message ) )
        {
            m_log << message.c_str() << std::endl;
        }

        m_log.flush();
    }
}  // end namespace fb
