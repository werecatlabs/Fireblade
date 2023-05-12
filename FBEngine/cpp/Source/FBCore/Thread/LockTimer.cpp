#include <FBCore/FBCorePCH.h>
#include <FBCore/Thread/LockTimer.h>
#include <FBCore/Thread/LockTracker.h>

#ifdef FB_USE_BOOST
#    include <boost/chrono/system_clocks.hpp>
#endif

namespace fb
{
    LockTimer::LockTimer( RecursiveMutex &mutex, const char *file, int line ) :
        m_file( file ),
        m_line( line ),
        m_mutex( mutex )
    {
#ifdef FB_USE_BOOST
        time_interval time =
            static_cast<double>(
                boost::chrono::high_resolution_clock::now().time_since_epoch().count() ) /
            static_cast<double>( 1000000000LL );
        m_startTime = static_cast<u32>( time * static_cast<time_interval>( 1000.0 ) );
#endif
    }

    LockTimer::~LockTimer()
    {
#ifdef FB_USE_BOOST
        time_interval time =
            static_cast<double>(
                boost::chrono::high_resolution_clock::now().time_since_epoch().count() ) /
            static_cast<double>( 1000000000LL );
        m_endTime = static_cast<u32>( time * static_cast<time_interval>( 1000.0 ) );
        m_timeTaken = m_endTime - m_startTime;

        // LockTracker::getSingleton().unlock(m_mutex, m_timeTaken, m_file.c_str(), m_line);
#endif
    }
}  // namespace fb
