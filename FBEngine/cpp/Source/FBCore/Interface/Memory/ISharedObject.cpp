#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Interface/Memory/ISharedObjectListener.h>
#include <FBCore/Interface/Memory/IData.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Memory/ObjectTracker.h>
#include <FBCore/Memory/GarbageCollector.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/IApplicationManager.h>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ISharedObject, IObject );

    ISharedObject::ScopedLock::ScopedLock( SmartPtr<ISharedObject> sharedObject ) :
        m_sharedObject( sharedObject )
    {
        if( m_sharedObject )
        {
            m_sharedObject->lock();
        }
    }

    ISharedObject::ScopedLock::~ScopedLock()
    {
        if( m_sharedObject )
        {
            m_sharedObject->unlock();
        }
    }

    s32 ISharedObject::addWeakReference( void *address, const c8 *file, u32 line, const c8 *func )
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_WEAK_REFERENCES
        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );
#    endif
#endif

        return ++( *m_weakReferences );
    }

    bool ISharedObject::removeWeakReference()
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_WEAK_REFERENCES
        auto address = (void *)this;
        const c8 *file = __FILE__;
        const u32 line = __LINE__;
        const c8 *func = __FUNCTION__;

        auto &objectTracker = ObjectTracker::instance();
        objectTracker.removeRef( this, address, file, line, func );
#    endif
#endif

        return --( *m_weakReferences ) == 0;
    }

    bool ISharedObject::removeWeakReference( void *address, const c8 *file, u32 line, const c8 *func )
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_WEAK_REFERENCES
        // If reference tracking is enabled, remove the weak reference tracking information.
        auto &objectTracker = ObjectTracker::instance();
        objectTracker.removeRef( this, address, file, line, func );
#    endif
#endif

        // Decrement the weak reference count and return true if the count is now 0.
        return --( *m_weakReferences ) == 0;
    }

    bool ISharedObject::removeReference( void *address, const c8 *file, const u32 line, const c8 *func )
    {
#if FB_TRACK_REFERENCES
        auto &objectTracker = ObjectTracker::instance();
        objectTracker.removeRef( this, address, file, line, func );
#endif

        if( --( *m_references ) == 0 )
        {
            auto typeInfo = getTypeInfo();
            auto &gc = GarbageCollector::instance();
            gc.destroyObject( typeInfo, m_objectId );
        }

        return *m_references == 0;
    }

    bool ISharedObject::removeReference()
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_STRONG_REFERENCES
        auto address = (void *)this;
        const c8 *file = __FILE__;
        const u32 line = __LINE__;
        const c8 *func = __FUNCTION__;

        auto &objectTracker = ObjectTracker::instance();
        objectTracker.removeRef( this, address, file, line, func );
#    endif
#endif

        if( --( *m_references ) == 0 )
        {
            auto &gc = GarbageCollector::instance();
            gc.destroyObject( getTypeInfo(), m_objectId );
            return true;
        }

        return false;
    }

    s32 ISharedObject::addReference( void *address, const c8 *file, const u32 line, const c8 *func )
    {
#if FB_TRACK_REFERENCES
        auto references = ++( *m_references );

        // FB_ASSERT( isGarbageCollected() && references > 0 );
        FB_ASSERT( references < 1e10 );

#    if FB_TRACK_STRONG_REFERENCES
        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );
#    endif

        return references;
#else
        return ++( *m_references );
