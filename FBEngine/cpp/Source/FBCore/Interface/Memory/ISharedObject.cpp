#include <FBCore/FBCorePCH.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Atomics/Atomics.h>
#include <FBCore/Core/Handle.h>
#include <FBCore/Core/Properties.h>
#include <FBCore/Core/StringTypes.h>
#include <FBCore/Interface/Memory/IData.h>
#include <FBCore/Interface/Memory/ISharedObjectListener.h>
#include <FBCore/Interface/System/IEventListener.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Memory/ObjectTracker.h>
#include <FBCore/Memory/PointerUtil.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/System/ApplicationManager.h>
#include <FBCore/System/RttiClassDefinition.h>
#include <boost/pool/singleton_pool.hpp>
#include <utility>

namespace fb
{
    FB_CLASS_REGISTER_DERIVED( fb, ISharedObject, IObject );
    using HandlePool = boost::singleton_pool<Handle, sizeof( Handle )>;

    ISharedObject::ScopedLock::ScopedLock( SmartPtr<ISharedObject> sharedObject ) :
        m_sharedObject( std::move( sharedObject ) )
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

    void ISharedObject::preUpdate()
    {
    }

    void ISharedObject::update()
    {
    }

    void ISharedObject::postUpdate()
    {
    }

    auto ISharedObject::addWeakReference( void *address, const c8 *file, u32 line, const c8 *func )
        -> s32
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_WEAK_REFERENCES
        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );
