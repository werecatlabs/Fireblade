#include <FBCore/FBCorePCH.h>
#include <FBCore/System/DebugUtil.h>

#if FB_ENABLE_TRACE
#    include <boost/stacktrace.hpp>
#endif

namespace fb
{

    auto DebugUtil::getStackTrace() -> String
    {
        std::stringstream strStream;

#if FB_ENABLE_TRACE
        strStream << boost::stacktrace::stacktrace();
#endif

        return strStream.str();
    }

    auto DebugUtil::getStackTraceForException( std::exception &e ) -> String
    {
        std::stringstream strStream;

#if FB_ENABLE_TRACE
        strStream << boost::stacktrace::stacktrace();
#endif

        return strStream.str();
    }

}  // namespace fb