#endif
    }

    void ISharedObject::load( SmartPtr<ISharedObject> data )
    {
    }

    void ISharedObject::reload( SmartPtr<ISharedObject> data )
    {
    }

    void ISharedObject::unload( SmartPtr<ISharedObject> data )
    {
    }

    const Atomic<LoadingState> &ISharedObject::getLoadingState() const
    {
        return *m_loadingState;
    }

    void ISharedObject::setLoadingState( const Atomic<LoadingState> &state )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto &gc = GarbageCollector::instance();

        auto oldState = gc.getLoadingState( getTypeInfo(), m_objectId );
        if( oldState != state )
        {
            *m_loadingState = state;

            auto args = Array<Parameter>();
            args.resize( 2 );

            args[0] = Parameter( (s32)oldState.load() );
            args[1] = Parameter( (s32)state.load() );

            if( applicationManager )
            {
                applicationManager->triggerEvent( IEvent::Type::Object, IEvent::loadingStateChanged,
                                                  args, this, this, nullptr );
            }
        }
    }

    bool ISharedObject::isLoading() const
    {
        return *m_loadingState == LoadingState::Loading;
    }

    bool ISharedObject::isLoadingQueued() const
    {
        return *m_loadingState == LoadingState::LoadingQueued;
    }

    bool ISharedObject::isLoaded() const
    {
        return *m_loadingState == LoadingState::Loaded;
    }

    bool ISharedObject::isThreadSafe() const
    {
        return false;
    }

    bool ISharedObject::isAlive() const
    {
        return ( *m_flags & GC_FLAG_OBJECT_ALIVE ) != 0;
    }

    void ISharedObject::setPoolElement( bool poolElement )
    {
        if( poolElement )
        {
            ( *m_flags ) = *m_flags | GC_FLAG_POOL_ELEMENT;
        }
        else
        {
            ( *m_flags ) = *m_flags & ~GC_FLAG_POOL_ELEMENT;
        }
    }

    bool ISharedObject::isPoolElement() const
    {
        return ( *m_flags & GC_FLAG_POOL_ELEMENT ) != 0;
    }

    SmartPtr<ISharedObject> ISharedObject::toData() const
    {
        return nullptr;
    }

    void ISharedObject::fromData( SmartPtr<ISharedObject> data )
    {
    }

    Array<SmartPtr<ISharedObject>> ISharedObject::getChildObjects() const
    {
        return Array<SmartPtr<ISharedObject>>();
    }

    Array<SmartPtr<IEventListener>> ISharedObject::getObjectListeners() const
    {
        if( auto p = getObjectListenersPtr() )
        {
            auto &listeners = *p;
            return Array<SmartPtr<IEventListener>>( listeners.begin(), listeners.end() );
        }

        return Array<SmartPtr<IEventListener>>();
    }

    u32 ISharedObject::getNumListeners() const
    {
        if( auto p = getObjectListenersPtr() )
        {
            auto &listeners = *p;
            return (u32)listeners.size();
        }

        return 0;
    }

    bool ISharedObject::hasObjectListener( SmartPtr<IEventListener> listener ) const
    {
        if( auto p = getObjectListenersPtr() )
        {
            auto &listeners = *p;
            auto it = std::find( listeners.begin(), listeners.end(), listener );
            if( it != listeners.end() )
            {
                return true;
            }
        }

        return false;
    }

    void ISharedObject::addObjectListener( SmartPtr<IEventListener> listener )
    {
        auto p = getObjectListenersPtr();
        if( !p )
        {
            p = fb::make_shared<ConcurrentArray<SmartPtr<IEventListener>>>();
            setObjectListenersPtr( p );
        }

        if( p )
        {
            auto &listeners = *p;
            listeners.push_back( listener );
        }
    }

    void ISharedObject::removeObjectListener( SmartPtr<IEventListener> listener )
    {
        auto listeners = getObjectListeners();
        listeners.erase( std::remove( listeners.begin(), listeners.end(), listener ), listeners.end() );

        auto p = fb::make_shared<ConcurrentArray<SmartPtr<IEventListener>>>( listeners.begin(),
                                                                             listeners.end() );
        setObjectListenersPtr( p );
    }

    SmartPtr<IEventListener> ISharedObject::findObjectListener( const String &id ) const
    {
        auto listeners = getObjectListeners();
        for( auto listener : listeners )
        {
            if( auto handle = listener->getHandle() )
            {
                if( handle->getName() == id )
                {
                    return listener;
                }
            }
        }

        return nullptr;
    }

    ISharedObjectListener *ISharedObject::getSharedObjectListener() const
    {
        return m_sharedObjectListener.load();
    }

    void ISharedObject::setSharedObjectListener( ISharedObjectListener *listener )
    {
        m_sharedObjectListener = listener;
    }

    bool ISharedObject::getEnableReferenceTracking() const
    {
        return ( ( *m_flags ) & GC_FLAG_ENABLE_REFERENCE_TRACKING ) != 0;
    }

    void ISharedObject::setEnableReferenceTracking( bool referenceTracking )
    {
        if( referenceTracking )
        {
            ( *m_flags ).fetch_or( GC_FLAG_ENABLE_REFERENCE_TRACKING );
        }
        else
        {
            ( *m_flags ).fetch_and( ~GC_FLAG_ENABLE_REFERENCE_TRACKING );
        }
    }

    bool ISharedObject::isGarbageCollected() const
    {
        return ( ( *m_flags ) & GC_FLAG_GARBAGE_COLLECTED ) != 0;
    }

    void ISharedObject::setGarbageCollected( bool garbageCollected )
    {
        if( garbageCollected )
        {
            ( *m_flags ).fetch_or( GC_FLAG_GARBAGE_COLLECTED );
        }
        else
        {
            ( *m_flags ).fetch_and( ~GC_FLAG_GARBAGE_COLLECTED );
        }
    }

    u32 ISharedObject::getObjectId() const
    {
        return m_objectId;
    }

    SmartPtr<ISharedObject> ISharedObject::getScriptData() const
    {
        return m_scriptData;
    }

    void ISharedObject::setScriptData( SmartPtr<ISharedObject> data )
    {
        m_scriptData = data;
    }

    void ISharedObject::setupGarbageCollectorData()
    {
        if( m_objectId != std::numeric_limits<u32>::max() )
        {
            auto &gc = GarbageCollector::instance();

            auto typeInfo = getTypeInfo();
            m_references = gc.getReferencesPtr( typeInfo, m_objectId );
            m_weakReferences = gc.getWeakReferencesPtr( typeInfo, m_objectId );
            m_flags = gc.getFlagPtr( typeInfo, m_objectId );
            m_handle = gc.getHandle( typeInfo, m_objectId );
            m_loadingState = gc.getLoadingStatePtr( typeInfo, m_objectId );
        }
    }

    void ISharedObject::lock()
    {
    }

    void ISharedObject::unlock()
    {
    }

    SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> ISharedObject::getObjectListenersPtr() const
    {
        return m_sharedEventListeners;
    }

    void ISharedObject::setObjectListenersPtr( SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> p )
    {
        m_sharedEventListeners = p;
    }
}  // end namespace fb
