#ifndef __FB_ThreadingTBB_H__
#define __FB_ThreadingTBB_H__

#if FB_USE_ONETBB
#    include <tbb/spin_rw_mutex.h>
#    include <tbb/queuing_rw_mutex.h>
#    include <mutex>
#elif FB_USE_TBB
#    include <tbb/spin_rw_mutex.h>
#    include <tbb/recursive_mutex.h>
#    include <tbb/queuing_rw_mutex.h>
#endif

#if FB_USE_ONETBB
#    define FB_MUTEX( name ) std::recursive_mutex name;
#    define FB_MUTEX_STATIC( name ) static std::recursive_mutex name;
#    define FB_MUTEX_EXTERN( name ) extern std::recursive_mutex name;
#    define FB_MUTEX_MUTABLE( name ) mutable std::recursive_mutex name;
#    define FB_MUTEX_STATIC_DECL( class_name, name ) std::recursive_mutex class_name::name;
#    define FB_MUTEX_DECL( name ) std::recursive_mutex name;

#    if FB_LOCK_TRACKER
#        define FB_LOCK_MUTEX( name ) \
            std::lock_guard<std::recursive_mutex> mutexLock( name ); \
            fb::LockTracker::getSingleton().lock( name, __FILE__, __LINE__ ); \
            fb::LockTimer timer( name, __FILE__, __LINE__ );
#    else
#        define FB_LOCK_MUTEX( name ) std::lock_guard<std::recursive_mutex> mutexLock( name );
#    endif

#    define FB_LOCK_MUTEX_NAMED( mutex, name ) std::lock_guard<std::recursive_mutex> name( mutex );
#elif FB_USE_TBB
#    define FB_MUTEX( name ) tbb::recursive_mutex name;
#    define FB_MUTEX_STATIC( name ) static tbb::recursive_mutex name;
#    define FB_MUTEX_EXTERN( name ) extern tbb::recursive_mutex name;
#    define FB_MUTEX_MUTABLE( name ) mutable tbb::recursive_mutex name;
#    define FB_MUTEX_STATIC_DECL( class_name, name ) tbb::recursive_mutex class_name::name;
#    define FB_MUTEX_DECL( name ) tbb::recursive_mutex name;

#    if FB_LOCK_TRACKER
#        define FB_LOCK_MUTEX( name ) \
            tbb::recursive_mutex::scoped_lock mutexLock( name ); \
            fb::LockTracker::getSingleton().lock( name, __FILE__, __LINE__ ); \
            fb::LockTimer timer( name, __FILE__, __LINE__ );
#    else
#        define FB_LOCK_MUTEX( name ) tbb::recursive_mutex::scoped_lock mutexLock( name );
#    endif

#    define FB_LOCK_MUTEX_NAMED( mutex, name ) tbb::recursive_mutex::scoped_lock name( mutex );
#endif

#define FB_SHARED_MUTEX( name ) mutable tbb::queuing_rw_mutex name;
#define FB_SHARED_DECL( name ) tbb::queuing_rw_mutex name;
#define FB_SHARED_READ_LOCK( name ) tbb::queuing_rw_mutex::scoped_lock nameLock( name, false )
#define FB_SHARED_WRITE_LOCK( name ) tbb::queuing_rw_mutex::scoped_lock nameLock( name, true )
#define FB_SHARED_READ_LOCK_NAMED( name, lockname ) \
    tbb::queuing_rw_mutex::scoped_lock lockname( name, false )
#define FB_SHARED_WRITE_LOCK_NAMED( name, lockname ) \
    tbb::queuing_rw_mutex::scoped_lock lockname( name, true )
#define FB_SHARED_MUTEX_MUTABLE( name ) mutable tbb::queuing_rw_mutex name;

#define FB_SPIN_MUTEX( mutex ) mutable tbb::spin_rw_mutex mutex;
#define FB_SPIN_MUTEX_MUTABLE( name ) mutable tbb::spin_rw_mutex name;
#define FB_SPIN_LOCK( mutex ) tbb::spin_rw_mutex::scoped_lock spinLock( mutex );
#define FB_SPIN_LOCK_READ( mutex ) tbb::spin_rw_mutex::scoped_lock spinLockRead( mutex, false );
#define FB_SPIN_LOCK_WRITE( mutex ) tbb::spin_rw_mutex::scoped_lock spinLockWrite( mutex, true );
#define FB_SPIN_LOCK_READ_NAMED( mutex, name ) tbb::spin_rw_mutex::scoped_lock name( mutex, false );
#define FB_SPIN_LOCK_WRITE_NAMED( mutex, name ) tbb::spin_rw_mutex::scoped_lock name( mutex, true );

#endif
