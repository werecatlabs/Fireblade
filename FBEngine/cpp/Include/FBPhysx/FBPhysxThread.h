#ifndef FBPhysxThread_h__
#define FBPhysxThread_h__

#if FB_TRACK_LOCKS
#define FB_PXSCENE_READ_LOCK( scene ) \
    physx::PxSceneReadLock sceneReadLock( *scene, __FILE__, __LINE__ ); \
    LockTracker::getSingleton().trackSceneLock( scene, __FILE__, __LINE__ );

#define FB_PXSCENE_WRITE_LOCK( scene ) \
    physx::PxSceneWriteLock sceneWriteLock( *scene, __FILE__, __LINE__ ); \
    LockTracker::getSingleton().trackSceneLock( scene, __FILE__, __LINE__ );
#else 
#    define FB_PXSCENE_READ_LOCK( scene ) \
        physx::PxSceneReadLock sceneReadLock( *scene, __FILE__, __LINE__ );
#    define FB_PXSCENE_READ_LOCK_NAMED( scene, name ) \
        physx::PxSceneReadLock name( *scene, __FILE__, __LINE__ );
#    define FB_PXSCENE_WRITE_LOCK( scene ) \
        physx::PxSceneWriteLock sceneWriteLock( *scene, __FILE__, __LINE__ );
#    define FB_PXSCENE_WRITE_LOCK_NAMED( scene, name ) \
        physx::PxSceneWriteLock name( *scene, __FILE__, __LINE__ );
#endif

#endif // FBPhysxThread_h__