#    endif
#endif

        return ++m_weakReferences;
    }

    auto ISharedObject::removeWeakReference() -> bool
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

    auto ISharedObject::removeWeakReference( void *address, const c8 *file, u32 line, const c8 *func )
        -> bool
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

    auto ISharedObject::removeReference( void *address, const c8 *file, const u32 line, const c8 *func )
        -> bool
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

    auto ISharedObject::addReference( void *address, const c8 *file, const u32 line, const c8 *func )
        -> s32
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

    String ISharedObject::getName() const
    {
        if( auto handle = getHandle() )
        {
            return handle->getName();
        }

        return {};
    }

    void ISharedObject::setName( const String &name )
    {
        if( auto handle = getHandle() )
        {
            handle->setName( name );
        }
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

    auto ISharedObject::getLoadingState() const -> const Atomic<LoadingState> &
    {
        return m_loadingState;
    }

    void ISharedObject::setLoadingState( const Atomic<LoadingState> &state )
    {
        auto applicationManager = core::ApplicationManager::instance();
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

                    args[0] = Parameter( static_cast<s32>( oldState.load() ) );
                    args[1] = Parameter( static_cast<s32>( state.load() ) );

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

    auto ISharedObject::isLoading() const -> bool
    {
        return m_loadingState == LoadingState::Loading;
    }

    auto ISharedObject::isLoadingQueued() const -> bool
    {
        return m_loadingState == LoadingState::LoadingQueued;
    }

    auto ISharedObject::isThreadSafe() const -> bool
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

    auto ISharedObject::isPoolElement() const -> bool
    {
        return ( m_objectFlags & GC_FLAG_POOL_ELEMENT ) != 0;
    }

    auto ISharedObject::toData() const -> SmartPtr<ISharedObject>
    {
        return nullptr;
    }

    void ISharedObject::fromData( SmartPtr<ISharedObject> data )
    {
    }

    auto ISharedObject::getProperties() const -> SmartPtr<Properties>
    {
        s32 references = getReferences();
        s32 weakReferences = getWeakReferences();
        auto loadingState = getLoadingState();
        auto iLoadingState = static_cast<s32>( loadingState.load() );

        auto properties = fb::make_ptr<Properties>();
        properties->setProperty( "references", references );
        properties->setProperty( "weakReferences", weakReferences );
        properties->setProperty( "loadingState", iLoadingState );
        return properties;
    }

    void ISharedObject::setProperties( SmartPtr<Properties> properties )
    {
        s32 references = 0;
        s32 weakReferences = 0;
        s32 loadingState = 0;

        properties->getPropertyValue( "references", references );
        properties->getPropertyValue( "weakReferences", weakReferences );
        properties->getPropertyValue( "loadingState", loadingState );

        m_references = references;
        m_weakReferences = weakReferences;
        m_loadingState = static_cast<LoadingState>( loadingState );
    }

    auto ISharedObject::getChildObjects() const -> Array<SmartPtr<ISharedObject>>
    {
        return {};
    }

    auto ISharedObject::getObjectListeners() const -> Array<SmartPtr<IEventListener>>
    {
        if( auto p = getObjectListenersPtr() )
        {
            auto &listeners = *p;
            return Array<SmartPtr<IEventListener>>( listeners.begin(), listeners.end() );
        }

        return {};
    }

    auto ISharedObject::getNumListeners() const -> u32
    {
        if( auto p = getObjectListenersPtr() )
        {
            auto &listeners = *p;
            return static_cast<u32>( listeners.size() );
        }

        return 0;
    }

    auto ISharedObject::hasObjectListener( SmartPtr<IEventListener> listener ) const -> bool
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

    auto ISharedObject::findObjectListener( const String &id ) const -> SmartPtr<IEventListener>
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

    auto ISharedObject::getSharedObjectListener() const -> ISharedObjectListener *
    {
        return m_sharedObjectListener.load();
    }

    void ISharedObject::setSharedObjectListener( ISharedObjectListener *listener )
    {
        m_sharedObjectListener = listener;
    }

    auto ISharedObject::getEnableReferenceTracking() const -> bool
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

    auto ISharedObject::isGarbageCollected() const -> bool
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

    auto ISharedObject::getObjectListenersPtr() const
        -> SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>>
    {
        return m_sharedEventListeners;
    }

    void ISharedObject::setObjectListenersPtr( SharedPtr<ConcurrentArray<SmartPtr<IEventListener>>> p )
    {
        m_sharedEventListeners = p;
    }

#if _DEBUG
    s32 ISharedObject::addWeakReference()
    {
#    if FB_TRACK_REFERENCES
#        if FB_TRACK_WEAK_REFERENCES
        auto address = (void *)this;
        const c8 *file = __FILE__;
        const u32 line = __LINE__;
        const c8 *func = __FUNCTION__;

        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );
#        endif
#    endif

        return ++m_weakReferences;
    }

    s32 ISharedObject::addReference()
    {
#    if FB_TRACK_REFERENCES
#        if FB_TRACK_STRONG_REFERENCES

        auto references = gc.addReference( T::typeInfo(), m_objectId );

        // FB_ASSERT( isGarbageCollected() && references > 0 );
        FB_ASSERT( references < 1e10 );

        auto address = (void *)this;
        const c8 *file = __FILE__;
        const u32 line = __LINE__;
        const c8 *func = __FUNCTION__;

        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );

        return references;
#        else

        return gc.addReference( T::typeInfo(), m_objectId );
#        endif
#    else
        return ++m_references;
#    endif
    }

    bool ISharedObject::removeReference()
    {
#    if FB_TRACK_REFERENCES
#        if FB_TRACK_STRONG_REFERENCES
        auto address = (void *)this;
        const c8 *file = __FILE__;
        const u32 line = __LINE__;
        const c8 *func = __FUNCTION__;

        auto &objectTracker = ObjectTracker::instance();
        objectTracker.removeRef( this, address, file, line, func );
#        endif
#    endif

        if( --m_references == 0 )
        {
            destroySharedObject();
            return true;
        }

        return false;
    }

    s32 ISharedObject::getWeakReferences() const
    {
        return m_weakReferences;
    }

    s32 ISharedObject::getReferences() const
    {
        return m_references;
    }

    bool ISharedObject::isAlive() const
    {
        return ( m_objectFlags & GC_FLAG_OBJECT_ALIVE ) != 0;
    }

    bool ISharedObject::isLoaded() const
    {
        return m_loadingState == LoadingState::Loaded;
    }

    SmartPtr<ISharedObject> ISharedObject::getScriptData() const
    {
        return m_scriptData;
    }

    void ISharedObject::setScriptData( SmartPtr<ISharedObject> data )
    {
        m_scriptData = data;
    }
#endif

}  // end namespace fb
