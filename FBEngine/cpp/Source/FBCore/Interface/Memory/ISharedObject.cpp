#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Interface/Memory/ISharedObjectListener.h>
#include <FBCore/Interface/Memory/IData.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Memory/ObjectTracker.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Interface/IApplicationManager.h>
#include <boost/pool/singleton_pool.hpp>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ISharedObject, IObject );
    typedef boost::singleton_pool<Handle, sizeof( Handle )> HandlePool;

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

    ISharedObject::ISharedObject()
    {
#if _DEBUG
        constexpr auto basesize = sizeof( IObject );
        constexpr auto size = sizeof( ISharedObject );
        constexpr auto handleSize = sizeof( Handle );
#endif

        auto pHandle = HandlePool::malloc();
        m_handle = new( pHandle ) Handle;
        m_objectFlags = GC_FLAG_OBJECT_ALIVE | GC_FLAG_GARBAGE_COLLECTED;
        m_loadingState = LoadingState::Allocated;

#if FB_TRACK_REFERENCES
        setEnableReferenceTracking( true );
#endif
    }

    ISharedObject::~ISharedObject()
    {
        if( auto handle = m_handle.load() )
        {
            handle->~Handle();
            HandlePool::free( handle );
            m_handle = nullptr;
        }

        //FB_ASSERT( m_references == 0 );
        //FB_ASSERT( m_weakReferences == 0 );
    }

    s32 ISharedObject::addWeakReference( void *address, const c8 *file, u32 line, const c8 *func )
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_WEAK_REFERENCES
        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );
#    endif
#endif

        return ++m_weakReferences;
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

        return --m_weakReferences == 0;
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
        return --m_weakReferences == 0;
    }

    bool ISharedObject::removeReference( void *address, const c8 *file, const u32 line, const c8 *func )
    {
#if FB_TRACK_REFERENCES
        auto &objectTracker = ObjectTracker::instance();
        objectTracker.removeRef( this, address, file, line, func );
#endif

        if( --m_references == 0 )
        {
            destroySharedObject();
            return true;
        }

        return false;
    }

    void ISharedObject::destroySharedObject()
    {
        m_loadingState = LoadingState::Unallocated;
        setObjectFlag( GC_FLAG_OBJECT_ALIVE, false );

        bool destroyed = false;

        if( auto listener = getSharedObjectListener() )
        {
            destroyed = listener->destroy( this );
        }

        if( !destroyed )
        {
            if( !isPoolElement() )
            {
                delete this;
            }
        }
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
        return ++m_references;
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
        return m_loadingState;
    }

    void ISharedObject::setLoadingState( const Atomic<LoadingState> &state )
    {
        auto applicationManager = core::IApplicationManager::instance();
        if( applicationManager )
        {
            if( applicationManager->isLoaded() )
            {
                auto oldState = getLoadingState();
                if( oldState != state )
                {
                    m_loadingState = state;

                    auto args = Array<Parameter>();
                    args.resize( 2 );

                    args[0] = Parameter( (s32)oldState.load() );
                    args[1] = Parameter( (s32)state.load() );

                    applicationManager->triggerEvent( IEvent::Type::Object, IEvent::loadingStateChanged,
                                                      args, this, this, nullptr );
                }
            }
            else
            {
                m_loadingState = state;
            }
        }
        else
        {
            m_loadingState = state;
        }
    }

    bool ISharedObject::isLoading() const
    {
        return m_loadingState == LoadingState::Loading;
    }

    bool ISharedObject::isLoadingQueued() const
    {
        return m_loadingState == LoadingState::LoadingQueued;
    }

    bool ISharedObject::isThreadSafe() const
    {
        return false;
    }

    void ISharedObject::setPoolElement( bool poolElement )
    {
        if( poolElement )
        {
            m_objectFlags = m_objectFlags | GC_FLAG_POOL_ELEMENT;
        }
        else
        {
            m_objectFlags = m_objectFlags & ~GC_FLAG_POOL_ELEMENT;
        }
    }

    bool ISharedObject::isPoolElement() const
    {
        return ( m_objectFlags & GC_FLAG_POOL_ELEMENT ) != 0;
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
        return ( m_objectFlags & GC_FLAG_ENABLE_REFERENCE_TRACKING ) != 0;
    }

    void ISharedObject::setEnableReferenceTracking( bool referenceTracking )
    {
        if( referenceTracking )
        {
            m_objectFlags.fetch_or( GC_FLAG_ENABLE_REFERENCE_TRACKING );
        }
        else
        {
            m_objectFlags.fetch_and( ~GC_FLAG_ENABLE_REFERENCE_TRACKING );
        }
    }

    bool ISharedObject::isGarbageCollected() const
    {
        return ( m_objectFlags & GC_FLAG_GARBAGE_COLLECTED ) != 0;
    }

    void ISharedObject::setGarbageCollected( bool garbageCollected )
    {
        if( garbageCollected )
        {
            m_objectFlags.fetch_or( GC_FLAG_GARBAGE_COLLECTED );
        }
        else
        {
            m_objectFlags.fetch_and( ~GC_FLAG_GARBAGE_COLLECTED );
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
