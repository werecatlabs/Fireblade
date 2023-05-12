#ifndef __FBThreadingBoost_H__
#define __FBThreadingBoost_H__

#pragma warning( disable : 4005 )
#pragma warning( disable : 4541 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4245 )  // for using all bits

#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/thread.hpp>

#define FB_MUTEX( name ) mutable boost::recursive_mutex name;
#define FB_MUTEX_STATIC( name ) static boost::recursive_mutex name;
#define FB_MUTEX_EXTERN( name ) extern boost::recursive_mutex name;
#define FB_LOCK_MUTEX( name ) boost::recursive_mutex::scoped_lock mutexLock( name );
#define FB_MUTEX_MUTABLE( name ) mutable boost::recursive_mutex name;
#define FB_MUTEX_STATIC_DECL( class_name, name ) boost::recursive_mutex class_name::name;

#define FB_SHARED_MUTEX( name ) boost::shared_mutex name;
#define FB_SHARED_MUTEX_MUTABLE( name ) mutable boost::shared_mutex name;
#define FB_SHARED_DECL( name ) boost::shared_mutex name;

#define FB_SHARED_READ_LOCK( name ) boost::shared_lock<boost::shared_mutex> nameLock( name )
#define FB_SHARED_WRITE_LOCK( name ) boost::unique_lock<boost::shared_mutex> nameLock( name )
#define FB_MUTEX_DECL( name ) boost::recursive_mutex name;

#define UNIQUE_UPGRADE_LOCK( x ) FB_SHARED_WRITE_LOCK( x )

#define FB_LOCK_MUTEX( mutex ) boost::recursive_mutex::scoped_lock scoped_lock( mutex );
#define RECURSIVE_LOCK_NAMED( mutex, name ) boost::recursive_mutex::scoped_lock name( mutex );

#define FB_SPIN_MUTEX( name ) boost::shared_mutex name;
#define FB_SPIN_MUTEX_MUTABLE( name ) mutable boost::shared_mutex name;
#define FB_SPIN_LOCK( name ) boost::unique_lock<boost::shared_mutex> nameLock( name )
#define FB_SPIN_LOCK_READ( name ) boost::shared_lock<boost::shared_mutex> nameLock( name )
#define FB_SPIN_LOCK_WRITE( name ) boost::unique_lock<boost::shared_mutex> nameLock( name )

#define FB_THREAD_SLEEP( ms ) \
    tbb::this_tbb_thread::sleep( tbb::tick_count::interval_t( double( ms ) / 1000 ) )

#endif
