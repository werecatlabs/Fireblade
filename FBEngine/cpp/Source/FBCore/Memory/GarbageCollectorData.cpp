#include <FBCore/FBCorePCH.h>
#include <FBCore/Memory/GarbageCollectorData.h>
#include <FBCore/Interface/Memory/ISharedObject.h>
#include <FBCore/Interface/Memory/ISharedObjectListener.h>
#include <FBCore/Memory/TypeManager.h>
#include <FBCore/Memory/Memory.h>
#include <FBCore/Core/Util.h>

#if _DEBUG
#    include <FBCore/Core/LogManager.h>
#endif

namespace fb
{
    GarbageCollectorData::GarbageCollectorData()
    {
    }

    GarbageCollectorData::~GarbageCollectorData()
    {
    }

    void GarbageCollectorData::reserve( size_t size )
    {
        FB_ASSERT( isValid() );

        RecursiveMutex::ScopedLock lock( m_mutex );

        auto currentSize = getSize();
        if( currentSize != size )
        {
            //auto oldObjects = Array<Atomic<ISharedObject *>>( m_objects.begin(), m_objects.end() );
            //for( auto &obj : oldObjects )
            //{
            //    auto object = obj.load();
            //    if( object )
            //    {
            //        object->addReference();
            //    }
            //}

            m_addReference.resize( size );
            m_removeReference.resize( size );
            m_removeQueue.resize( size );

            m_objects.resize( size );

            m_objectNames.resize( size );
            m_loadingStates.resize( size );
            m_listeners.resize( size );
            m_handles.resize( size );
            m_creatorData.resize( size );
            m_userData.resize( size );
            m_factoryData.resize( size );
            m_references.resize( size );
            m_flags.resize( size );

            for( size_t i = currentSize; i < size; ++i )
            {
                setFlag( static_cast<u32>( i ), GC_FLAG_GARBAGE_COLLECTED, true );
            }

            for( size_t i = currentSize; i < size; ++i )
            {
                m_loadingStates[i] = LoadingState::Unallocated;
            }

            for( size_t i = currentSize; i < size; ++i )
            {
                m_references[i] = 0;
            }

            for( size_t i = currentSize; i < size; ++i )
            {
                m_objects[i] = nullptr;
            }

            for( size_t i = 0; i < currentSize; ++i )
            {
                auto object = m_objects[i].load();
                if( object )
                {
                    object->setupGarbageCollectorData();
                }
            }

            m_size = static_cast<u32>( size );

            //for( auto &obj : oldObjects )
            //{
            //    auto object = obj.load();
            //    if( object )
            //    {
            //        object->removeReference();
            //    }
            //}
        }

        FB_ASSERT( isValid() );
    }

    size_t GarbageCollectorData::getSize() const
    {
        return m_size;
    }

    TypeGroups GarbageCollectorData::getTypeGroup() const
    {
        return m_typeGroup;
    }

    void GarbageCollectorData::setTypeGroup( TypeGroups typeGroup )
    {
        m_typeGroup = typeGroup;
    }

    bool GarbageCollectorData::isValid() const
    {
#ifdef _DEBUG
        //if( getTypeGroup() == TypeGroups::IOArchive )
        //{
        //    for( size_t i = 0; i < m_objects.size(); ++i )
        //    {
        //        auto object = m_objects[i].load();
        //        if( object )
        //        {
        //            object->setupGarbageCollectorData();
        //        }
        //    }
        //}
#endif

        return true;
    }

    void GarbageCollectorData::load()
    {
        auto state = getLoadingState();
        if( state == LoadingState::Allocated )
        {
            setLoadingState( LoadingState::Loading );

            //const auto size =
            //    FB_GC_MAX_SHARED_OBJECTS;  // todo work around to reduce resizing this can crash
            const auto size = 128;
            reserve( size );

            setLoadingState( LoadingState::Loaded );
        }
    }

