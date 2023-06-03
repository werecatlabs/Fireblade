namespace fb
{

    template <class T>
    SharedObject<T>::SharedObject()
    {
        auto &gc = GarbageCollector::instance();
        auto objectId = gc.addObject( this );
        m_objectId.store( objectId );
        FB_ASSERT( m_objectId != std::numeric_limits<u32>::max() );

#if FB_TRACK_REFERENCES
        setEnableReferenceTracking( true );
#endif

        setupGarbageCollectorData();

#if FB_TRACK_REFERENCES
        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addObjectRecord( this );
        objectTracker.addRef( this, this, __FILE__, __LINE__, __FUNCTION__ );
#endif
    }

    template <class T>
    SharedObject<T>::~SharedObject()
    {
        auto listeners = getObjectListeners();
        for( auto &listener : listeners )
        {
            listener->unload( nullptr );
        }

        setObjectListenersPtr( nullptr );

        //FB_ASSERT( getWeakReferences() == 0 );

        auto &gc = GarbageCollector::instance();

        auto typeInfo = getTypeInfo();

        //if( TypeManager::instance()->getTypeGroup( typeInfo ) == (u32)TypeGroups::Application )
        //{
        //    if( m_objectId.load() == 960 )
        //    {
        //        int stop = 0;
        //        stop = 0;
        //    }
        //}

        const auto id = m_objectId.load();
        gc.removeObject( typeInfo, id );

#if FB_TRACK_REFERENCES
        auto typeManager = TypeManager::instance();
        auto &objectTracker = ObjectTracker::instance();

        if( getWeakReferences() != 0 )
        {
            auto typeInfo = SharedObject<T>::getTypeInfo();
            auto typeName = typeManager->getName( typeInfo );
            auto filePath = typeName + ".log";
            objectTracker.dumpReport( this, filePath );
        }

        objectTracker.removeObjectRecord( this );
#endif
    }

    template <class T>
    void SharedObject<T>::preUpdate()
    {
    }

    template <class T>
    void SharedObject<T>::update()
    {
    }

    template <class T>
    void SharedObject<T>::postUpdate()
    {
    }

    template <class T>
    Handle *SharedObject<T>::getHandle() const
    {
        return m_handle;
    }

    template <class T>
    s32 SharedObject<T>::addWeakReference()
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_WEAK_REFERENCES
        auto address = (void *)this;
        const c8 *file = __FILE__;
        const u32 line = __LINE__;
        const c8 *func = __FUNCTION__;

        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );
