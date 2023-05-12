#include <FBCore/FBCorePCH.h>
#include <FBCore/Thread/LockTracker.h>
#include <FBCore/Thread/ILockTrackerListener.h>
#include <FBCore/Thread/Threading.h>

namespace fb
{
    FB_SHARED_DECL( LockTrackerMutex );

    LockTracker::LockTracker()
    {
    }

    LockTracker::~LockTracker()
    {
    }

    LockTracker &LockTracker::getSingleton()
    {
        static LockTracker lockTracker;
        return lockTracker;
    }

#if FB_USE_TBB
    void LockTracker::lock( tbb::recursive_mutex &mutex, const char *file /*= _FILE_*/,
                            int line /*= _LINE_*/ )
    {
        FB_SHARED_READ_LOCK( LockTrackerMutex );
        for( u32 i = 0; i < m_listeners.size(); ++i )
        {
            ILockTrackerListener *listener = m_listeners[i];
            if( listener )
                listener->lock( mutex, file, line );
        }
    }

    void LockTracker::unlock( tbb::recursive_mutex &mutex, u32 timeTaken /*= 0*/,
                              const char *file /*= __FILE__*/, int line /*= __LINE__*/ )
    {
        FB_SHARED_READ_LOCK( LockTrackerMutex );
        for( u32 i = 0; i < m_listeners.size(); ++i )
        {
            ILockTrackerListener *listener = m_listeners[i];
            if( listener )
                listener->unlock( mutex, timeTaken, file, line );
        }
    }
#else
    void LockTracker::lock( std::recursive_mutex &mutex, const char *file /*= _FILE_*/,
                            int line /*= _LINE_*/ )
    {
        FB_SHARED_READ_LOCK( LockTrackerMutex );
        for( u32 i = 0; i < m_listeners.size(); ++i )
        {
            ILockTrackerListener *listener = m_listeners[i];
            if( listener )
                listener->lock( mutex, file, line );
        }
    }

    void LockTracker::unlock( std::recursive_mutex &mutex, u32 timeTaken /*= 0*/,
                              const char *file /*= __FILE__*/, int line /*= __LINE__*/ )
    {
        FB_SHARED_READ_LOCK( LockTrackerMutex );
        for( u32 i = 0; i < m_listeners.size(); ++i )
        {
            ILockTrackerListener *listener = m_listeners[i];
            if( listener )
                listener->unlock( mutex, timeTaken, file, line );
        }
    }
#endif

    void LockTracker::addListener( ILockTrackerListener *listener )
    {
        FB_SHARED_WRITE_LOCK( LockTrackerMutex );
        m_listeners.push_back( listener );
    }

    void LockTracker::removeListener( ILockTrackerListener *listener )
    {
        FB_SHARED_WRITE_LOCK( LockTrackerMutex );

        auto it = std::find( m_listeners.begin(), m_listeners.end(), listener );
        if( it != m_listeners.end() )
        {
            m_listeners.erase( it );
        }
    }
}  // end namespace fb