    void GarbageCollectorData::unload()
    {
        auto state = getLoadingState();
        if( state == LoadingState::Loaded )
        {
            setLoadingState( LoadingState::Unloading );

            m_objects.clear();
            m_objectNames.clear();
            // if (m_objectNames)
            //{
            //	delete[] m_objectNames;
            //	m_objectNames = nullptr;
            // }

            //if(m_loadingStates)
            //{
            //    delete[] m_loadingStates;
            //    m_loadingStates = nullptr;
            //}

            //if( m_listeners )
            //{
            //    delete[] m_listeners;
            //    m_listeners = nullptr;
            //}

            //if( m_handles )
            //{
            //    delete[] m_handles;
            //    m_handles = nullptr;
            //}

            //m_creatorData.clear();
            //m_userData.clear();
            //m_factoryData.clear();

            //if( m_references )
            //{
            //    delete[] m_references;
            //    m_references = nullptr;
            //}

            //if( m_flags )
            //{
            //    delete[] m_flags;
            //    m_flags = nullptr;
            //}

            setLoadingState( LoadingState::Unloaded );
        }
    }

    void GarbageCollectorData::update()
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        const auto mode = getMode();
        switch( mode )
        {
        case GarbageCollectorMode::LowConsumption:
        {
        }
        break;
        case GarbageCollectorMode::Balanced:
        case GarbageCollectorMode::Deferred:
        {
            atomic_u32 numRemove = 0;
            atomic_u32 *removeList = nullptr;

            if( m_addReferenceCount > 0 )
            {
                //qsort( m_addReference, 6, sizeof( int ), CoreUtil::compare );

                for( size_t i = 0; i < m_addReferenceCount; ++i )
                {
                    auto id = m_addReference[i].load();

                    ++m_references[id];
                }

                m_addReferenceCount = 0;
            }

            if( m_removeReferenceCount > 0 )
            {
                //qsort( m_removeReference, 6, sizeof( int ), CoreUtil::compare );

                for( size_t i = 0; i < m_removeReferenceCount; ++i )
                {
                    auto id = m_removeReference[i].load();

                    if( --m_references[id] == 0 )
                    {
                        setLoadingState( id, LoadingState::QueuedGC );
                        m_removeQueue[m_numRemove++] = id;
                    }
                }

                m_removeReferenceCount = 0;
            }

            if( m_numRemove > 0 )
            {
                numRemove = m_numRemove.load();
                removeList = new atomic_u32[m_numRemove];

                for( size_t i = 0; i < numRemove; ++i )
                {
                    removeList[i] = m_removeQueue[i].load();
                }

                m_numRemove = 0;
            }

            FB_ASSERT( Memory::CheckHeap() );

            for( size_t i = 0; i < numRemove; ++i )
            {
                const auto &id = removeList[i];

                if( getFlag( id, GC_FLAG_GARBAGE_COLLECTED ) )
                {
                    const Atomic<LoadingState> queuedState = LoadingState::QueuedGC;
                    const auto &loadingState = getLoadingState( id );

                    if( loadingState == queuedState )
                    {
#if _DEBUG
                        const auto &objectName = m_objectNames[id];
                        // FB_LOG(objectName + " destroyed");
#endif

                        FB_ASSERT( m_references[id] <= 0 );

                        auto object = swapObject( id, nullptr );
                        if( object )
                        {
                            FB_ASSERT( id == object->getObjectId() );

                            auto listener = getObjectListener( id );
                            if( listener )
                            {
                                auto result = listener->destroy( object );
                                FB_ASSERT( result );
                            }
                            else
                            {
                                delete object;
                            }
                        }

                        setLoadingState( id, LoadingState::Unallocated );
                    }
                }
            }

            if( removeList )
            {
                delete[] removeList;
                removeList = nullptr;
            }

            FB_ASSERT( Memory::CheckHeap() );
        }
        break;
        default:
        {
        }
        break;
        }
    }

    u32 GarbageCollectorData::addObject( ISharedObject *object )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );

        const auto retries = 10;
        for( size_t i = 0; i < retries; ++i )
        {
            for( u32 i = m_lastFreeSlot.load() + 1; i < getSize(); ++i )
            {
                if( isFreeSlot( i ) )
                {
                    setReferences( i, 1 );
                    setLoadingState( i, LoadingState::Allocated );
                    setFlag( i, GC_FLAG_OBJECT_ALIVE, true );
                    setFlag( i, GC_FLAG_GARBAGE_COLLECTED, true );
                    setFlag( i, GC_FLAG_ENABLE_REFERENCE_TRACKING, false );
                    setFlag( i, GC_FLAG_OBJECT_CONSTRUCTED, false );

                    FB_ASSERT( getObject( i ) == nullptr );
                    setObject( i, object );

                    m_lastFreeSlot = i;
                    return i;
                }
            }

            for( u32 i = 0; i < m_lastFreeSlot; ++i )
            {
                if( isFreeSlot( i ) )
                {
                    setReferences( i, 1 );
                    setLoadingState( i, LoadingState::Allocated );
                    setFlag( i, GC_FLAG_OBJECT_ALIVE, true );
                    setFlag( i, GC_FLAG_GARBAGE_COLLECTED, true );
                    setFlag( i, GC_FLAG_ENABLE_REFERENCE_TRACKING, false );
                    setFlag( i, GC_FLAG_OBJECT_CONSTRUCTED, false );

                    FB_ASSERT( getObject( i ) == nullptr );
                    setObject( i, object );

                    m_lastFreeSlot = i;
                    return i;
                }
            }

            FB_ASSERT( isValid() );

            // grow the arrays
            const auto currentSize = getSize();
            reserve( currentSize + currentSize );

            FB_ASSERT( isValid() );
        }

        FB_ASSERT( isValid() );
        return std::numeric_limits<u32>::max();
    }

    void GarbageCollectorData::removeObject( u32 id )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );

        setObject( id, nullptr );
    }

    const Atomic<LoadingState> &GarbageCollectorData::getLoadingState( u32 id ) const
    {
        FB_ASSERT( id < getSize() );
        //FB_ASSERT( m_loadingStates[id].load() < LoadingState::Count );
        return m_loadingStates[id];
    }

    Atomic<LoadingState> *GarbageCollectorData::getLoadingStatePtr( u32 id ) const
    {
        FB_ASSERT( m_loadingStates[id].load() < LoadingState::Count );
        return (Atomic<LoadingState> *)&m_loadingStates[id];
    }

    void GarbageCollectorData::setLoadingState( u32 id, const Atomic<LoadingState> &state )
    {
        FB_ASSERT( state.load() < LoadingState::Count );
        FB_ASSERT( id < getSize() );

        m_loadingStates[id] = state.load();
    }

    ISharedObjectListener *GarbageCollectorData::getObjectListener( u32 id ) const
    {
        FB_ASSERT( id < getSize() );
        return m_listeners[id];
    }

    void GarbageCollectorData::setObjectListener( u32 id, ISharedObjectListener *listener )
    {
        FB_ASSERT( id < getSize() );
        m_listeners[id] = listener;
    }

    Handle *GarbageCollectorData::getHandle( u32 id ) const
    {
        FB_ASSERT( id < getSize() );
        return (Handle *)&m_handles[id];
    }

    void *GarbageCollectorData::getUserData( u32 id ) const
    {
        FB_ASSERT( id < m_userData.size() );
        return m_userData[id];
    }

    void GarbageCollectorData::setUserData( u32 id, void *userData )
    {
        FB_ASSERT( id < m_userData.size() );
        m_userData[id] = userData;
    }

    void GarbageCollectorData::setCreatorData( u32 id, void *creatorData )
    {
        FB_ASSERT( id < m_creatorData.size() );
        m_creatorData[id] = creatorData;
    }

    void *GarbageCollectorData::getCreatorData( u32 id ) const
    {
        FB_ASSERT( id < m_creatorData.size() );
        return m_creatorData[id];
    }

    hash32 GarbageCollectorData::getFactoryData( u32 id ) const
    {
        FB_ASSERT( id < m_factoryData.size() );
        return m_factoryData[id];
    }

    void GarbageCollectorData::setFactoryData( u32 id, hash32 factoryData )
    {
        FB_ASSERT( id < m_factoryData.size() );
        m_factoryData[id] = factoryData;
    }

    s32 GarbageCollectorData::getReferences( u32 id ) const
    {
        FB_ASSERT( id < getSize() );
        return m_references[id];
    }

    void GarbageCollectorData::setReferences( u32 id, s32 references )
    {
        FB_ASSERT( id < getSize() );
        m_references[id].store( references );
    }

    s32 GarbageCollectorData::addReference( u32 id )
    {
        switch( auto mode = getMode() )
        {
        case GarbageCollectorMode::LowConsumption:
        {
            if( getFlag( id, GC_FLAG_GARBAGE_COLLECTED ) )
            {
                auto &references = m_references[id];
                ++references;

                // FB_ASSERT(((s32)(references)) == references);
                return references;
            }

            return 0;
        }
        break;
        case GarbageCollectorMode::Balanced:
        {
            auto &references = m_references[id];
            if( getFlag( id, GC_FLAG_GARBAGE_COLLECTED ) )
            {
                ++references;
            }

            // FB_ASSERT(((s32)(references)) == references);
            return references;
        }
        break;
        case GarbageCollectorMode::Deferred:
        {
            if( getFlag( id, GC_FLAG_GARBAGE_COLLECTED ) )
            {
                m_addReference[m_addReferenceCount] = id;
            }

            return m_references[id];
        }
        break;
        default:
        {
        }
        break;
        }

        return 0;
    }

    s32 GarbageCollectorData::removeReference( u32 id )
    {
        auto mode = getMode();
        switch( mode )
        {
        case GarbageCollectorMode::LowConsumption:
        {
            if( getFlag( id, GC_FLAG_GARBAGE_COLLECTED ) )
            {
                auto &references = m_references[id];
                const atomic_s32 zero = 0;

                if( references > zero )
                {
                    --references;

                    if( references == zero )
                    {
#ifdef _DEBUG
                        if( id == 49 )
                        {
                            int stop = 0;
                            stop = 0;
                        }
#endif

                        setReferences( id, -1 );
                        setFlag( id, GC_FLAG_OBJECT_ALIVE, false );

                        if( auto object = swapObject( id, nullptr ) )
                        {
                            bool destroyed = false;

                            if( auto listener = object->getSharedObjectListener() )
                            {
                                destroyed = listener->destroy( object );
                                if( destroyed )
                                {
                                    break;
                                }
                            }

                            if( !destroyed )
                            {
                                delete object;
                            }
                        }

                        setLoadingState( id, LoadingState::Unallocated );
                    }
                }
            }
        }
        break;
        case GarbageCollectorMode::Balanced:
        {
            auto &references = m_references[id];

            if( getFlag( id, GC_FLAG_GARBAGE_COLLECTED ) )
            {
                const atomic_s32 zero = 0;

                if( references > zero )
                {
                    --references;

                    if( references == zero )
                    {
                        setReferences( id, -1 );
                        setFlag( id, GC_FLAG_OBJECT_ALIVE, false );
                        queueFreedObject( id );
                    }
                }
            }
        }
        break;
        case GarbageCollectorMode::Deferred:
        {
            if( getFlag( id, GC_FLAG_GARBAGE_COLLECTED ) )
            {
                m_removeReference[m_removeReferenceCount++] = id;
            }

            auto &references = m_references[id];
            return references;
        }
        break;
        default:
        {
        }
        break;
        }

        return 0;
    }

    void GarbageCollectorData::destroyObject( u32 id )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );

        setReferences( id, -1 );
        setFlag( id, GC_FLAG_OBJECT_ALIVE, false );

        if( auto object = swapObject( id, nullptr ) )
        {
            bool destroyed = false;

            if( auto listener = object->getSharedObjectListener() )
            {
                destroyed = listener->destroy( object );
            }

            if( !destroyed )
            {
                if( !object->isPoolElement() )
                {
                    delete object;
                }
            }
        }

        setLoadingState( id, LoadingState::Unallocated );

        FB_ASSERT( isValid() );
    }

    void GarbageCollectorData::updateReferences()
    {
        if( m_addReferenceCount > 0 )
        {
            //qsort( m_addReference, 6, sizeof( int ), CoreUtil::compare );

            for( size_t i = 0; i < m_addReferenceCount; ++i )
            {
                auto id = m_addReference[i].load();

                ++m_references[id];
            }

            m_addReferenceCount = 0;
        }

        // if( !m_removeReference.empty() )
        //{
        //     u32 id;
        //     while( m_removeReference.try_pop( id ) && m_numRemove < FB_GC_MAX_OBJECTS_REMOVED )
        //     {
        //         if( --m_references[id] == 0 )
        //         {
        //             setLoadingState( id, LoadingState::QueuedGC );
        //             m_removeList[m_numRemove++] = id;
        //         }
        //     }
        // }
    }

    void GarbageCollectorData::setFlag( u32 id, u8 flag, bool value )
    {
        //FB_ASSERT( id < getSize() );

        if( value )
        {
            m_flags[id].fetch_or( flag );
        }
        else
        {
            m_flags[id].fetch_and( ~flag );
        }
    }

    bool GarbageCollectorData::getFlag( u32 id, u8 flag ) const
    {
        FB_ASSERT( id < getSize() );
        u8 flags = m_flags[id].load();
        return ( flags & flag ) != 0;
    }

    atomic_u8 *GarbageCollectorData::getFlagPtr( u32 id ) const
    {
        return (atomic_u8 *)&m_flags[id];
    }

    Array<ISharedObject *> GarbageCollectorData::getObjects() const
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        Array<ISharedObject *> objects;
        for( size_t i = 0; i < getSize(); ++i )
        {
            const auto &object = m_objects[i];
            if( object )
            {
                objects.push_back( object );
            }
        }

        return objects;
    }

    ISharedObject **GarbageCollectorData::getObjectPtr( u32 id ) const
    {
        FB_ASSERT( isValid() );

        FB_ASSERT( id < getSize() );
        return (ISharedObject **)&m_objects[id];
    }

    ISharedObject *GarbageCollectorData::getObject( u32 id ) const
    {
        FB_ASSERT( isValid() );

        FB_ASSERT( id < getSize() );
        return m_objects[id];
    }

    void GarbageCollectorData::setObject( u32 id, ISharedObject *object )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );

        FB_ASSERT( id < getSize() );
        m_objects[id] = object;