#    endif
#endif

        return ++m_weakReferences;
    }

    template <class T>
    s32 SharedObject<T>::addWeakReference( void *address, const c8 *file, u32 line, const c8 *func )
    {
#if FB_TRACK_REFERENCES
#    if FB_TRACK_WEAK_REFERENCES
        auto &objectTracker = ObjectTracker::instance();
        objectTracker.addRef( this, address, file, line, func );
#    endif
#endif

        return ++m_weakReferences;
    }

    template <class T>
    bool SharedObject<T>::removeWeakReference()
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

    template <class T>
    bool SharedObject<T>::removeWeakReference( void *address, const c8 *file, u32 line, const c8 *func )
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

    template <class T>
    bool SharedObject<T>::isValid() const
    {
        return m_objectId < std::numeric_limits<u32>::max();
    }

    template <class T>
    void SharedObject<T>::load( SmartPtr<ISharedObject> data )
    {
    }

    template <class T>
    void SharedObject<T>::reload( SmartPtr<ISharedObject> data )
    {
    }

    template <class T>
    void SharedObject<T>::unload( SmartPtr<ISharedObject> data )
    {
    }

    template <class T>
    const Atomic<LoadingState> &SharedObject<T>::getLoadingState() const
    {
        auto &gc = GarbageCollector::instance();
        return gc.getLoadingState( getTypeInfo(), m_objectId );
    }

    template <class T>
    void SharedObject<T>::setLoadingState( const Atomic<LoadingState> &state )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto &gc = GarbageCollector::instance();

        auto oldState = gc.getLoadingState( getTypeInfo(), m_objectId );
        if( oldState != state )
        {
            gc.setLoadingState( getTypeInfo(), m_objectId, state );

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

    template <class T>
    bool SharedObject<T>::isLoading() const
    {
        const auto &loadingState = getLoadingState();
        return loadingState == LoadingState::Loading;
    }

    template <class T>
    bool SharedObject<T>::isLoadingQueued() const
    {
        const auto &loadingState = getLoadingState();
        return loadingState == LoadingState::LoadingQueued;
    }

    template <class T>
    bool SharedObject<T>::isLoaded() const
    {
        const auto &loadingState = getLoadingState();
        return loadingState == LoadingState::Loaded;
    }

    template <class T>
    bool SharedObject<T>::isThreadSafe() const
    {
        return false;
    }

    template <class T>
    bool SharedObject<T>::isAlive() const
    {
        return ( *m_flags & GC_FLAG_OBJECT_ALIVE ) != 0;
    }

    template <class T>
    void SharedObject<T>::setPoolElement( bool poolElement )
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

    template <class T>
    bool SharedObject<T>::isPoolElement() const
    {
        return ( *m_flags & GC_FLAG_POOL_ELEMENT ) != 0;
    }

    template <class T>
    void *SharedObject<T>::getCreatorData() const
    {
        return GarbageCollector::instance().getCreatorData( getTypeInfo(), m_objectId );
    }

    template <class T>
    void SharedObject<T>::setCreatorData( void *val )
    {
        GarbageCollector::instance().setCreatorData( getTypeInfo(), m_objectId, val );
    }

    template <class T>
    hash32 SharedObject<T>::getFactoryData() const
    {
        return GarbageCollector::instance().getFactoryData( getTypeInfo(), m_objectId );
    }

    template <class T>
    void SharedObject<T>::setFactoryData( hash32 factoryData )
    {
        GarbageCollector::instance().setFactoryData( getTypeInfo(), m_objectId, factoryData );
    }

    template <class T>
    String SharedObject<T>::toString() const
    {
        std::stringstream stream;

        auto typeInfo = SharedObject<T>::getTypeInfo();
        if( typeInfo )
        {
            auto typeManager = TypeManager::instance();
            auto classHierarchy = typeManager->getClassHierarchy( typeInfo );

            for( auto &className : classHierarchy )
            {
                stream << "ClassName: " << className << "\n";
            }
        }

        stream << "creator: " << getCreatorData() << "\n";
        stream << "factory: " << getFactoryData() << "\n";

        return stream.str();
    }

    template <class T>
    SmartPtr<ISharedObject> SharedObject<T>::toData() const
    {
        return nullptr;
    }

    template <class T>
    void SharedObject<T>::fromData( SmartPtr<ISharedObject> data )
    {
    }

    template <class T>
    Array<SmartPtr<ISharedObject>> SharedObject<T>::getChildObjects() const
    {
        return Array<SmartPtr<ISharedObject>>();
    }

    template <class T>
    void *SharedObject<T>::getUserData() const
    {
        return GarbageCollector::instance().getUserData( getTypeInfo(), m_objectId );
    }

    template <class T>
    void *SharedObject<T>::getUserData( [[maybe_unused]] hash32 id ) const
    {
        return nullptr;
    }

    template <class T>
    void SharedObject<T>::setUserData( [[maybe_unused]] hash32 id, [[maybe_unused]] void *userData )
    {
    }

    template <class T>
    void SharedObject<T>::setUserData( [[maybe_unused]] void *userData )
    {
        GarbageCollector::instance().setUserData( getTypeInfo(), m_objectId, userData );
    }

    template <class T>
    s32 SharedObject<T>::getWeakReferences() const
    {
        return m_weakReferences;
    }

    template <class T>
    String SharedObject<T>::toJson() const
    {
        return "";
    }

    template <class T>
    String SharedObject<T>::toXml() const
    {
        return "";
    }

    template <class T>
    Array<SmartPtr<IEventListener>> SharedObject<T>::getObjectListeners() const
    {
        if( auto p = getObjectListenersPtr() )
        {
            auto &listeners = *p;
            return Array<SmartPtr<IEventListener>>( listeners.begin(), listeners.end() );
        }

        return Array<SmartPtr<IEventListener>>();
    }

    template <class T>
    u32 SharedObject<T>::getNumListeners() const
    {
        if( auto p = getObjectListenersPtr() )
        {
            auto &listeners = *p;
            return (u32)listeners.size();
        }

        return 0;
    }

    template <class T>
    bool SharedObject<T>::hasObjectListener( SmartPtr<IEventListener> listener ) const
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

    template <class T>
    void SharedObject<T>::addObjectListener( SmartPtr<IEventListener> listener )
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

    template <class T>
    void SharedObject<T>::removeObjectListener( SmartPtr<IEventListener> listener )
    {
        auto listeners = getObjectListeners();
        auto it = std::find( listeners.begin(), listeners.end(), listener );
        if( it != listeners.end() )
        {
            listeners.erase( it );
        }

        auto p = fb::make_shared<ConcurrentArray<SmartPtr<IEventListener>>>();
        *p = ConcurrentArray<SmartPtr<IEventListener>>( listeners.begin(), listeners.end() );
        setObjectListenersPtr( p );
    }

    template <class T>
    bool SharedObject<T>::getEnableReferenceTracking() const
    {
        const auto &gc = GarbageCollector::instance();
        return gc.getFlag( getTypeInfo(), m_objectId, GC_FLAG_ENABLE_REFERENCE_TRACKING );
    }

    template <class T>
    void SharedObject<T>::setEnableReferenceTracking( bool referenceTracking )
    {
        auto &gc = GarbageCollector::instance();
        gc.setFlag( getTypeInfo(), m_objectId, GC_FLAG_ENABLE_REFERENCE_TRACKING, referenceTracking );
    }

    template <class T>
    bool SharedObject<T>::isGarbageCollected() const
    {
        const auto &gc = GarbageCollector::instance();
        return gc.getFlag( getTypeInfo(), m_objectId, GC_FLAG_GARBAGE_COLLECTED );
    }

    template <class T>
    void SharedObject<T>::setGarbageCollected( bool garbageCollected )
    {
        auto &gc = GarbageCollector::instance();
        gc.setFlag( getTypeInfo(), m_objectId, GC_FLAG_GARBAGE_COLLECTED, garbageCollected );
    }

    template <class T>
    u32 SharedObject<T>::getObjectId() const
    {
        return m_objectId;
    }

    template <class T>
    SmartPtr<ISharedObject> SharedObject<T>::getScriptData() const
    {
        return m_scriptData;
    }

    template <class T>
    void SharedObject<T>::setScriptData( SmartPtr<ISharedObject> data )
    {
        m_scriptData = data;
    }

#if FB_USE_CUSTOM_NEW_DELETE

    template <class T>
    void *SharedObject<T>::operator new( size_t sz )
    {
        void *ptr = nullptr;

#    if FB_OBJECT_ALIGNED_ALLOC
        ptr = Memory::ScalableAlignedMalloc( sz, FB_ALIGNMENT );
#    else
        ptr = Memory::malloc( sz );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_64
#            if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        backtraceMsg << boost::stacktrace::stacktrace();
        auto msg = backtraceMsg.str();
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, msg.c_str() );
#            else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, "" );
#            endif
#        else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, "" );
#        endif
#    endif

        return ptr;
    }

    template <class T>
    void *SharedObject<T>::operator new( size_t sz, void *ptr )
    {
        (void)sz;
        return ptr;
    }

    template <class T>
    void *SharedObject<T>::operator new( size_t sz, [[maybe_unused]] const c8 *file,
                                         [[maybe_unused]] s32 line, [[maybe_unused]] const c8 *func )
    {
        void *ptr = nullptr;

#    if FB_OBJECT_ALIGNED_ALLOC
        ptr = Memory::ScalableAlignedMalloc( sz, FB_ALIGNMENT );
#    else
        ptr = Memory::malloc( sz );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_64
#            if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        backtraceMsg << boost::stacktrace::stacktrace();
        auto msg = backtraceMsg.str();
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, msg.c_str() );
#            else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, "" );
#            endif
#        else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, "" );
#        endif
#    endif

        return ptr;
    }

    template <class T>
    void *SharedObject<T>::operator new( size_t sz, void *ptr, [[maybe_unused]] const c8 *file,
                                         [[maybe_unused]] s32 line, [[maybe_unused]] const c8 *func )
    {
        (void)sz;

#    if FB_ENABLE_MEMORY_TRACKER
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_64
#            if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        backtraceMsg << boost::stacktrace::stacktrace();
        auto msg = backtraceMsg.str();
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, msg.c_str() );
#            else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, "" );
#            endif
#        else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line, "" );
#        endif
#    endif

        return ptr;
    }

    template <class T>
    void *SharedObject<T>::operator new[]( size_t sz, [[maybe_unused]] const char *file,
                                           [[maybe_unused]] int line, [[maybe_unused]] const char *func )
    {
        void *ptr = nullptr;

#    if FB_OBJECT_ALIGNED_ALLOC
        ptr = Memory::ScalableAlignedMalloc( sz, FB_ALIGNMENT );
#    else
        ptr = Memory::malloc( sz );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, file, line );
