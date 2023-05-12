#ifndef LockTracker_h__
#define LockTracker_h__

#include <FBCore/FBCorePrerequisites.h>
#include <FBCore/Base/Array.h>

#if FB_USE_TBB
#    include <tbb/spin_rw_mutex.h>
#    include <tbb/recursive_mutex.h>
#    include <tbb/queuing_rw_mutex.h>
#    include <tbb/tbb_thread.h>
#else
#    include <mutex>
#endif

namespace fb
{
    class LockTracker
    {
    public:
        LockTracker();
        ~LockTracker();

        static LockTracker &getSingleton();

#if FB_USE_TBB
        void lock( tbb::recursive_mutex &mutex, const char *file = __FILE__, int line = __LINE__ );
        void unlock( tbb::recursive_mutex &mutex, u32 timeTaken = 0, const char *file = __FILE__,
                     int line = __LINE__ );
#else
        void lock( std::recursive_mutex &mutex, const char *file = __FILE__, int line = __LINE__ );
        void unlock( std::recursive_mutex &mutex, u32 timeTaken = 0, const char *file = __FILE__,
                     int line = __LINE__ );
#endif

        void addListener( ILockTrackerListener *listener );
        void removeListener( ILockTrackerListener *listener );

    private:
        Array<ILockTrackerListener *> m_listeners;
    };
}  // end namespace fb

#endif  // LockTracker_h__
