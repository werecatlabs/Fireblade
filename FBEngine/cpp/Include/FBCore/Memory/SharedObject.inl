namespace fb
{

    template <class T>
    SharedObject<T>::SharedObject()
    {
        auto &gc = GarbageCollector::instance();
        auto objectId = gc.addObject( this );
        SharedObject<T>::m_objectId.store( objectId );
        FB_ASSERT( SharedObject<T>::m_objectId != std::numeric_limits<u32>::max() );

#if FB_TRACK_REFERENCES
        setEnableReferenceTracking( true );
#endif

        SharedObject<T>::setupGarbageCollectorData();

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

        const auto id = SharedObject<T>::m_objectId.load();
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
        return T::m_handle;
    }

    template <class T>
    bool SharedObject<T>::isValid() const
    {
        return T::m_objectId < std::numeric_limits<u32>::max();
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
        return gc.getLoadingState( getTypeInfo(), T::m_objectId );
    }

    template <class T>
    void SharedObject<T>::setLoadingState( const Atomic<LoadingState> &state )
    {
        auto applicationManager = core::IApplicationManager::instance();
        auto &gc = GarbageCollector::instance();

        auto oldState = gc.getLoadingState( getTypeInfo(), T::m_objectId );
        if( oldState != state )
        {
            gc.setLoadingState( getTypeInfo(), T::m_objectId, state );

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
        return ( *T::m_flags & GC_FLAG_OBJECT_ALIVE ) != 0;
    }

    template <class T>
    void SharedObject<T>::setPoolElement( bool poolElement )
    {
        if( poolElement )
        {
            ( *T::m_flags ) = *T::m_flags | GC_FLAG_POOL_ELEMENT;
        }
        else
        {
            ( *T::m_flags ) = *T::m_flags & ~GC_FLAG_POOL_ELEMENT;
        }
    }

    template <class T>
    bool SharedObject<T>::isPoolElement() const
    {
        return ( *T::m_flags & GC_FLAG_POOL_ELEMENT ) != 0;
    }

    template <class T>
    void *SharedObject<T>::getCreatorData() const
    {
        return GarbageCollector::instance().getCreatorData( getTypeInfo(), T::m_objectId );
    }

    template <class T>
    void SharedObject<T>::setCreatorData( void *val )
    {
        GarbageCollector::instance().setCreatorData( getTypeInfo(), T::m_objectId, val );
    }

    template <class T>
    hash32 SharedObject<T>::getFactoryData() const
    {
        return GarbageCollector::instance().getFactoryData( getTypeInfo(), T::m_objectId );
    }

    template <class T>
    void SharedObject<T>::setFactoryData( hash32 factoryData )
    {
        GarbageCollector::instance().setFactoryData( getTypeInfo(), T::m_objectId, factoryData );
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
        return GarbageCollector::instance().getUserData( getTypeInfo(), T::m_objectId );
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
        GarbageCollector::instance().setUserData( getTypeInfo(), T::m_objectId, userData );
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
        return gc.getFlag( getTypeInfo(), T::m_objectId, GC_FLAG_ENABLE_REFERENCE_TRACKING );
    }

    template <class T>
    void SharedObject<T>::setEnableReferenceTracking( bool referenceTracking )
    {
        auto &gc = GarbageCollector::instance();
        gc.setFlag( getTypeInfo(), T::m_objectId, GC_FLAG_ENABLE_REFERENCE_TRACKING, referenceTracking );
    }

    template <class T>
    bool SharedObject<T>::isGarbageCollected() const
    {
        const auto &gc = GarbageCollector::instance();
        return gc.getFlag( getTypeInfo(), T::m_objectId, GC_FLAG_GARBAGE_COLLECTED );
    }

    template <class T>
    void SharedObject<T>::setGarbageCollected( bool garbageCollected )
    {
        auto &gc = GarbageCollector::instance();
        gc.setFlag( getTypeInfo(), T::m_objectId, GC_FLAG_GARBAGE_COLLECTED, garbageCollected );
    }

    template <class T>
    u32 SharedObject<T>::getObjectId() const
    {
        return T::m_objectId;
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

}  // end namespace fb
