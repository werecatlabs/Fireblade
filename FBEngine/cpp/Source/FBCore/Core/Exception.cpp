#include <FBCore/FBCorePCH.h>
#include <FBCore/Core/Exception.h>
#include <FBCore/Core/StringUtil.h>
#include <FBCore/Core/LogManager.h>
#include <iostream>
#include <sstream>

#if FB_ENABLE_TRACE
#    include <boost/stacktrace.hpp>
#endif

namespace fb
{

    Exception::Exception()
    {
    }

#if defined FB_PLATFORM_WIN32
    Exception::Exception( const String &description, const char *file, s32 line, const char *func ) :
        std::exception(
            ( description + " file: " + file + " line: " + StringUtil::toString( line ) ).c_str() )
#else
    Exception::Exception( const String &description, const char *file, s32 line, const char *func )
#endif
    {
        m_file = file ? file : "";
        m_line = line;
        m_source = func ? func : "";

        std::stringstream strStream;
        strStream << description.c_str() << std::endl;

        std::cout << strStream.str() << std::endl;

#if FB_ENABLE_TRACE
        strStream << boost::stacktrace::stacktrace();

        m_description = strStream.str() + String( " " ) + m_file + String( " " ) +
                        StringUtil::toString( (s32)m_line ) + String( " " ) + m_source;

        FB_LOG_ERROR( m_description );
#endif
    }

    Exception::~Exception() = default;

    RuntimeException::RuntimeException( const String &description, const char *file /*= __FILE__*/,
                                        long line /*= __LINE__*/, const char *func /*= 0 */ ) :
        Exception( description, file, line, func )
    {
    }

    FatalException::FatalException( const String &description, const char *file /*= __FILE__*/,
                                    long line /*= __LINE__*/, const char *func /*= 0 */ ) :
        Exception( description, file, line, func )
    {
    }

    ScriptException::ScriptException( const String &description, const char *file /*= __FILE__*/,
                                      long line /*= __LINE__*/, const char *func /*= 0 */ ) :
        Exception( description, file, line, func )
    {
    }
}  // namespace fb
