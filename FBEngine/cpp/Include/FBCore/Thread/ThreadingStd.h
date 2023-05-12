#ifndef __FB_ThreadingTBB_H__
#define __FB_ThreadingTBB_H__

#define FB_MUTEX( name )
#define FB_MUTEX_STATIC( name )
#define FB_MUTEX_EXTERN( name )
#define FB_MUTEX_MUTABLE( name )
#define FB_MUTEX_STATIC_DECL( class_name, name )
#define FB_LOCK_MUTEX( name )
#define FB_LOCK_MUTEX_NAMED( mutex, name )
#define FB_SHARED_MUTEX( name )
#define FB_SHARED_MUTEX_MUTABLE( name )
#define FB_SHARED_DECL( name )
#define FB_SHARED_READ_LOCK( name )
#define FB_SHARED_WRITE_LOCK( name )
#define FB_SHARED_READ_LOCK_NAMED( name, lockname )
#define FB_SHARED_WRITE_LOCK_NAMED( name, lockname )
#define FB_MUTEX_DECL( name )
#define FB_SPIN_MUTEX( mutex )
#define FB_SPIN_MUTEX_MUTABLE( mutex )
#define FB_SPIN_LOCK( mutex )
#define FB_SPIN_LOCK_READ( mutex )
#define FB_SPIN_LOCK_WRITE( mutex )
#define FB_SPIN_LOCK_READ_NAMED( mutex, name )
#define FB_SPIN_LOCK_WRITE_NAMED( mutex, name )

#endif