#if 1  // _DEBUG
        if( object )
        {
            auto typeManager = TypeManager::instance();
            auto typeInfo = object->getTypeInfo();
            auto name = typeManager->getName( typeInfo );
            m_objectNames[id] = name;
        }
#endif
    }

    atomic_s32 *GarbageCollectorData::getReferencesPtr( u32 id )
    {
        FB_ASSERT( id < getSize() );
        return &m_references[id];
    }

    GarbageCollectorMode GarbageCollectorData::getMode() const
    {
        return m_mode;
    }

    void GarbageCollectorData::setMode( GarbageCollectorMode mode )
    {
        m_mode = mode;
    }

    void GarbageCollectorData::collect()
    {
        FB_ASSERT( isValid() );

        for( u32 i = 0; i < m_numObjects; ++i )
        {
            if( !getFlag( i, GC_FLAG_GARBAGE_COLLECTED ) )
            {
                const auto &reference = m_references[i];
                if( reference == 0 )
                {
#if _DEBUG
                    const auto &objectName = m_objectNames[i];
                    FB_LOG( objectName + " destroyed" );
#endif

                    auto object = getObject( i );
                    if( object )
                    {
                        if( auto listener = object->getSharedObjectListener() )
                        {
                            auto result = listener->destroy( object );
                            FB_ASSERT( result );
                        }
                        else
                        {
                            delete object;
                        }

                        setObject( i, nullptr );
                    }

                    setLoadingState( i, LoadingState::Unallocated );
                }
            }
        }
    }

    bool GarbageCollectorData::isFreeSlot( u32 slot )
    {
        const Atomic<LoadingState> unallocatedState = LoadingState::Unallocated;
        const auto &loadingState = getLoadingState( slot );

        if( loadingState == unallocatedState )
        {
            return true;
        }

        return false;
    }

    void GarbageCollectorData::queueFreedObject( u32 id )
    {
        setLoadingState( id, LoadingState::QueuedGC );

        m_removeQueue[m_numRemove++] = id;
    }

    ISharedObject *GarbageCollectorData::swapObject( u32 id, ISharedObject *object )
    {
        RecursiveMutex::ScopedLock lock( m_mutex );

        FB_ASSERT( isValid() );

#if _DEBUG
        if( id == 0 && getTypeGroup() == TypeGroups::UI )
        {
            int stop = 0;
            stop = 0;
        }
#endif

        FB_ASSERT( id < getSize() );

        auto old = m_objects[id];
        m_objects[id] = object;

        FB_ASSERT( isValid() );

        return old;
    }

    Atomic<LoadingState> &GarbageCollectorData::getLoadingState()
    {
        return m_loadingState;
    }

    const Atomic<LoadingState> &GarbageCollectorData::getLoadingState() const
    {
        return m_loadingState;
    }

    void GarbageCollectorData::setLoadingState( Atomic<LoadingState> state )
    {
        m_loadingState.store( state );
    }

}  // namespace fb