#    endif

        return ptr;
    }

    template <class T>
    void *SharedObject<T>::operator new[]( size_t sz )
    {
        void *ptr = nullptr;

#    if FB_OBJECT_ALIGNED_ALLOC
        ptr = Memory::ScalableAlignedMalloc( sz, FB_ALIGNMENT );
#    else
        ptr = Memory::malloc( sz );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
#        if FB_ARCH_TYPE == FB_ARCHITECTURE_64
#            if FB_ENABLE_TRACE
        std::stringstream backtraceMsg;
        backtraceMsg << boost::stacktrace::stacktrace();
        auto msg = backtraceMsg.str();
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, msg.c_str() );
#            else
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, "" );
#            endif
#        endif
        MemoryTracker::get()._recordAlloc( ptr, sz, 0, "" );
#    endif
        return ptr;
    }

    template <class T>
    void SharedObject<T>::operator delete( void *ptr )
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    template <class T>
    void SharedObject<T>::operator delete( void *ptr, void * )
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    template <class T>
    void SharedObject<T>::operator delete[]( void *ptr )
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    template <class T>
    void SharedObject<T>::operator delete( void *ptr, [[maybe_unused]] const char *file,
                                           [[maybe_unused]] int line,
                                           [[maybe_unused]] const char *func ) throw()
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    template <class T>
    void SharedObject<T>::operator delete( void *ptr, void *, const char *file, int line,
                                           const char *func )
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }

    template <class T>
    void SharedObject<T>::operator delete[]( void *ptr, const char *file, int line,
                                             const char *func ) throw()
    {
#    if FB_OBJECT_ALIGNED_ALLOC
        Memory::ScalableAlignedFree( ptr );
#    else
        Memory::free( ptr );
#    endif

#    if FB_ENABLE_MEMORY_TRACKER
        MemoryTracker::get()._recordDealloc( ptr );
#    endif
    }
#endif
}  // end namespace fb
