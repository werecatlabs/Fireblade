#include <FBCore/FBCorePCH.h>
#include <FBCore/System/DebugUtil.h>

#if FB_ENABLE_TRACE
#    include <boost/stacktrace.hpp>
#endif

namespace fb
{

    String DebugUtil::getStackTrace()
    {
        std::stringstream strStream;

#if FB_ENABLE_TRACE
        strStream << boost::stacktrace::stacktrace();
#endif

        return strStream.str();
    }

    String DebugUtil::getStackTraceForException( std::exception &e )
    {
        std::stringstream strStream;

#if FB_ENABLE_TRACE
        strStream << boost::stacktrace::stacktrace();
#endif

        return strStream.str();
    }

}  // namespace fb
